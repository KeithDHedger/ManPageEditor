/*
 *
 * K.D.Hedger 2013 <kdhedger68713@gmail.com>
 *
 * files.cpp
 *
*/

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <sys/stat.h>
#include <glib.h>

#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcebuffer.h>

#include "globals.h"
#include "callbacks.h"
#include "script.h"

char*		saveFileName=NULL;
char*		saveFilePath=NULL;
bool		dropTextFile=false;

void makeDirty(GtkWidget* widget,gpointer data)
{
	dirty=true;
	setSensitive();
}

GtkWidget* makeNewTab(char* name,char* tooltip,pageStruct* page)
{
	GtkWidget*	evbox=gtk_event_box_new();
	GtkWidget*	hbox=gtk_hbox_new(false,0);
	GtkWidget*	label=gtk_label_new(name);

	gtk_box_pack_start(GTK_BOX(hbox),label,false,false,0);
	gtk_container_add(GTK_CONTAINER(evbox),hbox);
	gtk_signal_connect(GTK_OBJECT(evbox),"button-press-event",G_CALLBACK(tabPopUp),(void*)page);
	page->tabName=label;
	gtk_widget_show_all(evbox);
	
	return(evbox);
}

void setFilePrefs(GtkSourceView* sourceview)
{

	PangoFontDescription*	font_desc;

	gtk_source_view_set_auto_indent(sourceview,true);
	gtk_source_view_set_show_line_numbers(sourceview,false);
	gtk_source_view_set_highlight_current_line(sourceview,highLight);

	if(lineWrap==true)
		gtk_text_view_set_wrap_mode((GtkTextView*)sourceview,GTK_WRAP_WORD_CHAR);
	else
		gtk_text_view_set_wrap_mode((GtkTextView*)sourceview,GTK_WRAP_NONE);

	gtk_source_view_set_tab_width(sourceview,tabWidth);

	font_desc=pango_font_description_from_string(fontAndSize);
	gtk_widget_modify_font((GtkWidget*)sourceview,font_desc);
	pango_font_description_free(font_desc);

}

void resetAllFilePrefs(void)
{
	pageStruct*	page;

	for(int loop=0;loop<gtk_notebook_get_n_pages(notebook);loop++)
		{
			page=getPageStructPtr(loop);
			setFilePrefs(page->view);
		}
}

void dropText(GtkWidget *widget,GdkDragContext *context,gint x,gint y,GtkSelectionData *selection_data,guint info,guint32 time,gpointer user_data)
{
	gchar**			array=NULL;
	int				cnt;
	char*			filename;
	FILE*			fp;
	char*			command;
	GString*		str;
	char			line[1024];
	pageStruct*		page=(pageStruct*)user_data;
	GtkTextIter		iter;
	GtkTextMark*	mark;

	array=gtk_selection_data_get_uris(selection_data);
	if(array==NULL)
		{
			gtk_drag_finish (context,true,true,time);
			return;
		}

	cnt=g_strv_length(array);

	if(dropTextFile==false)
		{
			dropTextFile=true;

			for(int j=0;j<cnt;j++)
				{
					str=g_string_new(NULL);
					filename=g_filename_from_uri(array[j],NULL,NULL);
					asprintf(&command,"cat %s",filename);
					fp=popen(command, "r");
					while(fgets(line,1024,fp))
						g_string_append_printf(str,"%s",line);
					gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(page->buffer),str->str,str->len);

					mark=gtk_text_buffer_get_insert((GtkTextBuffer*)page->buffer);
					gtk_text_buffer_get_iter_at_mark((GtkTextBuffer*)page->buffer,&iter,mark);
					gtk_text_iter_forward_chars(&iter,str->len);
					gtk_text_buffer_place_cursor((GtkTextBuffer*)page->buffer,&iter);

					g_free(command);
					g_string_free(str,true);
					pclose(fp);
				}

			g_strfreev(array);
		}
	else
			dropTextFile=false;

	gtk_drag_finish (context,true,true,time);
}

bool getSaveFile(void)
{
	GtkWidget*	dialog;
	bool		retval=false;

	dialog=gtk_file_chooser_dialog_new("Save File",(GtkWindow*)window, GTK_FILE_CHOOSER_ACTION_SAVE,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_SAVE,GTK_RESPONSE_ACCEPT,NULL);

	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER (dialog),TRUE);
	if(saveFileName!=NULL)
		{
			if(saveFilePath!=NULL)
				gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),g_path_get_dirname(saveFilePath));
			gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog),saveFileName);
		}
 	else
		gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog),"Untitled");

	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_ACCEPT)
		{
			saveFilePath=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			saveFileName=g_path_get_basename(saveFilePath);
			retval=true;
		}

	gtk_widget_destroy(dialog);
	refreshMainWindow();
	return(retval);
}

char* escapeString(char* ptr)
{
	GString*	str=g_string_new(ptr);
	GString*	deststr=g_string_new(NULL);
	g_free(ptr);

	for(unsigned int j=0;j<str->len;j++)
		{
			switch(str->str[j])
				{
					case '-':
						g_string_append(deststr,"\\-");
						break;
//					case '<':
//						g_string_append(deststr,"\-");
//						break;
					default:
						g_string_append_c(deststr,str->str[j]);
				}
		}
	g_string_free(str,true);
	return(g_string_free(deststr,false));
}

char* doReplaceTags(char* str)
{
	char*		newstr=strdup(str);
	char*		tagstr=NULL;
	const char*	tagfrom[]={"&gt;","&lt;","&apos;","&quot;"};
	const char*	tagto[]={">","<","'","\""};
	bool		flag=true;


	while(flag==true)
		{
			tagstr=sliceInclude(newstr,(char*)"<apply_tag name=\"bold",(char*)"\">",true,true);
			if(tagstr==NULL)
				flag=false;
			else
				replaceAllSlice(&newstr,tagstr,(char*)"\\fB");
		}

	flag=true;
	tagstr=NULL;
	while(flag==true)
		{
			tagstr=sliceInclude(newstr,(char*)"<apply_tag name=\"italic",(char*)"\">",true,true);
			if(tagstr==NULL)
				flag=false;
			else
				replaceAllSlice(&newstr,tagstr,(char*)"\\fI");
		}
	
	replaceAllSlice(&newstr,(char*)"</apply_tag>",(char*)"\\fR");
	for(int j=0;j<4;j++)
		replaceAllSlice(&newstr,(char*)tagfrom[j],(char*)tagto[j]);

	g_free(str);
	return(newstr);
}

char* loadToString(pageStruct* page)
{
	guint8*		data;
	GtkTextIter	start;
	GtkTextIter	end;
	gsize		length;
	char*		ptr=NULL;

	GdkAtom	atom=gtk_text_buffer_register_serialize_tagset((GtkTextBuffer*)page->buffer,NULL);

	gtk_text_buffer_get_bounds((GtkTextBuffer*)page->buffer,&start,&end);
	data=gtk_text_buffer_serialize((GtkTextBuffer*)page->buffer,(GtkTextBuffer*)page->buffer,atom,&start,&end,&length);
	ptr=sliceInclude((char*)&data[31],(char*)"<text>",(char*)"</text>",false,false);
	g_free(data);
	return(ptr);
}

void exportFile(GtkWidget* widget,gpointer data)
{
	pageStruct*	page;
	GtkTextIter	start,end;
	gchar*		text;
	char*		ptr;
	int			numpages=gtk_notebook_get_n_pages(notebook);
	char		startChar[2];
	char*		linePtr;
	char*		holdPtr;
	bool		lastWasNL=false;
	FILE*		fd=NULL;
	char*		xmldata=NULL;

	if(exportPath==NULL || data!=NULL)
		{
			if(getSaveFile()==false)
				return;
			exportPath=strdup(saveFilePath);
		}

	fd=fopen(exportPath,"w");
	if (fd!=NULL)
		{
			fprintf(fd,".TH \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"\n",manName,manSection,manVersion,manAuthor,manCategory);

			ptr=text;
			startChar[1]=0;

			for(int loop=0;loop<numpages;loop++)
				{
					page=getPageStructPtr(loop);
					gtk_text_buffer_get_start_iter((GtkTextBuffer*)page->buffer,&start);
					gtk_text_buffer_get_end_iter((GtkTextBuffer*)page->buffer,&end);
					gtk_text_buffer_place_cursor((GtkTextBuffer*)page->buffer,&start);
					xmldata=loadToString(page);

					ptr=doReplaceTags(xmldata);
					fprintf(fd,".SH \"%s\"\n",page->fileName);
					while(strlen(ptr)>0)
						{
							startChar[0]=ptr[0];
							if(strcmp(startChar,"\n")!=0)
								{
									linePtr=sliceInclude(ptr,(char*)&startChar[0],(char*)"\n",true,false);
									if(linePtr==NULL && strlen(ptr)>0)
										{
											fprintf(fd,"%s\n.br\n",ptr);
											g_free(ptr);
											ptr=(char*)"";
										}
									else
										{
											fprintf(fd,"%s\n.br\n",linePtr);
											g_free(linePtr);
											linePtr=sliceInclude(ptr,(char*)&startChar[0],(char*)"\n",true,true);
											holdPtr=deleteSlice(ptr,linePtr);	
											g_free(linePtr);
											g_free(ptr);
											ptr=holdPtr;
											lastWasNL=false;
										}
								}
							else
								{
									holdPtr=deleteSlice(ptr,(char*)"\n");
									g_free(ptr);
									ptr=holdPtr;

									if(lastWasNL==false)
										fprintf(fd,"\n");
									lastWasNL=true;
								}
						}
				}
			fclose(fd);
		}
}

void saveConverted(pageStruct*	page)
{
	FILE		*output;
	guint8		*data;
	gsize		length;
	GtkTextIter	start;
	GtkTextIter	end;

	GdkAtom	atom=gtk_text_buffer_register_serialize_tagset((GtkTextBuffer*)page->buffer,NULL);
	gtk_text_buffer_get_bounds((GtkTextBuffer*)page->buffer,&start,&end);
	data=gtk_text_buffer_serialize((GtkTextBuffer*)page->buffer,(GtkTextBuffer*)page->buffer,atom,&start,&end,&length);

	output=fopen(page->filePath,"wb");
	fwrite(data,sizeof(guint8),length,output);
	fclose(output);
}

void saveManpage(GtkWidget* widget,gpointer data)
{
	int			numpages=gtk_notebook_get_n_pages(notebook);
	pageStruct*	page;
	FILE*		fd=NULL;
	char*		manifest;


	if(manFilePath==NULL)
		{
			if(getSaveFile()==false)
				return;
			if(g_str_has_suffix(saveFilePath,".mpz"))
				manFilePath=strdup(saveFilePath);
			else
				asprintf(&manFilePath,"%s.mpz",saveFilePath);
		}

	for(int loop=0;loop<numpages;loop++)
		{
			page=getPageStructPtr(loop);
			page->itsMe=true;
			saveConverted(page);
		}

	asprintf(&manifest,"%s/manifest",manFilename);
	fd=fopen(manifest,"w");
	if (fd!=NULL)
		{
			fprintf(fd,"manname %s\n",manName);
			fprintf(fd,"mansection %s\n",manSection);
			fprintf(fd,"manversion %s\n",manVersion);
			fprintf(fd,"manauthor %s\n",manAuthor);
			fprintf(fd,"mancategory %s\n",manCategory);
			for(int loop=0;loop<numpages;loop++)
				{
					page=getPageStructPtr(loop);
					fprintf(fd,"file %s\n",page->fileName);
				}
			fclose(fd);
		}
	g_free(manifest);

	asprintf(&manifest,"tar -cC %s -f %s .",manFilename,manFilePath);
	system(manifest);
	g_free(manifest);
	dirty=false;
	setSensitive();
}

void saveAs(GtkWidget* widget,gpointer data)
{
	if(manFilePath!=NULL)
		{
			g_free(manFilePath);
			manFilePath=NULL;
		}
	saveManpage(NULL,NULL);
}

pageStruct* makeNewPage(void)
{
	pageStruct*		page;

	page=(pageStruct*)malloc(sizeof(pageStruct));

	page->pane=gtk_vpaned_new();
	page->pageWindow=(GtkScrolledWindow*)gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(page->pageWindow),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	page->pageWindow2=(GtkScrolledWindow*)gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(page->pageWindow2),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

	page->buffer=gtk_source_buffer_new(NULL);
	page->view=(GtkSourceView*)gtk_source_view_new_with_buffer(page->buffer);

	g_signal_connect(G_OBJECT(page->view),"populate-popup",G_CALLBACK(populatePopupMenu),NULL);
	page->view2=(GtkSourceView*)gtk_source_view_new_with_buffer(page->buffer);

	setFilePrefs(page->view);

	gtk_paned_add1(GTK_PANED(page->pane),(GtkWidget*)page->pageWindow);
	gtk_container_add (GTK_CONTAINER(page->pageWindow),(GtkWidget*)page->view);
	g_signal_connect(G_OBJECT(page->view),"button-release-event",G_CALLBACK(whatPane),(void*)1);

	page->isFirst=true;
	page->itsMe=false;
	page->inTop=true;
	page->isSplit=false;
	page->lang=NULL;
	page->tabVbox=NULL;
//dnd
	gtk_drag_dest_set((GtkWidget*)page->view,GTK_DEST_DEFAULT_ALL,NULL,0,GDK_ACTION_COPY);
	gtk_drag_dest_add_uri_targets((GtkWidget*)page->view);
	gtk_drag_dest_add_text_targets((GtkWidget*)page->view);
	g_signal_connect_after(G_OBJECT(page->view),"drag-data-received",G_CALLBACK(dropText),(void*)page);

	gtk_text_buffer_set_modified(GTK_TEXT_BUFFER(page->buffer),false);
	g_signal_connect(G_OBJECT(page->buffer),"modified-changed",G_CALLBACK(makeDirty),NULL);
	gtk_widget_grab_focus((GtkWidget*)page->view);

	return(page);
}

void newManpage(GtkWidget* widget,gpointer data)
{
	GtkWidget*	dialog;
	gint		result;
	GtkWidget*	content_area;
	GtkWidget*	label;
	GtkWidget*	hbox;

	if(pageOpen==true)
		closePage(NULL,NULL);

	dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_OTHER,GTK_BUTTONS_NONE,"Create New Manpage");

	gtk_dialog_add_buttons((GtkDialog*)dialog,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OK,GTK_RESPONSE_YES,NULL);
	gtk_window_set_title(GTK_WINDOW(dialog),"Details");

	content_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	
	hbox=gtk_hbox_new(false,0);
		nameBox=gtk_entry_new();
		label=gtk_label_new("Name\t");
		gtk_box_pack_start(GTK_BOX(hbox),label,true,true,0);
		gtk_box_pack_start(GTK_BOX(hbox),nameBox,true,true,0);		
	gtk_container_add(GTK_CONTAINER(content_area),hbox);

	hbox=gtk_hbox_new(false,0);
		sectionBox=gtk_entry_new();
		label=gtk_label_new("Section\t");
		gtk_box_pack_start(GTK_BOX(hbox),label,true,true,0);
		gtk_box_pack_start(GTK_BOX(hbox),sectionBox,true,true,0);		
	gtk_container_add(GTK_CONTAINER(content_area),hbox);

	hbox=gtk_hbox_new(false,0);
		versionBox=gtk_entry_new();
		label=gtk_label_new("Version\t");
		gtk_box_pack_start(GTK_BOX(hbox),label,true,true,0);
		gtk_box_pack_start(GTK_BOX(hbox),versionBox,true,true,0);		
	gtk_container_add(GTK_CONTAINER(content_area),hbox);

	hbox=gtk_hbox_new(false,0);
		authorBox=gtk_entry_new();
		label=gtk_label_new("Author\t");
		gtk_box_pack_start(GTK_BOX(hbox),label,true,true,0);
		gtk_box_pack_start(GTK_BOX(hbox),authorBox,true,true,0);		
	gtk_container_add(GTK_CONTAINER(content_area),hbox);

	hbox=gtk_hbox_new(false,0);
		categoryBox=gtk_entry_new();
		label=gtk_label_new("Category\t");
		gtk_box_pack_start(GTK_BOX(hbox),label,true,true,0);
		gtk_box_pack_start(GTK_BOX(hbox),categoryBox,true,true,0);		
	gtk_container_add(GTK_CONTAINER(content_area),hbox);


	gtk_widget_show_all(content_area);
	result=gtk_dialog_run(GTK_DIALOG(dialog));

	if(result==GTK_RESPONSE_YES)
		{
			manFilename=tempnam(NULL,"ManEd");
			manName=strdup(gtk_entry_get_text((GtkEntry*)nameBox));
			manSection=strdup(gtk_entry_get_text((GtkEntry*)sectionBox));
			manVersion=strdup(gtk_entry_get_text((GtkEntry*)versionBox));
			manAuthor=strdup(gtk_entry_get_text((GtkEntry*)authorBox));
			manCategory=strdup(gtk_entry_get_text((GtkEntry*)categoryBox));
			pageOpen=true;
			g_mkdir_with_parents(manFilename,493);
		}

	gtk_widget_destroy(dialog);
}

char* getNewSectionName(void)
{
	GtkWidget*	dialog;
	gint		result;
	GtkWidget*	content_area;
	GtkWidget*	entrybox;
	char*		retval=NULL;

	dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_OTHER,GTK_BUTTONS_NONE,"Enter Section Name");

	gtk_dialog_add_buttons((GtkDialog*)dialog,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OK,GTK_RESPONSE_YES,NULL);
	gtk_window_set_title(GTK_WINDOW(dialog),"Section");

	content_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));	
	entrybox=gtk_entry_new();
	gtk_entry_set_activates_default((GtkEntry*)entrybox,true);
	gtk_dialog_set_default_response((GtkDialog*)dialog,GTK_RESPONSE_YES);
	gtk_container_add(GTK_CONTAINER(content_area),entrybox);
	gtk_widget_show_all(content_area);
	result=gtk_dialog_run(GTK_DIALOG(dialog));

	if(result==GTK_RESPONSE_YES)
		retval=strdup(gtk_entry_get_text((GtkEntry*)entrybox));

	gtk_widget_destroy(dialog);

	return(retval);
}

void newSection(GtkWidget* widget,gpointer data)
{
	GtkTextIter	iter;
	GtkWidget*	label;
	pageStruct*	page;
	char*		retval=NULL;
	GString*	str;

	retval=getNewSectionName();
	if(retval!=NULL)
		{
			page=makeNewPage();
			page->tabVbox=gtk_vbox_new(true,4);
			str=g_string_new(retval);
			g_string_ascii_up(str);

			page->fileName=g_string_free(str,false);
			asprintf(&page->filePath,"%s/%s",manFilename,page->fileName);

			label=makeNewTab(page->fileName,NULL,page);

/* move cursor to the beginning */
			gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(page->buffer),&iter);
			gtk_text_buffer_place_cursor(GTK_TEXT_BUFFER(page->buffer),&iter);

//connect to ntebook
			gtk_container_add(GTK_CONTAINER(page->tabVbox),GTK_WIDGET(page->pane));
			g_object_set_data(G_OBJECT(page->tabVbox),"pagedata",(gpointer)page);

			gtk_notebook_append_page(notebook,page->tabVbox,label);
			gtk_notebook_set_tab_reorderable(notebook,page->tabVbox,true);
			gtk_notebook_set_current_page(notebook,currentPage);
			currentPage++;
			gtk_widget_show_all((GtkWidget*)notebook);
		}
}

void loadBuffer(pageStruct* page)
{
	GtkTextIter	iter;
	gchar*		buffer=NULL;
	long		filelen;
	GdkAtom		atom=gtk_text_buffer_register_deserialize_tagset((GtkTextBuffer*)page->buffer,NULL);

	g_file_get_contents(page->filePath,&buffer,(gsize*)&filelen,NULL);
	gtk_text_buffer_get_start_iter((GtkTextBuffer*)page->buffer,&iter);
	gtk_text_buffer_deserialize_set_can_create_tags((GtkTextBuffer*)page->buffer,atom,true);
	gtk_text_buffer_deserialize((GtkTextBuffer*)page->buffer,(GtkTextBuffer*)page->buffer,atom,&iter,(const guint8*)buffer,filelen,NULL);
}

void openConvertedFile(char* filepath)
{
	GtkWidget*	label;
	gchar*		filename=g_path_get_basename(filepath);
	pageStruct*	page;
	char*		str=NULL;

	if(!g_file_test(filepath,G_FILE_TEST_EXISTS))
		return;

	page=makeNewPage();
	page->tabVbox=gtk_vbox_new(true,4);

	page->filePath=strdup(filepath);
	page->fileName=strdup(filename);

	label=makeNewTab(page->fileName,page->filePath,page);
	gtk_source_buffer_set_highlight_syntax(page->buffer,false);
	gtk_source_buffer_set_highlight_matching_brackets(page->buffer,false);
	gtk_source_buffer_begin_not_undoable_action(page->buffer);
		loadBuffer(page);
	gtk_source_buffer_end_not_undoable_action(page->buffer);

	g_free(filename);
	g_free(str);

//connect to ntebook
	gtk_container_add(GTK_CONTAINER(page->tabVbox),GTK_WIDGET(page->pane));
	g_object_set_data(G_OBJECT(page->tabVbox),"pagedata",(gpointer)page);

	gtk_notebook_append_page(notebook,page->tabVbox,label);
	gtk_notebook_set_tab_reorderable(notebook,page->tabVbox,true);
	gtk_notebook_set_current_page(notebook,currentPage);
	currentPage++;
	gtk_widget_grab_focus((GtkWidget*)page->view);

	gtk_text_buffer_set_modified(GTK_TEXT_BUFFER(page->buffer),false);

	gtk_widget_show_all((GtkWidget*)notebook);


}

void doOpenManpage(char* file)
{
	char*		command=NULL;
	FILE*		fp;
	char		buffer[4096];
	char		name[256];
	char		strarg[256];
	char*		recenturi;
	char*		dirname;
	char*		lowername;

	if(manFilename!=NULL)
		{
			sprintf((char*)&buffer[0],"rm -r \"%s\"",manFilename);
			system((char*)&buffer[0]);
			g_free(manFilename);
		}

	manFilename=tempnam(NULL,"ManEd");
	g_mkdir_with_parents(manFilename,493);
	asprintf(&command,"tar -xC %s -f %s",manFilename,file);
	system(command);
	g_free(command);
	asprintf(&command,"%s/manifest",manFilename);
	fp=fopen(command, "r");
	while(fgets(buffer,4096,fp))
		{
			strarg[0]=0;
			sscanf(buffer,"%s %"VALIDFILENAMECHARS"s",(char*)&name,(char*)&strarg);
			if(strcasecmp(name,"manname")==0)
				manName=strdup((char*)&strarg);
			if(strcasecmp(name,"mansection")==0)
				manSection=strdup((char*)&strarg);
			if(strcasecmp(name,"manversion")==0)
				manVersion=strdup((char*)&strarg);
			if(strcasecmp(name,"manauthor")==0)
				manAuthor=strdup((char*)&strarg);
			if(strcasecmp(name,"mancategory")==0)
				manCategory=strdup((char*)&strarg);
			if(strcasecmp(name,"file")==0)
				{
					sprintf((char*)&buffer[0],"%s/%s",manFilename,(char*)&strarg[0]);
					openConvertedFile((char*)&buffer);
				}
		}

	g_free(command);
	fclose(fp);
	if(manFilePath!=NULL)
		g_free(manFilePath);
	manFilePath=strdup(file);
	if(exportPath!=NULL)
		g_free(exportPath);

	dirname=g_path_get_dirname(manFilePath);
	lowername=g_ascii_strdown(manName,-1);
	asprintf(&exportPath,"%s/%s.%s",dirname,lowername,manSection);
	g_free(dirname);
	g_free(lowername);
	recenturi=g_filename_to_uri(manFilePath,NULL,NULL);
	gtk_recent_manager_add_item(gtk_recent_manager_get_default(),recenturi);
	pageOpen=true;
}

void openManpage(GtkWidget* widget,gpointer data)
{
	GtkWidget*	dialog;
	char*		filename;

	if(dirty==true)
		saveManpage(NULL,NULL);

	if(manFilename!=NULL)
		closeAllTabs(NULL,NULL);

	dialog=gtk_file_chooser_dialog_new("Open File",NULL,GTK_FILE_CHOOSER_ACTION_OPEN,GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,NULL);

	if (gtk_dialog_run(GTK_DIALOG (dialog))==GTK_RESPONSE_ACCEPT)
		{
			filename=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			doOpenManpage(filename);
			g_free(filename);
		}
	gtk_widget_destroy (dialog);
	dirty=false;
	setSensitive();

	refreshMainWindow();	
}

void deleteSection(GtkWidget* widget,gpointer data)
{
	pageStruct*	page=getPageStructPtr(-1);

	if(yesNo((char*)"Do you want to permanently delete",page->fileName)==GTK_RESPONSE_YES)
		{
			unlink(page->filePath);
			closeTab(NULL,(void*)1);
			makeDirty(NULL,NULL);
		}
}

void renameSection(GtkWidget* widget,gpointer data)
{
	pageStruct*	page=(pageStruct*)data;
	char*		retval=NULL;
	char*		command=NULL;
	GString*	str;

	retval=getNewSectionName();
	if(retval!=NULL)
		{
			str=g_string_new(retval);
			g_string_ascii_up(str);


			asprintf(&command,"mv \"%s\" \"%s/%s\"",page->filePath,manFilename,str->str);
			g_free(page->fileName);
			g_free(page->filePath);
			page->fileName=g_string_free(str,false);
			asprintf(&page->filePath,"%s/%s",manFilename,page->fileName);
			system(command);
			gtk_notebook_set_tab_label_text(notebook,page->tabVbox,page->fileName);
			makeDirty(NULL,NULL);
			g_free(retval);
		}
}











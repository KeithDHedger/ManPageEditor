/*
 *
 * K.D.Hedger 2013 <kdhedger68713@gmail.com>
 *
 * callbacks.cpp
 *
*/

#include <gtk/gtk.h>
#include <gtksourceview/gtksourceiter.h>
#include <gtksourceview/gtksourceprintcompositor.h>

#include "globals.h"
#include "files.h"
#include "guis.h"
#include "searchcallbacks.h"
#include "spellcheck.h"
#include "script.h"

GtkWidget*			tabMenu;
GtkPrintSettings*	settings=NULL;
int					boldnum=0;
int					italicnum=0;

void showHideWidget(GtkWidget* widget,bool show)
{
	if(show==false)
		gtk_widget_hide(widget);
	else
		gtk_widget_show(widget);
}

void refreshMainWindow(void)
{
	gtk_widget_show_all(window);
	showHideWidget(liveSearchWidget,showLiveSearch);
}

int yesNo(char* question,char* file)
{
	GtkWidget*	dialog;
	int			result;

	dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_NONE,"%s %s",question,file);

	gtk_dialog_add_buttons((GtkDialog*)dialog,GTK_STOCK_YES,GTK_RESPONSE_YES,GTK_STOCK_NO,GTK_RESPONSE_CANCEL,NULL);
	gtk_window_set_title(GTK_WINDOW(dialog),"What Do You Want To Do?");

	result=gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);

	return(result);
}

int show_question(char* filename)
{
	GtkWidget*	dialog;
	gint		result;
	char*		message;

	asprintf(&message,"Save file %s before closing?",filename);
	dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_NONE,message);

	gtk_dialog_add_buttons((GtkDialog*)dialog,GTK_STOCK_SAVE,GTK_RESPONSE_YES,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_NO,GTK_RESPONSE_NO,NULL);
	gtk_window_set_title(GTK_WINDOW(dialog),"Warning unsaved data!");

	result=gtk_dialog_run(GTK_DIALOG(dialog));

	gtk_widget_destroy(dialog);
	g_free(message);
	return(result);
}

void setSensitive(void)
{
//toolbar
	gtk_widget_set_sensitive((GtkWidget*)undoButton,dirty);
	gtk_widget_set_sensitive((GtkWidget*)redoButton,dirty);
	gtk_widget_set_sensitive((GtkWidget*)saveButton,dirty);
//menu
	gtk_widget_set_sensitive((GtkWidget*)undoMenu,dirty);
	gtk_widget_set_sensitive((GtkWidget*)redoMenu,dirty);
	gtk_widget_set_sensitive((GtkWidget*)saveMenu,dirty);
	gtk_widget_set_sensitive((GtkWidget*)saveAsMenu,pageOpen);
	gtk_widget_set_sensitive((GtkWidget*)exportMenu,pageOpen);
}

void closeTab(GtkWidget* widget,gpointer data)
{
	long		thispage;
	pageStruct*	page;

	if(data==NULL)
		thispage=0;
	else
		thispage=gtk_notebook_get_current_page(notebook);

	page=getPageStructPtr(thispage);
	if(page==NULL)
		return;

	if(page->filePath!=NULL)
		g_free(page->filePath);
	if(page->fileName!=NULL)
		g_free(page->fileName);
	if(page!=NULL)
		g_free(page);
	currentPage--;
	gtk_notebook_remove_page(notebook,thispage);
	setSensitive();
}

void closeAllTabs(GtkWidget* widget,gpointer data)
{
	int	numtabs=gtk_notebook_get_n_pages(notebook);

	for(int loop=0;loop<numtabs;loop++)
		closeTab(NULL,0);
}

void closePage(GtkWidget* widget,gpointer data)
{
	int		retval;
	char*	command;

	if(dirty==true)
		{
			retval=yesNo((char*)"Do you want to save?",manName);
			if(retval==GTK_RESPONSE_YES)
				saveManpage(NULL,NULL);
		}

	if(manFilename!=NULL)
		{
			closeAllTabs(NULL,NULL);
			asprintf(&command,"rm -r \"%s\"",manFilename);
			system(command);
			g_free(command);
			g_free(manFilename);
		}

	if(manName!=NULL)
		g_free(manName);
	if(manSection!=NULL)
		g_free(manSection);
	if(manVersion!=NULL)
		g_free(manVersion);
	if(manAuthor!=NULL)
		g_free(manAuthor);
	if(manFilePath!=NULL)
		g_free(manFilePath);
	if(exportPath!=NULL)
		g_free(exportPath);

	manName=NULL;
	manSection=NULL;
	manVersion=NULL;
	manAuthor=NULL;
	manFilePath=NULL;
	exportPath=NULL;
	manFilename=NULL;
	pageOpen=false;
	dirty=false;
	setSensitive();
}

void copyToClip(GtkWidget* widget,gpointer data)
{
	pageStruct*	page=getPageStructPtr(-1);
	gtk_text_buffer_copy_clipboard((GtkTextBuffer*)page->buffer,gtk_clipboard_get(GDK_SELECTION_CLIPBOARD));
}

void cutToClip(GtkWidget* widget,gpointer data)
{
	pageStruct*	page=getPageStructPtr(-1);
	gtk_text_buffer_cut_clipboard((GtkTextBuffer*)page->buffer,gtk_clipboard_get(GDK_SELECTION_CLIPBOARD),true);
	setSensitive();
}

void pasteFromClip(GtkWidget* widget,gpointer data)
{
	pageStruct*	page=getPageStructPtr(-1);
	gtk_text_buffer_paste_clipboard((GtkTextBuffer*)page->buffer,gtk_clipboard_get(GDK_SELECTION_CLIPBOARD),NULL,true);
	setSensitive();
}

void undo(GtkWidget* widget,gpointer data)
{
	pageStruct*	page=getPageStructPtr(-1);

	if(gtk_source_buffer_can_undo(page->buffer));
		{
			gtk_source_buffer_undo(page->buffer);
			page->isFirst=true;
			setSensitive();
		}
}

void redo(GtkWidget* widget,gpointer data)
{
	pageStruct*	page=getPageStructPtr(-1);

	if(gtk_source_buffer_can_redo(page->buffer));
		{
			gtk_source_buffer_redo(page->buffer);
			page->isFirst=true;
			setSensitive();
		}
}

void openHelp(GtkWidget* widget,gpointer data)
{
	char*	runhelp;
	asprintf(&runhelp,"xdg-open %s/help/help.html",DATADIR);
	system(runhelp);
}

void copyToClipboard(GtkWidget* widget,gpointer data)
{
	GtkClipboard*	clipboard=gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	gtk_clipboard_set_text(clipboard,(char*)data,-1);
}

void populatePopupMenu(GtkTextView *entry,GtkMenu *menu,gpointer user_data)
{
	pageStruct*		page=getPageStructPtr(-1);
	GtkTextIter		start;
	GtkTextIter		end;
	char*			selection=NULL;
	GtkWidget*		menuitem;
#ifdef _ASPELL_
	GtkWidget*		image;
#endif

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_prepend(GTK_MENU_SHELL(menu),menuitem);

	if(gtk_text_buffer_get_selection_bounds((GtkTextBuffer*)page->buffer,&start,&end))
		{
			selection=gtk_text_buffer_get_text((GtkTextBuffer*)page->buffer,&start,&end,false);
			if(selection!=NULL)
				{
#ifdef _ASPELL_
//spell check
					menuitem=gtk_image_menu_item_new_with_label("Check Spellling");
					image=gtk_image_new_from_stock(GTK_STOCK_SPELL_CHECK,GTK_ICON_SIZE_MENU);
					gtk_image_menu_item_set_image((GtkImageMenuItem *)menuitem,image);
					gtk_menu_shell_prepend(GTK_MENU_SHELL(menu),menuitem);
					gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(checkWord),NULL);
#endif
				}
		}
	gtk_widget_show_all((GtkWidget*)menu);
}

gboolean whatPane(GtkWidget *widget,GdkEvent *event,gpointer data)
{
	pageStruct* page=(pageStruct*)getPageStructPtr(-1);

	if((long)data==1)
		page->inTop=true;
	else
		page->inTop=false;

	return(false);
}

void doSplitView(GtkWidget *widget,gpointer user_data)
{
	pageStruct* page=(pageStruct*)user_data;

	if(gtk_paned_get_child2((GtkPaned*)page->pane)==NULL)
		{
			page->pageWindow2=(GtkScrolledWindow*)gtk_scrolled_window_new(NULL, NULL);
			gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(page->pageWindow2),GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

			page->view2 = (GtkSourceView*)gtk_source_view_new_with_buffer (page->buffer);
			g_signal_connect(G_OBJECT(page->view2),"populate-popup",G_CALLBACK(populatePopupMenu),NULL);
			setFilePrefs((GtkSourceView*)page->view2);

			gtk_paned_add2(GTK_PANED(page->pane),(GtkWidget*)page->pageWindow2);
			gtk_container_add(GTK_CONTAINER((GtkWidget*)page->pageWindow2),(GtkWidget*)page->view2);
			g_signal_connect(G_OBJECT(page->view2),"button-release-event",G_CALLBACK(whatPane),(void*)2);
			page->isSplit=true;
		}
	else
		{
			page->isSplit=false;
			gtk_widget_destroy(gtk_paned_get_child2((GtkPaned*)page->pane));
		}

	gtk_widget_show_all(page->pane);
}

bool tabPopUp(GtkWidget *widget, GdkEventButton *event,gpointer user_data)
{
	pageStruct* page;
	GtkWidget*	menuitem;
	GtkWidget*	image;

	if(event->button==3 && event->type==GDK_BUTTON_PRESS)
	    {
			tabMenu=gtk_menu_new();
			page=(pageStruct*)user_data;

#ifdef _ASPELL_
//check document
			image=gtk_image_new_from_stock(GTK_STOCK_SPELL_CHECK,GTK_ICON_SIZE_MENU);
			menuitem=gtk_image_menu_item_new_with_label("Spell Check Document");
			gtk_image_menu_item_set_image((GtkImageMenuItem*)menuitem,image);
			gtk_menu_shell_append(GTK_MENU_SHELL(tabMenu),menuitem);
			gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(doSpellCheckDoc),(void*)page->filePath);
#endif

//paned view
			image=gtk_image_new_from_stock(GTK_STOCK_NEW,GTK_ICON_SIZE_MENU);
			if(page->isSplit==true)
				menuitem=gtk_image_menu_item_new_with_label("Un-Split View");
			else
				menuitem=gtk_image_menu_item_new_with_label("Split View");

			gtk_image_menu_item_set_image((GtkImageMenuItem*)menuitem,image);
			gtk_menu_shell_append(GTK_MENU_SHELL(tabMenu),menuitem);
			gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(doSplitView),(void*)page);

//rename
			image=gtk_image_new_from_stock(GTK_STOCK_FIND_AND_REPLACE,GTK_ICON_SIZE_MENU);
			menuitem=gtk_image_menu_item_new_with_label("Rename Section");
			gtk_image_menu_item_set_image((GtkImageMenuItem*)menuitem,image);
			gtk_menu_shell_append(GTK_MENU_SHELL(tabMenu),menuitem);
			gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(renameSection),(void*)page);

			gtk_menu_attach_to_widget(GTK_MENU(tabMenu),widget,NULL);
			gtk_menu_popup(GTK_MENU(tabMenu),NULL,NULL,NULL,NULL,event->button,event->time);
			gtk_widget_show_all((GtkWidget*)tabMenu);


			return(true);
		}
	else
		return(false);
}

void writeExitData(void)
{
	GtkAllocation	alloc;
	FILE*			fd=NULL;
	char*			filename;
	int				winx;
	int				winy;

	gtk_widget_get_allocation(window,&alloc);
	gtk_window_get_position((GtkWindow*)window,&winx,&winy);

	asprintf(&filename,"%s/.ManPageEditor",getenv("HOME"));
	g_mkdir_with_parents(filename,493);
	g_free(filename);
	asprintf(&filename,"%s/.ManPageEditor/editor.window.rc",getenv("HOME"));
	fd=fopen(filename,"w");
	if(fd!=NULL)
		{
			fprintf(fd,"windowsize	%i %i %i %i\n",alloc.width,alloc.height,winx,winy);
			fclose(fd);
		}
	g_free(filename);
}

void writeConfig(void)
{
	GtkAllocation	alloc;
	FILE*			fd=NULL;
	char*			filename;
	int				winx;
	int				winy;

	gtk_widget_get_allocation(window,&alloc);
	gtk_window_get_position((GtkWindow*)window,&winx,&winy);

	asprintf(&filename,"%s/.ManPageEditor",getenv("HOME"));
	g_mkdir_with_parents(filename,493);
	g_free(filename);
	asprintf(&filename,"%s/.ManPageEditor/editor.rc",getenv("HOME"));
	fd=fopen(filename,"w");
	if(fd!=NULL)
		{
			fprintf(fd,"wrapline	%i\n",(int)lineWrap);
			fprintf(fd,"highlightcurrentline	%i\n",(int)highLight);
			fprintf(fd,"insenssearch	%i\n",(int)insensitiveSearch);
			fprintf(fd,"wrapsearch	%i\n",(int)wrapSearch);
			fprintf(fd,"showlivesearch	%i\n",(int)showLiveSearch);
			fprintf(fd,"tabwidth	%i\n",tabWidth);
			fprintf(fd,"font	%s\n",fontAndSize);
			fclose(fd);
		}
	g_free(filename);
}

void doShutdown(GtkWidget* widget,gpointer data)
{
	char*	command;
	int		result;

	if(dirty==true)
		{
			result=show_question(g_path_get_basename(manFilePath));
			switch(result)
				{
					case GTK_RESPONSE_YES:
						saveManpage(NULL,NULL);
						break;
					case GTK_RESPONSE_NO:
						break;
					default:
						return;
						break;
				}
		}
	if(manFilename!=NULL)
		{
			asprintf(&command,"rm -r \"%s\"",manFilename);
			system(command);
			g_free(command);
			g_free(manFilename);
		}

#ifdef _ASPELL_
	delete_aspell_config(aspellConfig);
	delete_aspell_speller(spellChecker);
#endif

	writeExitData();
	gtk_main_quit();
}

void setPrefs(GtkWidget* widget,gpointer data)
{
	if(strcmp(gtk_widget_get_name(widget),"wrap")==0)
		tmpLineWrap=gtk_toggle_button_get_active((GtkToggleButton*)data);
	if(strcmp(gtk_widget_get_name(widget),"high")==0)
		tmpHighLight=gtk_toggle_button_get_active((GtkToggleButton*)data);

	if(strcmp(gtk_widget_get_name(widget),"livesearch")==0)
		tmpShowLiveSearch=gtk_toggle_button_get_active((GtkToggleButton*)data);

	if(strcmp(gtk_widget_get_name(widget),"tabs")==0)
		tmpTabWidth=(int)gtk_spin_button_get_value((GtkSpinButton*)data);

	if(strcmp(gtk_widget_get_name(widget),"cancel")==0)
		gtk_widget_destroy(prefswin);

	if(strcmp(gtk_widget_get_name(widget),"apply")==0)
		{
			lineWrap=tmpLineWrap;
			highLight=tmpHighLight;
			showLiveSearch=tmpShowLiveSearch;
			showHideWidget(liveSearchWidget,showLiveSearch);

			if(fontAndSize!=NULL)
				{
					g_free(fontAndSize);
					fontAndSize=strdup(gtk_entry_get_text((GtkEntry*)fontBox));
				}

			tabWidth=tmpTabWidth;
			gtk_widget_destroy(prefswin);
			resetAllFilePrefs();
			writeConfig();
		}
}

void doAbout(GtkWidget* widget,gpointer data)
{
	const char*	authors[]={"K.D.Hedger <"MYEMAIL">\n",MYWEBSITE,"\nMore by the same author\n","Xfce-Theme-Manager\nhttp://xfce-look.org/content/show.php?content=149647\n","Xfce4-Composite-Editor\nhttp://gtk-apps.org/content/show.php/Xfce4-Composite-Editor?content=149523\n","KKEdit\nhttp://gtk-apps.org/content/show.php?content=158161\n","Manpage Editor\nhttp://gtk-apps.org/content/show.php?content=160219\n","GtkSu\nhttp://gtk-apps.org/content/show.php?content=158974",NULL};
	const char	copyright[] ="Copyright \xc2\xa9 2013 K.D.Hedger";
	const char*	aboutboxstring="ManPageEditor - Editor for linux manpages";
	char*		licence;

	g_file_get_contents(DATADIR"/docs/gpl-3.0.txt",&licence,NULL,NULL);

	gtk_show_about_dialog(NULL,"authors",authors,"comments",aboutboxstring,"copyright",copyright,"version",VERSION,"website",MYWEBSITE,"program-name","ManPageEditor","logo-icon-name","ManPageEditor","license",licence,NULL);

	g_free(licence);
}

void drawPage(GtkPrintOperation *operation,GtkPrintContext *context,gint page_nr,gpointer user_data)
{
	GtkSourcePrintCompositor *compositor;

	compositor=GTK_SOURCE_PRINT_COMPOSITOR(user_data);
	gtk_source_print_compositor_draw_page(compositor,context,page_nr);
}

void beginPrint(GtkPrintOperation *operation,GtkPrintContext *context,gpointer user_data)
{
	GtkSourcePrintCompositor*	compositor;
	gint						n_pages;

    compositor=GTK_SOURCE_PRINT_COMPOSITOR (user_data);

    while(!gtk_source_print_compositor_paginate(compositor,context));

    n_pages=gtk_source_print_compositor_get_n_pages(compositor);
    gtk_print_operation_set_n_pages(operation,n_pages);
}

void printFile(GtkWidget* widget,gpointer data)
{
	pageStruct*					page=getPageStructPtr(-1);
	GtkSourcePrintCompositor*	printview=gtk_source_print_compositor_new_from_view(page->view);
	GtkPrintOperation*			print;
	GtkPrintOperationResult		result;

	print=gtk_print_operation_new ();
	if (settings != NULL)
		gtk_print_operation_set_print_settings(print,settings);
	g_signal_connect(print,"begin-print",G_CALLBACK(beginPrint),(void*)printview);
	g_signal_connect(print,"draw-page",G_CALLBACK (drawPage),(void*)printview);
	result=gtk_print_operation_run(print,GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG,GTK_WINDOW(window),NULL);
	if (result==GTK_PRINT_OPERATION_RESULT_APPLY)
		{
			if (settings != NULL)
				g_object_unref(settings);
			settings=(GtkPrintSettings*)g_object_ref((gpointer)gtk_print_operation_get_print_settings(print));
		}
	g_object_unref(print);
	g_object_unref(printview);
}

void newEditor(GtkWidget* widget,gpointer data)
{
	system("manpageeditor");
}

void doFormat(GtkWidget* widget,gpointer data)
{
	pageStruct*			page=getPageStructPtr(-1);
	GtkTextMark*		mark;
	GtkTextIter			iter;
	GtkTextIter			start;
	GtkTextIter			end;
	GtkTextTag*			tag=NULL;
	char				tagname[64];
	GtkTextTagTable*	tagtable=gtk_text_buffer_get_tag_table((GtkTextBuffer*)page->buffer);

	mark=gtk_text_buffer_get_insert((GtkTextBuffer*)page->buffer);
	gtk_text_buffer_get_iter_at_mark((GtkTextBuffer*)page->buffer,&iter,mark);

	switch((long)data)
		{
			case BOLD:
					boldnum++;
					sprintf((char*)&tagname,"bold-%i",boldnum);
					tag=gtk_text_tag_table_lookup(tagtable,tagname);
					while(tag!=NULL)
						{
							boldnum++;
							sprintf((char*)&tagname,"bold-%i",boldnum);
							tag=gtk_text_tag_table_lookup(tagtable,tagname);
						}
					tag=gtk_text_buffer_create_tag((GtkTextBuffer*)page->buffer,tagname,"weight",PANGO_WEIGHT_BOLD,NULL);
					gtk_text_buffer_get_selection_bounds((GtkTextBuffer*)page->buffer,&start,&end);
					gtk_text_buffer_apply_tag((GtkTextBuffer*)page->buffer,tag,&start,&end);
				break;

			case NORMAL:
					gtk_text_buffer_get_selection_bounds((GtkTextBuffer*)page->buffer,&start,&end);
					gtk_text_buffer_remove_all_tags((GtkTextBuffer*)page->buffer,&start,&end);
				break;

			case ITALIC:
					italicnum++;
					sprintf((char*)&tagname,"italic-%i",italicnum);
					tag=gtk_text_tag_table_lookup(tagtable,tagname);
					while(tag!=NULL)
						{
							italicnum++;
							sprintf((char*)&tagname,"bold-%i",italicnum);
							tag=gtk_text_tag_table_lookup(tagtable,tagname);
						}
					tag=gtk_text_buffer_create_tag((GtkTextBuffer*)page->buffer,tagname,"style",PANGO_STYLE_ITALIC,NULL);
					gtk_text_buffer_get_selection_bounds((GtkTextBuffer*)page->buffer,&start,&end);
					gtk_text_buffer_apply_tag((GtkTextBuffer*)page->buffer,tag,&start,&end);
				break;
		}

	dirty=true;
	setSensitive();
	refreshMainWindow();
}

void redoProps(GtkWidget* widget,gpointer data)
{
	GtkWidget*	dialog;
	gint		result;
	GtkWidget*	content_area;
	GtkWidget*	label;
	GtkWidget*	hbox;

	dialog=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_OTHER,GTK_BUTTONS_NONE,"Set Properties");

	gtk_dialog_add_buttons((GtkDialog*)dialog,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OK,GTK_RESPONSE_YES,NULL);
	gtk_window_set_title(GTK_WINDOW(dialog),"Properties");

	content_area=gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	
	hbox=gtk_hbox_new(false,0);
		nameBox=gtk_entry_new();
		label=gtk_label_new("Name\t");
		gtk_box_pack_start(GTK_BOX(hbox),label,true,true,0);
		gtk_box_pack_start(GTK_BOX(hbox),nameBox,true,true,0);	
		gtk_entry_set_text((GtkEntry*)nameBox,manName);
	gtk_container_add(GTK_CONTAINER(content_area),hbox);

	hbox=gtk_hbox_new(false,0);
		sectionBox=gtk_entry_new();
		label=gtk_label_new("Section\t");
		gtk_box_pack_start(GTK_BOX(hbox),label,true,true,0);
		gtk_box_pack_start(GTK_BOX(hbox),sectionBox,true,true,0);		
		gtk_entry_set_text((GtkEntry*)sectionBox,manSection);
	gtk_container_add(GTK_CONTAINER(content_area),hbox);

	hbox=gtk_hbox_new(false,0);
		versionBox=gtk_entry_new();
		label=gtk_label_new("Version\t");
		gtk_box_pack_start(GTK_BOX(hbox),label,true,true,0);
		gtk_box_pack_start(GTK_BOX(hbox),versionBox,true,true,0);		
		gtk_entry_set_text((GtkEntry*)versionBox,manVersion);
	gtk_container_add(GTK_CONTAINER(content_area),hbox);

	hbox=gtk_hbox_new(false,0);
		authorBox=gtk_entry_new();
		label=gtk_label_new("Author\t");
		gtk_box_pack_start(GTK_BOX(hbox),label,true,true,0);
		gtk_box_pack_start(GTK_BOX(hbox),authorBox,true,true,0);		
		gtk_entry_set_text((GtkEntry*)authorBox,manAuthor);
	gtk_container_add(GTK_CONTAINER(content_area),hbox);

	hbox=gtk_hbox_new(false,0);
		categoryBox=gtk_entry_new();
		label=gtk_label_new("Category\t");
		gtk_box_pack_start(GTK_BOX(hbox),label,true,true,0);
		gtk_box_pack_start(GTK_BOX(hbox),categoryBox,true,true,0);		
		gtk_entry_set_text((GtkEntry*)categoryBox,manCategory);
	gtk_container_add(GTK_CONTAINER(content_area),hbox);


	gtk_widget_show_all(content_area);
	result=gtk_dialog_run(GTK_DIALOG(dialog));

	if(result==GTK_RESPONSE_YES)
		{
			manName=strdup(gtk_entry_get_text((GtkEntry*)nameBox));
			manSection=strdup(gtk_entry_get_text((GtkEntry*)sectionBox));
			manVersion=strdup(gtk_entry_get_text((GtkEntry*)versionBox));
			manAuthor=strdup(gtk_entry_get_text((GtkEntry*)authorBox));
			manCategory=strdup(gtk_entry_get_text((GtkEntry*)categoryBox));
			dirty=true;
			setSensitive();
		}
	gtk_widget_destroy(dialog);
}

/*
 *
 * K.D.Hedger 2013 <kdhedger68713@gmail.com>
 *
 * globals.cpp
 *
*/

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <sys/stat.h>
#include <gtksourceview/gtksourcebuffer.h>
#include <gtksourceview/gtksourcelanguage.h>
#include <gtksourceview/gtksourcelanguagemanager.h>

#include "config.h"
#ifdef BUILDDOCVIEWER
#include <webkit/webkit.h>
#endif

#ifdef _ASPELL_
#include <aspell.h>
#endif

#include "globals.h"
#include "searchcallbacks.h"
#include "callbacks.h"
#include "script.h"


//manpage
char*			manFilename=NULL;
char*			manName=NULL;
char*			manSection=NULL;
char*			manVersion=NULL;
char*			manAuthor=NULL;
char*			manCategory=NULL;
char*			manFilePath=NULL;
bool			pageOpen=false;
bool			dirty=false;
char*			exportPath=NULL;

GtkWidget*		nameBox;
GtkWidget*		sectionBox;
GtkWidget*		versionBox;
GtkWidget*		authorBox;
GtkWidget*		categoryBox;

GtkWidget*		window=NULL;
GtkNotebook*	notebook=NULL;
GtkWidget*		menubar=NULL;
GtkWidget*		menufile;
GtkWidget*		menuedit;



GtkWidget*		menuhelp;
GtkWidget*		menumanpage;



GtkWidget*		menuprint;
GtkWidget*		menuclose;
GtkWidget*		menucloseall;
GtkWidget*		menusaveall;
GtkWidget*		menurevert;

GtkWidget*		redoMenu;
GtkWidget*		undoMenu;
GtkWidget*		saveMenu;
GtkWidget*		exportMenu;

GtkWidget*		saveAsMenu;

GtkWidget*		lineNumberWidget;
GtkWidget*		liveSearchWidget;

int				currentPage=0;

GtkWidget*		prefswin;


bool			lineWrap;
bool			highLight;

int				tabWidth;
char*			fontAndSize;



bool			restoreBookmarks;
bool			showJumpToLine;


bool			showLiveSearch;

GtkWidget*		fontBox;
GtkWidget*		terminalBox;


bool			tmpLineWrap;
bool			tmpHighLight;

int				tmpTabWidth;



bool			tmpShowJumpToLine;


bool			tmpShowLiveSearch;

GtkWidget*		toolNameWidget;
GtkWidget*		commandLineWidget;
GtkWidget*		inTermWidget;
GtkWidget*		inPopupWidget;
GtkWidget*		syncWidget;
GtkWidget*		ignoreWidget;
GtkWidget*		pasteWidget;
GtkWidget*		replaceWidget;
GtkWidget*		showDocWidget;
GtkWidget*		toolSelect;

char*			selectedToolPath=NULL;
GList*			toolsList=NULL;

GtkWidget*		restoreBMs;







bool			showDoc=false;


int				windowWidth;
int				windowHeight;
int				windowX=-1;
int				windowY=-1;
bool			wrapSearch;
bool			insensitiveSearch;
bool			replaceAll;

int				currentTabNumber;
int 			untitledNumber=1;

GtkToolItem*	newButton;
GtkToolItem*	openButton;
GtkToolItem*	saveButton;
GtkToolItem*	saveasButton;
GtkToolItem*	closeButton;
GtkToolItem*	redoButton;
GtkToolItem*	undoButton;


GtkWidget*		findReplaceDialog;
GtkWidget*		findBox;
GtkWidget*		replaceBox;

char*			thePage=NULL;
//char*			htmlFile=NULL;
//char*			htmlURI=NULL;


//spellcheck
GtkWidget*		spellCheckWord;
GtkWidget*		wordListDropbox;
char*			badWord=NULL;
char*			goodWord=NULL;
#ifdef _ASPELL_
AspellConfig*	aspellConfig;
AspellSpeller*	spellChecker=0;
#endif

//tags
tagStruct*		tagList[100]={NULL,};

int				currentTagNum=0;

void scrollToIterInPane(pageStruct* page,GtkTextIter* iter)
{
	if(page->inTop==true)
		gtk_text_view_scroll_to_iter((GtkTextView*)page->view,iter,0,true,0,0.5);
	else
		gtk_text_view_scroll_to_iter((GtkTextView*)page->view2,iter,0,true,0,0.5);
}

pageStruct* getPageStructPtr(int pagenum)
{
	int			thispage;
	GtkWidget*	pageBox;

	if(pagenum==-1)
		thispage=gtk_notebook_get_current_page(notebook);
	else
		thispage=pagenum;

	pageBox=gtk_notebook_get_nth_page(notebook,thispage);
	if(pageBox==NULL)
		return(NULL);
	else
		return((pageStruct*)g_object_get_data((GObject*)pageBox,"pagedata"));
}

void getMimeType(char* filepath,void* ptr)
{
	char*	command;
	gchar	*stdout=NULL;
	gchar	*stderr=NULL;
	gint	retval=0;

	asprintf(&command,"file -b --mime-type %s",filepath);
	g_spawn_command_line_sync(command,&stdout,&stderr,&retval,NULL);
	if (retval==0)
		{
			stdout[strlen(stdout)-1]=0;
			*((char**)ptr)=strdup(stdout);
			g_free(stdout);
			g_free(stderr);
		}
}

void setLanguage(pageStruct* page)
{
	GtkSourceLanguage*			lang=NULL;
	GtkSourceLanguageManager*	lm=NULL;
	char*						mimetype;
	gboolean					result_uncertain;
	char*						command;
	char						line[1024];
	FILE*						fp;

	lm=gtk_source_language_manager_new();

	g_object_ref(lm);
	g_object_set_data_full(G_OBJECT(page->buffer),"languages-manager",lm,(GDestroyNotify)g_object_unref);
	lm=gtk_source_language_manager_get_default();
	mimetype=g_content_type_guess(page->filePath,NULL,0,&result_uncertain);

	if (result_uncertain)
		{
			g_free(mimetype);
			mimetype=NULL;

			asprintf(&command,"xdg-mime query filetype \"%s\"",page->filePath);
			fp=popen(command,"r");
			fgets(line,1024,fp);
			asprintf(&mimetype,"%s",sliceBetween(line,(char*)"",(char*)";"));
			pclose(fp);
		}

	lang=gtk_source_language_manager_guess_language(lm,page->filePath,mimetype);

	if (lang==NULL)
		{
			getMimeType((char*)page->filePath,&mimetype);
			lang=gtk_source_language_manager_guess_language(lm,page->filePath,mimetype);
			if (lang!=NULL)
				gtk_source_buffer_set_language(page->buffer,lang);
		}
	else
		{
			gtk_source_buffer_set_language(page->buffer,lang);
		}

	if(lang!=NULL)
		page->lang=gtk_source_language_get_name(lang);

	if(mimetype!=NULL)
		g_free(mimetype);
}











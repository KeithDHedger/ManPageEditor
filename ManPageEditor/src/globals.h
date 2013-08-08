/*
 *
 * K.D.Hedger 2013 <kdhedger68713@gmail.com>
 *
 * globals.h
 *
*/
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <sys/stat.h>
#include <gtksourceview/gtksourcebuffer.h>
#include <gtksourceview/gtksourceview.h>

#include "config.h"

#ifdef _ASPELL_
#include <aspell.h>
#endif

#ifndef _GLOBALS_
#define _GLOBALS_

#define REPLACE			100
#define FINDNEXT		200
#define FINDPREV		300

#define MYEMAIL "kdhedger68713@gmail.com"
#define MYWEBSITE "http://keithhedger.hostingsiteforfree.com"
#define KKEDIT_VERSION "0.0.1"

#define TOOL_ASYNC 8
#define TOOL_IGNORE_OP 0
#define TOOL_PASTE_OP 1
#define TOOL_REPLACE_OP 2
#define TOOL_SHOW_DOC 4
#define	TOOL_INSERT_MASK 3

#define VALIDCHARS "[]a-zA-Z0-9:/@ ()_-,.*#;[\"]"
#define VALIDFILENAMECHARS "[A-Za-z0-9_-./ ]"
#define VALIDFUNCTIONCHARS "[A-Za-z0-9_*]"

#define BOOKMAXMARKMENULEN 40
#define MAXRECENT 10

#define PLACEHOLDERINFO "PLACEHOLDERS:\n%t - Currently selected text. Passed to command as $KKEDIT_SELECTION\n%f - Filepath of the current document. Passed to command as $KKEDIT_CURRENTFILE\n%d - Directory of the current document or $HOME. Passed to command as $KKEDIT_CURRENTDIR\n%i -The location of the globally installed tools. Passed to command as $KKEDIT_DATADIR\n%h - Tempory file for displaying html in doc viewer. Passed to command as $KKEDIT_HTMLFILE"

struct pageStruct
{
	GtkWidget*			pane;
	GtkScrolledWindow*	pageWindow;
	GtkScrolledWindow*	pageWindow2;
	GtkSourceBuffer*	buffer;
	GtkSourceView*		view;
	GtkSourceView*		view2;
	char*				filePath;
	GtkWidget*			tabName;
	GtkTextIter			iter;
	GtkTextIter			match_start;
	GtkTextIter			match_end;
	bool				isFirst;
	char*				fileName;
	GFile*				gFile;
	GFileMonitor*		monitor;
	bool				itsMe;
	GList*				markList;
	bool				inTop;
	bool				isSplit;
	const char*			lang;
	GtkWidget*			tabVbox;
};

struct tagStruct
{
	GtkTextIter		start;
	GtkTextIter		end;
	char*			name;
	int				startLine;
	int				startOffset;
	int				endLine;
	int				endOffset;
	PangoWeight		weight;
	bool			isTag;
	GtkTextMark*	mark;
};

//manpage stuff
extern char*			manFilename;
extern char*			manName;
extern char*			manSection;
extern char*			manVersion;
extern char*			manAuthor;
extern char*			manCategory;
extern char*			manFilePath;
extern bool				pageOpen;
extern bool				dirty;

extern GtkWidget*		window;
extern GtkNotebook*		notebook;

extern GtkWidget*		menubar;
extern GtkWidget*		menufile;
extern GtkWidget*		menuedit;





extern GtkWidget*		menuhelp;
extern GtkWidget*		menuprint;
extern GtkWidget*		menuclose;
extern GtkWidget*		menucloseall;
extern GtkWidget*		menusaveall;
extern GtkWidget*		menurevert;
extern GtkWidget*		menumanpage;

extern GtkWidget*		redoMenu;
extern GtkWidget*		undoMenu;
extern GtkWidget*		saveMenu;
extern GtkWidget*		exportMenu;
extern GtkWidget*		saveAsMenu;

extern GtkWidget*		lineNumberWidget;
extern GtkWidget*		liveSearchWidget;

extern int				currentPage;

extern GtkWidget*		prefswin;


extern bool				lineWrap;
extern bool				highLight;

extern int				tabWidth;
extern char*			fontAndSize;




extern bool				showJumpToLine;


extern bool				showLiveSearch;

extern GtkWidget*		fontBox;
extern GtkWidget*		terminalBox;


extern bool				tmpLineWrap;
extern bool				tmpHighLight;

extern int				tmpTabWidth;



extern bool				tmpShowJumpToLine;


extern bool				tmpShowLiveSearch;

extern GtkWidget*		toolNameWidget;
extern GtkWidget*		commandLineWidget;
extern GtkWidget*		inTermWidget;
extern GtkWidget*		inPopupWidget;

extern GtkWidget*		syncWidget;
extern GtkWidget*		ignoreWidget;
extern GtkWidget*		pasteWidget;
extern GtkWidget*		replaceWidget;
extern GtkWidget*		showDocWidget;
extern GtkWidget*		toolSelect;

extern char*			selectedToolPath;
extern GList*			toolsList;

extern GtkWidget*		restoreBMs;








extern bool				showDoc;


extern int				windowWidth;
extern int				windowHeight;
extern int				windowX;
extern int				windowY;
extern bool				wrapSearch;
extern bool				insensitiveSearch;
extern bool				replaceAll;

extern int				currentTabNumber;
extern int 				untitledNumber;

extern GtkToolItem*		newButton;
extern GtkToolItem*		openButton;
extern GtkToolItem*		saveButton;
extern GtkToolItem*		saveasButton;
extern GtkToolItem*		closeButton;
extern GtkToolItem*		redoButton;
extern GtkToolItem*		undoButton;


extern GtkWidget*		findReplaceDialog;
extern GtkWidget*		findBox;
extern GtkWidget*		replaceBox;

extern char*			thePage;
extern char*			htmlFile;
extern char*			htmlURI;


extern GtkWidget*		spellCheckWord;
extern GtkWidget*		wordListDropbox;
extern char*			badWord;
extern char*			goodWord;
#ifdef _ASPELL_
extern AspellConfig*	aspellConfig;
extern AspellSpeller*	spellChecker;
#endif

//tags
extern tagStruct*		tagList[100];
extern int				currentTagNum;

pageStruct* getPageStructPtr(int pagenum);
void getMimeType(char* filepath,void* ptr);
void setLanguage(pageStruct* page);
void runCommand(char* commandtorun,void* ptr,bool interm,int flags);
void scrollToIterInPane(pageStruct* page,GtkTextIter* iter);

#endif





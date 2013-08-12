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

#define VALIDCHARS "[]a-zA-Z0-9:/@ ()_-,.*#;[\"]"
#define VALIDFILENAMECHARS "[A-Za-z0-9_-./ ]"
#define VALIDFUNCTIONCHARS "[A-Za-z0-9_*]"

enum {BOLD,ITALIC,NORMAL};

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
	bool				itsMe;
	bool				inTop;
	bool				isSplit;
	const char*			lang;
	GtkWidget*			tabVbox;
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
extern char*			exportPath;

extern GtkWidget*		nameBox;
extern GtkWidget*		sectionBox;
extern GtkWidget*		versionBox;
extern GtkWidget*		authorBox;
extern GtkWidget*		categoryBox;

extern GtkWidget*		window;
extern GtkNotebook*		notebook;

extern GtkWidget*		menubar;
extern GtkWidget*		menufile;
extern GtkWidget*		menuedit;
extern GtkWidget*		menuformat;
extern GtkWidget*		menuhelp;
extern GtkWidget*		menuprint;
extern GtkWidget*		menuclose;

extern GtkWidget*		redoMenu;
extern GtkWidget*		undoMenu;
extern GtkWidget*		saveMenu;
extern GtkWidget*		exportMenu;
extern GtkWidget*		exportAsMenu;
extern GtkWidget*		saveAsMenu;
extern GtkWidget*		closeSectionMenu;

extern GtkWidget*		liveSearchWidget;
extern int				currentPage;

extern GtkWidget*		prefswin;

extern bool				lineWrap;
extern bool				highLight;
extern int				tabWidth;
extern char*			fontAndSize;
extern bool				showLiveSearch;
extern GtkWidget*		fontBox;
extern bool				tmpLineWrap;
extern bool				tmpHighLight;
extern int				tmpTabWidth;
extern bool				tmpShowLiveSearch;

extern int				windowWidth;
extern int				windowHeight;
extern int				windowX;
extern int				windowY;
extern bool				wrapSearch;
extern bool				insensitiveSearch;
extern bool				replaceAll;

extern int				currentTabNumber;

extern GtkToolItem*		newButton;
extern GtkToolItem*		openButton;
extern GtkToolItem*		saveButton;
extern GtkToolItem*		closeButton;
extern GtkToolItem*		redoButton;
extern GtkToolItem*		undoButton;

extern GtkWidget*		findReplaceDialog;
extern GtkWidget*		findBox;
extern GtkWidget*		replaceBox;

extern char*			thePage;

extern GtkWidget*		spellCheckWord;
extern GtkWidget*		wordListDropbox;
extern char*			badWord;
extern char*			goodWord;
#ifdef _ASPELL_
extern AspellConfig*	aspellConfig;
extern AspellSpeller*	spellChecker;
#endif

pageStruct* getPageStructPtr(int pagenum);
void scrollToIterInPane(pageStruct* page,GtkTextIter* iter);

#endif





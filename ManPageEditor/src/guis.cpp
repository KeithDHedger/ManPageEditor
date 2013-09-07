/*
 *
 * K.D.Hedger 2013 <kdhedger68713@gmail.com>
 *
*/

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <sys/stat.h>

#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcebuffer.h>

#include "config.h"
#include "globals.h"
#include "files.h"
#include "callbacks.h"
#include "searchcallbacks.h"
#include "spellcheck.h"

void doPrefs(void)
{
	GtkWidget*	vbox;
	GtkWidget*	hbox;
	GtkWidget*	item;

	prefswin=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title((GtkWindow*)prefswin,"Preferences");
	vbox=gtk_vbox_new(false,8);

//wraplines
	item=gtk_check_button_new_with_label("Wrap Lines");
	gtk_widget_set_name(item,"wrap");
	gtk_toggle_button_set_active((GtkToggleButton*)item,lineWrap);
	gtk_box_pack_start(GTK_BOX(vbox),item,true,true,0);
	g_signal_connect(G_OBJECT(item),"toggled",G_CALLBACK(setPrefs),(void*)item);

//highlite
	item=gtk_check_button_new_with_label("Highlight Current Line");
	gtk_widget_set_name(item,"high");
	gtk_toggle_button_set_active((GtkToggleButton*)item,highLight);
	gtk_box_pack_start(GTK_BOX(vbox),item,true,true,0);
	g_signal_connect(G_OBJECT(item),"toggled",G_CALLBACK(setPrefs),(void*)item);

//use underline
	item=gtk_check_button_new_with_label("Use Underline");
	gtk_widget_set_name(item,"underline");
	gtk_toggle_button_set_active((GtkToggleButton*)item,useUnderline);
	gtk_box_pack_start(GTK_BOX(vbox),item,true,true,0);
	g_signal_connect(G_OBJECT(item),"toggled",G_CALLBACK(setPrefs),(void*)item);

//gzip manpages
	item=gtk_check_button_new_with_label("GZip Man Pages");
	gtk_widget_set_name(item,"gzip");
	gtk_toggle_button_set_active((GtkToggleButton*)item,gzipPages);
	gtk_box_pack_start(GTK_BOX(vbox),item,true,true,0);
	g_signal_connect(G_OBJECT(item),"toggled",G_CALLBACK(setPrefs),(void*)item);


//show live search in toolbar
	item=gtk_check_button_new_with_label("Show 'Live Search' in toolbar");
	gtk_widget_set_name(item,"livesearch");
	gtk_toggle_button_set_active((GtkToggleButton*)item,showLiveSearch);
	gtk_box_pack_start(GTK_BOX(vbox),item,true,true,0);
	g_signal_connect(G_OBJECT(item),"toggled",G_CALLBACK(setPrefs),(void*)item);

//tabwidth  -- CLEAN
	GtkObject*	adj=gtk_adjustment_new(tmpTabWidth,1,64,1,1,0);
	hbox=gtk_hbox_new(true,0);
	item=gtk_spin_button_new((GtkAdjustment*)adj,1,0);
	gtk_widget_set_name(item,"tabs");
	gtk_box_pack_start(GTK_BOX(hbox),gtk_label_new("Tab width: "),true,true,0);
	gtk_container_add(GTK_CONTAINER(hbox),item);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,true,true,0);
	g_signal_connect(G_OBJECT(item),"value-changed",G_CALLBACK(setPrefs),(void*)item);

//font
	fontBox=gtk_entry_new();
	hbox=gtk_hbox_new(true,0);
	gtk_box_pack_start(GTK_BOX(hbox),gtk_label_new("Font And Size: "),true,true,0);
	gtk_container_add(GTK_CONTAINER(hbox),fontBox);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,true,true,0);
	gtk_entry_set_text((GtkEntry*)fontBox,fontAndSize);

//terminalcommand
	terminalBox=gtk_entry_new();
	hbox=gtk_hbox_new(true,0);
	gtk_box_pack_start(GTK_BOX(hbox),gtk_label_new("Terminal Command: "),true,true,0);
	gtk_container_add(GTK_CONTAINER(hbox),terminalBox);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,true,true,0);
	gtk_entry_set_text((GtkEntry*)terminalBox,terminalCommand);
	gtk_widget_show_all(hbox);

//buttons
	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(),true,true,0);

	hbox=gtk_hbox_new(true,4);
	item=gtk_button_new_from_stock(GTK_STOCK_APPLY);
	gtk_box_pack_start(GTK_BOX(hbox),item,true,false,2);
	gtk_widget_set_name(item,"apply");
	g_signal_connect(G_OBJECT(item),"clicked",G_CALLBACK(setPrefs),(void*)item);	

	item=gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	gtk_box_pack_start(GTK_BOX(hbox),item,true,false,2);
	gtk_widget_set_name(item,"cancel");
	g_signal_connect(G_OBJECT(item),"clicked",G_CALLBACK(setPrefs),(void*)item);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,true,true,2);

//show it
	gtk_container_add(GTK_CONTAINER(prefswin),(GtkWidget*)vbox);
	gtk_widget_show_all(prefswin);
}

void buildMainGui(void)
{
	GtkWidget*					vbox;
	GtkWidget*					menuitem;
	GtkWidget*					menu;
	GtkWidget*					toolbar;
	GtkToolItem*				toolbutton;
	GtkAccelGroup*				accgroup;
	GtkWidget*					image;

	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title((GtkWindow*)window,"Manpage Editor");
	gtk_window_set_default_size((GtkWindow*)window,windowWidth,windowHeight);
	if(windowX!=-1 && windowY!=-1)
		gtk_window_move((GtkWindow *)window,windowX,windowY);

	g_signal_connect(G_OBJECT(window),"delete-event",G_CALLBACK(doShutdown),NULL);
	accgroup=gtk_accel_group_new();
	gtk_window_add_accel_group((GtkWindow*)window,accgroup);

	notebook=(GtkNotebook*)gtk_notebook_new();
	gtk_notebook_set_scrollable(notebook,true);
	gtk_signal_connect(GTK_OBJECT(notebook),"page-reordered",G_CALLBACK(reorderDirty),NULL);

	vbox=gtk_vbox_new(FALSE,0);
	menubar=gtk_menu_bar_new();
	toolbar=gtk_toolbar_new();

	gtk_box_pack_start(GTK_BOX(vbox),(GtkWidget*)menubar,false,true,0);
	gtk_box_pack_start(GTK_BOX(vbox),(GtkWidget*)toolbar,false,true,0);
	gtk_box_pack_start(GTK_BOX(vbox),(GtkWidget*)notebook,true,true,0);

//toolbar
//new
	newButton=gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
	gtk_toolbar_insert((GtkToolbar*)toolbar,newButton,-1);
	gtk_signal_connect(GTK_OBJECT(newButton),"clicked",G_CALLBACK(newSection),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)newButton,"New File");

//save
	saveButton=gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
	gtk_toolbar_insert((GtkToolbar*)toolbar,saveButton,-1);
	gtk_signal_connect(GTK_OBJECT(saveButton),"clicked",G_CALLBACK(saveManpage),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)saveButton,"Save File");

	gtk_toolbar_insert((GtkToolbar*)toolbar,gtk_separator_tool_item_new(),-1);

//edit buttons
//cut
	toolbutton=gtk_tool_button_new_from_stock(GTK_STOCK_CUT);
	gtk_toolbar_insert((GtkToolbar*)toolbar,toolbutton,-1);
	gtk_signal_connect(GTK_OBJECT(toolbutton),"clicked",G_CALLBACK(cutToClip),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton,"Cut");
//copy
	toolbutton=gtk_tool_button_new_from_stock(GTK_STOCK_COPY);
	gtk_toolbar_insert((GtkToolbar*)toolbar,toolbutton,-1);
	gtk_signal_connect(GTK_OBJECT(toolbutton),"clicked",G_CALLBACK(copyToClip),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton,"Copy");
//paste
	toolbutton=gtk_tool_button_new_from_stock(GTK_STOCK_PASTE);
	gtk_toolbar_insert((GtkToolbar*)toolbar,toolbutton,-1);
	gtk_signal_connect(GTK_OBJECT(toolbutton),"clicked",G_CALLBACK(pasteFromClip),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton,"Paste");

	gtk_toolbar_insert((GtkToolbar*)toolbar,gtk_separator_tool_item_new(),-1);

//undo
	undoButton=gtk_tool_button_new_from_stock(GTK_STOCK_UNDO);
	gtk_toolbar_insert((GtkToolbar*)toolbar,undoButton,-1);
	gtk_signal_connect(GTK_OBJECT(undoButton),"clicked",G_CALLBACK(undo),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)undoButton,"Undo");
//redo
	redoButton=gtk_tool_button_new_from_stock(GTK_STOCK_REDO);
	gtk_toolbar_insert((GtkToolbar*)toolbar,redoButton,-1);
	gtk_signal_connect(GTK_OBJECT(redoButton),"clicked",G_CALLBACK(redo),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)redoButton,"Redo");

	gtk_toolbar_insert((GtkToolbar*)toolbar,gtk_separator_tool_item_new(),-1);

//find
	toolbutton=gtk_tool_button_new_from_stock(GTK_STOCK_FIND);
	gtk_toolbar_insert((GtkToolbar*)toolbar,toolbutton,-1);
	gtk_signal_connect(GTK_OBJECT(toolbutton),"clicked",G_CALLBACK(find),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton,"Find/Replace");
		
//livesearch
	liveSearchWidget=gtk_entry_new();
	toolbutton=gtk_tool_item_new();
	gtk_container_add((GtkContainer *)toolbutton,liveSearchWidget);
	gtk_toolbar_insert((GtkToolbar*)toolbar,toolbutton,-1);
	g_signal_connect_after(G_OBJECT(liveSearchWidget),"key-release-event",G_CALLBACK(doLiveSearch),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton,"Live Search");

//menus
//file menu
	menufile=gtk_menu_item_new_with_label("File");
	menu=gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menufile),menu);
//new
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(newManpage),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,'N',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

//newsection
	menuitem=gtk_image_menu_item_new_with_label("New Section");
	image=gtk_image_new_from_stock(GTK_STOCK_NEW,GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuitem,image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(newSection),NULL);

//open
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(openManpage),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,'O',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

//newtemplate
	menuitem=gtk_image_menu_item_new_with_label("New From Template");
	image=gtk_image_new_from_stock(GTK_STOCK_NEW,GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuitem,image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(openManpage),(void*)1);

//new editor
	menuitem=gtk_image_menu_item_new_with_label("New Editor");
	image=gtk_image_new_from_stock(GTK_STOCK_NEW,GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuitem,image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(newEditor),NULL);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

//save
	saveMenu=gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),saveMenu);
	gtk_signal_connect(GTK_OBJECT(saveMenu),"activate",G_CALLBACK(saveManpage),NULL);
	gtk_widget_add_accelerator((GtkWidget *)saveMenu,"activate",accgroup,'S',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

//save as
	saveAsMenu=gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),saveAsMenu);
	gtk_signal_connect(GTK_OBJECT(saveAsMenu),"activate",G_CALLBACK(saveAs),NULL);
	gtk_widget_set_sensitive((GtkWidget*)saveAsMenu,false);

//export
	image=gtk_image_new_from_stock(GTK_STOCK_CONVERT,GTK_ICON_SIZE_MENU);
	exportAsMenu=gtk_image_menu_item_new_with_label("Export Man Page");
	gtk_image_menu_item_set_image((GtkImageMenuItem *)exportAsMenu,image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),exportAsMenu);
	gtk_signal_connect(GTK_OBJECT(exportAsMenu),"activate",G_CALLBACK(exportFile),NULL);

//export as
	image=gtk_image_new_from_stock(GTK_STOCK_CONVERT,GTK_ICON_SIZE_MENU);
	exportMenu=gtk_image_menu_item_new_with_label("Export As");
	gtk_image_menu_item_set_image((GtkImageMenuItem *)exportMenu,image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),exportMenu);
	gtk_signal_connect(GTK_OBJECT(exportMenu),"activate",G_CALLBACK(exportFile),(void*)1);

//import
	image=gtk_image_new_from_stock(GTK_STOCK_OPEN,GTK_ICON_SIZE_MENU);
	menuitem=gtk_image_menu_item_new_with_label("Import Manpage");
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuitem,image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(importManpage),NULL);

//import sysmanpage
	image=gtk_image_new_from_stock(GTK_STOCK_OPEN,GTK_ICON_SIZE_MENU);
	menuitem=gtk_image_menu_item_new_with_label("Import System Manpage");
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuitem,image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(importManpage),(void*)1);

//preview
	image=gtk_image_new_from_stock(GTK_STOCK_FIND,GTK_ICON_SIZE_MENU);
	previewMenu=gtk_image_menu_item_new_with_label("Preview Page");
	gtk_image_menu_item_set_image((GtkImageMenuItem *)previewMenu,image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),previewMenu);
	gtk_signal_connect(GTK_OBJECT(previewMenu),"activate",G_CALLBACK(previewPage),NULL);

//properties
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_PROPERTIES,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(redoProps),NULL);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

//printfile
	menuprint=gtk_image_menu_item_new_from_stock(GTK_STOCK_PRINT,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuprint);
	gtk_signal_connect(GTK_OBJECT(menuprint),"activate",G_CALLBACK(printFile),NULL);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
//close
	menuclose=gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuclose);
	gtk_signal_connect(GTK_OBJECT(menuclose),"activate",G_CALLBACK(closePage),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuclose,"activate",accgroup,'W',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
//close section
	image=gtk_image_new_from_stock(GTK_STOCK_DELETE,GTK_ICON_SIZE_MENU);
	closeSectionMenu=gtk_image_menu_item_new_with_label("Delete Section");
	gtk_image_menu_item_set_image((GtkImageMenuItem *)closeSectionMenu,image);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),closeSectionMenu);
	gtk_signal_connect(GTK_OBJECT(closeSectionMenu),"activate",G_CALLBACK(deleteSection),NULL);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
//quit
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(doShutdown),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,'Q',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

//edit menu
	menuedit=gtk_menu_item_new_with_label("Edit");
	menu=gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuedit),menu);
//undo
	undoMenu=gtk_image_menu_item_new_from_stock(GTK_STOCK_UNDO,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),undoMenu);
	gtk_signal_connect(GTK_OBJECT(undoMenu),"activate",G_CALLBACK(undo),NULL);
	gtk_widget_add_accelerator((GtkWidget *)undoMenu,"activate",accgroup,'Z',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

//redo
	redoMenu=gtk_image_menu_item_new_from_stock(GTK_STOCK_REDO,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),redoMenu);
	gtk_signal_connect(GTK_OBJECT(redoMenu),"activate",G_CALLBACK(redo),NULL);
	gtk_widget_add_accelerator((GtkWidget *)redoMenu,"activate",accgroup,'Z',(GdkModifierType)(GDK_SHIFT_MASK|GDK_CONTROL_MASK),GTK_ACCEL_VISIBLE);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

//cut
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_CUT,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(cutToClip),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,'X',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

//copy
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(copyToClip),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,'C',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

//paste
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(pasteFromClip),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,'V',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

//find
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_FIND,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(find),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,'F',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

//prefs
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_PREFERENCES,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(doPrefs),NULL);

//formattin menu
	menuformat=gtk_menu_item_new_with_label("Formating");
	menu=gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuformat),menu);
//bold
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_BOLD,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(doFormat),(void*)BOLD);
//italic
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_ITALIC,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(doFormat),(void*)ITALIC);
//normal
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_CLEAR,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(doFormat),(void*)NORMAL);
//break
//	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_GOTO_BOTTOM,NULL);
//	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
//	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(doFormat),(void*)3);

//help
	menuhelp=gtk_menu_item_new_with_label("Help");
	menu=gtk_menu_new();
//about
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuhelp),menu);
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT,NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(doAbout),NULL);
//help
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP,NULL);
	gtk_signal_connect(GTK_OBJECT(menuitem),"activate",G_CALLBACK(openHelp),NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),menufile);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),menuedit);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),menuformat);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),menuhelp);

	gtk_container_add(GTK_CONTAINER(window),(GtkWidget*)vbox);
	gtk_widget_set_sensitive((GtkWidget*)saveButton,false);
	gtk_widget_set_sensitive((GtkWidget*)saveMenu,false);
}

void buildFindReplace(void)
{
	GtkWidget*	content_area;
	GtkWidget*	replace;
	GtkWidget*	image;
	GtkWidget*	label;
	GtkWidget*	vbox;
	GtkWidget*	hbox;
	GtkWidget*	item;

	findReplaceDialog=gtk_dialog_new_with_buttons("Find/Replace",(GtkWindow*)window, GTK_DIALOG_DESTROY_WITH_PARENT,GTK_STOCK_GO_FORWARD,FINDNEXT,GTK_STOCK_GO_BACK,FINDPREV,"Replace",REPLACE,NULL);
	gtk_dialog_set_default_response((GtkDialog*)findReplaceDialog,GTK_RESPONSE_OK);
	g_signal_connect(G_OBJECT(findReplaceDialog),"response",G_CALLBACK(doFindReplace),NULL);
	content_area=gtk_dialog_get_content_area(GTK_DIALOG(findReplaceDialog));

	vbox=gtk_vbox_new(true,0);
	hbox=gtk_hbox_new(false,0);

	label=gtk_label_new("Find");
	gtk_container_add(GTK_CONTAINER(content_area),label);
	gtk_widget_show(label);

	findBox=gtk_entry_new();
	gtk_entry_set_text((GtkEntry*)findBox,"");
	gtk_entry_set_activates_default((GtkEntry*)findBox,true);
	gtk_container_add(GTK_CONTAINER(content_area),findBox);

	label=gtk_label_new("Replace With");
	gtk_container_add(GTK_CONTAINER(content_area),label);
	gtk_widget_show(label);

	replaceBox=gtk_entry_new();
	gtk_entry_set_text((GtkEntry*)replaceBox,"");
	gtk_entry_set_activates_default((GtkEntry*)replaceBox,true);
	gtk_container_add(GTK_CONTAINER(content_area),replaceBox);

	item=gtk_check_button_new_with_label("Case insensitive");
	gtk_toggle_button_set_active((GtkToggleButton*)item,insensitiveSearch);
	gtk_box_pack_start(GTK_BOX(hbox),item,true,true,0);
	gtk_widget_show(item);
	g_signal_connect(G_OBJECT(item),"toggled",G_CALLBACK(doSearchPrefs),(void*)1);

	item=gtk_check_button_new_with_label("Wrap");
	gtk_toggle_button_set_active((GtkToggleButton*)item,wrapSearch);
	gtk_box_pack_start(GTK_BOX(hbox),item,true,true,0);
	gtk_widget_show(item);
	g_signal_connect(G_OBJECT(item),"toggled",G_CALLBACK(doSearchPrefs),(void*)2);

	item=gtk_check_button_new_with_label("Replace All");
	gtk_toggle_button_set_active((GtkToggleButton*)item,replaceAll);
	gtk_box_pack_start(GTK_BOX(hbox),item,true,true,0);
	gtk_widget_show(item);
	g_signal_connect(G_OBJECT(item),"toggled",G_CALLBACK(doSearchPrefs),(void*)3);

	gtk_box_pack_start(GTK_BOX(vbox),hbox,true,true,0);
	gtk_box_pack_start(GTK_BOX(content_area),vbox,true,true,0);

	replace=gtk_dialog_get_widget_for_response((GtkDialog*)findReplaceDialog,100);
	image=gtk_image_new_from_stock(GTK_STOCK_FIND_AND_REPLACE,GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image((GtkButton*)replace,image);

	gtk_widget_show(findBox);
	gtk_widget_show(replaceBox);
	gtk_widget_show(vbox);
	gtk_widget_show(hbox);

	gtk_signal_connect_object(GTK_OBJECT(findReplaceDialog),"delete_event",GTK_SIGNAL_FUNC(gtk_widget_hide),GTK_OBJECT(findReplaceDialog));
	gtk_signal_connect(GTK_OBJECT(findReplaceDialog),"delete_event",GTK_SIGNAL_FUNC(gtk_true),NULL);
}

#ifdef _ASPELL_
void buildWordCheck(int documentCheck)
{
	GtkWidget*	vbox;
	GtkWidget*	button;
	GtkWidget*	hbox;
	GtkWidget*	label;
	GtkWidget*	image;
	char*		labeltext[512];
	int			docflag=documentCheck;

	spellCheckWord=gtk_dialog_new();
	gtk_window_set_title((GtkWindow*)spellCheckWord,"Spell check word");
	vbox=gtk_vbox_new(true,8);

	hbox=gtk_hbox_new(true,8);

	sprintf((char*)&labeltext,"Change <i><b>%s</b></i> to: ",badWord);
	label=gtk_label_new((char*)&labeltext);
	gtk_label_set_use_markup((GtkLabel*)label,true);
	gtk_box_pack_start(GTK_BOX(hbox),label,true,true,0);

	wordListDropbox=gtk_combo_box_text_new();
	gtk_box_pack_start(GTK_BOX(hbox),wordListDropbox,true,true,0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,true,true,0);

	hbox=gtk_hbox_new(true,8);
	button=gtk_button_new_from_stock(GTK_STOCK_APPLY);
	gtk_box_pack_start(GTK_BOX(hbox),button,true,true,0);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(doChangeWord),(gpointer)(long)docflag);

	button=gtk_button_new_with_label("Ignore");
	image=gtk_image_new_from_stock(GTK_STOCK_ADD,GTK_ICON_SIZE_MENU);
	gtk_button_set_image((GtkButton*)button,image);
	gtk_box_pack_start(GTK_BOX(hbox),button,true,true,0);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(doAddIgnoreWord),(gpointer)1);

	button=gtk_button_new_from_stock(GTK_STOCK_ADD);
	gtk_box_pack_start(GTK_BOX(hbox),button,true,true,0);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(doAddIgnoreWord),(gpointer)2);

	button=gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	gtk_box_pack_start(GTK_BOX(hbox),button,true,true,0);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(doCancelCheck),NULL);

	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(),true,true,0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,true,true,0);

	GtkWidget* content=gtk_dialog_get_content_area((GtkDialog *)spellCheckWord);
	gtk_container_add(GTK_CONTAINER(content),(GtkWidget*)vbox);

	gtk_signal_connect_object(GTK_OBJECT(spellCheckWord),"delete_event",GTK_SIGNAL_FUNC(gtk_widget_hide),GTK_OBJECT(spellCheckWord));
	gtk_signal_connect(GTK_OBJECT(spellCheckWord),"delete_event",GTK_SIGNAL_FUNC(gtk_true),NULL);
}
#endif





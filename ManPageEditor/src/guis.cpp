/*
 *
 * ©K. D. Hedger. Fri 27 Nov 11:45:02 GMT 2015 keithdhedger@gmail.com

 * This file (guis.cpp) is part of ManPageEditor.

 * ManPageEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * ManPageEditor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ManPageEditor.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gdk/gdkkeysyms.h>

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
	vbox=creatNewBox(NEWVBOX,false,8);

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

//tabwidth
#if 1
#ifdef _USEGTK3_
	GtkAdjustment*	adj=gtk_adjustment_new(tmpTabWidth,1,64,1,1,0);
#else
	GtkObject*	adj=gtk_adjustment_new(tmpTabWidth,1,64,1,1,0);
#endif
	hbox=creatNewBox(NEWHBOX,true,0);
	item=gtk_spin_button_new((GtkAdjustment*)adj,1,0);
	gtk_widget_set_name(item,"tabs");
	gtk_box_pack_start(GTK_BOX(hbox),gtk_label_new("Tab width: "),true,true,0);
	gtk_container_add(GTK_CONTAINER(hbox),item);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,true,true,0);
	g_signal_connect(G_OBJECT(item),"value-changed",G_CALLBACK(setPrefs),(void*)item);
#endif
//font
	fontBox=gtk_entry_new();
	hbox=creatNewBox(NEWHBOX,true,0);

	gtk_box_pack_start(GTK_BOX(hbox),gtk_label_new("Font And Size: "),true,true,0);
	gtk_container_add(GTK_CONTAINER(hbox),fontBox);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,true,true,0);
	gtk_entry_set_text((GtkEntry*)fontBox,fontAndSize);

//terminalcommand
	terminalBox=gtk_entry_new();
	hbox=creatNewBox(NEWHBOX,true,0);

	gtk_box_pack_start(GTK_BOX(hbox),gtk_label_new("Terminal Command: "),true,true,0);
	gtk_container_add(GTK_CONTAINER(hbox),terminalBox);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,true,true,0);
	gtk_entry_set_text((GtkEntry*)terminalBox,terminalCommand);
	gtk_widget_show_all(hbox);

//buttons
#ifdef _USEGTK3_
	gtk_box_pack_start(GTK_BOX(vbox),gtk_separator_new(GTK_ORIENTATION_HORIZONTAL),true,true,0);
#else
	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(),true,true,0);
#endif

	hbox=creatNewBox(NEWHBOX,true,4);

#ifdef _USEGTK3_
	item=gtk_button_new_with_label("Apply");
#else
	item=gtk_button_new_from_stock(GTK_STOCK_APPLY);
#endif
	gtk_box_pack_start(GTK_BOX(hbox),item,true,false,2);
	gtk_widget_set_name(item,"apply");
	g_signal_connect(G_OBJECT(item),"clicked",G_CALLBACK(setPrefs),(void*)item);

#ifdef _USEGTK3_
	item=gtk_button_new_with_label("Cancel");
#else
	item=gtk_button_new_from_stock(GTK_STOCK_CANCEL);
#endif
	gtk_box_pack_start(GTK_BOX(hbox),item,true,false,2);
	gtk_widget_set_name(item,"cancel");
	g_signal_connect(G_OBJECT(item),"clicked",G_CALLBACK(setPrefs),(void*)item);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,true,true,2);

//show it
	gtk_container_add(GTK_CONTAINER(prefswin),(GtkWidget*)vbox);
	gtk_widget_show_all(prefswin);
}

GtkToolItem* makeNewToolItem(const char* stock,const char* label)
{
	GtkToolItem*				button;
	GtkWidget*					image;
#ifdef _USEGTK3_
	image=gtk_image_new_from_icon_name(stock,GTK_ICON_SIZE_LARGE_TOOLBAR);
	button=gtk_tool_button_new(image,label);
#else
	button=gtk_tool_button_new_from_stock(stock);
#endif

	return(button);
}

void recentFileMenu(GtkRecentChooser *chooser,gpointer *data)
{
	gchar	*uri=NULL;
	char	*filename;

	uri=gtk_recent_chooser_get_current_uri((GtkRecentChooser*)chooser);
	if(uri!=NULL)
		{
			filename=g_filename_from_uri((const gchar*)uri,NULL,NULL);
			closePage(NULL,NULL);
			doOpenManpage(filename);
			dirty=false;
			setSensitive();
			g_free(uri);
			g_free(filename);
		}
}

void setupRecent(GtkMenuItem *menu)
{
	GtkRecentFilter	*filter;
	GtkWidget		*recent;

	recent=gtk_recent_chooser_menu_new();
	gtk_recent_chooser_set_local_only(GTK_RECENT_CHOOSER(recent),false);
	gtk_recent_chooser_set_sort_type(GTK_RECENT_CHOOSER(recent),GTK_RECENT_SORT_MRU);
	gtk_recent_chooser_set_limit(GTK_RECENT_CHOOSER(recent),10);

	filter=gtk_recent_filter_new();
	gtk_recent_filter_add_mime_type (filter,"application/x-maneditdoc");
	//gtk_recent_filter_add_application(filter,"manpageeditor");
	gtk_recent_chooser_set_filter(GTK_RECENT_CHOOSER(recent),filter);
	g_signal_connect(recent,"item_activated",G_CALLBACK(recentFileMenu),NULL);

	gtk_menu_item_set_submenu(menu,recent);
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
	g_signal_connect(G_OBJECT(notebook),"page-reordered",G_CALLBACK(reorderDirty),NULL);

	vbox=creatNewBox(NEWVBOX,false,0);

	menubar=gtk_menu_bar_new();
	toolbar=gtk_toolbar_new();

	gtk_box_pack_start(GTK_BOX(vbox),(GtkWidget*)menubar,false,true,0);
	gtk_box_pack_start(GTK_BOX(vbox),(GtkWidget*)toolbar,false,true,0);
	gtk_box_pack_start(GTK_BOX(vbox),(GtkWidget*)notebook,true,true,0);

//toolbar
//new
	newButton=makeNewToolItem(GTK_STOCK_NEW,"New");
	gtk_toolbar_insert((GtkToolbar*)toolbar,newButton,-1);
	g_signal_connect(G_OBJECT(newButton),"clicked",G_CALLBACK(newSection),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)newButton,"New File");

//save
	saveButton=makeNewToolItem(GTK_STOCK_SAVE,"Save");
	gtk_toolbar_insert((GtkToolbar*)toolbar,saveButton,-1);
	g_signal_connect(G_OBJECT(saveButton),"clicked",G_CALLBACK(saveManpage),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)saveButton,"Save File");

	gtk_toolbar_insert((GtkToolbar*)toolbar,gtk_separator_tool_item_new(),-1);

//edit buttons
//cut
	toolbutton=makeNewToolItem(GTK_STOCK_CUT,"Cut");
	gtk_toolbar_insert((GtkToolbar*)toolbar,toolbutton,-1);
	g_signal_connect(G_OBJECT(toolbutton),"clicked",G_CALLBACK(cutToClip),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton,"Cut");
//copy
	toolbutton=makeNewToolItem(GTK_STOCK_COPY,"Copy");
	gtk_toolbar_insert((GtkToolbar*)toolbar,toolbutton,-1);
	g_signal_connect(G_OBJECT(toolbutton),"clicked",G_CALLBACK(copyToClip),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton,"Copy");
//paste
	toolbutton=makeNewToolItem(GTK_STOCK_PASTE,"Paste");
	gtk_toolbar_insert((GtkToolbar*)toolbar,toolbutton,-1);
	g_signal_connect(G_OBJECT(toolbutton),"clicked",G_CALLBACK(pasteFromClip),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton,"Paste");

	gtk_toolbar_insert((GtkToolbar*)toolbar,gtk_separator_tool_item_new(),-1);

//undo
	undoButton=makeNewToolItem(GTK_STOCK_UNDO,"Undo");
	gtk_toolbar_insert((GtkToolbar*)toolbar,undoButton,-1);
	g_signal_connect(G_OBJECT(undoButton),"clicked",G_CALLBACK(undo),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)undoButton,"Undo");
//redo
	redoButton=makeNewToolItem(GTK_STOCK_REDO,"Redo");
	gtk_toolbar_insert((GtkToolbar*)toolbar,redoButton,-1);
	g_signal_connect(G_OBJECT(redoButton),"clicked",G_CALLBACK(redo),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)redoButton,"Redo");
	gtk_toolbar_insert((GtkToolbar*)toolbar,gtk_separator_tool_item_new(),-1);

//find
	toolbutton=makeNewToolItem(GTK_STOCK_FIND,"Find");
	gtk_toolbar_insert((GtkToolbar*)toolbar,toolbutton,-1);
	g_signal_connect(G_OBJECT(toolbutton),"clicked",G_CALLBACK(find),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton,"Find/Replace");

//livesearch
	liveSearchWidget=gtk_entry_new();
	toolbutton=gtk_tool_item_new();
	gtk_container_add((GtkContainer *)toolbutton,liveSearchWidget);
	gtk_toolbar_insert((GtkToolbar*)toolbar,toolbutton,-1);
	g_signal_connect_after(G_OBJECT(liveSearchWidget),"key-release-event",G_CALLBACK(doLiveSearch),NULL);
	gtk_widget_set_tooltip_text((GtkWidget*)toolbutton,"Live Search");
	gtk_widget_set_sensitive(liveSearchWidget,false);

//menus
//file menu
	menufile=gtk_menu_item_new_with_label("File");
	menu=gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menufile),menu);
//new
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_mnemonic("_New");
#else
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(newManpage),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,'N',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

//newsection
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_mnemonic("New Section");
#else
	menuitem=gtk_image_menu_item_new_with_label("New Section");
	image=gtk_image_new_from_stock(GTK_STOCK_NEW,GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuitem,image);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(newSection),NULL);

//open
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_mnemonic("_Open");
#else
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(openManpage),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,'O',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

//open recent menu
	menuitem=gtk_menu_item_new_with_mnemonic("_Recent");
	setupRecent((GtkMenuItem*)menuitem);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

//newtemplate
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_label("New From Template");
#else
	menuitem=gtk_image_menu_item_new_with_label("New From Template");
	image=gtk_image_new_from_stock(GTK_STOCK_NEW,GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuitem,image);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(openManpage),(void*)1);

//new editor
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_label("New Editor");
#else
	menuitem=gtk_image_menu_item_new_with_label("New Editor");
	image=gtk_image_new_from_stock(GTK_STOCK_NEW,GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuitem,image);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(newEditor),NULL);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

//save
#ifdef _USEGTK3_
	saveMenu=gtk_menu_item_new_with_mnemonic("_Save");
#else
	saveMenu=gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),saveMenu);
	g_signal_connect(G_OBJECT(saveMenu),"activate",G_CALLBACK(saveManpage),NULL);
	gtk_widget_add_accelerator((GtkWidget *)saveMenu,"activate",accgroup,'S',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

//save as
#ifdef _USEGTK3_
	saveAsMenu=gtk_menu_item_new_with_label("Save As");
#else
	saveAsMenu=gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),saveAsMenu);
	g_signal_connect(G_OBJECT(saveAsMenu),"activate",G_CALLBACK(saveAs),NULL);
	gtk_widget_set_sensitive((GtkWidget*)saveAsMenu,false);

//export
#ifdef _USEGTK3_
	exportMenu=gtk_menu_item_new_with_label("Export Man Page");
#else
	image=gtk_image_new_from_stock(GTK_STOCK_CONVERT,GTK_ICON_SIZE_MENU);
	exportMenu=gtk_image_menu_item_new_with_label("Export Man Page");
	gtk_image_menu_item_set_image((GtkImageMenuItem *)exportMenu,image);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),exportMenu);
	g_signal_connect(G_OBJECT(exportMenu),"activate",G_CALLBACK(exportFile),NULL);

//export as
#ifdef _USEGTK3_
	exportAsMenu=gtk_menu_item_new_with_label("Export As");
#else
	image=gtk_image_new_from_stock(GTK_STOCK_CONVERT,GTK_ICON_SIZE_MENU);
	exportAsMenu=gtk_image_menu_item_new_with_label("Export As");
	gtk_image_menu_item_set_image((GtkImageMenuItem *)exportAsMenu,image);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),exportAsMenu);
	g_signal_connect(G_OBJECT(exportAsMenu),"activate",G_CALLBACK(exportFile),(void*)1);

//import
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_label("Import Manpage");
#else
	image=gtk_image_new_from_stock(GTK_STOCK_OPEN,GTK_ICON_SIZE_MENU);
	menuitem=gtk_image_menu_item_new_with_label("Import Manpage");
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuitem,image);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(importManpage),NULL);

//import sysmanpage
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_label("Import System Manpage");
#else
	image=gtk_image_new_from_stock(GTK_STOCK_OPEN,GTK_ICON_SIZE_MENU);
	menuitem=gtk_image_menu_item_new_with_label("Import System Manpage");
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuitem,image);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(importManpage),(void*)1);

//preview
#ifdef _USEGTK3_
	previewMenu=gtk_menu_item_new_with_label("Preview Page");
#else
	image=gtk_image_new_from_stock(GTK_STOCK_FIND,GTK_ICON_SIZE_MENU);
	previewMenu=gtk_image_menu_item_new_with_label("Preview Page");
	gtk_image_menu_item_set_image((GtkImageMenuItem *)previewMenu,image);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),previewMenu);
	g_signal_connect(G_OBJECT(previewMenu),"activate",G_CALLBACK(previewPage),NULL);

//properties
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_label("Properties");
#else
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_PROPERTIES,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(redoProps),NULL);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

//printmanpage
#ifdef _USEGTK3_
	menuprint=gtk_menu_item_new_with_label("Print Manpage to LP");
#else
	image=gtk_image_new_from_stock(GTK_STOCK_PRINT,GTK_ICON_SIZE_MENU);
	menuprint=gtk_image_menu_item_new_with_label("Print Manpage to LP");
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuprint,image);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuprint);
	g_signal_connect(G_OBJECT(menuprint),"activate",G_CALLBACK(printFile),(void*)1);

//print manpage to pdf
#ifdef _USEGTK3_
	menuprint=gtk_menu_item_new_with_label("Print Manpage To PDF");
#else
	image=gtk_image_new_from_stock(GTK_STOCK_PRINT,GTK_ICON_SIZE_MENU);
	menuprint=gtk_image_menu_item_new_with_label("Print Manpage To PDF");
	gtk_image_menu_item_set_image((GtkImageMenuItem *)menuprint,image);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuprint);
	g_signal_connect(G_OBJECT(menuprint),"activate",G_CALLBACK(printFile),(void*)2);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

//close
#ifdef _USEGTK3_
	menuclose=gtk_menu_item_new_with_mnemonic("_Close");
#else
	menuclose=gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuclose);
	g_signal_connect(G_OBJECT(menuclose),"activate",G_CALLBACK(closePage),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuclose,"activate",accgroup,'W',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

//close section
#ifdef _USEGTK3_
	closeSectionMenu=gtk_menu_item_new_with_label("Delete Section");
#else
	image=gtk_image_new_from_stock(GTK_STOCK_DELETE,GTK_ICON_SIZE_MENU);
	closeSectionMenu=gtk_image_menu_item_new_with_label("Delete Section");
	gtk_image_menu_item_set_image((GtkImageMenuItem *)closeSectionMenu,image);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),closeSectionMenu);
	g_signal_connect(G_OBJECT(closeSectionMenu),"activate",G_CALLBACK(deleteSection),NULL);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
//quit
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_mnemonic("_Quit");
#else
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(doShutdown),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,'Q',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

//edit menu
	menuedit=gtk_menu_item_new_with_label("Edit");
	menu=gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuedit),menu);
//undo
#ifdef _USEGTK3_
	undoMenu=gtk_menu_item_new_with_mnemonic("_Undo");
#else
	undoMenu=gtk_image_menu_item_new_from_stock(GTK_STOCK_UNDO,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),undoMenu);
	g_signal_connect(G_OBJECT(undoMenu),"activate",G_CALLBACK(undo),NULL);
	gtk_widget_add_accelerator((GtkWidget *)undoMenu,"activate",accgroup,'Z',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

//redo
#ifdef _USEGTK3_
	redoMenu=gtk_menu_item_new_with_mnemonic("_Redo");
#else
	redoMenu=gtk_image_menu_item_new_from_stock(GTK_STOCK_REDO,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),redoMenu);
	g_signal_connect(G_OBJECT(redoMenu),"activate",G_CALLBACK(redo),NULL);
	gtk_widget_add_accelerator((GtkWidget *)redoMenu,"activate",accgroup,'Z',(GdkModifierType)(GDK_SHIFT_MASK|GDK_CONTROL_MASK),GTK_ACCEL_VISIBLE);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

//cut
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_mnemonic("_Cut");
#else
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_CUT,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(cutToClip),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,'X',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

//copy
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_mnemonic("_Copy");
#else
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(copyToClip),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,'C',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

//paste
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_mnemonic("_Paste");
#else
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(pasteFromClip),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,'V',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

//find
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_mnemonic("_Find");
#else
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_FIND,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(find),NULL);
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,'F',GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

	menuitem=gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);

//prefs
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_mnemonic("_Preferences");
#else
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_PREFERENCES,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(doPrefs),NULL);

//formattin menu
	menuformat=gtk_menu_item_new_with_label("Formating");
	menu=gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuformat),menu);
//bold
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_mnemonic("_Bold");
#else
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_BOLD,NULL);
#endif
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,GDK_KEY_B,(GdkModifierType)GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(doFormat),(void*)BOLD);
//italic
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_mnemonic("_Italic");
#else
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_ITALIC,NULL);
#endif
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,GDK_KEY_I,(GdkModifierType)GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(doFormat),(void*)ITALIC);
//normal
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_mnemonic("_Clear");
#else
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_CLEAR,NULL);
#endif
	gtk_widget_add_accelerator((GtkWidget *)menuitem,"activate",accgroup,GDK_KEY_R,(GdkModifierType)GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(doFormat),(void*)NORMAL);

//help
	menuhelp=gtk_menu_item_new_with_label("Help");
	menu=gtk_menu_new();
//about
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuhelp),menu);
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_mnemonic("_About");
#else
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT,NULL);
#endif
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(doAbout),NULL);
//help
#ifdef _USEGTK3_
	menuitem=gtk_menu_item_new_with_mnemonic("_Help");
#else
	menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP,NULL);
#endif
	g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(openHelp),NULL);
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

#ifdef _USEGTK3_
	findReplaceDialog=gtk_dialog_new_with_buttons("Find/Replace",(GtkWindow*)window, GTK_DIALOG_DESTROY_WITH_PARENT,"Forward",FINDNEXT,"Back",FINDPREV,"Replace",REPLACE,NULL);
#else
	findReplaceDialog=gtk_dialog_new_with_buttons("Find/Replace",(GtkWindow*)window, GTK_DIALOG_DESTROY_WITH_PARENT,GTK_STOCK_GO_FORWARD,FINDNEXT,GTK_STOCK_GO_BACK,FINDPREV,"Replace",REPLACE,NULL);
#endif
	gtk_dialog_set_default_response((GtkDialog*)findReplaceDialog,GTK_RESPONSE_OK);
	g_signal_connect(G_OBJECT(findReplaceDialog),"response",G_CALLBACK(doFindReplace),NULL);
	content_area=gtk_dialog_get_content_area(GTK_DIALOG(findReplaceDialog));

	vbox=creatNewBox(NEWVBOX,false,0);

	hbox=creatNewBox(NEWVBOX,false,0);

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
#ifndef _USEGTK3_
	image=gtk_image_new_from_stock(GTK_STOCK_FIND_AND_REPLACE,GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image((GtkButton*)replace,image);
#endif
	gtk_widget_show(findBox);
	gtk_widget_show(replaceBox);
	gtk_widget_show(vbox);
	gtk_widget_show(hbox);

//TODO
//	gtk_signal_connect_object(GTK_OBJECT(findReplaceDialog),"delete_event",GTK_SIGNAL_FUNC(gtk_widget_hide),GTK_OBJECT(findReplaceDialog));
//	g_signal_connect(G_OBJECT(findReplaceDialog),"delete_event",GTK_SIGNAL_FUNC(gtk_true),NULL);
}

#ifdef _ASPELL_
void buildWordCheck(int documentCheck)
{
#if 1
	GtkWidget*	vbox;
	GtkWidget*	button;
	GtkWidget*	hbox;
	GtkWidget*	image;
	char*		labeltext[512];
	int			docflag=documentCheck;

	spellCheckWord=gtk_dialog_new();
	gtk_window_set_title((GtkWindow*)spellCheckWord,"Spell check word");
	vbox=creatNewBox(NEWVBOX,true,8);
	hbox=creatNewBox(NEWHBOX,true,8);
//	vbox=gtk_vbox_new(true,8);
//
//	hbox=gtk_hbox_new(true,8);

	sprintf((char*)&labeltext,"Change <i><b>%s</b></i> to: ",badWord);
	badWordLabel=gtk_label_new((char*)&labeltext);
	gtk_label_set_use_markup((GtkLabel*)badWordLabel,true);
	gtk_box_pack_start(GTK_BOX(hbox),badWordLabel,true,true,0);

	wordListDropbox=gtk_combo_box_text_new();
	gtk_box_pack_start(GTK_BOX(hbox),wordListDropbox,true,true,0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,true,true,0);

	//hbox=gtk_hbox_new(true,8);
	hbox=creatNewBox(NEWHBOX,true,8);
#ifdef _USEGTK3_
	button=gtk_button_new_with_label("Apply");
#else
	button=gtk_button_new_from_stock(GTK_STOCK_APPLY);
#endif
	gtk_box_pack_start(GTK_BOX(hbox),button,true,true,0);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(doChangeWord),(gpointer)(long)docflag);


	button=gtk_button_new_with_label("Ignore");
//	image=gtk_image_new_from_stock(GTK_STOCK_ADD,GTK_ICON_SIZE_MENU);
//	gtk_button_set_image((GtkButton*)button,image);
	gtk_box_pack_start(GTK_BOX(hbox),button,true,true,0);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(doAddIgnoreWord),(gpointer)1);

#ifdef _USEGTK3_
	button=gtk_button_new_with_label("Add");
#else
	button=gtk_button_new_from_stock(GTK_STOCK_ADD);
#endif
	gtk_box_pack_start(GTK_BOX(hbox),button,true,true,0);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(doAddIgnoreWord),(gpointer)2);

#ifdef _USEGTK3_
	button=gtk_button_new_with_label("Cancel");
#else
	button=gtk_button_new_from_stock(GTK_STOCK_CANCEL);
#endif
	gtk_box_pack_start(GTK_BOX(hbox),button,true,true,0);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(doCancelCheck),NULL);

//	gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(),true,true,0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,true,true,0);

	GtkWidget* content=gtk_dialog_get_content_area((GtkDialog *)spellCheckWord);
	gtk_container_add(GTK_CONTAINER(content),(GtkWidget*)vbox);

//	gtk_signal_connect_object(GTK_OBJECT(spellCheckWord),"delete_event",GTK_SIGNAL_FUNC(gtk_widget_hide),GTK_OBJECT(spellCheckWord));
//	g_signal_connect(G_OBJECT(spellCheckWord),"delete_event",GTK_SIGNAL_FUNC(gtk_true),NULL);
#endif
}
#endif





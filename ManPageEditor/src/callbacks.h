/*
 *
 * ©K. D. Hedger. Fri 27 Nov 11:42:43 GMT 2015 keithdhedger@gmail.com

 * This file (callbacks.h) is part of ManPageEditor.

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

#include "globals.h"
#include "files.h"

#ifndef _CALLBACKS_
#define _CALLBACKS_

void doOpenFile(GtkWidget* widget,gpointer data);
void closeTab(GtkWidget* widget,gpointer data);
void setSensitive(void);
void copyToClip(GtkWidget* widget,gpointer data);
void cutToClip(GtkWidget* widget,gpointer data);
void pasteFromClip(GtkWidget* widget,gpointer data);
void undo(GtkWidget* widget,gpointer data);
void redo(GtkWidget* widget,gpointer data);

void openHelp(GtkWidget* widget,gpointer data);
void printSection(GtkWidget* widget,gpointer data);
void printFile(GtkWidget* widget,gpointer data);
void populatePopupMenu(GtkTextView *entry,GtkMenu *menu,gpointer user_data);
void tabPopUp(GtkWidget *my_widget, GdkEventButton *event,gpointer user_data);

void doShutdown(GtkWidget* widget,gpointer data);
void setPrefs(GtkWidget* widget,gpointer data);
void doAbout(GtkWidget* widget,gpointer data);
void writeExitData(void);
void closeAllTabs(GtkWidget* widget,gpointer data);
void showHideWidget(GtkWidget* widget,bool show);
void refreshMainWindow(void);
gboolean whatPane(GtkWidget *widget,GdkEvent *event,gpointer data);
void newEditor(GtkWidget* widget,gpointer data);
void doFormat(GtkWidget* widget,gpointer data);
void redoProps(GtkWidget* widget,gpointer data);
int yesNo(char* question,char* file);
void closePage(GtkWidget* widget,gpointer data);
void previewPage(GtkWidget* widget,gpointer data);
void reorderDirty(GtkNotebook *notebook,GtkWidget *child,guint page_num,gpointer user_data);

#endif

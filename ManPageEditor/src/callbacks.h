/*
 *
 * K.D.Hedger 2013 <kdhedger68713@gmail.com>
 *
 * callbacks.h
 *
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

/*
 *
 * K.D.Hedger 2013 <kdhedger68713@gmail.com>
 *
 * files.h
 *
*/

#ifndef _FILES_
#define _FILES_

void resetAllFilePrefs(void);

void exportFile(GtkWidget* widget,gpointer data);
void setFilePrefs(GtkSourceView* sourceview);

void newSection(GtkWidget* widget,gpointer data);
void newManpage(GtkWidget* widget,gpointer data);
void openManpage(GtkWidget* widget,gpointer data);
void saveManpage(GtkWidget* widget,gpointer data);
void saveAs(GtkWidget* widget,gpointer data);
void doOpenManpage(char* file);
void makeDirty(GtkWidget* widget,gpointer data);
void deleteSection(GtkWidget* widget,gpointer data);
void renameSection(GtkWidget* widget,gpointer data);
void importManpage(GtkWidget* widget,gpointer data);
void resetAllItalicTags(void);

#endif

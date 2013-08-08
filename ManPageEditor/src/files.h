/*
 *
 * K.D.Hedger 2013 <kdhedger68713@gmail.com>
 *
 * files.h
 *
*/
#ifndef _FILES_
#define _FILES_

bool openFile(const gchar *filepath,int linenumber);
void newFile(GtkWidget* widget,gpointer data);
void resetAllFilePrefs(void);

void reloadFile(GtkWidget* widget,gpointer data);
void exportFile(GtkWidget* widget,gpointer data);

void fileChangedOnDisk(GFileMonitor *monitor,GFile *file,GFile *other_file,GFileMonitorEvent event_type,gpointer user_data);
void setFilePrefs(GtkSourceView* sourceview);

void newSection(GtkWidget* widget,gpointer data);
void newManpage(GtkWidget* widget,gpointer data);
void openManpage(GtkWidget* widget,gpointer data);
void saveManpage(GtkWidget* widget,gpointer data);
void saveAs(GtkWidget* widget,gpointer data);

#endif

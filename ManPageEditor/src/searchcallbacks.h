/*
 *
 * K.D.Hedger 2013 <kdhedger68713@gmail.com>
 *
 * functioncallbacks.h
 *
*/

#include <gtk/gtk.h>
//#include <unique/unique.h>

#include "globals.h"
#include "files.h"

#ifndef _FUNCCALLBACKS_
#define _FUNCCALLBACKS_

void doFindReplace(GtkDialog *dialog,gint response_id,gpointer user_data);
void find(GtkWidget* widget,gpointer data);
void doSearchPrefs(GtkWidget* widget,gpointer data);
void docSearchFromBar(GtkWidget* widget,gpointer data);
void doLiveSearch(GtkWidget* widget,GdkEvent *event,gpointer data);

#endif
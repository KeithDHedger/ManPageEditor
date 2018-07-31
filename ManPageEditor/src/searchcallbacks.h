/*
 *
 * ©K. D. Hedger. Fri 27 Nov 11:45:59 GMT 2015 keithdhedger@gmail.com

 * This file (searchcallbacks.h) is part of ManPageEditor.

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

#ifndef _FUNCCALLBACKS_
#define _FUNCCALLBACKS_

void doFindReplace(GtkDialog *dialog,gint response_id,gpointer user_data);
void find(GtkWidget* widget,gpointer data);
void doSearchPrefs(GtkWidget* widget,gpointer data);
void docSearchFromBar(GtkWidget* widget,gpointer data);
void doLiveSearch(GtkWidget* widget,GdkEvent *event,gpointer data);

#endif
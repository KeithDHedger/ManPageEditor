/*
 *
 * ©K. D. Hedger. Fri 27 Nov 11:43:15 GMT 2015 keithdhedger@gmail.com

 * This file (files.h) is part of ManPageEditor.

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
bool doOpenManpage(char* file,bool warn=true);
void makeDirty(GtkWidget* widget,gpointer data);
void deleteSection(GtkWidget* widget,gpointer data);
void renameSection(GtkWidget* widget,gpointer data);
bool importManpage(GtkWidget* widget,gpointer data);
void resetAllItalicTags(void);

#endif

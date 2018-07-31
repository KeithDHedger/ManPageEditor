/*
 *
 * ©K. D. Hedger. Fri 27 Nov 11:46:53 GMT 2015 keithdhedger@gmail.com

 * This file (spellcheck.h) is part of ManPageEditor.

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

#ifndef _SPELLCHECK_
#define _SPELLCHECK_
#ifdef _ASPELL_

void checkWord(GtkWidget* widget,gpointer data);
void doChangeWord(GtkWidget* widget,gpointer data);
void doAddIgnoreWord(GtkWidget* widget,gpointer data);
void doSpellCheckDoc(GtkWidget* widget,gpointer data);
void doCancelCheck(GtkWidget* widget,gpointer data);

#endif
#endif

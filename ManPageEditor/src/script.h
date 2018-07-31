/*
 *
 * ©K. D. Hedger. Fri 27 Nov 11:45:42 GMT 2015 keithdhedger@gmail.com

 * This file (script.h) is part of ManPageEditor.

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

#ifndef _SCRIPT_
#define _SCRIPT_

char* slice(char* srcstring,int startchar,int endchar);
char* sliceBetween(char* srcstring,char* startstr,char* endstr);
char* sliceLen(char* srcstring,int tmpstartchar,int len);
char* sliceStrLen(char* srcstring,char* startstr,int len);
char* deleteSlice(char* srcstring,char* delstr);
char* sliceInclude(char* srcstring,char* startstr,char* endstr,bool includeFirst,bool includeLast);
void replaceAllSlice(char** srcstr,char* findstr,char* replacestr);
void replaceFirstSlice(char** srcstr,char* findstr,char* replacestr);
char* sliceCaseInclude(char* srcstring,char* startstr,char* endstr,bool includeFirst,bool includeLast);
void replaceAllCaseSlice(char** srcstr,char* findstr,char* replacestr);

#endif

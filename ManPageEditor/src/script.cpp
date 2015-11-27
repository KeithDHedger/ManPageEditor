/*
 *
 * ©K. D. Hedger. Fri 27 Nov 11:45:32 GMT 2015 kdhedger68713@gmail.com

 * This file (script.cpp) is part of ManPageEditor.

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

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <glib.h>

//string slicing

char* slice(char* srcstring,int tmpstartchar,int tmpendchar)
{
	char*	dest;
	int		strsize;
	int		startchar=tmpstartchar;
	int		endchar=tmpendchar;

	if(tmpstartchar<0)
		startchar=0;

	if((tmpendchar<0) || (tmpendchar>(int)strlen(srcstring)))
		endchar=strlen(srcstring)-1;

	strsize=endchar-startchar+1;

	dest=(char*)malloc(strsize+1);
	strncpy(dest,(char*)&srcstring[startchar],strsize);
	dest[strsize]=0;

	return(dest);
}

char* sliceBetween(char* srcstring,char* startstr,char* endstr)
{
	int		startchar;
	int		endchar;
	char*	ptr;
	char*	dest=NULL;

	ptr=strstr(srcstring,startstr);
	if(ptr==NULL)
		return(NULL);
	startchar=(int)(long)ptr+strlen(startstr)-(long)srcstring;

	ptr=strstr((char*)&srcstring[startchar],endstr);
	if(ptr==NULL)
		return(NULL);
	endchar=(int)(long)ptr-(long)srcstring-1;

	dest=slice(srcstring,startchar,endchar);
	return(dest);
}

char* sliceLen(char* srcstring,int tmpstartchar,int len)
{
	char*	dest;
	int		strsize;
	int		startchar=tmpstartchar;
	int		endchar=len;

	if(tmpstartchar<0)
		startchar=0;

	if((len<0) || (len+startchar>(int)strlen(srcstring)))
		endchar=strlen(srcstring)-startchar;

	strsize=endchar;

	dest=(char*)malloc(strsize+1);
	strncpy(dest,(char*)&srcstring[startchar],endchar);
	dest[endchar]=0;

	return(dest);

}

char* sliceStrLen(char* srcstring,char* startstr,int len)
{
	char*	ptr;
	int		startchar;

	ptr=strstr(srcstring,startstr);
	if(ptr==NULL)
		return(NULL);
	startchar=(int)(long)ptr+strlen(startstr)-(long)srcstring;
	printf("%i\n",startchar);
	return(sliceLen(srcstring,startchar,len));
}

char* deleteSlice(char* srcstring,char* delstr)
{
	GString*	str=g_string_new(srcstring);
	char*		ptr;

	ptr=strstr(str->str,delstr);
	g_string_erase(str,(long)ptr-(long)str->str,strlen(delstr));
	return(g_string_free(str,false));
}

char* sliceInclude(char* srcstring,char* startstr,char* endstr,bool includeFirst,bool includeLast)
{
	int		startchar;
	char*	ptr;
	char*	ptrend;
	int 	endoffset=0;
	int		numchars;
	GString*	gstr;
	char*		tptr;

	ptr=strstr(srcstring,startstr);
	if(ptr==NULL)
		return(NULL);

	if(includeFirst==true)
		startchar=0;
	else
		startchar=strlen(startstr);

	ptr=strstr((char*)srcstring,startstr);
	if(ptr==NULL)
		return(NULL);

	tptr=(char*)(long)ptr+strlen(startstr);
	ptrend=strstr((char*)tptr,endstr);

	if(includeLast==true)
		endoffset=strlen(endstr);
	else
		endoffset=0;

	numchars=(int)(long)ptrend-(long)ptr-startchar;
	gstr=g_string_new_len(&ptr[startchar],numchars+endoffset);
	
	return(g_string_free(gstr,false));
}

char* sliceCaseInclude(char* srcstring,char* startstr,char* endstr,bool includeFirst,bool includeLast)
{
	int		startchar;
	char*	ptr;
	char*	ptrend;
	int 	endoffset=0;
	int		numchars;
	GString*	gstr;
	char*		tptr;

	ptr=strcasestr(srcstring,startstr);
	if(ptr==NULL)
		return(NULL);

	if(includeFirst==true)
		startchar=0;
	else
		startchar=strlen(startstr);

	ptr=strcasestr((char*)srcstring,startstr);
	if(ptr==NULL)
		return(NULL);

	tptr=(char*)(long)ptr+strlen(startstr);
	ptrend=strcasestr((char*)tptr,endstr);

	if(includeLast==true)
		endoffset=strlen(endstr);
	else
		endoffset=0;

	numchars=(int)(long)ptrend-(long)ptr-startchar;
	gstr=g_string_new_len(&ptr[startchar],numchars+endoffset);
	
	return(g_string_free(gstr,false));
}

void replaceAllSlice(char** srcstr,char* findstr,char* replacestr)
{
	char*		found=NULL;
	int			startchar;
	GString*	deststr=g_string_new(*srcstr);

	g_free(*srcstr);
	found=strstr(deststr->str,findstr);
	while(found!=NULL)
		{
			startchar=(int)(long)found-(long)deststr->str;
			g_string_erase(deststr,startchar,strlen(findstr));
			g_string_insert(deststr,startchar,replacestr);
			found=strstr(deststr->str,findstr);
		}
	*srcstr=g_string_free(deststr,false);
}

void replaceAllCaseSlice(char** srcstr,char* findstr,char* replacestr)
{
	char*		found=NULL;
	int			startchar;
	GString*	deststr=g_string_new(*srcstr);

	g_free(*srcstr);
	found=strcasestr(deststr->str,findstr);
	while(found!=NULL)
		{
			startchar=(int)(long)found-(long)deststr->str;
			g_string_erase(deststr,startchar,strlen(findstr));
			g_string_insert(deststr,startchar,replacestr);
			found=strstr(deststr->str,findstr);
		}
	*srcstr=g_string_free(deststr,false);
}

void replaceFirstSlice(char** srcstr,char* findstr,char* replacestr)
{
	char*		found=NULL;
	int			startchar;
	GString*	deststr=g_string_new(*srcstr);

	g_free(*srcstr);
	found=strstr(deststr->str,findstr);
	if(found!=NULL)
		{
			startchar=(int)(long)found-(long)deststr->str;
			g_string_erase(deststr,startchar,strlen(findstr));
			g_string_insert(deststr,startchar,replacestr);
			found=strstr(deststr->str,findstr);
		}
	*srcstr=g_string_free(deststr,false);
}

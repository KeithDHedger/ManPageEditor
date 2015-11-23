/******************************************************
*
*     ©keithhedger Wed  7 Aug 11:56:27 BST 2013
*     kdhedger68713@gmail.com
*
*     script.h
* 
******************************************************/

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

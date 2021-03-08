/*
 *
 * ©K. D. Hedger. Fri 27 Nov 11:45:21 GMT 2015 keithdhedger@gmail.com

 * This file (main.cpp) is part of ManPageEditor.

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

#include <unistd.h>
#include <getopt.h>

#include "callbacks.h"
#include "guis.h"

void readConfig(void)
{
	FILE*	fd=NULL;
	char*	filename;
	char	buffer[1024];
	char	name[256];
	int		intarg;
	char	strarg[256];

	asprintf(&filename,"%s/.ManPageEditor/editor.rc",getenv("HOME"));
	fd=fopen(filename,"r");
	if(fd!=NULL)
		{
			while(feof(fd)==0)
				{
					fgets(buffer,1024,fd);
					sscanf(buffer,"%s %s",(char*)&name,(char*)&strarg);

					if(strcasecmp(name,"wrapline")==0)
						lineWrap=(bool)atoi(strarg);

					if(strcasecmp(name,"gzip")==0)
						gzipPages=(bool)atoi(strarg);

					if(strcasecmp(name,"highlightcurrentline")==0)
						highLight=(bool)atoi(strarg);

					if(strcasecmp(name,"useundeline")==0)
						useUnderline=(bool)atoi(strarg);

					if(strcasecmp(name,"insenssearch")==0)
						insensitiveSearch=(bool)atoi(strarg);

					if(strcasecmp(name,"wrapsearch")==0)
						wrapSearch=(bool)atoi(strarg);

					if(strcasecmp(name,"showlivesearch")==0)
						showLiveSearch=(bool)atoi(strarg);

					if(strcasecmp(name,"tabwidth")==0)
						tabWidth=atoi(strarg);

					if(strcasecmp(name,"font")==0)
						{
							sscanf(buffer,"%*s %s %i",(char*)&strarg,(int*)&intarg);
							asprintf(&fontAndSize,"%s %i",strarg,intarg);
						}
					if(strcasecmp(name,"terminalcommand")==0)
						{
							g_free(terminalCommand);
							sscanf(buffer,"%*s %" VALIDCHARS "s",(char*)&strarg);
							terminalCommand=strdup(strarg);
						}
				}
			fclose(fd);
		}

	g_free(filename);
	asprintf(&filename,"%s/.ManPageEditor/editor.window.rc",getenv("HOME"));
	fd=fopen(filename,"r");
	if(fd!=NULL)
		{
			while(feof(fd)==0)
				{
					fgets(buffer,1024,fd);
					sscanf(buffer,"%s %s",(char*)&name,(char*)&strarg);

					if(strcasecmp(name,"windowsize")==0)
						sscanf(buffer,"%*s %i %i %i %i",(int*)&windowWidth,(int*)&windowHeight,(int*)&windowX,(int*)&windowY);
				}
			fclose(fd);
		}
	g_free(filename);
}

void init(void)
{
	char*				filename;
#ifdef _ASPELL_
	AspellCanHaveError*	possible_err;
#endif
	lineWrap=true;
	highLight=true;
	useUnderline=true;

	tabWidth=4;
	fontAndSize=strdup("mono 10");
	terminalCommand=strdup("xterm -e");
	windowWidth=800;
	windowHeight=400;
	windowX=-1;
	windowY=-1;
	wrapSearch=true;
	insensitiveSearch=true;
	replaceAll=false;
	showLiveSearch=true;
	gzipPages=false;

	asprintf(&filename,"%s/.ManPageEditor",getenv("HOME"));
	g_mkdir_with_parents(filename,493);
	g_free(filename);

	readConfig();

	tmpGzipPages=gzipPages;
	tmpHighLight=highLight;
	tmpLineWrap=lineWrap;
	tmpTabWidth=tabWidth;
	tmpUseUnderline=useUnderline;
	tmpShowLiveSearch=showLiveSearch;

#ifdef _ASPELL_
	aspellConfig=new_aspell_config();
	possible_err=new_aspell_speller(aspellConfig);

	if(aspell_error_number(possible_err)!= 0)
		puts(aspell_error_message(possible_err));
	else
		spellChecker=to_aspell_speller(possible_err);
#endif

}

struct option		long_options[] =
{
					{"import",1,0,'i'},
					{"section",1,0,'s'},
					{"version",0,0,'v'},
					{"help",0,0,'?'},
					{0,0,0,0}
};

void printHelp(void)
{
	printf( PACKAGE_STRING "\n" COPYRITE "\n" MYEMAIL "\n\n");
	printf("-i, --import NAME: Import system manpage NAME\n");
	printf("-s, --section SECTION: Specify section SECTION\n");
	printf("-v, --version: Print version info and exit\n");
	printf("-?, --help: Print quick help\n\n");
}

int main(int argc,char **argv)
{
	int		c=-1;
	bool	flag=true;
	long	section=0;
	char	*manname=NULL;

	gtk_init(&argc,&argv);

	init();

	buildMainGui();
	buildFindReplace();

	while (1)
		{
			int option_index=0;
			c=getopt_long (argc,argv,"v?hi:s:",long_options,&option_index);
			if (c==-1)
				break;

			switch (c)
				{
					case 'v':
						printf( PACKAGE_STRING "\n" COPYRITE "\n" MYEMAIL "\n");
						return(0);
						break;
					case 's':
						section=atoi(optarg);
						break;
					case 'i':
						manname=optarg;
						break;
					case '?':
						printHelp();
						return(0);
						break;
				}
		}

	if(manname==NULL)
		{
			if(argc>1)
				if(!doOpenManpage(argv[1],false))
					{
						if(importManpage((GtkWidget*)section,(gpointer)argv[1])==false)
							{
								GtkWidget	*dialog;
								dialog=gtk_message_dialog_new((GtkWindow*)window,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Can't open/import '%s' ...",argv[1]);
								gtk_dialog_run(GTK_DIALOG(dialog));
								gtk_widget_destroy(dialog);
							}
					}
		}
	else
		{
			importManpage((GtkWidget*)section,(gpointer)manname);
		}

	dirty=false;
	setSensitive();
	refreshMainWindow();

	gtk_window_set_default_icon_name(PACKAGE);
	gtk_window_set_icon_name((GtkWindow*)window,PACKAGE);
	gtk_main();
}

/*
 *
 * K.D.Hedger 2013 <kdhedger68713@gmail.com>
 *
*/

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
							sscanf(buffer,"%*s %"VALIDCHARS"s",(char*)&strarg);
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

int main(int argc,char **argv)
{
	gtk_init(&argc,&argv);

	init();

	buildMainGui();
	buildFindReplace();

	if(argc>1)
		doOpenManpage(argv[1]);

	dirty=false;
	setSensitive();
	refreshMainWindow();

	gtk_window_set_default_icon_name(PACKAGE);
	gtk_window_set_icon_name((GtkWindow*)window,PACKAGE);
	gtk_main();
}

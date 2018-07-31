/*
 *
 * ©K. D. Hedger. Fri 27 Nov 11:45:51 GMT 2015 keithdhedger@gmail.com

 * This file (searchcallbacks.cpp) is part of ManPageEditor.

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

#include <gtksourceview/gtksourceview.h>

#include "globals.h"

void doFindReplace(GtkDialog *dialog,gint response_id,gpointer user_data)
{
	pageStruct* 			page=getPageStructPtr(-1);
	gchar*					selectedtext=NULL;
#ifdef _USEGTK3_
	GtkTextSearchFlags		flags=GTK_TEXT_SEARCH_TEXT_ONLY;
#else
	GtkSourceSearchFlags	flags=GTK_SOURCE_SEARCH_TEXT_ONLY;
#endif
	char*					searchtext;
	char*					replacetext;
	bool					replaceAllFlag;

	gtk_text_buffer_begin_user_action((GtkTextBuffer*)page->buffer);

	if(insensitiveSearch==true)
#ifdef _USEGTK3_
		flags=(GtkTextSearchFlags)(GTK_TEXT_SEARCH_TEXT_ONLY|GTK_TEXT_SEARCH_CASE_INSENSITIVE);
#else
		flags=(GtkSourceSearchFlags)(GTK_SOURCE_SEARCH_TEXT_ONLY|GTK_SOURCE_SEARCH_CASE_INSENSITIVE);
#endif

	searchtext=g_strcompress(gtk_entry_get_text((GtkEntry*)findBox));
	replacetext=g_strcompress(gtk_entry_get_text((GtkEntry*)replaceBox));

	switch (response_id)
		{
//forward search
		case FINDNEXT:

			if(!gtk_text_buffer_get_selection_bounds((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end))
				gtk_text_buffer_get_iter_at_mark((GtkTextBuffer*)page->buffer,&page->iter,gtk_text_buffer_get_insert((GtkTextBuffer*)page->buffer));

#ifdef _USEGTK3_
			if(gtk_text_iter_forward_search(&page->match_end,searchtext,flags,&page->match_start,&page->match_end,NULL))
#else
			if(gtk_source_iter_forward_search(&page->match_end,searchtext,flags,&page->match_start,&page->match_end,NULL))
#endif
				{
					gtk_text_buffer_select_range((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end);
					scrollToIterInPane(page,&page->match_start);
					page->iter=page->match_end;
				}
			else
				{
					if(wrapSearch==true)
						{
							gtk_text_buffer_get_start_iter((GtkTextBuffer*)page->buffer,&page->iter);
#ifdef _USEGTK3_
							if(gtk_text_iter_forward_search(&page->iter,searchtext,flags,&page->match_start,&page->match_end,NULL))
#else
							if(gtk_source_iter_forward_search(&page->iter,searchtext,flags,&page->match_start,&page->match_end,NULL))
#endif
								{
									gtk_text_buffer_select_range((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end);
									scrollToIterInPane(page,&page->match_start);
									page->iter=page->match_end;
								}
						}
				}

			break;

//backward search
		case FINDPREV:
			if(!gtk_text_buffer_get_selection_bounds((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end))
				gtk_text_buffer_get_iter_at_mark((GtkTextBuffer*)page->buffer,&page->iter,gtk_text_buffer_get_insert((GtkTextBuffer*)page->buffer));
#ifdef _USEGTK3_
			if(gtk_text_iter_backward_search(&page->match_start,searchtext,flags,&page->match_start,&page->match_end,NULL))
#else
			if(gtk_source_iter_backward_search(&page->match_start,searchtext,flags,&page->match_start,&page->match_end,NULL))
#endif
				{
					gtk_text_buffer_select_range((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end);
					scrollToIterInPane(page,&page->match_start);
					page->iter=page->match_start;
				}
			else
				{
					if(wrapSearch==true)
						{
							gtk_text_buffer_get_end_iter((GtkTextBuffer*)page->buffer,&page->iter);
#ifdef _USEGTK3_
							if(gtk_text_iter_backward_search(&page->iter,searchtext,flags,&page->match_start,&page->match_end,NULL))
#else
							if(gtk_source_iter_backward_search(&page->iter,searchtext,flags,&page->match_start,&page->match_end,NULL))
#endif
								{
									gtk_text_buffer_select_range((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end);
									scrollToIterInPane(page,&page->match_start);
									page->iter=page->match_start;
								}
						}
				}

			break;
//replace and search
		case REPLACE:
			if(replaceAll==true)
				{
					replaceAllFlag=true;
					gtk_text_buffer_get_start_iter((GtkTextBuffer*)page->buffer,&page->iter);
					gtk_text_buffer_place_cursor(GTK_TEXT_BUFFER(page->buffer),&page->iter);
#ifdef _USEGTK3_
					if(gtk_text_iter_forward_search(&page->iter,searchtext,flags,&page->match_start,&page->match_end,NULL))
#else
					if(gtk_source_iter_forward_search(&page->iter,searchtext,flags,&page->match_start,&page->match_end,NULL))
#endif
						{
							gtk_text_buffer_select_range((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end);
							scrollToIterInPane(page,&page->match_start);
							page->iter=page->match_end;
						}
					else
						break;
				}
			else
				replaceAllFlag=false;
			do
				{
					if(gtk_text_buffer_get_selection_bounds((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end))
						{
							selectedtext=gtk_text_buffer_get_text((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end,false);


							if((insensitiveSearch==true && strcasecmp(selectedtext,searchtext)==0) ||(insensitiveSearch==false && strcmp(selectedtext,searchtext)==0))
								{
									gtk_text_buffer_delete((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end);
									gtk_text_buffer_insert((GtkTextBuffer*)page->buffer,&page->match_start,replacetext,-1);
									gtk_text_buffer_get_iter_at_mark((GtkTextBuffer*)page->buffer,&page->iter,gtk_text_buffer_get_insert((GtkTextBuffer*)page->buffer));
#ifdef _USEGTK3_
									if(gtk_text_iter_forward_search(&page->iter,searchtext,flags,&page->match_start,&page->match_end,NULL))
#else
									if(gtk_source_iter_forward_search(&page->iter,searchtext,flags,&page->match_start,&page->match_end,NULL))
#endif
										{
											gtk_text_buffer_select_range((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end);
											scrollToIterInPane(page,&page->match_start);
											page->iter=page->match_end;
										}
									else
										{
											replaceAllFlag=false;
										}
								}
							if(selectedtext!=NULL)
								g_free(selectedtext);
						}
				}
			while(replaceAllFlag==true);
			break;

		default:
			gtk_text_buffer_get_iter_at_mark((GtkTextBuffer*)page->buffer,&page->iter,gtk_text_buffer_get_insert((GtkTextBuffer*)page->buffer));
			page->isFirst=true;
			page->match_start=page->iter;
			page->match_end=page->iter;
			break;
		}
	gtk_text_buffer_end_user_action((GtkTextBuffer*)page->buffer);
	g_free(searchtext);
	g_free(replacetext);
}

void find(GtkWidget* widget,gpointer data)
{
	gtk_widget_show(findReplaceDialog);
	gtk_dialog_run((GtkDialog *)findReplaceDialog);
}

void doSearchPrefs(GtkWidget* widget,gpointer data)
{
	GtkWidget*	button;

	switch ((long)data)
		{
		case 1:
			insensitiveSearch=gtk_toggle_button_get_active((GtkToggleButton*)widget);
			break;
		case 2:
			wrapSearch=gtk_toggle_button_get_active((GtkToggleButton*)widget);
			break;
		case 3:
			replaceAll=gtk_toggle_button_get_active((GtkToggleButton*)widget);
			button=gtk_dialog_get_widget_for_response((GtkDialog*)findReplaceDialog,100);
			if(replaceAll==false)
				gtk_button_set_label((GtkButton*)button,"Replace");
			else
				gtk_button_set_label((GtkButton*)button,"Replace All");
			break;
		}
}

void doLiveSearch(GtkWidget* widget,GdkEvent *event,gpointer data)
{
	pageStruct* 			page=getPageStructPtr(-1);
#ifdef _USEGTK3_
	GtkTextSearchFlags		flags=GTK_TEXT_SEARCH_TEXT_ONLY;
#else
	GtkSourceSearchFlags	flags=GTK_SOURCE_SEARCH_TEXT_ONLY;
#endif
	char*					searchtext;
	int						modkey=((GdkEventKey*)event)->state & GDK_SHIFT_MASK;

	gtk_text_buffer_begin_user_action((GtkTextBuffer*)page->buffer);

#ifdef _USEGTK3_
	flags=(GtkTextSearchFlags)(GTK_TEXT_SEARCH_TEXT_ONLY|GTK_TEXT_SEARCH_CASE_INSENSITIVE);
#else
	flags=(GtkSourceSearchFlags)(GTK_SOURCE_SEARCH_TEXT_ONLY|GTK_SOURCE_SEARCH_CASE_INSENSITIVE);
#endif

	searchtext=g_strcompress(gtk_entry_get_text((GtkEntry*)widget));
	if(modkey==0)
		{
			if(!gtk_text_buffer_get_selection_bounds((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end))
				gtk_text_buffer_get_iter_at_mark((GtkTextBuffer*)page->buffer,&page->iter,gtk_text_buffer_get_insert((GtkTextBuffer*)page->buffer));

#ifdef _USEGTK3_
			if(gtk_text_iter_forward_search(&page->match_end,searchtext,flags,&page->match_start,&page->match_end,NULL))
#else
			if(gtk_source_iter_forward_search(&page->match_end,searchtext,flags,&page->match_start,&page->match_end,NULL))
#endif
				{
					gtk_text_buffer_select_range((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end);
					scrollToIterInPane(page,&page->match_start);
					page->iter=page->match_end;
				}
			else
				{
					gtk_text_buffer_get_start_iter((GtkTextBuffer*)page->buffer,&page->iter);
#ifdef _USEGTK3_
					if(gtk_text_iter_forward_search(&page->iter,searchtext,flags,&page->match_start,&page->match_end,NULL))
#else
					if(gtk_source_iter_forward_search(&page->iter,searchtext,flags,&page->match_start,&page->match_end,NULL))
#endif
						{
							gtk_text_buffer_select_range((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end);
							scrollToIterInPane(page,&page->match_start);
							page->iter=page->match_end;
						}
				}
		}
	else
		{
			if(!gtk_text_buffer_get_selection_bounds((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end))
				gtk_text_buffer_get_iter_at_mark((GtkTextBuffer*)page->buffer,&page->iter,gtk_text_buffer_get_insert((GtkTextBuffer*)page->buffer));
#ifdef _USEGTK3_
			if(gtk_text_iter_backward_search(&page->match_start,searchtext,flags,&page->match_start,&page->match_end,NULL))
#else
			if(gtk_source_iter_backward_search(&page->match_start,searchtext,flags,&page->match_start,&page->match_end,NULL))
#endif
				{
					gtk_text_buffer_select_range((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end);
					scrollToIterInPane(page,&page->match_start);
					page->iter=page->match_start;
				}
			else
				{
					gtk_text_buffer_get_end_iter((GtkTextBuffer*)page->buffer,&page->iter);
#ifdef _USEGTK3_
					if(gtk_text_iter_backward_search(&page->iter,searchtext,flags,&page->match_start,&page->match_end,NULL))
#else
					if(gtk_source_iter_backward_search(&page->iter,searchtext,flags,&page->match_start,&page->match_end,NULL))
#endif
						{
							gtk_text_buffer_select_range((GtkTextBuffer*)page->buffer,&page->match_start,&page->match_end);
							scrollToIterInPane(page,&page->match_start);
							page->iter=page->match_start;
						}
				}
		}
	gtk_text_buffer_end_user_action((GtkTextBuffer*)page->buffer);
}


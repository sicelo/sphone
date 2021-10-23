/*
 * sphone
 * Copyright (C) Ahmed Abdel-Hamid 2010 <ahmedam@mail.usa.com>
 * 
 * sphone is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * sphone is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ctype.h>
#include <gtk/gtk.h>

#ifdef ENABLE_LIBHILDON
#include <hildon/hildon-gtk.h>
#include <hildon/hildon-pannable-area.h>
#endif

#include "datapipes.h"
#include "datapipe.h"
#include "comm.h"
#include "types.h"
#include "sphone-log.h"
#include "keypad.h"
#include "utils.h"
#include "gui-dialer.h"
#include "gui-contact-view.h"
#include "sphone-store-tree-model.h"

struct{
	GtkWidget *display;
	GtkWidget *main_window;
	GtkWidget *dials_view;
	GtkWidget *book;
	GtkWidget *container_book_landscape;
	GtkWidget *container_book_portrait;
	int is_portrait;
}g_gui_calls;

static int gui_dialer_book_update_model(void* user_data)
{
	(void)user_data;
	sphone_log(LL_DEBUG, "gui_dialer_book_update_model\n");
	gboolean isvisible;
	g_object_get(G_OBJECT(g_gui_calls.main_window),"visible",&isvisible,NULL);
	//if(!gtk_widget_get_visible(g_gui_calls.main_window))
	if(!isvisible)
		return FALSE;
	const gchar *dial=gtk_entry_get_text(GTK_ENTRY(g_gui_calls.display));
	
	SphoneStoreTreeModel *dials_store = sphone_store_tree_model_new(&SPHONE_STORE_TREE_MODEL_FILTER_MATCH_NAME_DIAL_FUZY, dial);

	gtk_tree_view_set_model(GTK_TREE_VIEW(g_gui_calls.dials_view), GTK_TREE_MODEL(dials_store));
	g_object_unref(G_OBJECT(dials_store));

	return FALSE;
}


static void gui_dialer_book_update_model_delayed(void)
{
	static int gui_dialer_book_timeout_source_id=-1;
	if(gui_dialer_book_timeout_source_id>-1)
		g_source_remove(gui_dialer_book_timeout_source_id);
	gui_dialer_book_timeout_source_id = g_timeout_add_seconds(1,(GSourceFunc)gui_dialer_book_update_model, NULL);
}

static void gui_dialer_book_delete_model(void)
{
	gtk_tree_view_set_model(GTK_TREE_VIEW(g_gui_calls.dials_view), NULL);
}

static void gui_call_callback(GtkButton button)
{
	(void)button;

	const gchar *dial=gtk_entry_get_text(GTK_ENTRY(g_gui_calls.display));

	CallProperties *call = g_malloc0(sizeof(*call));
	call->line_identifier = g_strdup(dial);
	call->backend = sphone_comm_default_backend()->id;
	call->state = SPHONE_CALL_DIALING;
	execute_datapipe(&call_dial_pipe, call);
	call_properties_free(call);

	gtk_entry_set_text(GTK_ENTRY(g_gui_calls.display),"");
	gtk_widget_hide(g_gui_calls.main_window);
}

static void gui_dialer_cancel_callback(void)
{
	gtk_entry_set_text(GTK_ENTRY(g_gui_calls.display),"");
	gtk_widget_hide(g_gui_calls.main_window);
}

static void gui_dialer_back_presses_callback(GtkWidget *button, GtkWidget *target)
{
	(void)button;
	gtk_editable_set_position(GTK_EDITABLE(target),-1);
	gint position=gtk_editable_get_position(GTK_EDITABLE(target));
	gtk_editable_delete_text (GTK_EDITABLE(target),position-1, position);

	gtk_editable_set_position(GTK_EDITABLE(target),position);
}

static void gui_dialer_book_focus_callback(void)
{
	gtk_widget_grab_focus(g_gui_calls.display);
	gtk_editable_set_position(GTK_EDITABLE(g_gui_calls.display),-1);
}

static void gui_dialer_book_click_callback(GtkTreeView *view, gpointer func_data)
{
	(void)func_data;
	GtkTreePath *path;
	gtk_tree_view_get_cursor(view,&path,NULL);
	sphone_log(LL_DEBUG, "gui_dialer_book_click_callback\n");
	if(path){
		GtkTreeModel *model=gtk_tree_view_get_model (view);
		GtkTreeIter iter;
		GValue value={0};
		gtk_tree_model_get_iter(GTK_TREE_MODEL(model),&iter,path);
		gtk_tree_path_free(path);
		gtk_tree_model_get_value(model,&iter,SPHONE_STORE_TREE_MODEL_COLUMN_DIAL,&value);
		const gchar *dial=g_value_get_string(&value);
		gtk_entry_set_text(GTK_ENTRY(g_gui_calls.display),dial);
		g_value_unset(&value);
	}
}

static void gui_dialer_book_double_click_callback(GtkTreeView *view, gpointer func_data)
{
	(void)func_data;

	GtkTreePath *path;
	gtk_tree_view_get_cursor(view,&path,NULL);
	sphone_log(LL_DEBUG, "gui_dialer_book_double_click_callback\n");
	if(path){
		GtkTreeModel *model=gtk_tree_view_get_model (view);
		GtkTreeIter iter;
		GValue value={0};
		gtk_tree_model_get_iter(GTK_TREE_MODEL(model),&iter,path);
		gtk_tree_path_free(path);
		gtk_tree_model_get_value(model,&iter,SPHONE_STORE_TREE_MODEL_COLUMN_DIAL,&value);
		const gchar *dial=g_value_get_string(&value);
		if(!gui_contact_open_by_dial(dial))
			gui_dialer_cancel_callback();
		g_value_unset(&value);
	}
}

static GtkWidget *gui_dialer_build_book(void)
{
	GtkWidget *scroll;
	g_gui_calls.dials_view = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(g_gui_calls.dials_view),FALSE);
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	renderer = gtk_cell_renderer_pixbuf_new();
	column = gtk_tree_view_column_new_with_attributes("Photo", renderer, "pixbuf", SPHONE_STORE_TREE_MODEL_COLUMN_PICTURE, NULL);
	gtk_tree_view_column_set_sizing(column,GTK_TREE_VIEW_COLUMN_FIXED);
	gtk_tree_view_column_set_min_width(column,40);
	gtk_tree_view_append_column(GTK_TREE_VIEW(g_gui_calls.dials_view), column);

	renderer = gtk_cell_renderer_text_new();
	g_object_set(G_OBJECT(renderer), "ellipsize", PANGO_ELLIPSIZE_END, NULL);
	column = gtk_tree_view_column_new_with_attributes("Name", renderer, "text", SPHONE_STORE_TREE_MODEL_COLUMN_NAME, NULL);
	gtk_tree_view_column_set_sizing(column,GTK_TREE_VIEW_COLUMN_FIXED);
	gtk_tree_view_column_set_expand(column,TRUE);
	gtk_tree_view_column_set_min_width(column,100);
	gtk_tree_view_append_column(GTK_TREE_VIEW(g_gui_calls.dials_view), column);

	renderer = gtk_cell_renderer_text_new();
	g_object_set(G_OBJECT(renderer), "ellipsize", PANGO_ELLIPSIZE_END, NULL);
	column = gtk_tree_view_column_new_with_attributes("Dial", renderer, "text", SPHONE_STORE_TREE_MODEL_COLUMN_DIAL, NULL);
	gtk_tree_view_column_set_sizing(column,GTK_TREE_VIEW_COLUMN_FIXED);
	gtk_tree_view_column_set_expand(column,TRUE);
	gtk_tree_view_column_set_min_width(column,100);
	gtk_tree_view_append_column(GTK_TREE_VIEW(g_gui_calls.dials_view), column);

	gtk_tree_view_set_fixed_height_mode(GTK_TREE_VIEW(g_gui_calls.dials_view),TRUE);
#ifdef ENABLE_LIBHILDON
	scroll = hildon_pannable_area_new();
#else
	scroll = gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
#endif
	gtk_widget_set_size_request(GTK_WIDGET(scroll), 0, 200);
	gtk_container_add (GTK_CONTAINER(scroll),g_gui_calls.dials_view);

	g_signal_connect(G_OBJECT(g_gui_calls.dials_view),"focus-in-event", G_CALLBACK(gui_dialer_book_focus_callback),NULL);
	g_signal_connect_after(G_OBJECT(g_gui_calls.dials_view),"cursor-changed", G_CALLBACK(gui_dialer_book_click_callback),NULL);
	g_signal_connect_after(G_OBJECT(g_gui_calls.dials_view),"row-activated", G_CALLBACK(gui_dialer_book_double_click_callback),NULL);
	g_signal_connect(G_OBJECT(g_gui_calls.main_window),"unmap-event", G_CALLBACK(gui_dialer_book_delete_model),NULL);
	g_signal_connect(G_OBJECT(g_gui_calls.display),"changed", G_CALLBACK(gui_dialer_book_update_model_delayed),NULL);

	return scroll;
}

static void gui_dialer_validate_callback(GtkEntry *entry,const gchar *text,gint length,gint *position,gpointer data)
{
	int i, count=0;
	gchar *result = g_new (gchar, length);

	for (i=0; i < length; i++) {
		if (!isdigit(text[i]) && text[i]!='*' && text[i]!='#'  && text[i]!='+')
			continue;
		result[count++] = text[i];
	}

	if (count > 0) {
		GtkEditable *editable=GTK_EDITABLE(entry);
		g_signal_handlers_block_by_func (G_OBJECT (editable),	G_CALLBACK (gui_dialer_validate_callback),data);
		gtk_editable_insert_text (editable, result, count, position);
		g_signal_handlers_unblock_by_func (G_OBJECT (editable),	G_CALLBACK (gui_dialer_validate_callback),data);
	}
	g_signal_stop_emission_by_name (G_OBJECT(entry), "insert_text");

	g_free (result);
}

void gui_dialer_init(void)
{
	g_gui_calls.main_window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(g_gui_calls.main_window),"Dialer");
	gtk_window_set_deletable(GTK_WINDOW(g_gui_calls.main_window),FALSE);
	gtk_window_set_default_size(GTK_WINDOW(g_gui_calls.main_window),400,220);
	gtk_window_maximize(GTK_WINDOW(g_gui_calls.main_window));
	GtkWidget *v1=gtk_vbox_new(FALSE, 5);
	GtkWidget *book_dial_area=gtk_vpaned_new();
	GtkWidget *actions_bar=gtk_hbox_new(TRUE,0);
	GtkWidget *display_back=gtk_button_new_with_label ("\n    <    \n");
	GtkWidget *display=gtk_entry_new();
	GtkWidget *display_bar=gtk_hbox_new(FALSE,4);
	GtkWidget *keypad=gui_keypad_setup(display);
	GtkWidget *call_button=gtk_button_new_with_label("\nCall\n");
	GtkWidget *cancel_button=gtk_button_new_with_label("\nCancel\n");
	GdkColor white, black;
	GtkWidget *e=gtk_event_box_new ();
	g_gui_calls.display=display;
	GtkWidget *book = gui_dialer_build_book();
	
#ifdef ENABLE_LIBHILDON
	hildon_gtk_window_set_portrait_flags(GTK_WINDOW(g_gui_calls.main_window), HILDON_PORTRAIT_MODE_REQUEST);
#endif
	
	gtk_widget_modify_font(display,pango_font_description_from_string("Monospace 24"));
	gtk_entry_set_alignment(GTK_ENTRY(display),1.0);
	gtk_entry_set_has_frame(GTK_ENTRY(display),FALSE);
		
	gdk_color_parse ("black",&black);
	gdk_color_parse ("white",&white);
	
	gtk_widget_modify_bg(e,GTK_STATE_NORMAL,&black);
	
	gtk_container_add(GTK_CONTAINER(e), display);
	gtk_box_pack_start(GTK_BOX(display_bar), e, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(display_bar), display_back, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(v1),display_bar,FALSE, FALSE, 0);
	gtk_paned_pack1(GTK_PANED(book_dial_area), book, TRUE, TRUE);
	gtk_paned_pack2(GTK_PANED(book_dial_area), keypad, TRUE, TRUE);
	gtk_box_pack_start(GTK_BOX(v1),book_dial_area,TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(actions_bar), call_button);
	gtk_container_add(GTK_CONTAINER(actions_bar), cancel_button);
	gtk_box_pack_start(GTK_BOX(v1),actions_bar,FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(g_gui_calls.main_window), v1);

	gtk_widget_show_all(v1);
	gtk_widget_grab_focus(display);
	
	g_signal_connect(G_OBJECT(call_button),"clicked", G_CALLBACK(gui_call_callback),NULL);
	g_signal_connect(G_OBJECT(cancel_button),"clicked", G_CALLBACK(gui_dialer_cancel_callback),NULL);
	g_signal_connect(G_OBJECT(g_gui_calls.main_window),"delete-event", G_CALLBACK(gtk_widget_hide_on_delete),NULL);
	g_signal_connect(G_OBJECT(display_back), "clicked", G_CALLBACK(gui_dialer_back_presses_callback), display);
	g_signal_connect(G_OBJECT(display), "insert_text", G_CALLBACK(gui_dialer_validate_callback),NULL);
}

void gui_dialer_show(const gchar *dial)
{
	gtk_window_present(GTK_WINDOW(g_gui_calls.main_window));
	gtk_widget_grab_focus(g_gui_calls.display);

	gui_dialer_book_update_model(NULL);
	if(dial)
		gtk_entry_set_text(GTK_ENTRY(g_gui_calls.display), dial);
}

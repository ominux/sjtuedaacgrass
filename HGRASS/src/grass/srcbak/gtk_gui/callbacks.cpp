#include"callbacks.h"
#include"curve.h"
#include"buffer.h"
#include<glade/glade.h>
#include<gtk/gtk.h>
#include<cstdio>
#include<cstdlib>
#include<cassert>
#include<fstream>
using std::printf;
using std::ofstream;
using std::strtod;

extern ofstream freqResonse2Dfile;

// Callback functions
namespace callbacks
{
extern "C"
{
void custom_quit(GtkWidget *widget, gpointer user_data)
{
	g_print("Custom quit + gtk_main_quit\n");
	gtk_main_quit();
}

gboolean on_magnitude_drawingarea_expose_event(
	GtkWidget *widget,
	GdkEventExpose *event,
	gpointer user_data)
{
	assert(widget);
	assert(event);
	assert(user_data);
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	pbuffer->draw_magnitude_log_dB();
	pbuffer->refresh_magnitude_buffers();
	pbuffer->recover_marks_on_magnitude();
	return TRUE;
}

gboolean on_phase_drawingarea_expose_event(
	GtkWidget *widget,
	GdkEventExpose *event,
	gpointer user_data)
{
	assert(widget);
	assert(event);
	assert(user_data);
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	pbuffer->draw_phase_log_linear();
	pbuffer->refresh_phase_buffers();
	pbuffer->recover_marks_on_phase();
	return TRUE;
}

gboolean on_magnitude_drawingarea_configure_event(
	GtkWidget *widget,
	GdkEventConfigure *event,
	gpointer user_data)
{
	assert(widget);
	assert(event);
	assert(user_data);
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	pbuffer->init_magnitude_buffers(widget);
	pbuffer->refresh_magnitude_buffers();
	pbuffer->recover_marks_on_magnitude();
	return TRUE;
}

gboolean on_phase_drawingarea_configure_event(
	GtkWidget *widget,
	GdkEventConfigure *event,
	gpointer user_data)
{
	assert(widget);
	assert(event);
	assert(user_data);
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	pbuffer->init_phase_buffers(widget);
	pbuffer->refresh_phase_buffers();
	pbuffer->recover_marks_on_phase();
	return TRUE;
}

inline void on_motion_notify_event(
	GtkWidget *widget,
	GdkEventMotion *event,
	gint *ptr_x,
	gint *ptr_y)
{
	assert(widget);
	assert(event);
	GdkModifierType state;
	if (event->is_hint)
	{
		gdk_window_get_pointer(event->window, ptr_x, ptr_y, &state);
	}
	else
	{
		*ptr_x = static_cast<gint>(event->x);
		*ptr_y = static_cast<gint>(event->y);
		// state = static_cast<GdkModifierType>(event->state);
	}
}

inline void refresh_buffers_with_marks(gint x, Buffer *pbuffer)
{
	assert(pbuffer);
	// Clear last marks on both drawables
	pbuffer->refresh_magnitude_buffers();
	pbuffer->refresh_phase_buffers();
	// Draw a crossing mark on both the magnitude and phase drawable
	pbuffer->draw_marks_on_magnitude(x);
	pbuffer->draw_marks_on_phase(x);
}

gboolean on_magnitude_drawingarea_motion_notify_event(
	GtkWidget *widget,
	GdkEventMotion *event,
	gpointer user_data)
{
	assert(widget);
	assert(event);
	assert(user_data);
	gint x, y;
	on_motion_notify_event(widget, event, &x, &y);
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	refresh_buffers_with_marks(x, pbuffer);
	return TRUE;
}

gboolean on_phase_drawingarea_motion_notify_event(
	GtkWidget *widget,
	GdkEventMotion *event,
	gpointer user_data)
{
	assert(widget);
	assert(event);
	assert(user_data);
	gint x, y;
	on_motion_notify_event(widget, event, &x, &y);
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	refresh_buffers_with_marks(x, pbuffer);
	return TRUE;
}

void on_par_name_combobox_changed(
	GtkComboBox *widget,
	gpointer user_data)
{
	assert(widget);
	assert(user_data);
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	GtkTreeModel *list_store = gtk_combo_box_get_model(widget);
	GtkTreeIter iter;
	if (gtk_combo_box_get_active_iter(widget, &iter) == TRUE)
	{
		gchar *str_data = 0;
		gtk_tree_model_get(list_store, &iter,
											 0, &str_data,
											 -1);
		pbuffer->set_parameter_controller(str_data);
#ifdef DEBUG
		pbuffer->print_parameter_controller();
#endif // DEBUG
		g_free(str_data);
	}
}
void on_par_unit_combobox_changed(
	GtkComboBox *widget,
	gpointer user_data)
{
	assert(widget);
	assert(user_data);
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	GtkTreeModel *list_store = gtk_combo_box_get_model(widget);
	GtkTreeIter iter;
	if (gtk_combo_box_get_active_iter(widget, &iter) == TRUE)
	{
		gchar *str_data = 0;
		gtk_tree_model_get(list_store, &iter,
											 0, &str_data,
											 -1);
		pbuffer->set_parameter_controller_unit(str_data);
#ifdef DEBUG
		pbuffer->print_parameter_controller();
#endif // DEBUG
		g_free(str_data);
	}
}

void on_par_min_entry_changed(
	GtkEntry *entry,
	gpointer user_data)
{
	assert(entry);
	assert(user_data);
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	const gchar *entry_str = gtk_entry_get_text(entry);
	pbuffer->set_parameter_controller_min(strtod(entry_str,0));
#ifdef DEBUG
	pbuffer->print_parameter_controller();
#endif // DEBUG
}

void on_par_max_entry_changed(
	GtkEntry *entry,
	gpointer user_data)
{
	assert(entry);
	assert(user_data);
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	const gchar *entry_str = gtk_entry_get_text(entry);
	pbuffer->set_parameter_controller_max(strtod(entry_str,0));
#ifdef DEBUG
	pbuffer->print_parameter_controller();
#endif // DEBUG
}

void on_par_step_entry_changed(
	GtkEntry *entry,
	gpointer user_data)
{
	assert(entry);
	assert(user_data);
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	const gchar *entry_str = gtk_entry_get_text(entry);
	pbuffer->set_parameter_controller_step(strtod(entry_str,0));
#ifdef DEBUG
	pbuffer->print_parameter_controller();
#endif // DEBUG
}

void on_par_value_reset_button_clicked(
	GtkButton *button,
	gpointer user_data)
{
	assert(button);
	assert(user_data);
	gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE); // Disable button
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	pbuffer->reset_parameter_value();
	gtk_widget_set_sensitive(GTK_WIDGET(button), TRUE); // Enable button
}

void on_apply_button_clicked(
	GtkButton *button,
	gpointer user_data)
{
	assert(button);
	assert(user_data);
	gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE); // Disable button
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	pbuffer->refresh_num_points(); // Reset num_points_entry if it changes.
	pbuffer->refresh_freq_range(); // Reset freq range if it changes.
	pbuffer->evaluate_all(); // Evaluate numerical data.
	pbuffer->refresh_all();   // Redraw all curves.
#ifdef PRINT_TO_FILE
	pbuffer->SaveToFile(freqResonse2Dfile);
#endif
	gtk_widget_set_sensitive(GTK_WIDGET(button), TRUE); // Enable button
}

void on_par_value_entry_activate(
	GtkEntry *entry,
	gpointer user_data)
{
	assert(entry);
	assert(user_data);
	gtk_widget_set_sensitive(GTK_WIDGET(entry), FALSE); // Disable entry 
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	pbuffer->get_parameter_value_from_par_value_entry();
	gtk_widget_set_sensitive(GTK_WIDGET(entry), TRUE); // Enable entry
}

void on_par_value_hscale_value_changed(
	GtkRange *range,
	gpointer user_data)
{
	assert(range);
	assert(user_data);
	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	gdouble value = gtk_range_get_value(range); // with unit
	pbuffer->change_parameter_value(value);
}

void on_export_toolbutton_clicked(
	GtkButton *button,
	gpointer user_data)
{
	assert(button);
	assert(user_data);
	gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE); // Disable button

	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	const gchar EXPORT_FILECHOOSERDIALOG[] = "export_filechooserdialog";
	const gchar DEFAULT_FILENAME[] = "Untitled.dat";
	const gchar CURRENT_DIR[] = ".";
	// Can Default property must be set to TRUE in Glade
	// for the file chooser dialog and its children
	// so that the assertion failure is suppressed.
	GladeXML *xml = glade_xml_new(GLADE_FILE, EXPORT_FILECHOOSERDIALOG, 0);
	GtkWidget *filechooserdialog =
		glade_xml_get_widget(xml, EXPORT_FILECHOOSERDIALOG);
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(filechooserdialog),
																		 	CURRENT_DIR);
	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(filechooserdialog),
																	 	DEFAULT_FILENAME);
	// Set Response ID in Glade: See GtkRespenseType or below for details:
	//////////////////////////////////////////////////////////////////////
	/* GTK returns this if a response widget has no response_id,
	 * or if the dialog gets programmatically hidden or destroyed.
	 */
	// GTK_RESPONSE_NONE = -1,

	/* GTK won't return these unless you pass them in
	 * as the response for an action widget. They are
	 * for your convenience.
	 */
	// GTK_RESPONSE_REJECT = -2,
	// GTK_RESPONSE_ACCEPT = -3,

	/* If the dialog is deleted. */
	// GTK_RESPONSE_DELETE_EVENT = -4,

	/* These are returned from GTK dialogs, and you can also use them
	 * yourself if you like.
	 */
	// GTK_RESPONSE_OK     = -5,
	// GTK_RESPONSE_CANCEL = -6,
	// GTK_RESPONSE_CLOSE  = -7,
	// GTK_RESPONSE_YES    = -8,
	// GTK_RESPONSE_NO     = -9,
	// GTK_RESPONSE_APPLY  = -10,
	// GTK_RESPONSE_HELP   = -11
	//////////////////////////////////////////////////////////////////////
	// gtk_dialog_run() will block the process until a response is emitted.
	if (gtk_dialog_run(GTK_DIALOG(filechooserdialog)) == GTK_RESPONSE_ACCEPT)
	{
		char *filename = 
			gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooserdialog));
		pbuffer->save_to_file(filename);
		g_free(filename);
	}
	gtk_widget_destroy(filechooserdialog);
	g_object_unref(G_OBJECT(xml)); 

	gtk_widget_set_sensitive(GTK_WIDGET(button), TRUE); // Enable button
}

void on_save_toolbutton_clicked(
	GtkButton *button,
	gpointer user_data)
{
	assert(button);
	assert(user_data);
	gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE); // Disable button

	Buffer *pbuffer = static_cast<Buffer *>(user_data);
	const gchar DEFAULT_FILENAME[] = "2Dcurve.dat";
	pbuffer->save_to_file(DEFAULT_FILENAME);

	gtk_widget_set_sensitive(GTK_WIDGET(button), TRUE); // Enable button
}

void on_quit_imagemenuitem_activate(
	GtkWidget *imagemenuitem,
	gpointer user_data)
{
	custom_quit(imagemenuitem, user_data);
}
} // extern "C"
}


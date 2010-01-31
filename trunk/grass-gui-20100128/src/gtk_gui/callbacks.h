#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_

#include<gtk/gtk.h>

#include<gmodule.h>
/* exporting and importing functions, this is special cased
 * to feature Windows dll stubs.
 * #define G_MODULE_IMPORT         extern
 * #ifdef G_PLATFORM_WIN32
 * #  define       G_MODULE_EXPORT         __declspec(dllexport)
 * #else // !G_PLATFORM_WIN32
 * #  define       G_MODULE_EXPORT
 * #endif // !G_PLATFORM_WIN32
 **/
namespace callbacks
{
extern "C"
{
// Callback functions for the main window
G_MODULE_EXPORT void custom_quit(GtkWidget *widget, gpointer user_data);

// Callback functions for the drawing areas
G_MODULE_EXPORT gboolean on_magnitude_drawingarea_expose_event(
	GtkWidget *widget,
	GdkEventExpose *event,
	gpointer user_data);

G_MODULE_EXPORT gboolean on_phase_drawingarea_expose_event(
	GtkWidget *widget,
	GdkEventExpose *event,
	gpointer user_data);

G_MODULE_EXPORT gboolean on_magnitude_drawingarea_configure_event(
	GtkWidget *widget,
	GdkEventConfigure *event,
	gpointer user_data);

G_MODULE_EXPORT gboolean on_phase_drawingarea_configure_event(
	GtkWidget *widget,
	GdkEventConfigure *event,
	gpointer user_data);

G_MODULE_EXPORT gboolean on_magnitude_drawingarea_motion_notify_event(
	GtkWidget *widget,
	GdkEventMotion *event,
	gpointer user_data);

G_MODULE_EXPORT gboolean on_phase_drawingarea_motion_notify_event(
	GtkWidget *widget,
	GdkEventMotion *event,
	gpointer user_data);

G_MODULE_EXPORT void on_par_name_combobox_changed(
	GtkComboBox *widget,
	gpointer user_data);

G_MODULE_EXPORT void on_par_unit_combobox_changed(
	GtkComboBox *widget,
	gpointer user_data);

G_MODULE_EXPORT void on_par_min_entry_changed(
	GtkEntry *entry,
	gpointer user_data);

G_MODULE_EXPORT void on_par_max_entry_changed(
	GtkEntry *entry,
	gpointer user_data);

G_MODULE_EXPORT void on_par_step_entry_changed(
	GtkEntry *entry,
	gpointer user_data);

G_MODULE_EXPORT void on_par_value_reset_button_clicked(
	GtkButton *button,
	gpointer user_data);

G_MODULE_EXPORT void on_apply_button_clicked(
	GtkButton *button,
	gpointer user_data);

G_MODULE_EXPORT void on_par_value_entry_activate(
	GtkEntry *entry,
	gpointer user_data);

G_MODULE_EXPORT void on_par_value_hscale_value_changed(
	GtkRange *range,
	gpointer user_data);

G_MODULE_EXPORT void on_export_toolbutton_clicked(
	GtkButton *button,
	gpointer user_data);

G_MODULE_EXPORT void on_save_toolbutton_clicked(
	GtkButton *button,
	gpointer user_data);

G_MODULE_EXPORT void on_quit_imagemenuitem_activate(
	GtkWidget *imagemenuitem,
	gpointer user_data);
}
}
#endif // _CALLBACKS_H_


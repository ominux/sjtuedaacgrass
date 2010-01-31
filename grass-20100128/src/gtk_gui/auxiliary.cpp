#include"auxiliary.h"

#include"buffer.h"
#include"callbacks.h"
#include<gtk/gtk.h>
#include<glade/glade.h>
#include<vector>
#include<string>

using std::vector;
using std::string;
using namespace callbacks;

namespace
{
// names of important GtkWidgets
// main window
const gchar MAIN_WINDOW[] = "main_window";
// drawing area container
const gchar DRAWING_VPANED[] = "drawing_vpaned";
// two drawing areas: magnitude & phase
const gchar MAGNITUDE_DRAWINGAREA[] = "magnitude_drawingarea";
const gchar PHASE_DRAWINGAREA[] = "phase_drawingarea";
// Text entries
const gchar FREQ_ENTRY[] = "freq_entry";
const gchar MAGNITUDE_ENTRY[] = "magnitude_entry";
const gchar PHASE_ENTRY[] = "phase_entry";
// Parameter controllers
const gchar PAR_UNIT_COMBOBOX[] = "par_unit_combobox";
const int UNIT_LIST_SIZE = 11;
const gchar *UNIT_LIST[UNIT_LIST_SIZE] = {
	"",    // 1
	"f",   // 10^-15
	"p",   // 10^-12
	"n",   // 10^-9
	"u",   // 10^-6
	"mil", // 25.4 * 10^-6
	"m",   // 10^-3
	"k",   // 10^3
	"M",   // 10^6
	"G",   // 10^9
	"T"    // 10^12
};
const gchar PAR_NAME_COMBOBOX[] = "par_name_combobox";
const gchar PAR_MIN_ENTRY[] = "par_min_entry";
const gchar PAR_MAX_ENTRY[] = "par_max_entry";
const gchar PAR_STEP_ENTRY[] = "par_step_entry";
const gchar PAR_VALUE_ENTRY[] = "par_value_entry";
const gchar PAR_VALUE_RESET_BUTTON[] = "par_value_reset_button";
const gchar APPLY_BUTTON[] = "apply_button";
const gchar PAR_VALUE_HSCALE[] = "par_value_hscale";
const gchar FREQ_MIN_ENTRY[] = "freq_min_entry";
const gchar FREQ_MAX_ENTRY[] = "freq_max_entry";
const gchar NUM_POINTS_ENTRY[] = "num_points_entry";
const gchar EXPORT_TOOLBUTTON[] = "export_toolbutton";
const gchar SAVE_TOOLBUTTON[] = "save_toolbutton";
const gchar QUIT_IMAGEMENUITEM[] = "quit_imagemenuitem";

#ifdef DEBUG
// Test GtkListStore
void test_gtkliststore(GtkTreeModel *list_store)
{
	GtkTreeIter iter;
	gboolean valid = gtk_tree_model_get_iter_first(list_store,
																								 &iter);
	gint row_count = 0;
	while (valid)
	{
		gchar *str_data;
		gtk_tree_model_get(list_store, &iter,
											 0, &str_data,
											 -1);
		g_print("Row %d: %s\n", row_count, str_data);
		g_free(str_data);
		++row_count;
		valid = gtk_tree_model_iter_next(list_store, &iter);
	}
}
#endif // DEBUG

// Init the list of strings stored in GtkComboBox *widget
// with a const gchar array.
void init_combobox(GtkWidget *widget, const gchar *str_list[], int str_list_size)
{
	const gint N_COLUMNS = 1;
	const gint COLUMN_STRING = N_COLUMNS - 1;
	GtkTreeIter iter;
	GtkListStore *list_store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	for (int i=0; i < str_list_size; ++i)
	{
		// Add a new row to the model.
		// The store will keep a copy of the string internally.
		gtk_list_store_append(list_store, &iter);
		// Sets the value of one or more celss in the row refereced by iter.
		// The variable argument list should contain integer column numbers,
		// each column number followed by the value to be set.
		// NOTE: The list is terminated by a -1.
		gtk_list_store_set(list_store, &iter,
											 COLUMN_STRING, str_list[i],
											 -1);
	}
	// GtkCellRenderer is used primarily by GtkTreeView widget.
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(widget), renderer, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(widget), renderer, "text", 0, NULL);
	// Add list_store to widget
	gtk_combo_box_set_model(GTK_COMBO_BOX(widget), GTK_TREE_MODEL(list_store));
#ifdef DEBUG
	test_gtkliststore(GTK_TREE_MODEL(list_store)); // Debug: Test list_store
#endif // DEBUG
}

// Init the list of strings stored in GtkComboBox *widget
// with a vector<string> object.
void init_combobox(GtkWidget *widget, const vector<string> &par_name_list)
{
	const gint N_COLUMNS = 1;
	const gint COLUMN_STRING = N_COLUMNS - 1;
	GtkTreeIter iter;
	GtkListStore *list_store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	for (vector<string>::const_iterator cit = par_name_list.begin();
			 cit != par_name_list.end();
			 ++cit)
	{
		// Add a new row to the model.
		// The store will keep a copy of the string internally.
		gtk_list_store_append(list_store, &iter);
		// Sets the value of one or more celss in the row refereced by iter.
		// The variable argument list should contain integer column numbers,
		// each column number followed by the value to be set.
		// NOTE: The list is terminated by a -1.
		gtk_list_store_set(list_store, &iter,
											 COLUMN_STRING, cit->c_str(),
											 -1);
	}
	// GtkCellRenderer is used primarily by GtkTreeView widget.
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(widget), renderer, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(widget), renderer, "text", 0, NULL);
	// Add list_store to widget
	gtk_combo_box_set_model(GTK_COMBO_BOX(widget), GTK_TREE_MODEL(list_store));
#ifdef DEBUG
	test_gtkliststore(GTK_TREE_MODEL(list_store)); // Debug: Test list_store
#endif // DEBUG
}
}

// Connect all callback functions to corresponding signals.
void custom_xml_signal_connect(GladeXML *xml, Buffer *ptrbuffer)
{
	// Look foR required GtkWidgets from GladeXML.
	// Main window
	GtkWidget *main_window =
		glade_xml_get_widget(xml, MAIN_WINDOW);
	g_signal_connect(G_OBJECT(main_window),
									 "destroy",
									 G_CALLBACK(custom_quit),
									 0);
	
	// Drawing areas and location entries
	GtkWidget *magnitude_drawingarea =
		glade_xml_get_widget(xml, MAGNITUDE_DRAWINGAREA);
	GtkWidget *phase_drawingarea =
		glade_xml_get_widget(xml, PHASE_DRAWINGAREA);
	GtkWidget *freq_entry =
		glade_xml_get_widget(xml, FREQ_ENTRY);
	GtkWidget *magnitude_entry =
		glade_xml_get_widget(xml, MAGNITUDE_ENTRY);
	GtkWidget *phase_entry =
		glade_xml_get_widget(xml, PHASE_ENTRY);
	// Initialize the direct buffer for curves.
	ptrbuffer->init_magnitude_buffers(magnitude_drawingarea);
	ptrbuffer->init_phase_buffers(phase_drawingarea);
	ptrbuffer->set_freq_entry(freq_entry);
	ptrbuffer->set_magnitude_entry(magnitude_entry);
	ptrbuffer->set_phase_entry(phase_entry);
	// two drawing areas: magnitude & phase
	g_signal_connect(G_OBJECT(magnitude_drawingarea),
									 "expose-event",
									 G_CALLBACK(on_magnitude_drawingarea_expose_event),
									 ptrbuffer);
	g_signal_connect(G_OBJECT(phase_drawingarea),
									 "expose-event",
									 G_CALLBACK(on_phase_drawingarea_expose_event),
									 ptrbuffer);
	g_signal_connect(G_OBJECT(magnitude_drawingarea),
									 "configure-event",
									 G_CALLBACK(on_magnitude_drawingarea_configure_event),
									 ptrbuffer);
	g_signal_connect(G_OBJECT(phase_drawingarea),
									 "configure-event",
									 G_CALLBACK(on_phase_drawingarea_configure_event),
									 ptrbuffer);
	// motion notify event on the drawing areas
	g_signal_connect(G_OBJECT(magnitude_drawingarea),
									 "motion-notify-event",
									 G_CALLBACK(on_magnitude_drawingarea_motion_notify_event),
									 ptrbuffer);
	g_signal_connect(G_OBJECT(phase_drawingarea),
									 "motion-notify-event",
									 G_CALLBACK(on_phase_drawingarea_motion_notify_event),
									 ptrbuffer);

	// Widgets in the parameter control panel:
	// par_unit_combobox, par_name_combobox
	GtkWidget *par_unit_combobox =
		glade_xml_get_widget(xml, PAR_UNIT_COMBOBOX);
	init_combobox(par_unit_combobox, UNIT_LIST, UNIT_LIST_SIZE);
	g_signal_connect(G_OBJECT(par_unit_combobox),
									 "changed",
									 G_CALLBACK(on_par_unit_combobox_changed),
									 ptrbuffer);
	GtkWidget *par_name_combobox = 
		glade_xml_get_widget(xml, PAR_NAME_COMBOBOX);
	vector<string> par_name_list;
	ptrbuffer->get_parameter_name_list(par_name_list);
	init_combobox(par_name_combobox, par_name_list);
	g_signal_connect(G_OBJECT(par_name_combobox),
									 "changed",
									 G_CALLBACK(on_par_name_combobox_changed),
									 ptrbuffer);

	// par_min_entry, par_max_entry, par_step_entry
	GtkWidget *par_min_entry =
		glade_xml_get_widget(xml, PAR_MIN_ENTRY);
	GtkWidget *par_max_entry =
		glade_xml_get_widget(xml, PAR_MAX_ENTRY);
	GtkWidget *par_step_entry =
		glade_xml_get_widget(xml, PAR_STEP_ENTRY);
	ptrbuffer->set_par_min_entry(par_min_entry);
	ptrbuffer->set_par_max_entry(par_max_entry);
	ptrbuffer->set_par_step_entry(par_step_entry);
	g_signal_connect(G_OBJECT(par_min_entry),
									 "changed",
									 G_CALLBACK(on_par_min_entry_changed),
									 ptrbuffer);
	g_signal_connect(G_OBJECT(par_max_entry),
									 "changed",
									 G_CALLBACK(on_par_max_entry_changed),
									 ptrbuffer);
	g_signal_connect(G_OBJECT(par_step_entry),
									 "changed",
									 G_CALLBACK(on_par_step_entry_changed),
									 ptrbuffer);

	// par_value_entry
	GtkWidget *par_value_entry =
		glade_xml_get_widget(xml, PAR_VALUE_ENTRY);
	ptrbuffer->set_par_value_entry(par_value_entry);
	g_signal_connect(G_OBJECT(par_value_entry),
									 "activate",
									 G_CALLBACK(on_par_value_entry_activate),
									 ptrbuffer);

	// par_value_reset_button
	GtkWidget *par_value_reset_button =
		glade_xml_get_widget(xml, PAR_VALUE_RESET_BUTTON);
	g_signal_connect(G_OBJECT(par_value_reset_button),
									 "clicked",
									 G_CALLBACK(on_par_value_reset_button_clicked),
									 ptrbuffer);

	// apply_button
	GtkWidget *apply_button =
		glade_xml_get_widget(xml, APPLY_BUTTON);
	g_signal_connect(G_OBJECT(apply_button),
									 "clicked",
									 G_CALLBACK(on_apply_button_clicked),
									 ptrbuffer);

	// par_value_hscale
	GtkWidget *par_value_hscale =
		 glade_xml_get_widget(xml, PAR_VALUE_HSCALE);
	ptrbuffer->set_par_value_hscale(par_value_hscale);
	g_signal_connect(G_OBJECT(par_value_hscale),
									 "value-changed",
									 G_CALLBACK(on_par_value_hscale_value_changed),
									 ptrbuffer);
	// Frequency range controllers
	GtkWidget *freq_min_entry =
		glade_xml_get_widget(xml, FREQ_MIN_ENTRY);
	GtkWidget *freq_max_entry =
		glade_xml_get_widget(xml, FREQ_MAX_ENTRY);
	GtkWidget *num_points_entry =
		glade_xml_get_widget(xml, NUM_POINTS_ENTRY);
	ptrbuffer->set_freq_min_entry(freq_min_entry);
	ptrbuffer->set_freq_max_entry(freq_max_entry);
	ptrbuffer->set_num_points_entry(num_points_entry);
	// Export button: export the H(s) data points to a text file
	GtkWidget *export_toolbutton =
		glade_xml_get_widget(xml, EXPORT_TOOLBUTTON);
	g_signal_connect(G_OBJECT(export_toolbutton ),
									 "clicked",
									 G_CALLBACK(on_export_toolbutton_clicked),
									 ptrbuffer);
	// Save button: save the H(s) 2D curve data
 	// points to a text file with a default name
	GtkWidget *save_toolbutton =
		glade_xml_get_widget(xml, SAVE_TOOLBUTTON);
	g_signal_connect(G_OBJECT(save_toolbutton ),
									 "clicked",
									 G_CALLBACK(on_save_toolbutton_clicked),
									 ptrbuffer);
	// Quit imagemenuitem: quit the 2D GUI
	GtkWidget *quit_imagemenuitem =
		glade_xml_get_widget(xml, QUIT_IMAGEMENUITEM);
	g_signal_connect(G_OBJECT(quit_imagemenuitem),
									 "activate",
									 G_CALLBACK(on_quit_imagemenuitem_activate),
									 ptrbuffer);
}


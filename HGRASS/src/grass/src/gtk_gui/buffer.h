#ifndef _BUFFER_H_
#define _BUFFER_H_
// Create a 2D curve buffer for a transfer function
// that is provided by an Analyser object.
//
// There are 3 optional scales:
// linear, logarithmic(log10) and dB(20*log10)
// where linear and log10(default) for freq-axis,
// linear and dB(default) for magnitude-axis.
// The scale of phase-axis is always linear.
#include"analyser.h"
#include"curve.h"
#include<gtk/gtk.h>
#include<map>
#include<string>
#include<cassert>
#include<cstdlib>

// Where to load the Glade XML interface description file.
#if (defined(_WIN32)) && (!defined(__MINGW32__))
	const char GLADE_FILE[] = "..\\..\\src\\gtk_gui\\grass.glade";
#else
#ifdef GRASS
	const char GLADE_FILE[] = "gtk_gui/grass.glade";
#else
	const char GLADE_FILE[] = "src/grass/src/gtk_gui/grass.glade";
#endif
#endif


class Buffer
{
public:
	typedef enum {LIN, LOG, DB} SCALE_TYPE;
	Buffer(Analyser *ptr_analyser,
				 double freq_min, // Unit of freqency: Hz.
				 double freq_max,
				 int num_points,
				 SCALE_TYPE freq_scale,
				 SCALE_TYPE magnitude_scale); // The defalut phase scale is linear.
	~Buffer();
	void get_parameter_name_list(std::vector<std::string> &par_name_list) const;
	void set_freq_scale(SCALE_TYPE);
	void set_magnitude_scale(SCALE_TYPE);
	void print() const; // Debug
	void save_to_file(const std::string &filename) const;
	double freq_start() const; // Hz
	double freq_stop() const; // Hz
	double freq_min() const; // log10(Hz)
	double freq_max() const; // log10(Hz)
	double magnitude_min() const;
	double magnitude_max() const;
	double phase_min() const;
	double phase_max() const;
	int num_points() const;
	void init_magnitude_buffers(GtkWidget *magnitude_widget);
	void init_phase_buffers(GtkWidget *phase_widget);
	void set_freq_entry(GtkWidget *freq_entry);
	void set_magnitude_entry(GtkWidget *magnitude_entry);
	void set_phase_entry(GtkWidget *phase_entry);
	void set_par_min_entry(GtkWidget *par_min_entry);
	void set_par_max_entry(GtkWidget *par_max_entry);
	void set_par_step_entry(GtkWidget *par_step_entry);
	void set_par_value_entry(GtkWidget *par_value_entry);
	void set_par_value_hscale(GtkWidget *par_value_hscale);
	void set_freq_min_entry(GtkWidget *freq_min_entry);
	void set_freq_max_entry(GtkWidget *freq_max_entry);
	void set_num_points_entry(GtkWidget *num_points_entry);
	void set_parameter_controller_unit(const std::string &unit_str);
	void set_parameter_controller(const std::string &par_name);
	void set_parameter_controller_min(double min);
	void set_parameter_controller_max(double max);
	void set_parameter_controller_step(double step);
	void reset_parameter_value();
	void change_parameter_value(double d);
	void get_parameter_value_from_par_value_entry();
#ifdef DEBUG
	void print_parameter_controller() const;
#endif // DEBUG
	void draw_magnitude_log_dB();
	void draw_phase_log_linear();
	void draw_marks_on_magnitude(gint x);
	void draw_marks_on_phase(gint x);
	void recover_marks_on_magnitude(); // Used for configure-event etc.
	void recover_marks_on_phase(); // Used for configure-event etc.
	void refresh_magnitude_buffers();
	void refresh_phase_buffers();
	void refresh_num_points();
	void refresh_freq_range();
	void print_on_par_min_entry(double d);
	void print_on_par_max_entry(double d);
	void print_on_par_step_entry(double d);
	void print_on_par_value_entry(double d);
	void print_on_freq_min_entry(double d);
	void print_on_freq_max_entry(double d);
	void print_on_num_points_entry(double d);
	void evaluate_all();
	void refresh_all();
	static const double EPS_;
	static const double PI_;
	static void gtk_entry_print_double(GtkWidget *widget, double d);
private:
	typedef Curve::Curve2D Curve2D; 
	Analyser *ptr_analyser_; // Contain a test case of class Buffer
	double parameter_unit_; // 10^x, defalut: 1.0
	double freq_min_, freq_max_; // Unit: Hz.
	double magnitude_min_, magnitude_max_;
	double phase_min_, phase_max_;
	int num_points_, num_points_reserved_;
	double *freq_vector_;
	double *magnitude_vector_;
	double *phase_vector_;
	Curve2D magnitude_curve_;
	Curve2D phase_curve_;
	SCALE_TYPE freq_scale_, magnitude_scale_;
	// direct graph buffers for curves
	Curve *magnitude_curve_buffer_, *phase_curve_buffer_;
	gdouble freq_mark_ratio_magnitude_, freq_mark_ratio_phase_;
	GtkWidget *freq_entry_, *magnitude_entry_, *phase_entry_;
	GtkWidget *par_min_entry_, *par_max_entry_, *par_step_entry_;
	GtkWidget *par_value_entry_;
	GtkWidget *par_value_hscale_;
	GtkWidget *freq_min_entry_, *freq_max_entry_;
	GtkWidget *num_points_entry_;
	// Parameter controller
	struct ParameterController_
	{
		ParameterController_() :
			pMosfet_(0),mosfet_flag_(-1),
			ptr_value_(0), original_value_(0.0),
			min_(0.0), max_(0.0), step_(0.0) {}
		~ParameterController_() { ptr_value_ = 0; }
		Mosfet *pMosfet_;
		int	mosfet_flag_;
		double *ptr_value_;
		double  original_value_;
		double  min_, max_, step_; // Variation settings
	};
	typedef std::map<std::string, ParameterController_ *> ParameterController;
	std::map<std::string, ParameterController_ *> parameter_controller_map_;
	ParameterController_ *ptr_current_parameter_controller_;
	double current_unit_;
	void init_buffer();
	void find_boundaries();
};

// Inline functions

inline void Buffer::get_parameter_name_list(
	std::vector<std::string> &par_name_list) const
{ ptr_analyser_->get_name_list(par_name_list); }

// Hz
inline double Buffer::freq_start() const
{ return freq_min_; }

// Hz
inline double Buffer::freq_stop() const
{ return freq_max_; }

// log10(Hz)
inline double Buffer::freq_min() const
{ return freq_vector_[0]; }

// log10(Hz)
inline double Buffer::freq_max() const
{ return freq_vector_[num_points_ - 1]; }

inline double Buffer::magnitude_min() const
{ return magnitude_min_; }

inline double Buffer::magnitude_max() const
{ return magnitude_max_; }

inline double Buffer::phase_min() const
{ return phase_min_; }

inline double Buffer::phase_max() const
{ return phase_max_; }

inline int Buffer::num_points() const
{ return num_points_; }

inline void Buffer::refresh_magnitude_buffers()
{
	assert(magnitude_curve_buffer_);
	magnitude_curve_buffer_->clear();
	magnitude_curve_buffer_->draw_log_dB();
	magnitude_curve_buffer_->flush();
}

inline void Buffer::refresh_phase_buffers()
{
	assert(phase_curve_buffer_);
	phase_curve_buffer_->clear();
	phase_curve_buffer_->draw_log_linear();
	phase_curve_buffer_->flush();
}

inline void Buffer::draw_magnitude_log_dB()
{ magnitude_curve_buffer_->flush(); }

inline void Buffer::draw_phase_log_linear()
{ phase_curve_buffer_->flush(); }

inline void Buffer::set_freq_entry(GtkWidget *freq_entry)
{ 
	assert(freq_entry);
 	freq_entry_ = freq_entry;
}

inline void Buffer::set_magnitude_entry(GtkWidget *magnitude_entry)
{
	assert(magnitude_entry);
 	magnitude_entry_ = magnitude_entry;
}

inline void Buffer::set_phase_entry(GtkWidget *phase_entry)
{
	assert(phase_entry);
 	phase_entry_ = phase_entry;
}

inline void Buffer::set_par_min_entry(GtkWidget *par_min_entry)
{
	assert(par_min_entry);
 	par_min_entry_ = par_min_entry;
}

inline void Buffer::set_par_max_entry(GtkWidget *par_max_entry)
{
	assert(par_max_entry);
 	par_max_entry_ = par_max_entry;
}

inline void Buffer::set_par_step_entry(GtkWidget *par_step_entry)
{
	assert(par_step_entry);
 	par_step_entry_ = par_step_entry;
}

inline void Buffer::set_par_value_entry(GtkWidget *par_value_entry)
{
	assert(par_value_entry);
 	par_value_entry_ = par_value_entry;
}

inline void Buffer::set_par_value_hscale(GtkWidget *par_value_hscale)
{
	assert(par_value_hscale);
 	par_value_hscale_ = par_value_hscale;
}

inline void Buffer::set_freq_min_entry(GtkWidget *freq_min_entry)
{
	assert(freq_min_entry);
	freq_min_entry_ = freq_min_entry;
	gtk_entry_print_double(freq_min_entry, freq_min_);
}

inline void Buffer::set_freq_max_entry(GtkWidget *freq_max_entry)
{
	assert(freq_max_entry);
	freq_max_entry_ = freq_max_entry;
	gtk_entry_print_double(freq_max_entry, freq_max_);
}

inline void Buffer::set_num_points_entry(GtkWidget *num_points_entry)
{
	assert(num_points_entry);
	num_points_entry_ = num_points_entry;
	gtk_entry_print_double(num_points_entry, num_points_);
}

// print double to GTK_ENTRY
inline void Buffer::gtk_entry_print_double(GtkWidget *widget, double d)
{
	assert(widget);
	gchar buf[50];
	const int precision = 6;
	(void)sprintf(buf, "%-.*g", precision, d);
	gtk_entry_set_text(GTK_ENTRY(widget), buf);
}

inline void Buffer::print_on_par_min_entry(double d)
{ gtk_entry_print_double(par_min_entry_, d); }

inline void Buffer::print_on_par_max_entry(double d)
{ gtk_entry_print_double(par_max_entry_, d); }

inline void Buffer::print_on_par_step_entry(double d)
{ gtk_entry_print_double(par_step_entry_, d); }

inline void Buffer::print_on_par_value_entry(double d)
{ gtk_entry_print_double(par_value_entry_, d); }

inline void Buffer::print_on_freq_min_entry(double d)
{ gtk_entry_print_double(freq_min_entry_, d); }

inline void Buffer::print_on_freq_max_entry(double d)
{ gtk_entry_print_double(freq_max_entry_, d); }

inline void Buffer::print_on_num_points_entry(double d)
{ gtk_entry_print_double(num_points_entry_, d); }

inline void Buffer::evaluate_all()
{
	// initial the memory
	init_buffer();
	// set magnitude_min_, magnitude_max_, phase_min_, phase_max_
	find_boundaries();
}

inline void Buffer::refresh_all()
{
	// Redraw curves
	init_magnitude_buffers(magnitude_curve_buffer_->widget());
	refresh_magnitude_buffers();
	recover_marks_on_magnitude();
	init_phase_buffers(phase_curve_buffer_->widget());
	refresh_phase_buffers();
	recover_marks_on_phase();
}

#ifdef DEBUG
inline void Buffer::print_parameter_controller() const
{
	if (!ptr_current_parameter_controller_)
	{
		g_print("unit = %g\n", current_unit_);
		return;
	}
	g_print("ParameterController{ptr_value=%p, original_value=%g, unit=%g, min=%g, max=%g, step=%g}\n",
					ptr_current_parameter_controller_->ptr_value_,
					ptr_current_parameter_controller_->original_value_,
					current_unit_,
					ptr_current_parameter_controller_->min_,
					ptr_current_parameter_controller_->max_,
					ptr_current_parameter_controller_->step_);
}
#endif // DEBUG

#endif // _BUFFER_H_


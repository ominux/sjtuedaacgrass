#include"buffer.h"
#include"analyser.h"

#include<map>
#include<string>
#include<utility>
#include<complex>

#ifdef _OPENMP
#include<omp.h>
#endif // _OPENMP
#include<gtk/gtk.h>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cassert>

using std::printf;
using std::fprintf;
using std::FILE;
using std::fopen;
using std::fclose;
using std::make_pair;
using std::map;
using std::string;
using std::fabs;
using std::log10;
using std::pow;
using std::floor;
using std::ceil;
using std::pair;

const double Buffer::EPS_(1.7e-308);
const double Buffer::PI_(3.14159265358979);

// Evaluate numerical points using given symbolic expression
void Buffer::init_buffer()
{
	assert(ptr_analyser_);
	assert(freq_min_ > EPS_);
	assert(freq_min_ < freq_max_);
	assert(num_points_ >= 2);
	assert(freq_vector_);
	assert(magnitude_vector_);
	assert(phase_vector_);
	assert(freq_scale_ != DB);
	assert(magnitude_scale_ != LOG);
	// NOTE:
	// Evaluate all coefficients of the network function
 	// every time when drawing the whole curves.
	ptr_analyser_->evaluate_nf_coeff();
	double f0, fstep;
	if (freq_scale_ == LIN)
	{
		f0 = freq_min_;
		fstep = (freq_max_ - freq_min_) / (num_points_ - 1);
	}
	else // freq_scale_ == LOG
	{
		f0 = log10(freq_min_);
		fstep = log10(freq_max_ / freq_min_) / (num_points_ - 1);
	}
#ifdef _OPENMP
#pragma omp parallel for schedule(static)
#endif // _OPENMP
	for (int i=0; i < num_points_; ++i)
	{
		freq_vector_[i] = f0 + fstep*i;
		double freq = (freq_scale_==LIN)? freq_vector_[i]: pow(10.0, freq_vector_[i]);
		complex_double z(ptr_analyser_->network_function(freq));
		phase_vector_[i] = arg(z) * 180.0 / PI_; // Degree
		if (phase_vector_[i] > EPS_)
		 	phase_vector_[i] -= 360.0; // keep phase continuous.
		double magn_z = abs(z);
		if (magnitude_scale_ == LIN)
		{
			magnitude_vector_[i] = magn_z;
		}
		else // magnitude_scale_ == dB
		{
			assert(magn_z > EPS_); // yy must be positive
			magnitude_vector_[i] = 20.0 * log10(magn_z);
		}
	}
}

// Evaluate magnitude_min_, magnitude_max_, phase_min_, phase_max_
void Buffer::find_boundaries()
{
	magnitude_min_ = magnitude_max_ = magnitude_vector_[0];
	phase_min_ = phase_max_ = phase_vector_[0];
	for (int i = 0; i < num_points_; ++i)
	{
		// find boundary of magnitude
		if (magnitude_min_ > magnitude_vector_[i])
			magnitude_min_ = magnitude_vector_[i];
		else if (magnitude_max_ < magnitude_vector_[i])
			magnitude_max_ = magnitude_vector_[i];
		// find boundary of phase
		if (phase_min_ > phase_vector_[i])
			phase_min_ = phase_vector_[i];
		else if (phase_max_ < phase_vector_[i])
			phase_max_ = phase_vector_[i];
	}
}

Buffer::Buffer(Analyser *ptr_analyser,
							 double freq_min,
							 double freq_max,
							 int num_points,
							 SCALE_TYPE freq_scale,
							 SCALE_TYPE magnitude_scale) :
	ptr_analyser_(ptr_analyser),
	parameter_unit_(1.0),
	freq_min_(freq_min), freq_max_(freq_max),
	magnitude_min_(0.0), magnitude_max_(0.0),
	phase_min_(0.0), phase_max_(0.0),
	num_points_((num_points < 2) ? 2 : num_points), // Clamp num_points_
	num_points_reserved_(num_points_),
	freq_vector_(new double[num_points_]),
	magnitude_vector_(new double[num_points_]),
	phase_vector_(new double[num_points_]),
	freq_scale_(freq_scale),
	magnitude_scale_(magnitude_scale),
	magnitude_curve_buffer_(0), phase_curve_buffer_(0),
	freq_mark_ratio_magnitude_(0.0), freq_mark_ratio_phase_(0.0),
	freq_entry_(0), magnitude_entry_(0), phase_entry_(0),
	par_min_entry_(0), par_max_entry_(0), par_step_entry_(0),
 	par_value_entry_(0), par_value_hscale_(0),
	freq_min_entry_(0), freq_max_entry_(0), num_points_entry_(0),
	ptr_current_parameter_controller_(0), current_unit_(1.0)
{
	assert(ptr_analyser_);
	assert(freq_min_ > EPS_);
	assert(freq_min_ < freq_max_);
	assert(num_points_ > 2);
	assert(freq_vector_);
	assert(magnitude_vector_);
	assert(phase_vector_);
	assert(freq_scale_ != DB);
	assert(magnitude_scale_ != LOG);

	magnitude_curve_.x = freq_vector_;
	magnitude_curve_.y = magnitude_vector_;
	phase_curve_.x = freq_vector_;
	phase_curve_.y = phase_vector_;
	evaluate_all();
}

Buffer::~Buffer()
{
	assert(freq_vector_);
	assert(magnitude_vector_);
	assert(phase_vector_);
	assert(magnitude_curve_buffer_);
	assert(phase_curve_buffer_);
	for (map<string, ParameterController_ *>::iterator
			 it = parameter_controller_map_.begin();
			 it != parameter_controller_map_.end();
			 ++it)
		delete it->second;
	delete freq_vector_;
	delete magnitude_vector_;
	delete phase_vector_;
	delete magnitude_curve_buffer_;	
	delete phase_curve_buffer_;
}

void Buffer::set_freq_scale(SCALE_TYPE fscale)
{
	if (fscale == freq_scale_)
		return ;
	else
		freq_scale_ = fscale;
	evaluate_all();
	refresh_all();
}

void Buffer::set_magnitude_scale(SCALE_TYPE magn_scale)
{
	assert(magn_scale != LOG);
	if (magn_scale == magnitude_scale_)
		return ;
	else
		magnitude_scale_ = magn_scale;
	// Magnitude scale transfermation: Revaluate all points.
	if (magnitude_scale_ == LIN) // dB -> linear
	{
#ifdef _OPENMP
#pragma omp parallel for schedule(static)
#endif // _OPENMP
		for (int i = 0; i < num_points_; ++i)
			magnitude_vector_[i] = pow(10.0, magnitude_vector_[i] / 20.0);
	}
	else // linear -> dB
	{
#ifdef _OPENMP
#pragma omp parallel for schedule(static)
#endif // _OPENMP
		for (int i = 0; i < num_points_; ++i)
		{
			assert(magnitude_vector_[i] > EPS_); // magnitude cannot be zero(EPS_)
			magnitude_vector_[i] = 20.0 * log10(magnitude_vector_[i]);
		}
	}
	find_boundaries(); // set magnitude_min_, magnitude_max_, phase_min_, phase_max_
	refresh_magnitude_buffers();
	refresh_phase_buffers();
}

// Dynamic memery management: may alloc raw memories
void Buffer::refresh_num_points()
{
	int num_points = static_cast<int>
		(strtod(gtk_entry_get_text(GTK_ENTRY(num_points_entry_)), 0));
	if (num_points_ == num_points)
		return; // Do nothing if num_points_ does not change.
	if (num_points < 2)
		num_points = 2; // Clamp num_points at least 2
	if (num_points <= num_points_reserved_)
	{
		num_points_ = num_points;
	}
	else
	{
		// Realloc memeories
		num_points_reserved_ = num_points_ = num_points;
		assert(freq_vector_);
		assert(magnitude_vector_);
		assert(phase_vector_);
		delete freq_vector_;
		freq_vector_ = new double[num_points];
		delete magnitude_vector_;
		magnitude_vector_ = new double[num_points];
		delete phase_vector_;
		phase_vector_ = new double[num_points];
		magnitude_curve_.x = freq_vector_;
		magnitude_curve_.y = magnitude_vector_;
		phase_curve_.x = freq_vector_;
		phase_curve_.y = phase_vector_;
	}
}

// Refresh the frequency range but do NOT evaluate or redraw curves
void Buffer::refresh_freq_range()
{
	double freq_min = strtod(gtk_entry_get_text(GTK_ENTRY(freq_min_entry_)), 0);
	double freq_max = strtod(gtk_entry_get_text(GTK_ENTRY(freq_max_entry_)), 0);
	if (fabs(freq_min - freq_min_) < EPS_ && fabs(freq_max - freq_max_) < EPS_)
		return; // Do nothing if neither freq_min_ nor freq_max_ changes.
	freq_min_ = freq_min;
	freq_max_ = freq_max;
}

// Debug
void Buffer::print() const
{
#if 0
	FILE *fp = fopen("out_curve.dat", "w");
	fprintf(fp, "freq(log10) magnitude(dB) phase(linear)\n");
	for (int i = 0; i < num_points_; ++i)
		fprintf(fp, "%+5e %+5e %+5e\n", freq_vector_[i], magnitude_vector_[i], phase_vector_[i]);
#endif // 0
}

// Save data points of H(s) to a txt file
// Format:
// Freq (Hz)   |H(s)| (dB)    arg(H(s)) (Degree)
void Buffer::save_to_file(const string &filename) const
{
	FILE *poutfile = fopen(filename.c_str(), "w");
	if (poutfile)
	{
		fprintf(poutfile, "Freq (Hz)\t|H(s)| (dB)\targ(H(s)) (Degree)\n");
		for (int i = 0; i < num_points_; ++i)
			fprintf(poutfile, "%9.6e\t%11.6e\t%.6e\n",
							pow(10.0, freq_vector_[i]), magnitude_vector_[i], phase_vector_[i]);
		fclose(poutfile);
	}
	else
	{
		printf("Error! Cannot open file: %s\n", filename.c_str());
	}
}

void Buffer::init_magnitude_buffers(GtkWidget *magnitude_widget)
{
	Curve *tmpptr = 0;
	assert(magnitude_widget);
	tmpptr = new Curve(magnitude_curve_,
										 freq_vector_[0],
										 freq_vector_[num_points_ - 1],
										 magnitude_min_,
										 magnitude_max_,
										 num_points_,
										 magnitude_widget);
	if (magnitude_curve_buffer_)
		delete magnitude_curve_buffer_;
	magnitude_curve_buffer_ = tmpptr;
}

void Buffer::init_phase_buffers(GtkWidget *phase_widget)
{
	Curve *tmpptr = 0;
	assert(phase_widget);
	tmpptr = new Curve(phase_curve_,
										 freq_vector_[0],
										 freq_vector_[num_points_ - 1],
										 phase_min_,
										 phase_max_,
										 num_points_,
										 phase_widget);
	if (phase_curve_buffer_)
		delete phase_curve_buffer_;
	phase_curve_buffer_ = tmpptr;
}

namespace
{
void draw_marks(gint          x,
								Curve        *curve_buffer,
							 	gdouble      &freq_mark_ratio,
								double        value_min,
							 	const double *value_vector,
								int           num_points,
								GtkWidget    *widget)
{
	assert(curve_buffer);
	assert(curve_buffer->inner_width());
	assert(value_vector);
	assert(widget);
	gint x_mark = 0;
	if (x < curve_buffer->inner_left())
		x_mark = curve_buffer->inner_left();
	else if (x > curve_buffer->inner_right())
		x_mark = curve_buffer->inner_right();
	else
		x_mark = x;
	// Save the mark ratio
	freq_mark_ratio =
		static_cast<gdouble>(x_mark - curve_buffer->inner_left())
		/ static_cast<gdouble>(curve_buffer->inner_width());
	gdouble i_mark = freq_mark_ratio * static_cast<gdouble>(num_points - 1);
	int i_lower = static_cast<int>(floor(i_mark));
	int i_upper = static_cast<int>(ceil(i_mark));
	gdouble dy_mark = 0.0;
	if (i_lower == i_upper)
		dy_mark = value_vector[i_upper];
	else
		dy_mark = value_vector[i_lower]
			+ (value_vector[i_upper] - value_vector[i_lower])
			/ (i_upper - i_lower)
			* (i_mark - i_lower);
	gdouble y_mark = curve_buffer->inner_bottom()
		- (dy_mark - value_min)
		* curve_buffer->yratio();
	// Draw marks on the curves
	curve_buffer->draw_crossing_marks(static_cast<gint>(x_mark),
																		static_cast<gint>(y_mark));
	// Refresh the gtk widget related to the drawing area,
	// which may be a magnitude entry or a phase entry.
	Buffer::gtk_entry_print_double(widget, dy_mark);
}
}

void Buffer::draw_marks_on_magnitude(gint x)
{
	draw_marks(x,
						 magnitude_curve_buffer_,
						 freq_mark_ratio_magnitude_,
						 magnitude_min_,
						 magnitude_vector_,
						 num_points_,
						 magnitude_entry_);
	// Refresh the frequency entry
	double freq = 0.0;
	// scaled frequency: LIN or LOG
	if (freq_scale_ == LIN)
		freq = freq_min_ + freq_mark_ratio_magnitude_
			* (freq_max_ - freq_min_);
	else // freq_scale_ == LOG
		freq = freq_min_ * pow(freq_max_ / freq_min_, freq_mark_ratio_magnitude_);
	gtk_entry_print_double(freq_entry_, freq); // Refresh the freq entry
}

void Buffer::draw_marks_on_phase(gint x)
{
	draw_marks(x,
						 phase_curve_buffer_,
						 freq_mark_ratio_phase_,
						 phase_min_,
						 phase_vector_,
						 num_points_,
						 phase_entry_);
	// The frequency entry is refreshed
	// in Buffer::draw_marks_on_magnitude()
}

namespace
{
void recover_marks(Curve        *curve_buffer,
									 double        freq_mark_ratio,
									 double        value_min,
									 const double *value_vector,
									 int           num_points,
									 GtkWidget    *widget)
{
	assert(curve_buffer);
	assert(curve_buffer->inner_width());
	assert(value_vector);
	assert(widget);
	// Use freq_mark_ratio to recover marks
	gdouble i_mark = freq_mark_ratio * static_cast<gdouble>(num_points - 1);
	int i_lower = static_cast<int>(floor(i_mark));
	int i_upper = static_cast<int>(ceil(i_mark));
	gdouble dy_mark = 0.0;
	if (i_lower == i_upper)
		dy_mark = value_vector[i_upper];
	else
		dy_mark = value_vector[i_lower]
			+ (value_vector[i_upper] - value_vector[i_lower])
			/ (i_upper - i_lower)
			* (i_mark - i_lower);
	gdouble y_mark = curve_buffer->inner_bottom()
		- (dy_mark - value_min)
		* curve_buffer->yratio();
	gdouble x_mark_new = freq_mark_ratio
		* curve_buffer->inner_width()
		+ curve_buffer->inner_left();
	// Draw marks on the curves
	curve_buffer->draw_crossing_marks(
		static_cast<gint>(x_mark_new),
		static_cast<gint>(y_mark));
	// Refresh the gtk entries
	Buffer::gtk_entry_print_double(widget, dy_mark);
}
}

void Buffer::recover_marks_on_magnitude()
{
	recover_marks(magnitude_curve_buffer_,
								freq_mark_ratio_magnitude_,
								magnitude_min_,
								magnitude_vector_,
								num_points_,
								magnitude_entry_);
	// Refresh the frequency entry
	double freq = 0.0;
	// scaled frequency: LIN or LOG
	if (freq_scale_ == LIN)
		freq = freq_min_ + freq_mark_ratio_magnitude_
			* (freq_max_ - freq_min_);
	else // freq_scale_ == LOG
		freq = pow(10,
							 log10(freq_min_) + freq_mark_ratio_magnitude_
							 * log10(freq_max_ / freq_min_));
	gtk_entry_print_double(freq_entry_, freq); // Refresh the freq entry
}

void Buffer::recover_marks_on_phase()
{
	recover_marks(phase_curve_buffer_,
								freq_mark_ratio_phase_,
								phase_min_,
								phase_vector_,
								num_points_,
								phase_entry_);
	// The frequency entry is refreshed
	// in Buffer::recover_marks_on_magnitude()
}

void Buffer::reset_parameter_value()
{
	if (!ptr_current_parameter_controller_)
		return;
	double original_value =
	 	ptr_current_parameter_controller_->original_value_;
	*ptr_current_parameter_controller_->ptr_value_ = original_value;
	double min = original_value, max = min + min, step = min;
	ptr_current_parameter_controller_->min_ = min;
	ptr_current_parameter_controller_->max_ = max;
	ptr_current_parameter_controller_->step_ = step;
	// Refresh entries
	print_on_par_min_entry(min /= current_unit_);
	print_on_par_max_entry(max /= current_unit_);
	print_on_par_step_entry(step /= current_unit_);
	print_on_par_value_entry(original_value /= current_unit_);
	// Reset the hscale
	gtk_range_set_range(GTK_RANGE(par_value_hscale_), min, max);
	gtk_range_set_increments(GTK_RANGE(par_value_hscale_), step, step * 2.0);
	gtk_range_set_value(GTK_RANGE(par_value_hscale_), original_value);
	// Rfresh curves
	change_parameter_value(original_value);
}

void Buffer::change_parameter_value(double d)
{
	double new_value = d * current_unit_;
	if (!ptr_current_parameter_controller_)
		return ;
	assert(ptr_current_parameter_controller_->ptr_value_);
	if (fabs(*ptr_current_parameter_controller_->ptr_value_ - new_value) < EPS_)
		return ; // Do nothing if the value does not change.
	*ptr_current_parameter_controller_->ptr_value_ = new_value;
	print_on_par_value_entry(new_value / current_unit_);
	evaluate_all(); // Reinit buffers and boundaries
	refresh_all();  // Redraw all curves and marks
}

void Buffer::get_parameter_value_from_par_value_entry()
{
	const gchar *entry_str = gtk_entry_get_text(GTK_ENTRY(par_value_entry_));
	double entry_d = std::strtod(entry_str, 0);
	change_parameter_value(entry_d); // Redraw curves
	// Force to change the boundary.
	double value = entry_d * current_unit_;
	if (value < ptr_current_parameter_controller_->min_)
	{
		ptr_current_parameter_controller_->min_ = value;
		print_on_par_min_entry(entry_d);
		set_parameter_controller_min(entry_d);
	}
	else if (value > ptr_current_parameter_controller_->max_)
	{
		ptr_current_parameter_controller_->max_ = value;
		print_on_par_max_entry(entry_d);
		set_parameter_controller_max(entry_d);
	}
	gtk_range_set_value(GTK_RANGE(par_value_hscale_), entry_d);
}

void Buffer::set_parameter_controller_unit(const string &unit_str)
{
	if (unit_str == string(""))
		current_unit_ = 1.0;
	else if (unit_str == string("f"))
		current_unit_ = 1.0e-15;
	else if (unit_str == string("p")) 
		current_unit_ = 1.0e-12;
	else if (unit_str == string("n"))
		current_unit_ = 1.0e-9;
	else if (unit_str == string("u"))
		current_unit_ = 1.0e-6;
	else if (unit_str == string("mil"))
		current_unit_ = 25.4e-6;
	else if (unit_str == string("m"))
		current_unit_ = 1.0e-3;
	else if (unit_str == string("k"))
		current_unit_ = 1.0e+3;
	else if (unit_str == string("M"))
		current_unit_ = 1.0e+6;
	else if (unit_str == string("G"))
		current_unit_ = 1.0e+9;
	else if (unit_str == string("T"))
		current_unit_ = 1.0e+12;
	else
		assert(!"Unkown unit!");
	if (ptr_current_parameter_controller_)
	{
		double min =
			ptr_current_parameter_controller_->min_ / current_unit_;
		double max =
			ptr_current_parameter_controller_->max_ / current_unit_;
		double step =
			ptr_current_parameter_controller_->step_ / current_unit_;
		double value =
			*ptr_current_parameter_controller_->ptr_value_ / current_unit_;
		// Refresh entries
		print_on_par_min_entry(min);
		print_on_par_max_entry(max);
		print_on_par_step_entry(step);
		print_on_par_value_entry(value);
		// Reset the hscale
		set_parameter_controller_min(min);
		set_parameter_controller_max(max);
		set_parameter_controller_step(step);
		gtk_range_set_value(GTK_RANGE(par_value_hscale_), value);
	}
}

void Buffer::set_parameter_controller(const string &par_name)
{
	ParameterController::iterator it = parameter_controller_map_.find(par_name);
	// Add a parameter controller for a new selected parameter.
	if (it == parameter_controller_map_.end())
	{
		pair<ParameterController::iterator, bool> insert_ret =
			parameter_controller_map_.insert(
				make_pair(par_name, new ParameterController_()));
		assert(insert_ret.second == true);
		ptr_current_parameter_controller_ = insert_ret.first->second;
		assert(ptr_current_parameter_controller_);
		ptr_current_parameter_controller_->ptr_value_ =
			ptr_analyser_->get_value_pointer(par_name);
		ptr_current_parameter_controller_->original_value_ =
			*ptr_current_parameter_controller_->ptr_value_;
		double original_value = ptr_current_parameter_controller_->original_value_;
		double min = original_value - EPS_; 
		double max = original_value * 2.0 + EPS_;
	 	double step = original_value;
		ptr_current_parameter_controller_->min_ = min;
		ptr_current_parameter_controller_->max_ = max;
		ptr_current_parameter_controller_->step_ = step;
	}
	else
	{
		ptr_current_parameter_controller_ = it->second;
		assert(ptr_current_parameter_controller_);
	}
	// Some callback functions below emit the "value_changed" signal
	// and may change the current value.
	// So here the current value must be recovered at the end.
	double value = *ptr_current_parameter_controller_->ptr_value_;
	// Refresh entries
	print_on_par_min_entry(
		ptr_current_parameter_controller_->min_ / current_unit_);
	print_on_par_max_entry(
		ptr_current_parameter_controller_->max_ / current_unit_);
	print_on_par_step_entry(
		ptr_current_parameter_controller_->step_ / current_unit_);
	print_on_par_value_entry(
		value / current_unit_);
	// Reset the hscale.
	gtk_range_set_range(
		GTK_RANGE(par_value_hscale_),
		ptr_current_parameter_controller_->min_ / current_unit_,
		ptr_current_parameter_controller_->max_ / current_unit_);
	gtk_range_set_increments(
		GTK_RANGE(par_value_hscale_),
		ptr_current_parameter_controller_->step_ / current_unit_,
		ptr_current_parameter_controller_->step_ / current_unit_ * 2.0);
	gtk_range_set_value(GTK_RANGE(par_value_hscale_), value / current_unit_);
	*ptr_current_parameter_controller_->ptr_value_ = value;
}

void Buffer::set_parameter_controller_min(double min)
{
	double max = ptr_current_parameter_controller_->max_ / current_unit_;
	if (ptr_current_parameter_controller_)
	{
		// Cache the current unavailable value
		ptr_current_parameter_controller_->min_ = min * current_unit_;
		// gtk_range_set_range() will emitte the "value_changed" signal.
		// The call back function on_par_value_hscale_value_changed()
		// invokes Buffer::change_parameter_value().
		// Therefore the parameter value will be clamped between min and max.
		if (min < max)
			gtk_range_set_range(GTK_RANGE(par_value_hscale_), min, max);
	}
}

void Buffer::set_parameter_controller_max(double max)
{
	double min = ptr_current_parameter_controller_->min_ / current_unit_;
	if (ptr_current_parameter_controller_)
	{
		// Cache the current unavailable value
		ptr_current_parameter_controller_->max_ = max * current_unit_;
		// gtk_range_set_range() will emitte the "value_changed" signal.
		// The call back function on_par_value_hscale_value_changed()
		// invokes Buffer::change_parameter_value().
		// Therefore the parameter value will be clamped between min and max.
		if (min < max)
			gtk_range_set_range(GTK_RANGE(par_value_hscale_), min, max);
	}
}

void Buffer::set_parameter_controller_step(double step)
{
	if (ptr_current_parameter_controller_)
	{
		ptr_current_parameter_controller_->step_ = step * current_unit_;
		gtk_range_set_increments(GTK_RANGE(par_value_hscale_), step, step * 2.0);
	}
}


#ifdef GUI3D

// 3D GUI Headers
#include "callback.h"
#include "surface.h"
#include "main_wrapper.h"
#include "parameter_group.h"
// OpenGL Headers
#ifdef _WIN32
#ifndef _STDCALL_SUPPORTED
#define _STDCALL_SUPPORTED
#endif // _STDCALL_SUPPORTED
#ifndef _M_IX86
#define _M_IX86
#endif // _M_IX86
#endif // _WIN32
#include <cstdlib> // Override another version of printf() defined in OpenGL
#include <GL/glut.h>

#else // GUI3D

// 2D GUI Headers
#include "buffer.h"
#include "analyser.h"
#include "auxiliary.h"
#include <glade/glade.h>
#include <gtk/gtk.h>

#endif // GUI3D

// Symbolic Analysis
#include "Analysis.h"
// C++ Standard Headers
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <stack>
#include <ctime>
#include <complex>
#include <vector>
#include <limits>
// C Standard Headers
#include <cstdio>
#include <cstddef>
#include <cmath>
#include <cfloat>
#include <cctype>
#include <cstring>
#include <cassert>
// Parallel Library Header
#ifdef _OPENMP
#include <omp.h>
#endif // _OPENMP

using namespace std;

#ifdef GUI3D
/// Phase Margin Parameter Group
ParameterGroup *ptr_parameter_group_pm_x = 0, *ptr_parameter_group_pm_y = 0;
/// Band width Parameter Group
ParameterGroup *ptr_parameter_group_bw_x = 0, *ptr_parameter_group_bw_y = 0;

/// 2D Normal pdf
/// global parameters of 2D normal pdf
GLdouble rou = 0.0;
GLdouble mu1 = 0.0, mu2 = 0.0;
/// 3 * sigma = scale / 4
GLdouble sigma1 = Surface<GLdouble>::xscale() / 12.0;
GLdouble sigma2 = Surface<GLdouble>::yscale() / 12.0;
GLdouble normal_pdf_2D(const GLdouble x, const GLdouble y, void *)
{
  static const GLdouble pi = 3.14159265358979323846264338327950288;
	return 1.0 / (2.0 * pi * sigma1 * sigma2 * sqrt(1.0 - rou * rou)) *
		exp(-1.0 / (2.0 * (1.0 - rou * rou)) *
				((x - mu1) / sigma1 * (x - mu1) / sigma1 +
				 (y - mu2) / sigma2 * (y - mu2) / sigma2 -
				 2.0 * rou * (x - mu1) / sigma1 * (y - mu2) / sigma2));
}

namespace
{
double gain_trigger_phase_margin = 0.0; // dB
double gain_trigger_band_width = 0.0; // dB
double freq_start = 1.0e-6, freq_end = 1.0e9; // Hz
int num_each_tf_samples = 50;

inline void string_to_upper(string &str)
{
  for (string::iterator it = str.begin(); it != str.end(); ++it)
    *it = toupper(*it);
}

template<typename T>
void swap(T &a, T &b)
{
  T tmp(a);
  a = b;
  b = tmp;
}

/// Allocate memory for ptr_pg within parse_parameter_group_line()
int parse_parameter_group_line(istream &istrm, ParameterGroup *&ptr_pg)
{
  string line, word;      // will hold a line and word from input, respectively
  cout << "Start Testing ->" << endl;
  // Ignore blank lines
  while (line.empty())
  {
    if (!getline(istrm, line)) // read a line from the input into line
    {
      cerr << "No input." << endl;
      return EXIT_FAILURE;
    }
  }
  string_to_upper(line); // convert all chars to their upper counterparts
  // do per-line processing
  vector<string> token_buffer;
  istringstream stream(line); // bind to stream to the line we read
  while (stream >> word)
  { // read a word from line
    // do per-word processing
    token_buffer.push_back(word);
    cout << "Token = " << word << endl;
  }
  stream.clear();

  cout << "buffe size: " << token_buffer.size() << endl;

  assert(token_buffer.size());
  switch (token_buffer.size())
  {
  case 1:
    cerr << "No related component is specified." << endl;
    return EXIT_FAILURE;
  case 2:
    cout << "A component and its alias are specified." << endl;
    cout << "Name_0: " << token_buffer[0] << endl;
    cout << "Alias:  " << token_buffer[1] << endl;
    // Construct a ParameterGroup object
    ptr_pg = new ParameterGroup(token_buffer[1], token_buffer[0]);
    return EXIT_SUCCESS;
  case 3:
    cerr << "Error: Missing arguments: alias!" << endl;
    return EXIT_FAILURE;
  }

  assert(token_buffer.size() > 3);
  vector<string>::size_type i = 0;
  for (; i < token_buffer.size() - 2; ++i)
    cout << "Name_" << i << ": " << token_buffer[i] << endl;
  double sc = atof(token_buffer[i].c_str());
  if (fabs(sc) < numeric_limits<double>::min())
  {
    cerr << "sc =" << sc << endl;
    cerr << "Error:" << endl;
    cerr << "A numerical value is expected as a scaling coefficient!" << endl;
    return EXIT_FAILURE;
  }
  if (++i >= token_buffer.size())
  {
    cerr << "Error: No alias specified." << endl;
    return EXIT_FAILURE;
  }
  cout << "Scaling Coefficient: " << sc << endl;
  cout << "Alias: " << token_buffer[token_buffer.size() - 1] << endl;
  vector<string> param_group;
  for (size_t i = 1; i < token_buffer.size() - 2; ++i)
    param_group.push_back(token_buffer[i]);
  // Construct a ParameterGroup object
  ptr_pg = new ParameterGroup(token_buffer[token_buffer.size() - 1],
                              token_buffer[0],
                              param_group,
                              sc);
  return EXIT_SUCCESS;
}

/// Interaction with customs
int get_sim_parameter(istream &istrm)
{
  // Show the banner
  cout << "+---------------------+" << endl;
  cout << "| Simulation Settings |" << endl;
  cout << "+---------------------+" << endl;

  cout << "Parameter 1: " << flush;
  // Allocate memory for ptr_parameter_group_pm_x
  if (parse_parameter_group_line(istrm, ptr_parameter_group_pm_x) == EXIT_FAILURE)
    return EXIT_FAILURE;

  double par_min = 0.0, par_max = 0.0;
  cout << "Parameter 1 Range: " << flush;
  istrm >> par_min >> par_max;
  if (par_min > par_max) swap(par_min, par_max);
  cout << par_min << ", " << par_max << endl;
  ptr_parameter_group_pm_x->min(par_min);
  ptr_parameter_group_pm_x->max(par_max);

  int num_par_samples = 0;
  cout << "Number of Parameter 1 samples: " << flush;
  istrm >> num_par_samples;
  cout << num_par_samples << endl;
  ptr_parameter_group_pm_x->num_samples(num_par_samples);

  cout << "Parameter 2: " << flush;
  // Allocate memory for ptr_parameter_group_pm_y
  if (parse_parameter_group_line(istrm, ptr_parameter_group_pm_y) == EXIT_FAILURE)
  {
    delete ptr_parameter_group_pm_x;
    ptr_parameter_group_pm_x = 0;
    return EXIT_FAILURE;
  }

  cout << "Parameter 2 Range: " << flush;
  istrm >> par_min >> par_max;
  if (par_min > par_max) swap(par_min, par_max);
  cout << par_min << ", " << par_max << endl;
  ptr_parameter_group_pm_y->min(par_min);
  ptr_parameter_group_pm_y->max(par_max);

  cout << "Number of Parameter 2 samples: " << flush;
  istrm >> num_par_samples;
  cout << num_par_samples << endl;
  ptr_parameter_group_pm_y->num_samples(num_par_samples);

  // Copy a ParameterGroup object for another design target
  ptr_parameter_group_bw_x = new ParameterGroup(*ptr_parameter_group_pm_x);
  ptr_parameter_group_bw_y = new ParameterGroup(*ptr_parameter_group_pm_y);

  // Other global settings
  cout << "Gain trigger for phase margin (dB): " << flush;
  istrm >> gain_trigger_phase_margin;
  cout << gain_trigger_phase_margin << endl;

  cout << "Gain trigger for band width (dB): " << flush;
  istrm >> gain_trigger_band_width;
  cout << gain_trigger_band_width << endl;

  cout << "Frequency Range(Hz): " << flush;
  istrm >> freq_start >> freq_end;
  cout << freq_start << ", " << freq_end << endl;

  cout << "Number of samples on each H(s) curve: " << flush;
  istrm >> num_each_tf_samples;
  cout << num_each_tf_samples << endl;

  // statistics
  istrm >> sigma1 >> sigma2 >> rou;
  cout << "sigma1 = " << sigma1 << endl;
  cout << "sigma2 = " << sigma2 << endl;
  cout << "row = " << rou << endl;
  sigma1 *= Surface<GLdouble>::xscale() /
  (ptr_parameter_group_pm_x->max() - ptr_parameter_group_pm_x->min());
  sigma2 *= Surface<GLdouble>::yscale() /
  (ptr_parameter_group_pm_y->max() - ptr_parameter_group_pm_y->min());

  return EXIT_SUCCESS;
}

/// Phase margin @ Gain == gain_trigger_phase_margin dB
GLdouble phase_margin(const GLdouble x, const GLdouble y, void *objptr)
{
#if 1
  Analyser *analyser = static_cast<Analyser*>(objptr);
  assert(analyser);
  // *par[0][0] = x;
  // *par[0][1] = y;
  ptr_parameter_group_pm_x->set_param(x);
  ptr_parameter_group_pm_y->set_param(y);
  analyser->evaluate_nf_coeff();
  // Hz
  const GLdouble logdf = log10(freq_end / freq_start) / num_each_tf_samples;
  GLdouble last_gain =
  20.0 * log10(abs(analyser->transfer_function(freq_start))); //dB
  GLdouble last_freq = freq_start;
  GLdouble current_gain = 0.0;
  GLdouble freq_at_gain = 0.0;
  for (GLdouble freq = freq_start * pow(10.0, logdf);
       freq < freq_end;
       freq *= pow(10.0, logdf))
  {
    current_gain =
    20.0 * log10(abs(analyser->transfer_function(freq))); //dB
    if ((last_gain > gain_trigger_phase_margin &&
         current_gain < gain_trigger_phase_margin) ||
        (last_gain < gain_trigger_phase_margin &&
         current_gain > gain_trigger_phase_margin))
    {
      freq_at_gain = freq +
      (gain_trigger_phase_margin - current_gain) *
      (last_freq - freq) / (last_gain - current_gain);
      break;
    }
    else
    {
      last_freq = freq;
      last_gain = current_gain;
    }
  }
  GLdouble phase = arg(analyser->transfer_function(freq_at_gain))
  * 180.0 / 3.1415926536; // here -180.0 <= phase <= 180.0
  return 180.0 + phase; // Phase margin unit: Degree
#else // 0
  // return std::sin(x + y*y);
  return 1 / (2*x*x + 3*y*y + 1);
#endif // 0
}

/// Band width (Hz) @ Gain == gain_trigger_band_width dB
GLdouble band_width(const GLdouble x, const GLdouble y, void *objptr)
{
#if 1
  Analyser *analyser = static_cast<Analyser*>(objptr);
  assert(analyser);
  // *par[1][0] = x;
  // *par[1][1] = y;
  ptr_parameter_group_bw_x->set_param(x);
  ptr_parameter_group_bw_y->set_param(y);
  analyser->evaluate_nf_coeff();
  // Hz
  const GLdouble logdf = log10(freq_end / freq_start) / num_each_tf_samples;
  GLdouble last_gain =
  20.0 * log10(abs(analyser->transfer_function(freq_start))); //dB
  GLdouble last_freq = freq_start;
  GLdouble current_gain = 0.0;
  GLdouble cutoff_freq = 0.0;
  for (GLdouble freq = freq_start * pow(10.0, logdf);
       freq < freq_end;
       freq *= pow(10.0, logdf))
  {
    current_gain =
    20.0 * log10(abs(analyser->transfer_function(freq))); //dB
    if (current_gain < gain_trigger_band_width)
    {
      cutoff_freq = freq +
      (gain_trigger_band_width - current_gain) * (last_freq - freq) /
      (last_gain - current_gain);
      break;
    }
    else
    {
      last_freq = freq;
      last_gain = current_gain;
    }
  }
  return cutoff_freq;
#else // 0
  // return std::sin(x*x + y*y);
  return 2*sqrt(4*x*x*x*x+9*y*y*y*y) / (1+2*x*x+3*y*y);
#endif // 0
}

typedef GLdouble
(*TargetFunctionType)(const GLdouble x, const GLdouble y, void *objptr);

inline GLdouble
normalized_sens_x_forward(GLdouble x0, GLdouble y0, GLdouble dx, void *objptr,
                          TargetFunctionType targ_fun)
{
  assert(fabs(dx) > numeric_limits<double>::min());
  GLdouble z0 = targ_fun(x0, y0, objptr);
  if (fabs(z0) < numeric_limits<double>::min())
    z0 = numeric_limits<double>::min(); 
  const GLdouble zforward = targ_fun(x0 - dx, y0, objptr);
  return x0 / z0 * (z0 - zforward) / dx;
}

inline GLdouble
normalized_sens_x_backward(GLdouble x0, GLdouble y0, GLdouble dx, void *objptr,
                           TargetFunctionType targ_fun)
{
  assert(fabs(dx) > numeric_limits<double>::min());
  GLdouble z0 = targ_fun(x0, y0, objptr);
  if (fabs(z0) < numeric_limits<double>::min())
    z0 = numeric_limits<double>::min(); 
  const GLdouble zbackward = targ_fun(x0 + dx, y0, objptr);
  return x0 / z0 * (zbackward - z0) / dx;
}

inline GLdouble
normalized_sens_x(GLdouble x0, GLdouble y0, GLdouble dx, void *objptr,
                  TargetFunctionType targ_fun)
{
  assert(fabs(dx) > numeric_limits<double>::min());
  GLdouble z0 = targ_fun(x0, y0, objptr);
  if (fabs(z0) < numeric_limits<double>::min())
    z0 = numeric_limits<double>::min(); 
  const GLdouble zforward = targ_fun(x0 - dx, y0, objptr);
  const GLdouble zbackward = targ_fun(x0 + dx, y0, objptr);
  return x0 / z0 * (zbackward - zforward) / dx / 2.0;
}

inline GLdouble
normalized_sens_y_forward(GLdouble x0, GLdouble y0, GLdouble dy, void *objptr,
                          TargetFunctionType targ_fun)
{
  assert(fabs(dy) > numeric_limits<double>::min());
  GLdouble z0 = targ_fun(x0, y0, objptr);
  if (fabs(z0) < numeric_limits<double>::min())
    z0 = numeric_limits<double>::min(); 
  const GLdouble zforward = targ_fun(x0, y0 - dy, objptr);
  return y0 / z0 * (z0 - zforward) / dy;
}

inline GLdouble
normalized_sens_y_backward(GLdouble x0, GLdouble y0, GLdouble dy, void *objptr,
                           TargetFunctionType targ_fun)
{
  assert(fabs(dy) > numeric_limits<double>::min());
  GLdouble z0 = targ_fun(x0, y0, objptr);
  if (fabs(z0) < numeric_limits<double>::min())
    z0 = numeric_limits<double>::min(); 
  const GLdouble zbackward = targ_fun(x0, y0 + dy, objptr);
  return y0 / z0 * (zbackward - z0) / dy;
}

inline GLdouble
normalized_sens_y(GLdouble x0, GLdouble y0, GLdouble dy, void *objptr,
                  TargetFunctionType targ_fun)
{
  assert(fabs(dy) > numeric_limits<double>::min());
  GLdouble z0 = targ_fun(x0, y0, objptr);
  if (fabs(z0) < numeric_limits<double>::min())
    z0 = numeric_limits<double>::min(); 
  const GLdouble zforward = targ_fun(x0, y0 - dy, objptr);
  const GLdouble zbackward = targ_fun(x0, y0 + dy, objptr);
  return y0 / z0 * (zbackward - zforward) / dy / 2.0;
}

/// Normalized sensitivity
/// sens_x = x0 / z0 * dz/dx, where x=x0, y=y0, z0=targ_fun(x0,y0)
GLdouble
sens_x(GLdouble x0, GLdouble y0, void *objptr,
       GLdouble xmin, GLdouble xmax, int num_points,
       TargetFunctionType targ_fun)

{
  assert (xmax > xmin);
  const GLdouble dx = 0.01 * (xmax - xmin) / (num_points - 1);
  if (x0 - dx > xmin && x0 + dx < xmax)
    return normalized_sens_x(x0, y0, dx, objptr, targ_fun);
  else if (x0 - dx <= xmin)
    return normalized_sens_x_backward(x0, y0, dx, objptr, targ_fun);
  else if (x0 + dx >= xmax)
    return normalized_sens_x_forward(x0, y0, dx, objptr, targ_fun);
  else
    assert(0); // Uncovered case
  return 0.0; // Erroneous case
}

/// Normalized sensitivity
/// sens_x = y0 / z0 * dz/dy, where x=x0, y=y0, z0=targ_fun(x0,y0)
GLdouble
sens_y(GLdouble x0, GLdouble y0, void *objptr,
       GLdouble ymin, GLdouble ymax, int num_points,
       TargetFunctionType targ_fun)
{
  assert(ymax > ymin);
  const GLdouble dy = 0.01 * (ymax - ymin) / (num_points - 1);
  if (y0 - dy > ymin && y0 + dy < ymax)
    return normalized_sens_y(x0, y0, dy, objptr, targ_fun);
  else if (y0 - dy <= ymin)
    return normalized_sens_y_backward(x0, y0, dy, objptr, targ_fun);
  else if (y0 + dy >= ymax)
    return normalized_sens_y_forward(x0, y0, dy, objptr, targ_fun);
  else
    assert(0); // Uncovered case
  return 0.0; // Erroneous case
}

/// Modulus of a gradient of phase margin
/// sqrt(sens_x^2 + sens_y^2)
GLdouble
sens_phase_margin(const GLdouble x, const GLdouble y, void *objptr)
{
#if 1
  assert(ptr_parameter_group_pm_x);
  assert(ptr_parameter_group_pm_y);
  assert(objptr);
  const GLdouble pm_sens_x = sens_x(x, y, objptr,
                                    ptr_parameter_group_pm_x->min(),
                                    ptr_parameter_group_pm_x->max(),
                                    ptr_parameter_group_pm_x->num_samples(),
                                    phase_margin);
  const GLdouble pm_sens_y = sens_y(x, y, objptr,
                                    ptr_parameter_group_pm_y->min(),
                                    ptr_parameter_group_pm_y->max(),
                                    ptr_parameter_group_pm_y->num_samples(),
                                    phase_margin);
  return sqrt(pm_sens_x*pm_sens_x + pm_sens_y*pm_sens_y);
#else
  return 1;
#endif
}

/// Modulus of a gradient of band width
/// sqrt(sens_x^2 + sens_y^2)
GLdouble
sens_band_width(const GLdouble x, const GLdouble y, void *objptr)
{
#if 1
  assert(ptr_parameter_group_bw_x);
  assert(ptr_parameter_group_bw_y);
  assert(objptr);
  const GLdouble bw_sens_x = sens_x(x, y, objptr,
                                    ptr_parameter_group_bw_x->min(),
                                    ptr_parameter_group_bw_x->max(),
                                    ptr_parameter_group_bw_x->num_samples(),
                                    band_width);
  const GLdouble bw_sens_y = sens_y(x, y, objptr,
                                    ptr_parameter_group_bw_y->min(),
                                    ptr_parameter_group_bw_y->max(),
                                    ptr_parameter_group_bw_y->num_samples(),
                                    band_width);
  return sqrt(bw_sens_x*bw_sens_x + bw_sens_y*bw_sens_y);
#else
  return 0;
#endif
}
} // unamed namespace

/// Global data pointers
MainWrapper *main_wrapper_global_phase_margin = 0;
MainWrapper *main_wrapper_global_phase_margin_sens = 0;
MainWrapper *main_wrapper_global_band_width = 0;
MainWrapper *main_wrapper_global_band_width_sens = 0;
MainWrapper *main_wrapper_global_normal_pdf_2D = 0;
#endif // GUI3D

/// Dump help information on screen.
inline void help_info()
{
  cout << "Synopsis:" << endl;
  cout << "grass Netlist[.cir] [-c ConfigFile]" << endl;
}


Analysis *ptr_analysis1 = 0;
Analyser *ptr_analyser1 = 0;

Analysis *ptr_analysis2 = 0;
Analyser *ptr_analyser2 = 0;

int main(int argc, char *argv[])
{
  string file_in;
  bool have_configure_file = false;

  // Check command line arguments
  for (int i = 1; i < argc; ++i)
  {
    // Use the configure file instead of interactive input if it exists.
    if (!strcmp(argv[i], "-c"))
    {
      ++i; // Next argument must be of "-c".
      if (i == argc)
      {
        cerr << "Missing argument for \"-c\"!" << endl;
        help_info();
        return EXIT_FAILURE;
      }
      ifstream ifstrm(argv[i]);
      if (!ifstrm)
      {
        cerr << "Cannot open configure file: " << argv[i] << endl;
        return EXIT_FAILURE;
      }
#ifdef GUI3D
      if (get_sim_parameter(ifstrm) == EXIT_FAILURE)
      {
        cerr << "Configure file parsing error!" << endl;
        return EXIT_FAILURE;
      }
      have_configure_file = true;
#endif // GUI3D
    }
    else
    {
      // Get the input file name and make an output file name.
      file_in = string(argv[i]);
    }
  }
#ifdef GUI3D
  if (!have_configure_file && get_sim_parameter(cin) == EXIT_FAILURE)
    return EXIT_FAILURE;
#endif // GUI3D

  // Output File: Inputfile + ".gph"
  string file_out = file_in + ".gph";

  cout << "\n Input file name: " << file_in
  << "\nOutput file name: " << file_out << endl;

#ifdef PARALLEL_COMPUTING
  cout << "Enable parallel mode (OpenMP)!" << endl;
#endif // PARALLEL_COMPUTING

  // Check OpenMP
#ifdef PARALLEL_COMPUTING
#ifdef _OPENMP
  cout << "OpenMP is taking effects." << endl;
#else // _OPENMP
  cout << "OpenMP is NOT taking effects." << endl;
#endif // _OPENMP
#endif // PARALLEL_COMPUTING

#ifndef GUI3D // GUI2D = 1
  ptr_analysis1 = new Analysis(); // Create a symbolic analysis
  ptr_analyser1 = new Analyser(ptr_analysis1); // Create a wrapper of symbolic analysis
  // Default scale type: phase_scale is always LINEAR.
  Buffer::SCALE_TYPE freq_scale = Buffer::LOG;
  Buffer::SCALE_TYPE magnitude_scale = Buffer::DB;
  // Default frequency range
  double start_freq = 0.01, stop_freq = 1.0e9; // Hz
  // Default number of numerical points plotting on a curve
  int num_points = 100;
  Buffer buffer(ptr_analyser1,
                start_freq, stop_freq, num_points,
                freq_scale, magnitude_scale);
  gtk_init(&argc, &argv);
  /* load the graphic user interface */
  GladeXML *xml = glade_xml_new(GLADE_FILE, "main_window", NULL);
  glade_xml_signal_autoconnect(xml);
  /* connect all signals to corresponding GtkWidgets */
  custom_xml_signal_connect(xml, &buffer);
  /* start the event loop */
  gtk_main();
  g_object_unref(G_OBJECT(xml));
  delete ptr_analyser1; // Destroy the wrapper
  delete ptr_analysis1; // Destroy the symbolic analysis
#else // GUI3D
  // Parallelly compute the phase margin and band width surfaces.
#ifdef PARALLEL_COMPUTING
#pragma omp parallel sections
  {
#pragma omp section
    {
#endif // PARALLEL_COMPUTING
      // Phase Margin
      printf("+-------------------------+\n");
      printf("| Phase Margin Evaluation |\n");
      printf("+-------------------------+\n");
      // Create a symbolic analysis.
      ptr_analysis1 = new Analysis();
      // Create a wrapper of symbolic analysis.
      ptr_analyser1 = new Analyser(ptr_analysis1);
      assert(ptr_parameter_group_pm_x);
      assert(ptr_parameter_group_pm_y);
      ptr_parameter_group_pm_x->set_ptr(ptr_analyser1);
      ptr_parameter_group_pm_y->set_ptr(ptr_analyser1);
      // Here are the global MainWrapper objects.
      printf("Phase Margin:\n");
      main_wrapper_global_phase_margin = new MainWrapper(
        "PM (Degree)",
        ptr_analyser1,
        ptr_parameter_group_pm_x->min(),
        ptr_parameter_group_pm_x->max(),
        ptr_parameter_group_pm_x->num_samples(),
        ptr_parameter_group_pm_y->min(),
        ptr_parameter_group_pm_y->max(),
        ptr_parameter_group_pm_y->num_samples(),
        phase_margin);
      printf("Gradient of Phase Margin:\n");
      main_wrapper_global_phase_margin_sens = new MainWrapper(
        "|grad(PM)|",
        ptr_analyser1,
        ptr_parameter_group_pm_x->min(),
        ptr_parameter_group_pm_x->max(),
        ptr_parameter_group_pm_x->num_samples(),
        ptr_parameter_group_pm_y->min(),
        ptr_parameter_group_pm_y->max(),
        ptr_parameter_group_pm_y->num_samples(),
        sens_phase_margin);
#ifdef PARALLEL_COMPUTING
    }
#pragma omp section
    {
#endif // PARALLEL_COMPUTING
      // Band Width
      printf("+-----------------------+\n");
      printf("| Band Width Evaluation |\n");
      printf("+-----------------------+\n");
      // Create a symbolic analysis which is totally the same as ptr_analysis1 
      // so that different threads get the different results from the same circuit.
      ptr_analysis2 = new Analysis();
      // Create a wrapper of symbolic analysis.
      ptr_analyser2 = new Analyser(ptr_analysis2);
      assert(ptr_parameter_group_bw_x);
      assert(ptr_parameter_group_bw_y);
      ptr_parameter_group_bw_x->set_ptr(ptr_analyser2);
      ptr_parameter_group_bw_y->set_ptr(ptr_analyser2);
      // Here are the global MainWrapper objects.
      printf("Band Width:\n");
      main_wrapper_global_band_width = new MainWrapper(
        "GBW (Hz)",
        ptr_analyser2,
        ptr_parameter_group_bw_x->min(),
        ptr_parameter_group_bw_x->max(),
        ptr_parameter_group_bw_x->num_samples(),
        ptr_parameter_group_bw_y->min(),
        ptr_parameter_group_bw_y->max(),
        ptr_parameter_group_bw_y->num_samples(),
        band_width);
      printf("Gradient of Band Width:\n");
      main_wrapper_global_band_width_sens = new MainWrapper(
        "|grad(GBW)|",
        ptr_analyser2,
        ptr_parameter_group_bw_x->min(),
        ptr_parameter_group_bw_x->max(),
        ptr_parameter_group_bw_x->num_samples(),
        ptr_parameter_group_bw_y->min(),
        ptr_parameter_group_bw_y->max(),
        ptr_parameter_group_bw_y->num_samples(),
        sens_band_width);
      printf("2D Normal Distribution:\n");
      main_wrapper_global_normal_pdf_2D = new MainWrapper(
        "2D Normal pdf",
        0,
        -0.5 * Surface<GLdouble>::xscale(),
        +0.5 * Surface<GLdouble>::xscale(),
        ptr_parameter_group_bw_x->num_samples(),
        -0.5 * Surface<GLdouble>::yscale(),
        +0.5 * Surface<GLdouble>::yscale(),
        ptr_parameter_group_bw_y->num_samples(),
        normal_pdf_2D);
#ifdef PARALLEL_COMPUTING
    }
  }
#endif // PARALLEL_COMPUTING
  glutInit(&argc, argv);

  // text status window
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowPosition(0, 485);
  glutInitWindowSize(425, 130); 

  main_wrapper_global_phase_margin->status_window = 
  glutCreateWindow("Status Window");
  main_wrapper_global_phase_margin_sens->status_window =
  main_wrapper_global_phase_margin->status_window;
  main_wrapper_global_band_width->status_window =
  main_wrapper_global_phase_margin->status_window;
  main_wrapper_global_band_width_sens->status_window =
  main_wrapper_global_phase_margin->status_window;
  main_wrapper_global_normal_pdf_2D->status_window =
  main_wrapper_global_phase_margin->status_window;

  glutDisplayFunc(Callback::display_status);
  glutReshapeFunc(Callback::reshape_status);

  // 2D contour window
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(425, 425); 

  main_wrapper_global_phase_margin->contour_window =
  glutCreateWindow("Contour Window");
  main_wrapper_global_phase_margin_sens->contour_window =
  main_wrapper_global_phase_margin->contour_window;
  main_wrapper_global_band_width->contour_window =
  main_wrapper_global_phase_margin->contour_window;
  main_wrapper_global_band_width_sens->contour_window =
  main_wrapper_global_phase_margin->contour_window;
  main_wrapper_global_normal_pdf_2D->contour_window =
  main_wrapper_global_phase_margin->contour_window;

  glutDisplayFunc(Callback::display_contour);
  glutReshapeFunc(Callback::reshape_contour);
  glutKeyboardFunc(Callback::keyboard_contour);
  glutMouseFunc(Callback::mouse_contour);

  // 3D surface window
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowPosition(435, 0);
  glutInitWindowSize(585, 585); 

  main_wrapper_global_phase_margin->surface_window =
  glutCreateWindow("Surface Window");
  main_wrapper_global_phase_margin_sens->surface_window =
  main_wrapper_global_phase_margin->surface_window;
  main_wrapper_global_band_width->surface_window =
  main_wrapper_global_phase_margin->surface_window;
  main_wrapper_global_band_width_sens->surface_window =
  main_wrapper_global_phase_margin->surface_window;
  main_wrapper_global_normal_pdf_2D->surface_window =
  main_wrapper_global_phase_margin->surface_window;

  glutDisplayFunc(Callback::display_surface);
  glutReshapeFunc(Callback::reshape_surface);
  glutKeyboardFunc(Callback::keyboard_surface);
  glutSpecialFunc(Callback::special_keyboard);
  glutMotionFunc(Callback::mouse_motion);

  // Create right button menus on 3D surface window
  glutCreateMenu(Callback::menu_surface);
  glutAddMenuEntry("Phase Margin", 1);
  glutAddMenuEntry("|grad(Phase Margin)|", 2);
  glutAddMenuEntry("Band Width", 3);
  glutAddMenuEntry("|grad(Band Width)|", 4);
  glutAddMenuEntry("2D Normal Distribution", 5);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  Callback::init();

  glutMainLoop();
#endif // GUI3D
  cout << "GRASS(2D) Exit normally." << endl;
  return 0;
}


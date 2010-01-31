#include"curve.h"
#include<cassert>
#include<cstdio>
#include<cmath>
#include<vector>
#include<gtk/gtk.h>
using std::sprintf;
using std::ceil;
using std::floor;
using std::vector;
using std::pow;
using std::log10;
using std::fabs;

inline int dtoi(double x)
{
	const double EPS = 1.7e-308;
	double dfloor = floor(x);
	assert(x >= dfloor);
	if (x - dfloor < EPS)
		return static_cast<int>((x < 0.0) ? (x - 0.1) : (x + 0.1));
	else if (x - dfloor < 0.5)
		return static_cast<int>((x < 0.0) ? (x - 1.0) : x);
	else
		return static_cast<int>((x < 0.0) ? x : (x + 1.0));
}

const double Curve::EPS_(1.7e-308);

Curve::Curve(const Curve2D &curve2d,
						 double xmin, double xmax,
						 double ymin, double ymax,
						 int npoints,
						 GtkWidget *widget) :
	pcurve2d_(&curve2d),
	xmin_(xmin), xmax_(xmax),
	ymin_(ymin), ymax_(ymax),
	num_points_(npoints),
	widget_(widget),
	width_(widget_->allocation.width),
	height_(widget_->allocation.height),
	frontground_gc_(widget_->style->fg_gc[GTK_WIDGET_STATE (widget)]),
	background_gc_(widget_->style->white_gc),
	curve_gc_(gdk_gc_new(widget_->window)),
	curve_mark_gc_(gdk_gc_new(widget_->window)),
	pixmap_(gdk_pixmap_new(widget_->window, width_, height_, -1))
{
	assert(pcurve2d_);
	assert(num_points_ >= 2);
	assert(xmax_ > xmin_);
	assert(ymax_ >= ymin_);
	assert(widget_);
	assert(width_ > 0);
	assert(height_ > 0);
	assert(frontground_gc_);
	assert(background_gc_);
	assert(curve_gc_);
	assert(pixmap_);
	// settings of curve color (RGB)
	GdkColor color;
	color.red = 0;
	color.green = 0;
	color.blue = 65535;
	gdk_gc_set_rgb_fg_color(curve_gc_, &color);
	// settings of mark color (RGB)
	color.red = 0;
	color.green = 65535;
	color.blue = 0;
	gdk_gc_set_rgb_fg_color(curve_mark_gc_, &color);
	gdk_gc_set_line_attributes(curve_mark_gc_,
													 	1,
													 	GDK_LINE_SOLID,
														GDK_CAP_NOT_LAST,
														GDK_JOIN_MITER);
	// settings of outer border
	// ratio of margins with reference to drawing area
	outer_.left_margin_ = 0.05;
	outer_.right_margin_ = 0.01;
	outer_.top_margin_ = 0.01;
	outer_.bottom_margin_ = 0.15;
	// boundary of outer border (pixels)
	outer_.left_ = dtoi(width_ * outer_.left_margin_);
	outer_.right_ = dtoi(width_ * (1 - outer_.right_margin_));
	outer_.top_ = dtoi(height_ * outer_.top_margin_);
	outer_.bottom_ = dtoi(height_ * (1 - outer_.bottom_margin_));
	// settings of inner border
	// ratio of margins with reference to outer border
	inner_.left_margin_ = 0.0;
	inner_.right_margin_ = 0.0;
	inner_.top_margin_ = 0.025;
	inner_.bottom_margin_ = 0.025;
	// boundary of inner border (pixels)
	gint width_outer = outer_.right_ - outer_.left_;
	gint height_outer = outer_.bottom_ - outer_.top_;
	inner_.left_ = dtoi(
		outer_.left_ + width_outer * inner_.left_margin_);
	inner_.right_ = dtoi(
		outer_.right_ -  width_outer * inner_.right_margin_);
	inner_.top_ = dtoi(
		outer_.top_ + height_outer * inner_.top_margin_);
	inner_.bottom_ = dtoi(
		outer_.bottom_ - height_outer * inner_.bottom_margin_);
	// init curve scales
	xratio_ = (inner_.right_ - inner_.left_) / (xmax_ - xmin_);
	yratio_ = (ymax_ - ymin_ > EPS_) ?
		(inner_.bottom_ - inner_.top_) / (ymax_ - ymin_) : 0.0;
}

Curve::~Curve()
{
	// Free the pix buffers
	if (pixmap_)
		g_object_unref(G_OBJECT(pixmap_)); 
}

namespace
{
inline double log1000(double x)
{ return log10(x) / log10(1000.0); }

// For each n = p * 10^q, where 1<p<1000, q=3*k, k is an integer,
// get the p and q.
// Use its absolute value first for a negative number.
PangoLayout *double_to_PangoLayout_eng(GtkWidget *widget, double x)
{
	const double EPS = 1.7e-308;
	double absx = fabs(x);
	double dk;
	double log1000p = modf(log1000(absx), &dk);
	int p, q, k;
	if (absx < 1.0)
	{
		p = dtoi(pow(1000.0, log1000p + 1.0));
		k = dtoi(dk - 1.0);
	}
	else
	{
		p = dtoi(pow(1000.0, log1000p));
		k = dtoi(dk);
	}
	q = 3 * k;
	if (x < 0.0) p = -p; // Revise p when x is negative.
	gchar text[50]; // b buffer string
	switch (q)
	{
	case -15: sprintf(text, "%d%s", p, "f"); break;
	case -12: sprintf(text, "%d%s", p, "p"); break;
	case -9:  sprintf(text, "%d%s", p, "n"); break;
	case -6:  sprintf(text, "%d%s", p, "u"); break;
	case -3:  sprintf(text, "%d%s", p, "m"); break;
	case  0:  sprintf(text, "%d", p); break;
	case  3:  sprintf(text, "%d%s", p, "k"); break;
	case  6:  sprintf(text, "%d%s", p, "M"); break;
	case  9:  sprintf(text, "%d%s", p, "G"); break;
	case 12:  sprintf(text, "%d%s", p, "T"); break;
	default:
						if (x < EPS) // zero
							sprintf(text, "0");
						else if (q < -15)
							sprintf(text, "%.0e%s", p * pow(10.0, q + 15), "f");
						else if (q > 12)
							sprintf(text, "%.0e%s", p * pow(10.0, q - 12), "T");
						else
							assert(!"The q is unbelievable!");
	}
	return gtk_widget_create_pango_layout(widget, text);
}
}

// X-axis Ratio: log10
void Curve::draw_axis_x_log()
{
	// for each x = n * 10^m
	// where n = 1,2,...,9, and m is an integer,
	// get the coefficient (n) and the exponent (m).
	// xscales = m + log10(n),
	// where m = start_m, start_m + 1, ..., stop_m - 1, stop_m,
	// and n = 1, 2, ..., 9.
	int start_n, start_m;
	if (xmin_ > 0)
	{
		double intpart;
		start_n = dtoi(ceil(pow(10.0, modf(xmin_, &intpart))));
		start_m = dtoi(intpart);
	}
	else // xmin_ <= 0
	{
		start_m = dtoi(floor(xmin_));
		start_n = dtoi(ceil(pow(10.0, xmin_ - start_m)));
	}
	// 10 * 10^m --> 1 * 10^(m+1)
	if (start_n == 10)
	{
		start_m += 1;
		start_n = 1;
	}
	assert(start_n > 0 && start_n < 10);
	int stop_n, stop_m;
	if (xmax_ > 0)
	{
		double intpart;
		stop_n = dtoi(floor(pow(10.0, modf(xmax_, &intpart))));
		stop_m =  dtoi(intpart);
	}
	else // xmax_ <= 0
	{
		stop_m = dtoi(floor(xmax_));
		stop_n = dtoi(floor(pow(10.0, xmax_ - stop_m)));
	}
	// 10 * 10^m --> 1 * 10^(m+1)
	if (stop_n == 10)
	{
		stop_m += 1;
		stop_n = 1;
	}
	assert(stop_n > 0 && stop_n < 10);
	assert(start_m <= stop_m);
	// calculate the array of scales on x-axis
	vector<double> xscale; // Logarithmic scale vector of x-axis
	if (start_m == stop_m)
	{
		for (int n = start_n; n <= stop_n; ++n)
			xscale.push_back(start_m + log10(double(n)));
	}
	else // start_m < stop_m
	{
		for (int n = start_n; n < 10; ++n)
			xscale.push_back(start_m + log10(double(n)));
		for (int m = start_m + 1; m < stop_m; ++m)
			for (int n = 1; n < 10; ++n)
				xscale.push_back(m + log10(double(n)));
		for (int n = 1; n <= stop_n; ++n)
			xscale.push_back(stop_m + log10(double(n)));
	}
	// draw a dummy horizontal ruler
	gint16 ruler_position = outer_.bottom_ + 3;
	gdk_draw_line(pixmap_, frontground_gc_,
								inner_.left_, ruler_position,
								inner_.right_, ruler_position);
	// draw scales by short lines with reference to the size of inner border
	int center = 0, last_center = 0; // pixels
	int width = 0, last_width = 0; // pixels
	const gint16 gap = 5; // pixels
	for (vector<double>::iterator it=xscale.begin(); it!=xscale.end(); ++it)
	{
		gint16 x = dtoi(inner_.left_ + (*it - xmin_) * xratio_);
		double intpart;
		gint16 len; // pixels
		if (fabs(modf(*it, &intpart)) < EPS_)
		{
			// using a longer line as the scale when *it is an integer.
			len = 5;
			// Draw numerical marks with longer scales
			center = x;
			PangoLayout *layout = double_to_PangoLayout_eng(widget_, pow(10.0, *it));
			pango_layout_get_pixel_size(layout, &width, NULL);
			if (center - last_center - gap > (width + last_width) / 2
					&& width_ - center > width / 2)
			{
				gdk_draw_layout(pixmap_, frontground_gc_,
												center - width / 2, ruler_position + len + 1,
												layout);
				last_center = center;
				last_width = width;
			}
			g_object_unref(layout);
		}
		else
		{
			// using a shorter line as the scale when *it is not an integer.
			len = 3;
		}
		// Draw scales
		gdk_draw_line(pixmap_, frontground_gc_,
									x, ruler_position,
									x, ruler_position + len);
	}
}

// Y-axis Ratio: 20 * log10
void Curve::draw_axis_y_dB()
{
	// for each y = n * 10^m
	// where n = 1,2,...,9, and m is an integer,
	// get the coefficient (n) and the exponent (m).
	// yscales = 20 * (m + log10(n)),
	// where m = start_m, start_m + 1, ..., stop_m - 1, stop_m,
	// and n = 1, 2, ..., 9.
	int start_n, start_m;
	if (ymin_ > 0)
	{
		double intpart;
		start_n = dtoi(ceil(pow(10.0, modf(ymin_/20.0, &intpart))));
		start_m = dtoi(intpart);
	}
	else // ymin_ <= 0
	{
		start_m = dtoi(floor(ymin_/20.0));
		start_n = dtoi(ceil(pow(10.0, ymin_/20.0 - start_m)));
	}
	// 10 * 10^m --> 1 * 10^(m+1)
	if (start_n == 10)
	{
		start_m += 1;
		start_n = 1;
	}
	assert(start_n > 0 && start_n < 10);
	int stop_n, stop_m;
	if (ymax_ > 0)
	{
		double intpart;
		stop_n = dtoi(floor(pow(10.0, modf(ymax_/20.0, &intpart))));
		stop_m =  dtoi(intpart);
	}
	else // ymax_ <= 0
	{
		stop_m = dtoi(floor(ymax_/20.0));
		stop_n = dtoi(floor(pow(10.0, ymax_/20.0 - stop_m)));
	}
	// 10 * 10^m --> 1 * 10^(m+1)
	if (stop_n == 10)
	{
		stop_m += 1;
		stop_n = 1;
	}
	assert(stop_n > 0 && stop_n < 10);
	assert(start_m <= stop_m);
	// Calculate the array of scales on y-axis
	vector<double> yscale; // Logarithmic scale vector of y-axis
	if (start_m == stop_m)
	{
		for (int n = start_n; n <= stop_n; ++n)
			yscale.push_back(20.0 * (start_m + log10(double(n))));
	}
	else // start_m < stop_m
	{
		for (int n = start_n; n < 10; ++n)
			yscale.push_back(20.0 * (start_m + log10(double(n))));
		for (int m = start_m + 1; m < stop_m; ++m)
			for (int n = 1; n < 10; ++n)
				yscale.push_back(20.0 * (m + log10(double(n))));
		for (int n = 1; n <= stop_n; ++n)
			yscale.push_back(20.0 * (stop_m + log10(double(n))));
	}
	// draw a dummy vertical ruler
	gint16 ruler_position = outer_.left_ - 3;
	gdk_draw_line(pixmap_, frontground_gc_,
								ruler_position, inner_.top_,
								ruler_position, inner_.bottom_);
	// draw scales by short lines with reference to the size of inner border
	int center = 0, last_center = height_; // pixels
	int height = 0, last_height = 0; // pixels
	int width = 0; // pixels
	const gint16 gap = 1; // pixels
	for (vector<double>::iterator it=yscale.begin(); it!=yscale.end(); ++it)
	{
		gint16 y = dtoi(inner_.bottom_ - (*it - ymin_) * yratio_);
		gint16 len; // pixels
		double intpart;
		if (fabs(modf(*it, &intpart)) < EPS_)
		{
			// using a longer line as the scale when *it is an integer.
			len = 4;
			// Draw numerical marks with longer scales
			center = y;
			PangoLayout *layout =
			 	double_to_PangoLayout_eng(widget_, *it);
			pango_layout_get_pixel_size(layout, &width, &height);
			if (last_center - center - gap > (height + last_height) / 2)
			{
				gdk_draw_layout(pixmap_, frontground_gc_,
												ruler_position - len - width -1, center - height / 2,
												layout);
				last_center = center;
				last_height = height;
			}
			g_object_unref(layout);
		}
		else
		{
			// using a shorter line as the scale when *it is not an integer.
			len = 2;
		}
		// Draw scales
		gdk_draw_line(pixmap_, frontground_gc_,
									ruler_position - len, y,
									ruler_position, y);
	}
}

void Curve::draw_axis_y_linear()
{
	const int num_scales = 9; // Specify the number of scales
	double distance = ymax_ - ymin_;
	vector<double> yscale;
	// Calculate the array of scales on y-axis
	for (int i = 0; i < num_scales; ++i)
		yscale.push_back(ymin_ + distance * i / (num_scales - 1));
	// Draw a dummy vertical ruler
	gint16 ruler_position = outer_.left_ - 3;
	gdk_draw_line(pixmap_, frontground_gc_,
								ruler_position, inner_.top_,
								ruler_position, inner_.bottom_);
	// Draw scales by short lines with reference to the size of inner border
	int center = 0, last_center = height_; // pixels
	int height = 0, last_height = 0; // pixels
	int width = 0; // pixels
	const gint16 gap = 1; // pixels
	for (vector<double>::iterator it=yscale.begin(); it!=yscale.end(); ++it)
	{
		gint16 y = dtoi(inner_.bottom_ - (*it - ymin_) * yratio_);
		gint16 len = 3; // pixels
		// Draw numerical marks with longer scales
		center = y;
		PangoLayout *layout =
			double_to_PangoLayout_eng(widget_, *it);
		pango_layout_get_pixel_size(layout, &width, &height);
		if (last_center - center - gap > (height + last_height) / 2)
		{
			gdk_draw_layout(pixmap_, frontground_gc_,
											ruler_position - len - width -1, center - height / 2,
											layout);
			last_center = center;
			last_height = height;
		}
		g_object_unref(layout);
		// Draw scales
		gdk_draw_line(pixmap_, frontground_gc_,
									ruler_position - len, y,
									ruler_position, y);
	}
}

void Curve::draw_curve()
{
	struct {double x, y;} last, current;
	last.x = inner_.left_;
	last.y = inner_.bottom_ - (pcurve2d_->y[0] - ymin_) * yratio_;
	for (int i=1; i < num_points_; ++i)
	{
		current.x = inner_.left_ + (pcurve2d_->x[i] - xmin_) * xratio_;
		current.y = inner_.bottom_ - (pcurve2d_->y[i] - ymin_) * yratio_;
		// connect points by short straight lines
		gdk_draw_line(pixmap_,
									curve_gc_,
									dtoi(last.x),
									dtoi(last.y),
									dtoi(current.x),
									dtoi(current.y));
		last.x = current.x;
		last.y = current.y;
	}
}

// Draw the marks on a drawable directly
void Curve::draw_crossing_marks(gint x_mark, gint y_mark)
{
	// Draw the bottom layout
	// flush();
	// Vertical mark line
	gdk_draw_line(widget_->window,
								curve_mark_gc_,
								x_mark, outer_.top_,
								x_mark, outer_.bottom_ + 3);
	// Horizontal mark line
	gdk_draw_line(widget_->window,
								curve_mark_gc_,
								outer_.left_ - 3, y_mark,
								outer_.right_, y_mark);
}

#ifndef _CURVE_H_
#define _CURVE_H_
#include<gtk/gtk.h>

class Curve
{
public:
	typedef struct { const double *x, *y; } Curve2D;
	Curve(const Curve2D &curve2d,
				double xmin, double xmax,
				double ymin, double ymax,
				int npoints,
				GtkWidget *widget);
	~Curve();
	void clear();
	void flush();
	void draw_log_dB();     // x->log, y->dB: e.g. Magnitude Curve
	void draw_log_linear(); // x->log, y->linear: e.g. Phase Curve
	gint inner_left() const;
	gint inner_right() const;
	gint inner_top() const;
	gint inner_bottom() const;
	gint inner_width() const;
	gdouble xratio() const;
	gdouble yratio() const;
	void draw_crossing_marks(gint x_mark, gint y_mark);
	GtkWidget *widget();
private:
	void draw_axis_x_log();
	void draw_axis_y_dB();
	void draw_axis_y_linear();
	void draw_border();
	void draw_curve();
	static const double EPS_;
	// Original data
	const Curve2D *pcurve2d_;
	double xmin_, xmax_;
	double ymin_, ymax_;
	int num_points_;
	//                        DRAWING AREA (widget)
	// Note: Curves are drawn on the inner area only
	// |←————————————————————————— width_ ———————————————————————————→|┊
	// ├──────────────────────────────────────────────────────────────┼——┈┈
	// │ outer_.left_                          outer_.right_          │ ↑
	// │ ↓                                          ↓                 │ |
	// │ ├──────────────────────────────────────────┼ ←outer_.top_    │ |
	// │ │ inner_.left_      inner_.right_          │                 │ |
	// │ │ ↓                      ↓                 │                 │ |
	// │ │ ├──────────────────────┼ ←inner_.top_    │                 │height_
	// │ │ │                      │                 │                 │ |
	// │ │ │                      │                 │                 │ |
	// │ │ │                      │                 │                 │ |
	// │ │ └──────────────────────┴ ←inner_.bottom_ │                 │ |
	// │ └──────────────────────────────────────────┴ ←outer_.bottom_ │ ↓
	// └──────────────────────────────────────────────────────────────┴——┈┈ 
	GtkWidget *widget_;     // drawing area
	gint width_;            // width f drawing area (pixel)
	gint height_;           // height of drawing area (pixel)
	GdkGC *frontground_gc_; // color of frontground
	GdkGC *background_gc_;  // color of background
	GdkGC *curve_gc_;       // color of curves
	GdkGC *curve_mark_gc_;  // color of marks on a curve
	GdkPixmap *pixmap_;     // buffer for curves (bottom layout)
	// settings of borders
	struct Border
	{
		// ratio of margins
		gdouble left_margin_, right_margin_;
		gdouble top_margin_, bottom_margin_;
		// boundary (pixels)
		gint left_, right_;
		gint top_, bottom_;
	} outer_, inner_;
	// curve ratios
	gdouble xratio_, yratio_;
};

// Inline Functions

// Draw the outer border
inline void Curve::draw_border()
{
	gdk_draw_rectangle(pixmap_,
										 frontground_gc_,
										 FALSE,
										 outer_.left_,
										 outer_.top_,
										 outer_.right_ - outer_.left_,
										 outer_.bottom_ - outer_.top_);
}

// Clear the buffer (pixmap_) of drawing area
inline void Curve::clear()
{
	gdk_draw_rectangle(pixmap_, background_gc_, TRUE, 0, 0, width_, height_);
}

// Copy the graph buffer to the drawing area
inline void Curve::flush()
{
	// copy the bottom layout to the drawable
	gdk_draw_drawable(widget_->window,  // destination drawable
										frontground_gc_,  // a GdkGC sharing the drawable's visual
										pixmap_,          // source drawable
										0, 0,             // starting position on source drawable
										0, 0,             // starting position on destination drawable
										width_, height_); // size of copying area (pixel)
}

// x->log, y->dB
inline void Curve::draw_log_dB()
{
	draw_border();
	draw_axis_x_log();
	draw_axis_y_dB();
	draw_curve();
}

// x->log, y->linear
inline void Curve::draw_log_linear()
{
	draw_border();
	draw_axis_x_log();
	draw_axis_y_linear();
	draw_curve();
}

inline gint Curve::inner_left() const
{ return inner_.left_; }

inline gint Curve::inner_right() const
{ return inner_.right_; }

inline gint Curve::inner_top() const
{ return inner_.top_; }

inline gint Curve::inner_bottom() const
{ return inner_.bottom_; }

inline gint Curve::inner_width() const
{ return inner_.right_ - inner_.left_; }

inline gdouble Curve::xratio() const
{ return xratio_; }

inline gdouble Curve::yratio() const
{ return yratio_; }

inline GtkWidget *Curve::widget()
{ return widget_; }
#endif // _CURVE_H_

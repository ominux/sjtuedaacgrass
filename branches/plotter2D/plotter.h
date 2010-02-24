// Reference: C++ GUI Programming with Qt 4, Chapter 5, Double Buffering
#ifndef PLOTTER_H
#define PLOTTER_H

#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>

//! forward declaration
class QToolButton;
class PlotSettings;

//! The Plotter widget displays one or more curves specified as vectors of coordinates.
/*!
 	The user can draw a rubber band on the image, and the Plotter will zoom in
	on the area enclosed by the rubber band. The user draws the rubber band by
	clicking a point on the graph, dragging the mouse to another position with
	the left mouse button held down, and releasing the mouse button.
	The user can zoom in repeatedly by drawing a rubber band multiple times,
	zooming out using the Zoom Out button and then zooming back in using
	the Zoom In button. The Zoom In and Zoom Out buttons appear the first time
	they become available so that they don't clutter the display if the user
	doesn't zoom the graph.
	The Plotter widget can hold the data for any number of curves. It also
	maintains a stack of PlotSettings objects, each of which corresponds to
	a particular zoom level.
 */
class Plotter : public QWidget
{
	Q_OBJECT

public:
	enum {
		CURVES = 4 //!< size of the buffer which contains all curves
	};

	//! default constructor
	Plotter(QWidget *parent = 0);
	//! destructor
	virtual ~Plotter();
	//! The setPlotSettings() function is used to specify the PlotSettings to use
	//! for displaying the plot.
	void setPlotSettings(const PlotSettings &settings);
	//! The setCurveData() function sets the curve data for a given curve ID.
	/*!
	 	If a curve with the same ID already exists in curveMap, it is replaced with
	 	the new curve data; otherwise, the new curve is simply inserted.
		\sa curveMap, refreshPixmap().
	 */
	void setCurveData(int id,
										const QVector<QPointF> &data,
									 	const PlotSettings &settings);
	//! The clearCurve() function removes the specified curve from the curve map.
	//! \sa curveMap, refreshPixmap().
	void clearCurve(int id);
	//! Specifies a widget's ideal minimum size.
 	//! A layout never resizes a widget below its minimum size hint.
	//! \sa sizeHint().
	QSize minimumSizeHint() const;
	//! Return an "ideal" size.
	//! \sa minimumSizeHint().
	QSize sizeHint() const;

public slots:
	//!	Zoom out if the graph is zoomed in.
	/*!
	 	It decrements the current zoom level and enables the Zoom Out button
	 	depending on whether the graph can be zoomed out any more or not. The
	 	Zoom In button is enabled and shown, and the display is updated with a call
	 	to refreshPixmap().
		\sa zoomIn(), refreshPixmap().
	 */
	void zoomOut();
	//! Inverse operation of zoom out.
	/*!
		If the user has previously zoomed in and then out again, the PlotSettings
		for the next zoom level will be in the zoom stack, and we can zoom in.
		Otherwise, it is still possible to zoom in using a rubber band.
		The slot increments curZoom to move one level deeper into the zoom stack,
	 	sets the Zoom In button enabled or disabled depending on whether it's
	 	possible to zoom in any further, and enables and shows the Zoom Out button.
	 	Always call refreshPixmap() to make the plotter use the latest zoom settings.
		\sa zoomOut(), refreshPixmap().
	 */
	void zoomIn();

	//! override functions (i.e. event handlers) derived from QWidget
protected:
	//! Render the entire plot area.
	/*!
		Render the entire plot simply by copying the pixmap onto the widget at
	 	position (0, 0). All the plot drawing is done beforehand in refreshPixmap().
		\sa refreshPixmap().
	 */
	void paintEvent(QPaintEvent *event);
	//! Whenever the Plotter widget is resized, Qt generates a "resize" event.
	//! Qt always generates a resize event before a widget is shown for the first time.
	void resizeEvent(QResizeEvent *event);
	//! When the user presses the left mouse button, display a rubber band.
	/*!
		This involves setting rubberBandIsShown to true, initializing the
	 	rubberBandRect member variable with the current mouse pointer position,
	 	scheduling a paint event to paint the rubber band, and changing the mouse
	 	cursor to have a crosshair shape.
		\sa rubberBandIsShown.
	 */
	void mousePressEvent(QMouseEvent *event);
	//! When the user moves the mouse cursor while holding down the left button,
	//! effectively erases the rubber band and redraws it at the new coordinates.
	//! \sa updateRubberBandRegion()
	void mouseMoveEvent(QMouseEvent *event);
	//! When the user releases the left mouse button, erase the rubber band and
	//!	restore the standard arrow cursor.
	void mouseReleaseEvent(QMouseEvent *event);
	//! When the user presses a key and the Plotter widget has focus,
	//! the keyPressEvent() function is called.
	//! The Shift, Ctrl, and Alt modifier keys are available
	//! through QKeyEvent::modifiers().
	void keyPressEvent(QKeyEvent *event);
	//! Wheel events occur when a mouse wheel is turned.
	//! Wheel events go to the widget that has the focus.
	void wheelEvent(QWheelEvent *event);

private:
	//! Force a repaint of the (tiny) area covered by the rubber band.
	void updateRubberBandRegion();
	//! Keep a QPixmap up-to-date at all times.
	/*!
	 	After regenerating the pixmap, refreshPixmap() calls update() to copy
	 	the pixmap onto the widget.
	 */
	void refreshPixmap();
	//! Draw the grid behind the curves and the axes.
	void drawGrid(QPainter *painter);
	//! Draw the curves on top of the grid.
	void drawCurves(QPainter *painter);

	enum { Margin = 30 }; //!< provide some spacing around the graph

	QToolButton *zoomOutButton; //!< Zoom Out Button
	QToolButton *zoomInButton;  //!< Zoom In Button 
	QMap<int, QVector<QPointF> > curveMap; //!< hold points of all curves
	QVector<PlotSettings> zoomStack; //!< hold the different zoom settings
	int curZoom; //!< hold the current PlotSettings's index in the zoomStack.
	//! hold the status of drawing a rubber band when zooming in or out
	bool rubberBandIsShown;
	QRect rubberBandRect; //!< hold the rubber band rectangle
	//! render area buffer
	/*!
		Variable pixmap holds a copy of the whole widget's rendering,
		identical to what is shown on-screen.
		The plot is always drawn onto this off-screen buffer (pixmap) first;
		then the buffer is copied onto the widget (i.e. double buffering).
	 */
	QPixmap pixmap; 
};

//! The PlotSettings class specifies the range of the x- and y-axes and 
//! the number of ticks for these axes.
class PlotSettings
{
public:
	//! default constructor
	PlotSettings();
	//! constructor
	PlotSettings(double minX, double MaxX, int numXTicks,
							 double minY, double MaxY, int numYTicks);
	//! copy constructor
	PlotSettings(const PlotSettings &settings);
	//! destructor
	~PlotSettings();
	//! overloaded operator=()
	PlotSettings &operator=(const PlotSettings &settings);
	double spanX() const;
	double spanY() const;
	//! Increment (or decrement) minX, maxX, minY, and maxY by the interval
 	//! between two ticks times a given number.
 	//! This function is used to implement scrolling in Plotter::keyPressEvent().
	//! \sa Plotter::keyPressEvent().
	void scroll(int dx, int dy);
	//! round the minX, maxX, minY, and maxY values to "nice" values
 	//! and to determine the number of ticks appropriate for each axis
	//! \sa adjustAxis();
	void adjust();

public:
	double minX; //!< minimum value of all X
	double maxX; //!< maximum value of all x
	int numXTicks; //!< number of ticks on x-axis
	double minY; //!< minimum value of all y
	double maxY; //!< minimum value of all y
	int numYTicks; //!< minimum value of all y

private:
	//! Determine an appropriate value for the interval between two ticks (the "step")
	/*!
		Converts its min and max parameters into "nice" numbers and sets its
		numTicks parameter to the number of ticks it calculates to be appropriate
	 	for the given [min, max] range.
	 */
	static void adjustAxis(double &min, double &max, int &numTicks);
};

#endif

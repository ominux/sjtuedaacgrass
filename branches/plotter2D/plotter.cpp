// Reference: C++ GUI Programming with Qt 4, Chapter 5, Double Buffering
#include "plotter.h"

#include <QtGui>
#include <cmath>

using namespace std;

Plotter::
Plotter(QWidget *parent) :
 	QWidget(parent), zoomOutButton(0), zoomInButton(0),
	curZoom(0), rubberBandIsShown(false)
{
	/*!
		The setBackgroundRole() call tells QWidget to use the "dark" component
	 	of the palette as the color for erasing the widget, instead of the "window"
	 	component. This gives Qt a default color that it can use to fill any newly
	 	revealed pixels when the widget is resized to a larger size, before
	 	paintEvent() even has the chance to paint the new pixels. need to call 
		setAutoFillBackground(true) to enable this mechanism. (By default,
	 	child widgets inherit the background from their parent widget.)
	 */
	setBackgroundRole(QPalette::Dark);
	setAutoFillBackground(true);
	/*!
		The setSizePolicy() call sets the widget's size policy to
		QSizePolicy::Expanding in both directions. This tells any layout manager
		that is responsible for the widget that the widget is especially willing to
	 	grow, but can also shrink. This setting is typical for widgets that can
	 	take up a lot of screen space. The default is QSizePolicy::Preferred in
	 	both directions, which means that the widget prefers to be the size of its
	 	size hint, but it can be shrunk to its minimum size hint or expanded 
		indefinitely if necessary.
		*/
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	/*!
		The setFocusPolicy(Qt::StrongFocus) call makes the widget accept focus by
	 	clicking or by pressing Tab. When the Plotter has focus, it will receive
	 	events for key presses.
	 */
	setFocusPolicy(Qt::StrongFocus);
	/*!
		Create two QToolButtons, each with an icon. These buttons allow the user to
	 	zoom in and out. The buttons' icons are stored in a resource file.
	 */
	//! zoom in
	zoomInButton = new QToolButton(this);
	zoomInButton->setIcon(QIcon(":/images/zoomin.png"));
	/*!
		The adjustSize() calls on the buttons set their sizes to be that of their
	 	size hints. The buttons are not put in a layout; instead, we will position
	 	them manually in the Plotter's resize event.
	 	Since we are not using any layouts, we must specify the buttons' parent
	 	explicitly by passing this to the QToolButton constructor.
	 */
	zoomInButton->adjustSize();
	connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
	//! zoom out
	zoomOutButton = new QToolButton(this);
	zoomOutButton->setIcon(QIcon(":/images/zoomout.png"));
	zoomOutButton->adjustSize();
	connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));
}

Plotter::
~Plotter()
{}

/*!
	After the call to setPlotSettings(), the zoomStack contains only one entry,
	and the ZoomIn and ZoomOut buttons are hidden. These buttons will not be
	shown until calling show() on them in the zoomIn() and zoomOut() slots.
	Normally, it is sufficient to call show() on the top-level widget to show
	all the children. But when explicitly call hide() on a child widget, it is
	hidden until calling show() right on it.
 */
void Plotter::
setPlotSettings(const PlotSettings &settings)
{
	zoomStack.clear();
	zoomStack.append(settings);
	curZoom = 0;
	zoomInButton->hide();
	zoomOutButton->hide();
	refreshPixmap();
}

void Plotter::
zoomOut()
{
	if (curZoom > 0) {
		--curZoom;
		zoomOutButton->setEnabled(curZoom > 0);
		zoomInButton->setEnabled(true);
		zoomInButton->show();
		refreshPixmap();
	}
}

void Plotter::
zoomIn()
{
	if (curZoom < zoomStack.count() - 1) {
		++curZoom;
		zoomInButton->setEnabled(curZoom < zoomStack.count() - 1);
		zoomOutButton->setEnabled(true);
		zoomOutButton->show();
		refreshPixmap();
	}
}

void Plotter::
setCurveData(int id, const QVector<QPointF> &data, const PlotSettings &settings)
{
	setPlotSettings(settings);
	curveMap[id] = data;
	refreshPixmap();
}

void Plotter::
clearCurve(int id)
{
	curveMap.remove(id);
	refreshPixmap();
}

QSize Plotter::
minimumSizeHint() const
{
	return QSize(6 * Margin, 4 * Margin);
}

QSize Plotter::
sizeHint() const
{
	return QSize(12 * Margin, 8 * Margin);
}

void Plotter::
paintEvent(QPaintEvent * /* event */)
{
	QStylePainter painter(this);
	painter.drawPixmap(0, 0, pixmap);

	if (rubberBandIsShown) {
		painter.setPen(palette().light().color());
		painter.drawRect(rubberBandRect.normalized()
										 .adjusted(0, 0, -1, -1));
	}

	if (hasFocus()) {
		QStyleOptionFocusRect option;
		option.initFrom(this);
		option.backgroundColor = palette().dark().color();
		painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
	}
}

void Plotter::
resizeEvent(QResizeEvent * /* event */)
{
	int x = width() - (zoomInButton->width()
										 + zoomOutButton->width() + 10);
	//! place the Zoom In and Zoom Out buttons at the top right
	zoomInButton->move(x, 5);
	zoomOutButton->move(x + zoomInButton->width() + 5, 5);
	//! redraw the pixmap at the new size
	refreshPixmap();
}

void Plotter::
mousePressEvent(QMouseEvent *event)
{
	QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);

	if (event->button() == Qt::LeftButton) {
		if (rect.contains(event->pos())) {
			rubberBandIsShown = true;
			rubberBandRect.setTopLeft(event->pos());
			rubberBandRect.setBottomRight(event->pos());
			updateRubberBandRegion();
			//! set the cursor shape to use when the mouse hovers over rect
			setCursor(Qt::CrossCursor);
		}
	}
}

void Plotter::
mouseMoveEvent(QMouseEvent *event)
{
	if (rubberBandIsShown) {
		//! schedule a paint event to repaint the area where the rubber band was
		updateRubberBandRegion();
		//! recompute rubberBandRect to account for the mouse move
		rubberBandRect.setBottomRight(event->pos());
		//! repaint the area where the rubber band has moved to
		updateRubberBandRegion();
	}
}

void Plotter::
mouseReleaseEvent(QMouseEvent *event)
{
	if ((event->button() == Qt::LeftButton) && rubberBandIsShown) {
		rubberBandIsShown = false;
		updateRubberBandRegion();
		unsetCursor();

		QRect rect = rubberBandRect.normalized();
		//! perform the zoom if the rubber band is at least 4 x 4
		if (rect.width() < 4 || rect.height() < 4)
			return;
		rect.translate(-Margin, -Margin);
		/*!
			The plotter starts out at its default zoom level. Each time the user zooms
			in, a new PlotSettings instance is created and put onto the zoom stack. 
		 */
		PlotSettings prevSettings(zoomStack[curZoom]);
		PlotSettings settings(prevSettings);
		double dx = prevSettings.spanX() / (width() - 2 * Margin);
		double dy = prevSettings.spanY() / (height() - 2 * Margin);
		settings.minX = prevSettings.minX + dx * rect.left();
		settings.maxX = prevSettings.minX + dx * rect.right();
		settings.minY = prevSettings.maxY - dy * rect.bottom();
		settings.maxY = prevSettings.maxY - dy * rect.top();
		settings.adjust();

		zoomStack.resize(curZoom + 1);
		zoomStack.append(settings);
		zoomIn();
	}
}

void Plotter::
keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Plus:
		zoomIn();
		break;
	case Qt::Key_Minus:
		zoomOut();
		break;
	case Qt::Key_Left:
		zoomStack[curZoom].scroll(-1, 0);
		refreshPixmap();
		break;
	case Qt::Key_Right:
		zoomStack[curZoom].scroll(+1, 0);
		refreshPixmap();
		break;
	case Qt::Key_Down:
		zoomStack[curZoom].scroll(0, -1);
		refreshPixmap();
		break;
	case Qt::Key_Up:
		zoomStack[curZoom].scroll(0, +1);
		refreshPixmap();
		break;
	default:
		QWidget::keyPressEvent(event);
	}
}

void Plotter::
wheelEvent(QWheelEvent *event)
{
	//! The delta() function returns the distance the wheel was rotated in eighths of a degree.
	int numDegrees = event->delta() / 8;
	//! Mice typically work in steps of 15 degrees.
	int numTicks = numDegrees / 15;
	//! scroll by the requested number of ticks by modifying the topmost item
 	//! on the zoomStack
	if (event->orientation() == Qt::Horizontal) {
		zoomStack[curZoom].scroll(numTicks, 0);
	} else {
		zoomStack[curZoom].scroll(0, numTicks);
	}
	//! update the display
	refreshPixmap();
}

void Plotter::
updateRubberBandRegion()
{
	/*!
		If the user moves the mouse upward or leftward, it's likely that
	 	rubberBandRect's nominal bottom-right corner will end up above or to the
	 	left of its top-left corner. If this occurs, the QRect will have a negative
	 	width or height.
	 	QRect::normalized() in paintEvent() to ensure that the top-left and
	 	bottom-right coordinates are adjusted to obtain a nonnegative width and
	 	height.
	 */
	QRect rect = rubberBandRect.normalized();
	update(rect.left(), rect.top(), rect.width(), 1);
	update(rect.left(), rect.top(), 1, rect.height());
	update(rect.left(), rect.bottom(), rect.width(), 1);
	update(rect.right(), rect.top(), 1, rect.height());
}

void Plotter::
refreshPixmap()
{
	pixmap = QPixmap(size());
	pixmap.fill(this, 0, 0);

	QPainter painter(&pixmap);
	painter.initFrom(this);
	drawGrid(&painter);
	drawCurves(&painter);
	update();
}

void Plotter::
drawGrid(QPainter *painter)
{
	/*!
		The area on which we draw the grid is specified by rect. If the widget
	 	isn't large enough to accommodate the graph, we return immediately.
	 */
	QRect rect(Margin, Margin,
						 width() - 2 * Margin, height() - 2 * Margin);
	if (!rect.isValid())
		return;

	PlotSettings settings = zoomStack[curZoom];
	QPen quiteDark = palette().dark().color().light();
	QPen light = palette().light().color();
	//! draw the grid's vertical lines and the ticks along the x-axis
	for (int i = 0; i <= settings.numXTicks; ++i) {
		int x = rect.left() + (i * (rect.width() - 1)
													 / settings.numXTicks);
		double label = settings.minX + (i * settings.spanX()
																		/ settings.numXTicks);
		painter->setPen(quiteDark);
		painter->drawLine(x, rect.top(), x, rect.bottom());
		painter->setPen(light);
		painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
		//! draw the numbers corresponding to the tick marks on x-axes
		/*!
			The calls to drawText() have the following syntax:
			painter->drawText(x, y, width, height, alignment, text);
			where (x, y, width, height) define a rectangle, alignment the position of
		 	the text within that rectangle, and text the text to draw.
			Here we have calculated the rectangle in which to draw the text manually.
		 	A more adaptable alternative would involve calculating the text's bounding rectangle using QFontMetrics.
		 */
		painter->drawText(x - 50, rect.bottom() + 5, 100, 15,
											Qt::AlignHCenter | Qt::AlignTop,
											QString::number(label));
	}
	//! draw the grid's horizontal lines and the ticks along the y-axis
	for (int j = 0; j <= settings.numYTicks; ++j) {
		int y = rect.bottom() - (j * (rect.height() - 1)
														 / settings.numYTicks);
		double label = settings.minY + (j * settings.spanY()
																		/ settings.numYTicks);
		painter->setPen(quiteDark);
		painter->drawLine(rect.left(), y, rect.right(), y);
		painter->setPen(light);
		painter->drawLine(rect.left() - 5, y, rect.left(), y);
		//! draw the numbers corresponding to the tick marks on y-axes
		painter->drawText(rect.left() - Margin, y - 10, Margin - 5, 20,
											Qt::AlignRight | Qt::AlignVCenter,
											QString::number(label));
	}
	//! draw a rectangle along the margins
	painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

void Plotter::
drawCurves(QPainter *painter)
{
	static const QColor colorForIds[CURVES] = {
		// Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow
		Qt::red, Qt::green, Qt::blue, Qt::cyan
	};
	PlotSettings settings(zoomStack[curZoom]);
	/*!
		Set the QPainter's clip region to the rectangle that contains the curves
		excluding the margins and the frame around the graph.
		QPainter will then ignore drawing operations on pixels outside the area.
	 */
	QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
	if (!rect.isValid())
		return;
	painter->setClipRect(rect.adjusted(+1, +1, -1, -1));
	QMapIterator<int, QVector<QPointF> > i(curveMap);
	while (i.hasNext()) {
		i.next();

		int id = i.key();
		const QVector<QPointF> &data = i.value();
		QPolygonF polyline(data.count()); //! buffer for each curve
		/*!
			Convert each QPointF from plotter coordinates to widget coordinates
		 	and stores them in the polyline variable.
		 */
		for (int j = 0; j < data.count(); ++j) {
			double dx = data[j].x() - settings.minX;
			double dy = data[j].y() - settings.minY;
			double x = rect.left() + (dx * (rect.width() - 1)
																/ settings.spanX());
			double y = rect.bottom() - (dy * (rect.height() - 1)
																	/ settings.spanY());
			polyline[j] = QPointF(x, y);
		}
		//! set the pen color for the curve using one of a set of predefined colors
		painter->setPen(colorForIds[uint(id) % CURVES]);
		//! draw a line that goes through all the curve's points
		painter->drawPolyline(polyline);
	}
}

PlotSettings::
PlotSettings() :
	minX(0.0), maxX(1.0), numXTicks(2),
	minY(0.0), maxY(1.0), numYTicks(2)
{}

PlotSettings::
PlotSettings(double minX, double maxX, int numXTicks,
						 double minY, double maxY, int numYTicks) :
	minX(minX), maxX(maxX), numXTicks(numXTicks),
	minY(minY), maxY(maxY), numYTicks(numYTicks)
{}

// copy constructor
PlotSettings::
PlotSettings(const PlotSettings &settings) :
	minX(settings.minX), maxX(settings.maxX), numXTicks(settings.numXTicks),
	minY(settings.minY), maxY(settings.maxY), numYTicks(settings.numYTicks)
{}

PlotSettings::
~PlotSettings()
{}

PlotSettings &PlotSettings::
operator=(const PlotSettings &settings)
{
	// handle self-assignment
	if (this == &settings)
		return *this;
	minX = settings.minX;
 	maxX = settings.maxX;
	numXTicks = settings.numXTicks;

	minY = settings.minY;
 	maxY = settings.maxY;
	numYTicks = settings.numYTicks;
	return *this;
}

double PlotSettings::
spanX() const
{
 	return maxX - minX;
}

double PlotSettings::
spanY() const
{
 	return maxY - minY;
}

void PlotSettings::
scroll(int dx, int dy)
{
	double stepX = spanX() / numXTicks;
	minX += dx * stepX;
	maxX += dx * stepX;

	double stepY = spanY() / numYTicks;
	minY += dy * stepY;
	maxY += dy * stepY;
}

void PlotSettings::
adjust()
{
	adjustAxis(minX, maxX, numXTicks);
	adjustAxis(minY, maxY, numYTicks);
}

//! ticks are of the format: 10^n, 2*10^n or 5*10^n
void PlotSettings::
adjustAxis(double &min, double &max, int &numTicks)
{
	const int MinTicks = numTicks;
	double grossStep = (max - min) / MinTicks;
	double step = pow(10.0, floor(log10(grossStep)));

	if (5 * step < grossStep) {
		step *= 5;
	} else if (2 * step < grossStep) {
		step *= 2;
	}

	numTicks = int(ceil(max / step) - floor(min / step));
	if (numTicks < MinTicks)
		numTicks = MinTicks;
	min = floor(min / step) * step;
	max = ceil(max / step) * step;
}

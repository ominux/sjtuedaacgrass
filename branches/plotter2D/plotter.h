#ifndef PLOTTER_H
#define PLOTTER_H

#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>

class QToolButton;
class PlotSettings;

class Plotter : public QWidget
{
	Q_OBJECT

public:
	enum {CURVES = 6}; // curve buffer size

	Plotter(QWidget *parent = 0);
	void setPlotSettings(const PlotSettings &settings);
	void setCurveData(int id,
										const QVector<QPointF> &data,
									 	const PlotSettings &settings);
	void clearCurve(int id);
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

public slots:
	void zoomIn();
	void zoomOut();

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
	void updateRubberBandRegion();
	void refreshPixmap();
	void drawGrid(QPainter *painter);
	void drawCurves(QPainter *painter);

	enum { Margin = 50 };

	QToolButton *zoomInButton;
	QToolButton *zoomOutButton;
	QMap<int, QVector<QPointF> > curveMap;
	QVector<PlotSettings> zoomStack;
	int curZoom;
	bool rubberBandIsShown;
	QRect rubberBandRect;
	QPixmap pixmap; //!< render area buffer
};

class PlotSettings
{
public:
	PlotSettings();
	PlotSettings(double minX, double MaxX, int numXTicks,
							 double minY, double MaxY, int numYTicks);
	PlotSettings(const PlotSettings &settings);
	~PlotSettings();

	PlotSettings &operator=(const PlotSettings &settings);
	double spanX() const;
	double spanY() const;
	void scroll(int dx, int dy);
	void adjust();

public:
	double minX;
	double maxX;
	int numXTicks;
	double minY;
	double maxY;
	int numYTicks;

private:
	static void adjustAxis(double &min, double &max, int &numTicks);
};

#endif

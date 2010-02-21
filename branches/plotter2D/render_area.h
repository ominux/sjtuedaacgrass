#ifndef RENDER_AREA_H
#define RENDER_AREA_H

#include <QPen>
#include <QWidget>

class RenderArea : public QWidget
{
	Q_OBJECT
public:
	RenderArea(QWidget *parent = 0);
	virtual ~RenderArea();

protected:
		void paintEvent(QPaintEvent* /*event*/);

private:
		QPen pen;
};
#endif // RENDER_AREA_H

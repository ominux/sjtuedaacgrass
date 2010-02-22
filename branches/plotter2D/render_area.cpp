#include "render_area.h"
#include <QtGui>

RenderArea::
RenderArea(QWidget *parent) : QWidget(parent)
{
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
}

RenderArea::
~RenderArea()
{ /* Qt will free all memory. */ }

// unit test for 2D painting
void RenderArea::
paintEvent(QPaintEvent* /* event */)
{
	QPainterPath path;
	path.moveTo(20, 80);
	path.lineTo(20, 30);
	path.cubicTo(80, 0, 50, 50, 80, 80);

	QPainter painter(this);
	painter.setPen(pen);
	painter.setRenderHint(QPainter::Antialiasing, true);

	for (int x = 0; x < width(); x += 100) {
		for (int y = 0; y < height(); y += 100) {
			// draw curve
			painter.save();
			painter.translate(x, y);
			painter.drawPath(path);
			painter.restore();
			// draw text
			painter.drawText(QPoint(x,y), tr("Qt 4\xB1\xB0"));
		}
	}
	painter.setRenderHint(QPainter::Antialiasing, false);
	painter.setPen(palette().dark().color());
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}


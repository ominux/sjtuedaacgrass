#include "plotter2D.h"
#include <QtGui>

Plotter2D::Plotter2D() : form(new QWidget)
{
	// setup top widget
	setupUi(form);
	// setup magnitude render area in tab 1
	verticalLayout[MAGNITUDE] = new QVBoxLayout(
		Ui::FormPlotter2D::groupBox_magnitude);
	renderArea[MAGNITUDE] = new RenderArea(
		Ui::FormPlotter2D::groupBox_magnitude);
	verticalLayout[MAGNITUDE]->addWidget(renderArea[MAGNITUDE]);
	// setup phase render area in tab 1
	verticalLayout[PHASE] = new QVBoxLayout(
		Ui::FormPlotter2D::groupBox_phase);
	renderArea[PHASE] = new RenderArea(
		Ui::FormPlotter2D::groupBox_phase);
	verticalLayout[PHASE]->addWidget(renderArea[PHASE]);
	// setup plotter in tab 2
	verticalLayout_tab_2 = new QVBoxLayout(
		Ui::FormPlotter2D::tab_2);
	plotter = new Plotter(Ui::FormPlotter2D::tab_2);
	verticalLayout_tab_2->addWidget(plotter);
}

Plotter2D::~Plotter2D()
{ /* Qt will free all memory. */ }

void Plotter2D::loadData(const QString &fileName)
{
	QVector<QPointF> data[6];
	double factX = 0.0013;
	double factY[6] = { 0.0008, 0.1, 0.2, 0.2, 0.1, 0.8 };
	double offsY[6] = { +500, -55, +309, +308, 0, 0 };
	int pos[6] = { 3, 6, 7, 8, 9, 10 };
	QFile file(fileName);
	double offsX = 0.0;

	if (file.open(QIODevice::ReadOnly)) {
		QTextStream in(&file);
		while (!in.atEnd()) {
			QString line = in.readLine();
			QStringList coords = line.split(' ',
																			QString::SkipEmptyParts);
			if (coords.count() >= 6) {
				double x = factX * coords[0].toDouble();
				if (data[0].isEmpty())
					offsX = x;
				for (int i = 0; i < 6; ++i) {
					double y = coords[pos[i]].toDouble();
					data[i].append(QPointF(x - offsX,
																 factY[i] * (y - offsY[i])));
				}
			}
		}
	}

	plotter->setCurveData(0, data[0]);
	plotter->setCurveData(1, data[1]);
	plotter->setCurveData(2, data[2]);
	plotter->setCurveData(3, data[3]);
	plotter->setCurveData(4, data[4]);
	plotter->setCurveData(5, data[5]);
}

void Plotter2D::show() const
{
	form->show();
}

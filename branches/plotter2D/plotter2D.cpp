#include "plotter2D.h"

#include "plotter.h"

#include <QtGui>

Plotter2D::
Plotter2D() : form(new QWidget)
{
	//! setup top widget
	setupUi(form);
	//! setup magnitude render area in tab 1
	verticalLayout[MAGNITUDE] = new QVBoxLayout(
		Ui::FormPlotter2D::groupBox_magnitude);
	plotter[MAGNITUDE] = new Plotter(
		Ui::FormPlotter2D::groupBox_magnitude);
	verticalLayout[MAGNITUDE]->addWidget(plotter[MAGNITUDE]);
	//! setup phase render area in tab 1
	verticalLayout[PHASE] = new QVBoxLayout(
		Ui::FormPlotter2D::groupBox_phase);
	plotter[PHASE] = new Plotter(
		Ui::FormPlotter2D::groupBox_phase);
	verticalLayout[PHASE]->addWidget(plotter[PHASE]);
	//! setup plotter in tab 2: unit testing for 2D painting
	verticalLayout_tab_2 = new QVBoxLayout(
		Ui::FormPlotter2D::tab_2);
	plotter_tab_2 = new Plotter(Ui::FormPlotter2D::tab_2);
	verticalLayout_tab_2->addWidget(plotter_tab_2);
}

Plotter2D::
~Plotter2D()
{ /* Qt will free all memory. */ }

void Plotter2D::
loadData(const QString &fileName)
{
	QVector<QPointF> data[Plotter::CURVES]; // buffer of curves on render area
	// Magic numbers associated to in1.txt start
	//double factX = 0.0013;
	//double factY[Plotter::CURVES] = { 0.0008, 0.1, 0.2, 0.2, 0.1, 0.8 };
	//double offsY[Plotter::CURVES] = { +500, -55, +309, +308, 0, 0 };
	//double offsX = 0.0;
	//int pos[Plotter::CURVES] = { 3, 6, 7, 8, 9, 10 };
	double factX = 1;
	double factY[Plotter::CURVES] = { 1, 1, 1, 1 };
	double offsY[Plotter::CURVES] = { -5, -5, -5, -5 };
	double offsX = 0.0;
	int pos[Plotter::CURVES] = { 1, 2, 3, 4 };
	// Magic numbers end

	QFile file(fileName);

	if (file.open(QIODevice::ReadOnly)) {
		QTextStream in(&file);
		while (!in.atEnd()) {
			QString line = in.readLine();
			QStringList coords = line.split(' ', QString::SkipEmptyParts);
			// ignore lines with less than 6 items
			if (coords.count() >= pos[Plotter::CURVES-1]) {
				double x = factX * coords[0].toDouble();
				if (data[0].isEmpty())
					offsX = x;
				for (int i = 0; i < Plotter::CURVES; ++i) {
					double y = coords[pos[i]].toDouble();
					data[i].append(QPointF(x - offsX, factY[i] * (y - offsY[i])));
				}
			}
		}
	}
	// settings with magic numbers associated to in1.txt
	PlotSettings settings(0.0, 13.0, 17,
												0.0, 17.0, 7);
	settings.adjust();
	for (int i = 0; i < Plotter::CURVES; ++i)
		plotter_tab_2->setCurveData(i, data[i], settings);
}

void Plotter2D::
loadData(const Analyser *)
{
}

void Plotter2D::
show() const
{
	form->show();
}

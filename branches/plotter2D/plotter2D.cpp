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
	plotter[MAGNITUDE]->setCurveData(fileName);
	plotter[PHASE]->setCurveData(fileName);
	plotter_tab_2->setCurveData(fileName);
}

void Plotter2D::
loadData(const Analyser *)
{}

void Plotter2D::
show() const
{
	form->show();
}

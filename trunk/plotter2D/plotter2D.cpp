#include "plotter2D.h"

#include "plotter.h"
#include "analyser.h"

#include <QtGui>

#include <iostream>

Plotter2D::
Plotter2D() : form(new QWidget), symbolicAnalyser(0)
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
{
	symbolicAnalyser = 0;
}

void Plotter2D::
loadData(const QString &fileName)
{
	// plotter[MAGNITUDE]->setCurveData(fileName);
	// plotter[PHASE]->setCurveData(fileName);
	plotter_tab_2->setCurveData(fileName);
}

void Plotter2D::
loadData(Analyser *analyser)
{
	symbolicAnalyser = analyser;
	symbolicAnalyser->evaluate_tf_coeff();
	// fixed symblic simulation settings for unit testing
	SymbolicSimSettings sssettings(1, 10e6, 100);
	// set and show current curves
	plotter[MAGNITUDE]->setSymbolicAnalyser(symbolicAnalyser,
																				 	&sssettings,
																					Plotter::MAGNITUDE);
	plotter[PHASE]->setSymbolicAnalyser(symbolicAnalyser,
																			&sssettings,
																			Plotter::PHASE);
}

void Plotter2D::
show() const
{
	form->show();
}

SymbolicSimSettings::
SymbolicSimSettings(double startf, double stopf, int samples) :
		startFreq(startf), stopFreq(stopf), numSamples(samples)
{
	if (startFreq <= 0.0 || stopFreq <= 0.0)
 	{
		std::cerr << "Frequency cannot be minus!"
			<< "startFreq = " << startFreq << "; "
			<< "stopFreq = " << stopFreq << std::endl;
		startFreq = 1;
		stopFreq = 1;
		numSamples = 0; // numSamples <= 0 results in nothing plotted
	}
 	else if (numSamples <= 0)
 	{
		std::cerr << "Number of samples must be a positive integer!"
			<< "numSamples = " << numSamples << std::endl;
	}
	else if (stopFreq < startFreq)
	{
		double f = stopFreq;
		stopFreq = startFreq;
		startFreq = f;
	}
}

SymbolicSimSettings::
~SymbolicSimSettings()
{}

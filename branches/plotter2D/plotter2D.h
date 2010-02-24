#ifndef PLOTTER2D_H
#define PLOTTER2D_H

#include "complex.h"
#include "ui_plotter2D.h"

class QString;
class QWidget;
class QVBoxLayout;
class Plotter;
class Analyser; //!< symbolic analyser

class Plotter2D : public Ui::FormPlotter2D
{
	enum {MAGNITUDE, PHASE, NUM_RENDERAREA};
public:
	Plotter2D(); //!< default constructor
	virtual ~Plotter2D(); //!< destructor
 	//!< unit testing: load curve data file
	void loadData(const QString &fileName);
	void loadData(const Analyser *analyser); //!< interface to symbolic ananlyer
	Complex tf() const;
	void show() const; //!< show all widgets
private:
	QWidget *form; //!< top widget
	QVBoxLayout *verticalLayout[NUM_RENDERAREA];
	Plotter *plotter[NUM_RENDERAREA]; //!< plot curves using double buffering
	QVBoxLayout *verticalLayout_tab_2;
	Plotter *plotter_tab_2; //!< for unit testing
};

#endif // PLOTTER2D_H

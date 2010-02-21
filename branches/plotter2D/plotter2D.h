#ifndef PLOTTER2D_H
#define PLOTTER2D_H

#include "render_area.h"
#include "ui_plotter2D.h"
#include "plotter.h"

class Plotter2D : public Ui::FormPlotter2D
{
	enum {MAGNITUDE, PHASE, NUM_RENDERAREA};
public:
	Plotter2D();
	virtual ~Plotter2D();
	void loadData(const QString &fileName); // load data file
	void show() const; // show all widgets
private:
	QWidget *form; //!< top widget
	RenderArea *renderArea[NUM_RENDERAREA];
	QVBoxLayout *verticalLayout[NUM_RENDERAREA];
	QVBoxLayout *verticalLayout_tab_2;
	Plotter *plotter;
};

#endif // PLOTTER2D_H

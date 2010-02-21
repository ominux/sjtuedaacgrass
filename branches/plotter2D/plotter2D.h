#ifndef PLOTTER2D_H
#define PLOTTER2D_H

#include "ui_plotter2D.h"

class Plotter2D : public Ui::FormPlotter2D
{
public:
	Plotter2D();
	virtual ~Plotter2D();
private:
	QWidget *form;
};

#endif // PLOTTER2D_H

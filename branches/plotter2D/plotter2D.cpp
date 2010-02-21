#include "plotter2D.h"
#include <cassert>

Plotter2D::Plotter2D() : form(new QWidget)
{
	setupUi(form);
	form->show();
}

Plotter2D::~Plotter2D()
{
	assert(form);
	delete form;
	form = 0;
}

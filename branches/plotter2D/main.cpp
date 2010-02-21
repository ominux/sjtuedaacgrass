#include "plotter2D.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	Plotter2D plotter2D;
	plotter2D.loadData("in1.txt");
	plotter2D.show();
	return app.exec();
}

#include "plotter2D.h"
#include "analyser.h"
#include "Analysis.h"

int main(int argc, char *argv[])
{
	Analysis analysis;
	Analyser analyser(&analysis);

	QApplication app(argc, argv);
	Plotter2D plotter2D;
	plotter2D.loadData("in1.txt");
	plotter2D.loadData(&analyser);
	plotter2D.show();
	return app.exec();
}

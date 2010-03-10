#ifndef _PLOTTER2D_H_
#define _PLOTTER2D_H_

#include "complex.h"
#include "ui_plotter2D.h"

class QString;
class QWidget;
class QVBoxLayout;
class Plotter;
class Analyser; //!< symbolic analyser
struct SymbolicSimSettings; //!< settings for symbolic analyser sampling

class Plotter2D : public Ui::FormPlotter2D
{
	enum {MAGNITUDE, PHASE, NUM_RENDERAREA};
public:
	Plotter2D(); //!< default constructor
	virtual ~Plotter2D(); //!< destructor
 	//!< unit testing: load curve data file
	void loadData(const QString &fileName);
	void loadData(Analyser *analyser); //!< interface to symbolic ananlyer
	void show() const; //!< show all widgets
private:
	QWidget *form; //!< top widget
	QVBoxLayout *verticalLayout[NUM_RENDERAREA];
	Plotter *plotter[NUM_RENDERAREA]; //!< plot curves using double buffering
	QVBoxLayout *verticalLayout_tab_2;
	Plotter *plotter_tab_2; //!< for unit testing
	Analyser *symbolicAnalyser; //!< symbolic analyser of GRASS
};

//!< simple settings for symbolic analyser
class SymbolicSimSettings
{
public:
	SymbolicSimSettings(double startf, double stopf, int samples);
	~SymbolicSimSettings();

	double startFreq; //!< Hz
	double stopFreq; //!< Hz
	int numSamples; //!< number of samples (log10)
};

#endif // _PLOTTER2D_H_

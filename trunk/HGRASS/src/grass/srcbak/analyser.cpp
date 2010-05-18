#include"analyser.h"
#include"Analysis.h"
#include<map>
#include<utility>
#include<string>
#include<cassert>
using std::map;
using std::string;
using std::cout;
using std::endl;

const double Analyser::PI_ = 3.1415926535897932384626433832795028841971;

Analyser::Analyser(Analysis *ptr_analysis,
									 map<string, double *> &name_pvalue_map,
									 map<string, Mosfet *> &name_pmosfet_map) :
	ptr_analysis_(ptr_analysis), name_ptrvalue_map_(name_pvalue_map),
	name_ptrmosfet_map_(name_pmosfet_map)
{
	assert(ptr_analysis_);
}

Analyser::~Analyser()
{}


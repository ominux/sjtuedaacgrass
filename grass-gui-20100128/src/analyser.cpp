#include"analyser.h"

const double Analyser::PI_ = 3.1415926535897932384626433832795028841971;

Analyser::Analyser(Analysis *ptr_analysis) :
	ptr_analysis_(ptr_analysis)
{
	assert(ptr_analysis_);
  ptr_analysis_->generate_parname2ptrvalue_map(parname2ptrvalue_map_);
}

Analyser::~Analyser()
{
  ptr_analysis_ = 0;
}


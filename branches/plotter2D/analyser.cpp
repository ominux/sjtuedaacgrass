#include "analyser.h"
#include "Analysis.h"

#include <cassert>

const double Analyser::PI_ = 3.1415926535897932384626433832795028841971;

Analyser::
Analyser(Analysis *ptr_analysis) :
	ptr_analysis_(ptr_analysis)
{
	assert(ptr_analysis_);
  ptr_analysis_->generate_parname2ptrvalue_map(parname2ptrvalue_map_);
}

Analyser::
~Analyser()
{
  ptr_analysis_ = 0;
}

void Analyser::
evaluate_tf_coeff()
{ ptr_analysis_->EvaluateSCoeff(); }

Complex Analyser::
tf(const double freq) const
{ return ptr_analysis_->EvaluateSCoeffFreq(freq); }

double *Analyser::
get_value_pointer(const std::string &par_name) const
{
	std::map<std::string, double *>::const_iterator it =
	 	parname2ptrvalue_map_.find(par_name);
	if (it != parname2ptrvalue_map_.end())
		return it->second;
	else
		std::cerr << "No such parameter: " << par_name << std::endl;
	return 0;
}

void Analyser::
get_name_list(std::vector<std::string> &par_names) const
{
	for (std::map<std::string, double *>::const_iterator
			 it = parname2ptrvalue_map_.begin();
			 it != parname2ptrvalue_map_.end();
			 ++it)
		par_names.push_back(it->first);
}


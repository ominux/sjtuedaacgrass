#ifndef _ANALYSER_H_
#define _ANALYSER_H_
#include"Analysis.h"
#include<map>
#include<vector>
#include<string>
#include<iostream>
#include<cassert>

/// \brief This is a wrapper class for a symbolic analysis.

//! It's the only interface between GUI(both 2D and 3D) and the GRASS engine.
class Analyser
{
public:
	Analyser(Analysis *ptr_analysis);
	~Analyser();
  /// \brief If any parameter changes, this function MUST be invoked to
  /// reevaluate the coefficients of the s-expanded H(s) first.
	void
  evaluate_nf_coeff();
  /// \brief This is a wrapper to evaluate the transfer function (TF).
	/// Unit: Hz
	complex_double
  transfer_function(const double freq) const;
  /// Find out the data pointer to a parameter specified by name.
	double *
  get_value_pointer(const std::string &par_name) const;
  /// Return the list of all parameter names.
	void
  get_name_list(std::vector<std::string> &par_names) const;
private:
  /// a pointer to the symbolic analysis
	Analysis *ptr_analysis_;
  /// a map from each parameter name to the pointer to its value
	std::map<std::string, double *> parname2ptrvalue_map_;
  /// Mathematical constant \f$\pi\f$.
	static const double PI_;
};

inline void
Analyser::evaluate_nf_coeff()
{ ptr_analysis_->EvaluateSCoeff(); }

inline complex_double
Analyser::transfer_function(const double freq) const
{ return ptr_analysis_->EvaluateSCoeffFreq(freq); }

inline double *
Analyser::get_value_pointer(const std::string &par_name) const
{
	std::map<std::string, double *>::const_iterator it =
	 	parname2ptrvalue_map_.find(par_name);
	if (it != parname2ptrvalue_map_.end())
		return it->second;
	else
		std::cerr << "No such parameter: " << par_name << std::endl;
	return 0;
}

inline void
Analyser::get_name_list(
	std::vector<std::string> &par_names) const
{
	for (std::map<std::string, double *>::const_iterator
			 it = parname2ptrvalue_map_.begin();
			 it != parname2ptrvalue_map_.end();
			 ++it)
		par_names.push_back(it->first);
}

#endif // _ANALYSER_H_


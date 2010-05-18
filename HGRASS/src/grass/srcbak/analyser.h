#ifndef _ANALYSER_H_
#define _ANALYSER_H_
#include"Analysis.h"
#include<map>
#include<vector>
#include<string>
#include<complex>
#include<iostream>
#include<cassert>

typedef std::complex<double> complex_double;

class Analyser
{
public:
	Analyser(Analysis *ptr_analysis,
					 std::map<std::string, double *> &name_pvalue_map,
					 std::map<std::string, Mosfet *> &name_pmosfet_map);
	~Analyser();
	void evaluate_nf_coeff();
	complex_double transfer_function(const double ) const;
	double *get_value_pointer(const std::string &) const;
	Mosfet *get_mosfet_pointer(const std::string &) const;
	int get_mosfet_flag(const std::string &) const;
	void get_name_list(std::vector<std::string> &) const;
private:
	Analysis *ptr_analysis_;
	std::map<std::string, double *> name_ptrvalue_map_;
	std::map<std::string, Mosfet *> name_ptrmosfet_map_;
	static const double PI_;
};

// If any parameter changes, this function must be used to reevaluate
// the new s-expanding coeffients.
inline void Analyser::evaluate_nf_coeff()
{ ptr_analysis_->EvaluateSCoeff(); }

// f: Hz
inline complex_double Analyser::transfer_function(const double f) const
{ return ptr_analysis_->stdEvaluateSCoeffFreq(f); }

inline double *Analyser::get_value_pointer(const std::string &par_name) const
{
	std::map<std::string, double *>::const_iterator it =
	 	name_ptrvalue_map_.find(par_name);
	if (it != name_ptrvalue_map_.end())
		return it->second;
	else
		std::cerr << "Cannot find parameter: " << par_name << std::endl;
	return 0;
}

inline Mosfet *Analyser::get_mosfet_pointer(const std::string &par_name) const
{
	std::map<std::string, Mosfet *>::const_iterator it =
                  name_ptrmosfet_map_.find(par_name);
        if (it != name_ptrmosfet_map_.end())
                  return it->second;
        else
                  std::cerr << "Cannot find parameter: " << par_name << std::endl;
        return 0;
}

inline int Analyser::get_mosfet_flag(const std::string &par_name) const
{
	char key = par_name[0];
	if(key == '(')
	  return 0;
	else if(key == 'W')
	  return 1;
	else if(key == 'L')
	  return 2;
	else
	  return -1;
	return -1;
}

inline void Analyser::get_name_list(
	std::vector<std::string> &par_name_list) const
{
	for (std::map<std::string, double *>::const_iterator
			 it = name_ptrvalue_map_.begin();
			 it != name_ptrvalue_map_.end();
			 ++it)
		par_name_list.push_back(it->first);
}

#endif // _ANALYSER_H_


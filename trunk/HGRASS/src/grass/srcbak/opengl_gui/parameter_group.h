#include "analyser.h"
#include <string>
#include <vector>
#include <cassert>
#include <cstdio>
class ParameterGroup
{
public:
	ParameterGroup(
		const std::string &gname,
		const std::string &prname,
		const std::vector<std::string> &pgname = std::vector<std::string>(),
		double coeff = 0.0)
		:
			group_name_(gname),
			param_ref_name_(prname),
			param_ref_(0),
			param_ref_pmosfet_(0),
			param_ref_mosfet_flag_(-1),
			param_group_name_(pgname),
			param_group_pmosfet_(std::vector<Mosfet*>()),
			param_group_mosfet_flag_(std::vector<int>()),
			coeff_(coeff) {}

	// Copy constructor
	ParameterGroup(const ParameterGroup &pg):
		group_name_(pg.group_name_),
		param_ref_name_(pg.param_ref_name_),
		param_ref_(pg.param_ref_),
		param_ref_pmosfet_(pg.param_ref_pmosfet_),
		param_ref_mosfet_flag_(pg.param_ref_mosfet_flag_),
		param_group_name_(pg.param_group_name_),
		param_group_(pg.param_group_),
		param_group_pmosfet_(pg.param_group_pmosfet_),
		param_group_mosfet_flag_(pg.param_group_mosfet_flag_),
		coeff_(pg.coeff_),
	 	num_samples_(pg.num_samples_),
		param_ref_min_(pg.param_ref_min_),
		param_ref_max_(pg.param_ref_max_) {}

	~ParameterGroup()
 	{
		std::printf("Destruct %s @ %p.\n", group_name_.c_str(), this);
	}

	// Assign a value to the parameter group
	void set_param(double d)
	{
		*param_ref_ = d;
		if(param_ref_pmosfet_)
		  param_ref_pmosfet_->updateValue(d,param_ref_mosfet_flag_);
		int i=0;
		for (std::vector<double*>::iterator it = param_group_.begin();
				 it != param_group_.end();
				 ++it)
		{
			**it = coeff_ * d;
			if(param_group_pmosfet_[i])
				param_group_pmosfet_[i]->updateValue(coeff_ * d,
								     param_group_mosfet_flag_[i]);
			i++;
		}
	}
	// Set parameter pointers by looking up the component symbol table
	void set_ptr(const Analyser *ptr_analyser)
	{
		assert(ptr_analyser);
		param_ref_ = ptr_analyser->get_value_pointer(param_ref_name_);
		param_ref_pmosfet_ = ptr_analyser->get_mosfet_pointer(param_ref_name_);
		param_ref_mosfet_flag_ = ptr_analyser->get_mosfet_flag(param_ref_name_);
		for (std::vector<std::string>::iterator it = param_group_name_.begin();
				 it != param_group_name_.end();
				 ++it)
		{
			param_group_.push_back(ptr_analyser->get_value_pointer(*it));
			param_group_pmosfet_.push_back(ptr_analyser->get_mosfet_pointer(*it));
			param_group_mosfet_flag_.push_back(ptr_analyser->get_mosfet_flag(*it));
		}
	}
	// Access functions
	const std::string &group_name() const {return group_name_;}
	void group_name(const std::string &str) {group_name_ = str;}

	int num_samples() const {return num_samples_;}
	void num_samples(int ns) {num_samples_ = ns;}

	double min() const {return param_ref_min_;}
	void min(double param_min) {param_ref_min_ = param_min;}

	double max() const {return param_ref_max_;}
	void max(double param_max) {param_ref_max_ = param_max;}
private:
	std::string group_name_;

	std::string param_ref_name_;
	double *param_ref_;
	Mosfet *param_ref_pmosfet_;
	int param_ref_mosfet_flag_;

	std::vector<std::string> param_group_name_;
	std::vector<double*> param_group_;
	std::vector<Mosfet*> param_group_pmosfet_;
	std::vector<int> param_group_mosfet_flag_;

	double coeff_;
	int num_samples_;
	double param_ref_min_;
	double param_ref_max_;
};


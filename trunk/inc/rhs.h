#ifndef _RHS_H
#define _RHS_H

#include "node.h"
#include "branch.h"
#include "element.h"

class RHSElement;
class CompElemList;
class ACAnalysisManager;
class MNAMatrix;
class CktCall;
class Circuit;
class DDDmanager;

class RHSVector{
	int num_elem;
protected:
	RHSElement* elem_list;
	MNAMatrix* matrix;
	Circuit* ckt;
	DDDmanager* ddd_mgr;
  friend class ACAnalysisManager;
public:
	RHSVector(Circuit* _ckt, MNAMatrix *_matrix, DDDmanager* _ddd_mgr);
	~RHSVector();

	 RHSElement * new_rhs_elem(Node *node);
	 void print_rhs();
	 void calc_value();
	 void process_subckt_call(CktCall *_call);
	 CompElemList* build_comp_expr(int);
	 RHSElement* query_element(Node*);
	 RHSElement* create_element(Node*);
	 void add_element(RHSElement*);
	 RHSElement* get_rhs_element(int);
};

class RHSElement : Element{
	Node* pivot;
	BranchList* blist;
	CompElemList* clist;
	complex_t value;
	RHSElement* next;
  friend class RHSVector;
  friend class ACAnalysisManager;
public:
	RHSElement(Node* _pivot, RHSElement* _next);
	~RHSElement();

    void     process_branch(Branch *branch);
    void     add_branch( Branch *branch, int sign)
	 { blist->add_branch(branch, sign);}
    RHSElement * Next() const { return next; }
    void Next(RHSElement *_next) { next = _next; }
	 complex_t elem_value(double);
	 complex_t get_value(){return value;}
	 BranchList* get_blist(){return blist;}
	 boolean check_valid();
	 void add_clist(CompElemList*);
	 CompElemList* get_clist(){return clist;}
	 Node* get_pivot(){return pivot;}
};

#endif

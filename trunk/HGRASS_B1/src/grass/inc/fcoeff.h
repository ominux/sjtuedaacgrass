#ifndef _FCOEFF_H
#define _FCOEFF_H

/*
 *    $RCSfile: fcoeff.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

#include "unixstd.h"
#include "globals.h"
#include "dddbase.h"
#include <iostream>

/* we deal with fully expanded ( with respect to each circuit element )
   coefficient in following classes definition.
*/

class FCoeffList;
class Cofactor;

class FLabelMap {
    int offset;
    int modulus;

    public:
    FLabelMap( int _offset, int _mod);
    ~FLabelMap() {;};

    int ddd2fcoeff(int _olabel, int _index);
//        { return (_olabel*modulus + offset + _index); }
    int fcoeff2ddd(int _mlabel)
        { return (_mlabel - offset)/modulus; }
    int get_index(int _mlabel)
        { return (_mlabel - offset)%modulus; }
};

class FCoeff {
    int    order;     // order of the laplace variable s, i,e. s^order
    //complex_t    value; // value of the real coefficient
    double    dval; // the value with without sign coming from power of s
    int    num_paths; // number of product terms(paths)
    DDDnode *fcoeff_ddd;
    //boolean    keep; //keep the coefficient?
    DDDnode *prev_path; // path found in previous simplifying step
    double  prev_pval; // the corresponding path value

    friend class FCoeffList;
public:
    boolean    keep; //keep the coefficient?
    complex_t    value; // value of the real coefficient
    FCoeff();
    ~FCoeff();
    void    set_ddd_mgr(DDDmanager *);
    void      set_order(int _order){ order = _order;}
    int    get_order() const { return order;}
    DDDnode *get_ddd() {return fcoeff_ddd;}
    void    set_ddd( DDDnode *_ddd ){ fcoeff_ddd = _ddd; }
    complex_t    fcoeff_value() const { return value;}
    double    get_dvalue() const { return dval; }
    void     calc_fcoeff(DDDmanager *, HasherTree *);
    void    statistic(std::ostream& out);
    boolean    keep_val(){ return keep;}
    DDDnode * & Prev_path() { return prev_path;}
    double & Prev_pval() { return prev_pval;}
    };

class FCoeffList {

    //FCoeff    *fcoeff;
    int      max_order; // number of coefficients with different order
    int      offset;    // order offset; since the order is from -n to n
    DDDmanager *ddd_mgr; // the ddd manager used.
    int      supp_init; // flag for coefficient suppressions
    int      fcoeff_calced; // if the fcoefficient has been calculated?
    double      cratio; // fcoeff[pindex]/fcoeff[pindex+1];
    
    public:
    FCoeff    *fcoeff;
    FCoeffList( DDDmanager *_ddd_mgr, int _num);
    ~FCoeffList();

    FCoeffList *fco_product(FCoeffList *_list, int _index, 
                int _bindex, ElemPart _part);
    FCoeffList *fco_union(FCoeffList *_list1, FCoeffList *_list2);
    FCoeffList *fco_copy();
    FCoeffList *list_ddd_product(FCoeffList *_list, 
        DDDnode *_node, PolyType ptype);
    FCoeffList *first_fcoefflist(DDDnode *_node, PolyType ptype);

    void     calc_fcoeff_list(HasherTree *ht);
    complex_t evaluate( double); 
    void    statistic( std::ostream& out);
    void     keep_fcoeff_ddd();
    void     unkeep_fcoeff_ddd();
    int     get_max_order() { return max_order;}
    DDDnode *get_fcoeff_ddd( int ind ) 
        { return fcoeff[ind].fcoeff_ddd; }
    void  set_fcoeff_ddd( DDDnode *_fcddd, int _ind ) 
        { fcoeff[_ind].fcoeff_ddd = _fcddd;}
    FCoeff *get_fcoeff( int ind ) { return &fcoeff[ind]; }


    void fcoeff_suppress(double _freq, double mag_ref);
    void fcoeff_suppress(int);
    void fcoeff_supp_init();
    void reset_fcoeff_calced() { fcoeff_calced = 0;}

    void fcoeff_elem_elim(int* index, int* bindex);
    void fcoeff_decancel(int, int, int, int);
    DDDnode * fcoeff_dyn_decancel(DDDnode *, DDDnode *, DDDnode *);
    DDDnode * fcoeff_decancel_index(DDDnode *, int, int, int, int);

    // functions for printing out symbolic terms
    void print_fcoeff_list(std::ostream& );
    void print_fcoeffs(DDDnode *, std::ostream& );
    void print_symbolic_path(DDDnode *, std::ostream& );

    // for pole and zero extraction
    void    calc_cratio( int pindex ) { 
        cratio =  fcoeff[pindex].get_dvalue() /
					fcoeff[pindex+1].get_dvalue();
				std::cout<<"exact pole/zero: "<<cratio<<std::endl;}
    double    get_cratio( int pindex ) { return cratio;}
};

extern FLabelMap *theFLabMap;

#endif //_FCOEFF_H


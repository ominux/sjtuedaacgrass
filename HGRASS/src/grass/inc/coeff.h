#ifndef _COEFF_H
#define _COEFF_H

/*
 *    $RCSfile: coeff.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

#include "unixstd.h"
#include "globals.h"
#include "dddbase.h"

/* we only deal with partial expanded coefficient (with respect to
   Resistor, Capacitor, Inductor) in the following classes definition.
*/

class CoeffList;
class Cofactor;

class LabelMap {
    int offset;

    public:
    LabelMap( int _offset);
    ~LabelMap() {;};

    int ddd2coeff(int _olabel, ElemPart _part);
    int coeff2ddd(int _mlabel);
    ElemPart which_part(int _mlabel);
};

class Coeff {
    int    order; // order of the laplace variable s, i,e. s^order
    complex_t    value; // value of the real coefficient
    int        num_paths; // number of product terms(paths)
    DDDnode *    coeff_ddd;

    friend class CoeffList;

public:
    Coeff();
    ~Coeff();
    void    set_ddd_mgr(DDDmanager *ddd_mgr);
    void      set_order(int _order){ order = _order;}
    int    	   get_order() const { return order;} 
	complex_t    coeff_value() const { return value;} 
	void     calc_coeff(DDDmanager *, HasherTree *);
    void     statistic(std::ostream& out);
};

class CoeffList {

    Coeff *coeff;
    int  max_order; // number of coefficients with different order
    int  offset;    // order offset; since the order is from -n to n
    DDDmanager *ddd_mgr; // the ddd manager used

    public:
    CoeffList( DDDmanager *, int );
    ~CoeffList();

    CoeffList *co_product(CoeffList *, int , ElemPart );
    CoeffList *co_union(CoeffList *, CoeffList *);
    CoeffList *co_copy();
    CoeffList *list_ddd_product(CoeffList *, DDDnode *);
    CoeffList *first_coefflist(DDDnode *);

    void     calc_coeff_list(HasherTree *);
    complex_t evaluate( double); 
    void    statistic( std::ostream& out);
    void     keep_coeff_ddd();

	// for noise analysis
	void maple_cmd(std::ostream& out);
};

extern LabelMap *theLabMap;

#endif //_COEFF_H

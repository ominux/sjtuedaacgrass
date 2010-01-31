/*
 *    ckt_call.h
 *    class def related to sub-circuit calling
*/

/*
 *    $RCSfile: ckt_call.h,v $
 *    $Revision: 1.2 $
 *    $Date: 1999/07/18 17:36:28 $
 */


#ifndef _CKT_CALL_H
#define _CKT_CALL_H

#include "globals.h"
#include "branch.h"
#include "node.h"
#include "element.h"

class NList;
class DVarList;
class SubCircuit;
class Cofactor;

class CktCall {

    char        *call_name; // the name of the instance  
    char        *ckt_name; // the name of subckt called

  protected:
    NList        *interface0; // the calling node interface
    DVarList    *var_list;  // the parameter list
    SubCircuit    *sub_ckt;    // the corresponding sub-circuit
    CktCall        *next;        

  friend class SubCircuit;
  friend class Circuit;
  friend class MNAMatrix;
  friend class RHSVector;

  public:
    CktCall( char *_call_name);
    ~CktCall();
    void add_inter_node( int node );
    void add_var_value(char *_name, double _value );
    void match_sub_ckt(char *_ckt_name, SubCircuit *_sub_ckt_list);

    CompElemList *build_expression( int _row, int _col);
    CompElemList *build_rhs_expression( int _row);
    MNAElement *find_element( int _row, int _col);
    RHSElement *find_rhs_element( int _row);

    int p2c(int _node);// node real number mapping from parent ckt to child ckt
    int c2p(int _node);// node real number mapping from child ckt to parent ckt
    void expand_subckt();
    CktCall    *Next() { return next;}
};

class CompElem {

    Element    *A_left;
    Element    *A_right;
    Cofactor    *delta;
    CompElem    *next;

    friend class CompElemList;
    public:
    CompElem( Element *_left, Element *_right, Cofactor *_cof );
    ~CompElem();
    CompElem  *Next() { return next;}
    complex_t   evaluate(double, HasherTree *, bool); 
	 void		statDDD(DDDmanager*, HasherTree *);
};

class CompElemList {

    CompElem    *list;
    int            num_elem;
    SubCircuit    *subckt;
	 int				visited;
    CompElemList *next;

    public:
    CompElemList( SubCircuit *_ckt );
    ~CompElemList();

    void    add_elem( CompElem *_elem );
    CompElem * get_list() { return list;}
    int     get_num() { return num_elem;}
    complex_t    evaluate( double freq); 
    CompElemList *Next() { return next;}
    void *Next(CompElemList *_next) { return next = _next;}
	 void statDDD();
};

#endif //_CKT_CALL_H

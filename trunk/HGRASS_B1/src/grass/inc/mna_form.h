#ifndef MNA_FORM_H
#define MNA_FORM_H

/*
 *    $RCSfile: mna_form.h,v $
 *    $Revision: 1.2 $
 *    $Date: 1999/07/18 17:35:31 $
 */

#include "node.h"
#include "branch.h"
#include "rhs.h"
#include "element.h"

#ifndef bool
#define bool boolean
#endif

class HasherTree;

class DDDnode;
class CktCall;

/* MNA data structures: 
      An MNA matrix consists of a list of MNA rows (MNARow's).
      
      Each row has a list of MNAElements (->elem_list). 
      Each row also has a pointer to the pivot element for 
      that row (->pivot).  The MNA columns are maintained 
      by keeping pointers to variable nodes in each MNA 
      element (->node).


      e.g.          v1   v2    v3

                1   x,y  -x     z   
                
                2   a,-x  -a,x      

                3   -a    a     b

  mna{1}->row { v1, (x,y)} ->next { v2, (-x)} ->next { v3, (z)} ->next{NULL} 
    \
    next{2}->row { v1, (a,-x)} ->next { v2, (-a,x)} ->next{NULL}
      \
      next{3}->row { v1, (-a)} ->next { v2, (a)} ->next{ v3, (b)} ->next{NULL}
        \
        NULL


*/

class MNAMatrix;
class IndexMatrix;
class DDDmanager;
class CompElemList;

/* 
** the Modified Nodal Formulation for circuit equations setup 
*/

class MNAElement : Element
{
    int     flag;    // used to indicate the state of the element
    int     deleted;
    int     num_deled;// deleted in numerator
    int     den_deled; //deleted in denominator
    Node    *pivot;  // the corresponding pivot(row) node 
    Node    *node;    // the corresponding node variable for the element
    BranchList  *blist; // branch related to the element
    CompElemList *clist; /* the expressions from subckt 
                		  if it is boundary element. */
    MNAElement  *next;
    double      freq;    // frequency now used
    complex_t     value;    // the numerical value of this node for given freq

    friend class MNARow;
    friend class MNAMatrix;
    friend class RHSVector;

  public:
    MNAElement(Node * _node, Node *_pivot);
    ~MNAElement();

    int        get_row() { return pivot->get_index();}
    int        get_col() { return node->get_index();}
    BranchList    *get_blist(){ return blist;}
    CompElemList  *get_clist() { return clist;}
    int     &     get_flag() { return this->flag; }
    Node  * &     get_pivot() { return this->pivot; }
    Node  * &     get_node() { return this->node; }
    boolean        check_valid();
    complex_t elem_value( double _freq); // overriding 
    complex_t elem_cur_value() const {return value;} 
    rcl_complex get_cvalue() const { return this->blist->get_cvalue();}
    rcl_complex get_cvalue(PolyType ptype) const 
        {    this->blist->calculate_rcl_value(ptype); 
            return this->blist->get_cvalue();}
    void     add_clist( CompElemList *_clist);

    void     add_branch( Branch *branch, int sign) 
            { blist->add_branch(branch, sign);}
    void     add_branch( Branch *branch) 
            { blist->add_branch(branch, 1);}
    void     add_branch( Branch *_branch, double _value, int _sign) 
            { blist->add_branch(_branch, _value, _sign);}

    // for decancellation
    int  find_findex(char *name, int cind);

    // for whole element elimination
    void calc_cvalue() { blist->calculate_rcl_value();}
    void update_cvalue(char *name, PolyType ptype) 
        { blist->update_cvalue(name, ptype);}
    void restore_cvalue(char *name,PolyType ptype) { blist->restore_cvalue(name, ptype);}
    void update_cvalue1(char *name, PolyType ptype)
        { blist->update_cvalue1(name, ptype);}
    void restore_cvalue1(char *name,PolyType ptype) { blist->restore_cvalue1(name, ptype);}
    int  delete_branch(char *name, PolyType ptype) 
        {return blist->delete_branch(name,ptype);}
    int num_branch( PolyType ptype) { return blist->num_branch( ptype ); }
	void update(){deleted = 1;}
	void restore(){deleted = 0;}
	int get_deleted(){return deleted;}

    MNAElement * Next() const { return next; }
    void Next(MNAElement *_next) { next = _next; }
};

// Implemented in buildmna.cc
class MNARow {	
    int isSpCurEq; /* boolean, indicates MNA row is a
            special current output variable
            equation */
    int  size;    // num of elements in the row
    Node *pivot;    // the pivot node for this row
    MNAElement *elem_list;    // the element list in this row    
    MNAMatrix  *matrix; // the MNA matrix the row belongs to
    MNARow       *next;        

  friend class MNAMatrix; 
  friend class RHSVector; 

  public:
    MNARow(Node *_node, MNAMatrix *_matrix, MNARow *_next);
    ~MNARow();

    MNARow * Next() const { return next;}
    void Next(MNARow *_next) { next = _next;}

    int    get_size() { return size;}
    Node *    get_pivot() { return pivot;}
	MNAElement * get_elem_list(){ return elem_list; }

    MNAElement * create_element( Node *node );
    MNAElement * add_create_elem( Node *node );
    void     add_element( MNAElement *elem );
    MNAElement * query_element( Node *node );

    int     check_element();

    void     calc_elems_value();
    void     calc_elems_value(PolyType ptype);

    void     process_branch(Branch *branch);
    void     process_rcl(Branch *branch);
    void     process_cvs(Branch *branch);
    void     process_vccs(Branch *branch);
    void     process_cccs(Branch *branch);
    void     process_vi(Branch *branch);
    void     process_switch(Branch *branch);
    //void     process_mosfet(Branch *branch);
    //void     process_jfet(Branch *branch);
    //void     process_bjt(Branch *branch);
    //void     process_diode(Branch *branch);

    void     print_info();
};

// the mapping from node index to MNA entry
typedef struct _melen {
    MNAElement     *mna_elem;
    unsigned     int row;
    unsigned     int col;
} MELEM;

class Label2MNA {
    MELEM    *mlist;
    int        size;
    int        init_label; // initial label value for the circuit

    public:
    Label2MNA( int _size);
    ~Label2MNA();

    void set_elem(int _index, MNAElement *_mna_elem) 
        { //ASSERT(_index - init_label>0, "invalid index.");
        mlist[_index - init_label].mna_elem = _mna_elem;}
    MNAElement * get_elem(int _index) 
        { //ASSERT(_index - init_label>0, "invalid index.");
        return mlist[_index - init_label].mna_elem;}
    
    void set_row( int _index , int _row )
        { //ASSERT(_index - init_label>0, "invalid index.");
        mlist[_index - init_label].row = _row;}
    int get_row(int _index) 
        { //ASSERT(_index - init_label>0, "invalid index.");
        return mlist[_index - init_label].row;}

    void set_col( int _index , int _col )
        { //ASSERT(_index - init_label>0, "invalid index.");
        mlist[_index - init_label].col = _col;}
    int get_col(int _index) 
        { //ASSERT(_index - init_label>0, "invalid index.");
        return mlist[_index - init_label].col;}
};

class MNAMatrix {

    int    num_elem;        // number of elements in MNA matrix
    int     size;            // the dimension of the MNA matrix

  protected:

    Circuit    *ckt;        // the circuit for the MNA
    DDDnode    *sys_det;    // the system determinant
    DDDnode    *y_det;    // the y determinant for the reduced ckt without switches
    MNARow    *rowlist;    // MNA row list
    MNARow    *cur_row;    // the row under processing
    IndexMatrix *index_mat;    // index matrix used for matrix expansion
    Label2MNA *label2elem;    // the mapping from index to MNA elements
    DDDmanager *ddd_mgr;    // DDD manager
    int    init_label;    // initial label for the circuit

  friend class MNARow; 
  friend class Circuit;
  friend class ACAnalysisManager;

  public:

    MNAMatrix(Circuit *_ckt,  DDDmanager *_ddd);
    ~MNAMatrix();
    MNARow * new_mna_row(Node *node);
    int     get_label(int _row, int _col);

    // matrix dimension reduction
    void     delete_element( int label );

    Label2MNA *get_label2elem() { return label2elem;}
    MNARow      *find_row( int _node); // in local real number    
    void       process_subckt_call( CktCall *_call );

    // in local real num ber
    CompElemList  *build_comp_expr(int _row, int _col); 

    // in local real number
    MNAElement *get_mna_element(int _row, int _col); 
    
    int    get_num_elem() { return num_elem;}
    int    get_matrix_size() { return size;}
    IndexMatrix *get_imatrix() { return index_mat;}

    int     max_branch_num();
    void     calc_value();
    void     calc_value(PolyType ptype);
    void     print_mna();
    void     print_mna(PolyType ptype);
    void     print_bool(); 
    void     create_index();
    void     create_mapping();
    DDDnode *build_sys_ddd();
    DDDnode *get_sys_ddd(){return sys_det;}
    DDDnode *build_y_ddd();
    complex_t evaluate_sys_det( double freq, HasherTree *, bool);
    int    real2label( int real );
};

#endif /* ifndef MNA_FORM_H */ 

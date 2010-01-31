#ifndef BRANCH_H
#define BRANCH_H

/*
 *    $RCSfile: branch.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */


/*
 * branch.h:
 *
 *     'branch' contains the row and column data structs
 *      and routines for manipulating them.
 *
 * DEPENDENCY: node.h must be included prior to branch.h 
 */
 
#include "node.h"
#include "rclcompx.h"

/* branch states */
typedef enum {
    B_USED,     /* used branch */
    B_SYMB,     /* symbolic branch which including variable */
    B_DELETED,  /* deleted branch */
    B_EUQAL     /* the incident nodes which should be merged */
} BranchStat;
 
#define PLUS     1
#define MINUS     -1

/* DEFINE's and TYPEDEF's */

class BranchList;
class Circuit;

class Branch {

  protected:

     BranchStat stat ;         // branch state
     int wasProcessed;      /* indicating if this branch
                  has been processed previously */
     DeviceType type ;          /* Type of element, such as r,c,l  */
     int part;            /* partition index */
     int num ;
     char *name;        // branch name
     char *var_name;        // branch variable name if it is symoblic branch
     Node *node1 ;
     Node *node2 ;
     union {
        Node *cnode1 ;
        Node *node3 ;
     } extra1 ;
     union {
        Node *cnode2;     /* For controlled sources           */
        Node *node4;      /* For active elements              */
     } extra2 ;

    /* extra nodes for current or voltage variables: */
     Node *cvar ; // for current variable
     Node *evar ; // for voltage variable
     Branch *cbranch; // control branch for CCCS and CCVS
     char   *model_name; // the model name for nonlinear device

     double value ;      /*numeric value of element 0 if purely symolic*/
     rcl_complex cvalue; /* the rlc complex_t value */
     Branch *next ;

     friend class BranchList;
     friend class DummBranch;
     friend class Circuit;

  public:
    // two nodes branch
    Branch(char *_keyword, DeviceType _type, 
            Node *n1, Node *n2, double _value, int _index);
    Branch(char *_keyword, DeviceType _type, 
            Node *n1, Node *n2, char *_var_name, int _index);
    // four nodes branch
    Branch(char *_keyword, DeviceType _type, 
        Node *n1, Node *n2, Node *n3, Node *n4, 
        double _value, int _index );
    Branch(char *keyword, DeviceType _type, 
        Node *n1, Node *n2, Node *n3, Node *n4, 
        char *_var_name, int _index );

    ~Branch();
    Node *get_node1() { return node1;}
    Node *get_node2() { return node2;}
    Node *get_node3() { return extra1.node3;}
    Node *get_node4() { return extra2.node4;}
    BranchStat & get_stat() { return this->stat;}
    char * get_name() { return this->name;}
    DeviceType get_type() { return this->type;}
    Branch * get_cbranch() { return this->cbranch;}
    char * get_model_name() { return model_name; }

    int & was_processed() { return this->wasProcessed;} 

    void check_branch();
    void lump_branch(char *_keyword, double _value);

    void calc_rcl_value();
    rcl_complex get_cvalue() { return this->cvalue;}
	 void set_value(double);
    double get_dvalue() { return this->value;}

    Branch * Next() { return next;}
    void Next(Branch *_next) { next = _next;}

    void add_cur_variable(Node *var) { cvar = var;}
    void add_vol_variable(Node *var) { evar = var;}
    Node *get_cur_var() { return cvar;} 
    Node *get_vol_var() { return evar;} 

    void add_ctrl_branch(Branch *branch) { cbranch = branch;}
    void add_model_name(char *model); 

};

/* 
** Since there exist several branch lists in the course of
** circuit analysis, we need a dummy branch list
*/

class DummBranch {

  protected:
    int        num_deleted; // flag for numerator;
    int        den_deleted; // flag for denominator;
    int        num_deleted1; // flag for numerator;
    int        den_deleted1; // flag for denominator;

    Branch         *branch;
    BType        type; // Res, Cap or Ind type branch
    DummBranch     *next;
    double           sign;    // sign in cirucit equation 
    int        IsSpecial;// if special branch(value is 1 or -1)
    double         value;     // can only be 1 or -1.
    double        bvalue;    // the branch value    

  friend class BranchList;

  public:
    DummBranch(Branch *_branch, int _sign ) 
        { this->branch = _branch; this->next = NULL; 
        IsSpecial = 0; this->sign = (double)_sign; 
        calc_bvalue(); num_deleted = den_deleted = num_deleted1 = den_deleted1 = 0;}

    DummBranch(Branch *_branch)
        { this->branch = _branch; this->next = NULL; 
        IsSpecial = 0; this->sign = 1.0; 
        calc_bvalue();num_deleted = den_deleted = num_deleted1 = den_deleted1 = 0;} 

    DummBranch(Branch *_branch, double _value, int _sign)
        { this->branch = _branch; IsSpecial = 1; 
        this->value = _value; this->sign = (double)_sign; 
        this->next = NULL; calc_bvalue();
        num_deleted = den_deleted = num_deleted1 = den_deleted1 = 0;} 

    DummBranch( DummBranch *_dbranch){
        branch = _dbranch->branch;
        next = NULL;
        sign = _dbranch->sign;
        IsSpecial = _dbranch->IsSpecial;
        value = _dbranch->value;
        calc_bvalue();
        num_deleted = den_deleted = num_deleted1 = den_deleted1 = 0;}

    ~DummBranch(){ if(next) delete next;}

    // for full expansion
    BType get_type() { return this->type; }
    double get_bvalue() { return this->bvalue; }
    char * get_bname() { return this->branch->get_name(); }
    void calc_bvalue();
    int get_num_deleted() { return num_deleted;}
    int get_den_deleted() { return den_deleted;}
    int get_num_deleted1() { return num_deleted1;}
    int get_den_deleted1() { return den_deleted1;}


    Branch * get_branch() { return branch;}

    int is_special() { return (int)(this->IsSpecial);}

    int get_sign() { return (int)sign;}

    DummBranch    * Next() const { return next;}
    void Next(DummBranch *_next) { next = _next;}
};

class BranchList {

  protected:

    DummBranch    *blist;
    int        num;
    int        Nnum; // for numerator;
    int        Dnum; // for denominator;
    int        Nnum1; // for numerator;
    int        Dnum1; // for denominator;

    rcl_complex     cvalue;

  public:

    BranchList() { blist = NULL; num = 0; cvalue = 0.0; Nnum = Dnum = Nnum1 = Dnum1 = 0;}
    ~BranchList(){ delete blist;}

    DummBranch *get_blist() const {return blist;}
    int        get_num() const {return num;}

    void add_branch(Branch *branch, int sign);
    void add_branch(Branch *branch, double value, int sign);
    void add_branch(Branch *branch) { this->add_branch(branch, PLUS); }
    void add_dbranch(DummBranch *dbranch);

    void merge_blist(BranchList *_blist);
    BranchList * copy_blist( BranchList *_blist);

    rcl_complex get_cvalue() { return cvalue;}

    Branch * query_branch(const char *name);

    void calculate_rcl_value();
    void calculate_rcl_value(PolyType ptype);

    int chk_exist(const Branch *branch);

    void print_branch( std::ostream & out);
    void print_branch( std::ostream & out, PolyType ptype);

    // for full expansion
    DummBranch * get_dummbranch( int bindex );
    int get_branch_index( char *name );
    double    get_dbranch_val(int bindex) 
            { return get_dummbranch( bindex )->get_bvalue();}
    BType   get_dbranch_type( int bindex )
            { return get_dummbranch( bindex )->get_type();}
    char  * get_dbranch_name( int bindex )
            { return get_dummbranch( bindex )->get_bname();}
    
    // for whole element elimination
    void update_cvalue(char *name, PolyType ptype);
    void restore_cvalue(char *name, PolyType ptype);
    void update_cvalue1(char *name, PolyType ptype);
    void restore_cvalue1(char *name, PolyType ptype);
    int  delete_branch(char *name, PolyType ptype);
    int  num_branch(PolyType ptype);
    int  num_branch1(PolyType ptype);
};

#endif //BRANCH_H

#ifndef DDD_BASE_H
#define DDD_BASE_H

/*
 *    $RCSfile: dddbase.h,v $
 *    $Revision: 1.2 $
 *    $Date: 1999/10/18 20:37:20 $
 */

#include "rclcompx.h"
#include "comparable.h"
#include "hasher.h"
#include "hashcach.h"
#include "tnode.h"
#include "floatobj.h"

#ifndef boolean
#define bool boolean
#endif

#include "hashcach.h"
#include "floatobj.h"
#include "mna_form.h"

#include <iostream>
#include <sstream>
#include <stdlib.h>


#define UNION       1 
#define INTSECT     2 
#define SUBSET1     3
#define SUBSET0     5
#define CHANGE      7
#define DIFF        11 
#define PRODUCT     12 
#define MINOR       13 
#define DELETION    14 

#define    	DDD_PRIME  	203311  
// Other pime numbers:
// 104729, 203311, 218833  
#define 	MAXINDEX    4095    
//#define MAXINDEX       8190

class DDDnode;
class DDDmanager;
class DDDcomputed1;
class DDDcomputed2;
class DDDcomputed3;
class FCoeffList;
class CoeffList;
//**
extern integer ZRand;

class DDDnode : public Tnode
{
  integer index;
  bool dc;     // if dc is true, all variables are don't care

  bool sign:1;        // node sign in denominator 
  bool visit_flag:1;        // used for DDD traversal
  complex_t *val_ptr; // the determinant value of the node and its subtree
  // The memory is not allocated a priori. Remember to allocate memory for
  // it before first-time using it.
  CoeffList *coefflist;    // the coefficient list that the DDD represents
  FCoeffList *fcoefflist;// the fully expanded coefficient list.

  /* used in approximation */
  bool color:1;    // 0:white and 1:black. The color of the DDD in BFS or DFS.
  DDDnode *pi;     // the predecessor of the DDDnode(in BFS)

public:

  DDDnode(integer v, DDDnode *a, DDDnode *b, boolean dcare)
           {/*index=v; left=a; right=b;*/ dc=dcare; val_ptr=NULL;
           sign = 1; visit_flag=0; color = 0; pi = NULL; coefflist = NULL;
           fcoefflist = NULL;}
  DDDnode( DDDnode* a ) 
       { /*index=a->index; left=a->left; right=a->right;*/ dc=a->dc;
         val_ptr=a->val_ptr; sign = a->sign; visit_flag=a->visit_flag;
	 color=a->color; pi=a->pi; coefflist =a->coefflist;
	 fcoefflist=a->fcoefflist;}

  ~DDDnode() { if(val_ptr) delete val_ptr;}

  // Implements the virtual hash functions
  /* Using (index), (left), and (right) to locate a node in the
   * hash tree. 
   */
  unsigned Hash( unsigned h = 0 ) { 
    /*
    return ( (index) ^ ((int)Then() << 5 ) ^ ((int)Else() << 11) 
    	^ ZRand ^ (h << 1) )  % DDD_PRIME; */
   	ulong  x = (ulong) Then();
   	ulong  y = (ulong) Else();
    return ( index + (x ) + (y ) )% DDD_PRIME;
   	//return ( rand() % DDD_PRIME );
  }
  
  integer isa() { return DDDNODE;}
  virtual integer Compare(Comparable *);
  inline integer Index() {return index;}
  inline complex_t GetValue() {return *val_ptr;}
  inline complex_t *GetValueP() {return val_ptr;}
  inline void SetValue(complex_t value)  { *val_ptr = value; }
  inline void SetValueP(complex_t *val_ptr0) { this->val_ptr = val_ptr0; }
  inline bool GetVisitFlag() { return visit_flag; }
  inline void SetVisitFlag(bool visited)  { visit_flag = visited; }
  boolean isDC()  {return dc;}
  inline DDDnode* Then() { return (DDDnode *) left;}
  inline DDDnode* Else() { return (DDDnode *) right;}

  friend class DDDmanager;
  
  void Print( std::ostream& out = std::cout );
};


class DDDmanager : public Object {

protected:

  DDDnode *one;           // terminal 1
  DDDnode *zero;          // terminal 0
  integer num_inputs;      // total number of variables (levels)      
  integer num_nodes;       // total number of allocated nodes
  integer num_terms;       // total number of 1-paths

  Label2MNA    *label2elem;
  
  HasherTree** unique;         //Keeps each node unique
  HashCache *computed;         //Cache for computed nodes

  float CountR( DDDnode* P, HasherTree* ht);
//  void NodesInFunctionR(DDDnode *f, HasherTree *ht);
   
  void doGC(integer);
  void RGC(DDDnode*);

public:

  DDDmanager();
  ~DDDmanager();

  void set_label2elem(Label2MNA *_ind){ label2elem = _ind; }
  Label2MNA * get_label2elem(){ return label2elem; }

  integer NumInputs()   { return num_inputs; }
  integer NumLiterals() { return num_nodes; }
  integer NumTerms()    { return num_terms; }
  integer NumNodes()    { return num_nodes; }   

  DDDnode* Empty()     { return zero; }
  DDDnode* Base()      { return one;  }

//    Generates a node with index v, and subgraphs g and h
  DDDnode* GetNode( integer v, DDDnode* g, DDDnode* h );
  DDDnode* InputNode( integer v ){ return GetNode( v, one, zero );}

  DDDnode* Subset1( DDDnode* P, integer var ); //Returns the 1 subset of P at var
  DDDnode* Subset0( DDDnode* P, integer var ); //Returns the 0 subset of P at var
  DDDnode* Change(  DDDnode* P, integer var );  //Returns P when var is inverted
//
  DDDnode* ExistentialAbstraction( DDDnode* P, integer var ) {
    DDDnode* temp = Union(Subset0(P,var),Subset1(P,var));
    return ( Union(temp,Change(temp,var)));
  }
//
  DDDnode* Union( DDDnode *P, DDDnode *Q );   //Returns P or Q
  DDDnode* Product( DDDnode *P, DDDnode *Q );   //Returns P * Q
  DDDnode* Intsect( DDDnode* P, DDDnode* Q ); //Returns P and Q
  DDDnode* Diff( DDDnode* P, DDDnode* Q );    //Returns P - Q
  float     Count( DDDnode* P );                //Returns # of elements
  float     PathsInFunction(DDDnode* P) { return Count( P ); }
  integer   NodesInFunction(DDDnode* P);
  void NodesInFunctionR(DDDnode *f, HasherTree *ht);

  DDDnode* One()  { return one; }
  DDDnode* Zero() { return zero; }

  DDDnode* And( DDDnode* P, DDDnode* Q ) { return Intsect( P, Q ); }
  DDDnode* Or ( DDDnode* P, DDDnode* Q ) { return Union( P, Q ); }

  DDDnode* Universe(integer);

  void Keep  ( DDDnode *P) { P->Reference(); }
  void UnKeep( DDDnode *P) { P->UnReference(); }
  void ForceGC();
  void GC();
  
  void PrintComputed() { computed->Print(); }
  //Prints Sets in marks.
  void PrintSets( DDDnode* P, std::ostream& out = std::cout );   
  void PrintSetsR( DDDnode* P, char *set, std::ostream& out = std::cout );
  // The next two methods were added by GShi, 04-06-04.
  // Prints Sets in indexes. PrintSets2() calls PrintIndexR() recursively.
  void PrintSets2( DDDnode* P, std::ostream& out = std::cout );   
  void PrintIndexR( DDDnode* P, int *head, int *tail, std::ostream& out = std::cout );
  
  void PrintNodes( DDDnode* P, std::ostream& out = std::cout );  //Prints DDD structure P
  void TermPrint( std::ostream& out = std::cout );         //Prints terminal nodes
  void PrintDDDTree( DDDnode *); // Print DDD tree in a bottom up fasion
  void PrintDDDTreeR( DDDnode *, HasherTree * ); 

  void PrintStats( std::ostream& out = std::cout );        //unique/computed stats... 
  void statistic( DDDnode * P); // prints the statistis of DDDnode P

  // functions for minor hashing April, 1997
  DDDnode * GetMinor(DDDnode* row, DDDnode* col);
  void PutMinor(DDDnode* minor, DDDnode* row, DDDnode* col);

  // function related to determinant calculation
  int CalcSign(DDDnode * , DDDnode * ); 
  void PreCalculationR(DDDnode * );
  complex_t GetDetermValueR(DDDnode * ,double, HasherTree *, bool); 
  complex_t GetDetermValueNR(DDDnode * ,double, bool); 
  complex_t GetValueFromDDD(DDDnode * P) {return *(P->val_ptr);}
  void ResetVisitFlag(DDDnode*);

  /* the functions related to approximation */
  void DFS_ISS(DDDnode *);
  void DFS_Restore(DDDnode *);
  void Relax(DDDnode *, DDDnode*, double );
  DDDnode * DDDShortestPath(DDDnode*,double );
  DDDnode * ExtractPath();
  complex_t PathValue(DDDnode*, double );

  void FCRelax(DDDnode *, DDDnode*);
  DDDnode * FCDDDShortestPath(DDDnode*);
  double FCPathValue(DDDnode*);

  /* function related to coefficient generation */
  CoeffList *generate_all_coeffs(DDDnode *, int);
  CoeffList *generate_all_coeffsR(DDDnode *, int, HasherTree *);
  double GetCoeffValueR(DDDnode *, HasherTree *); 
  void PreCoeffCalculationR(DDDnode * P);
  int CoeffCalcSign(DDDnode * Parent, DDDnode * Child); 
  double CalcCoeffCubeValue(char *set, double error);

  /* function related to fully expanded coefficient generation */
  FCoeffList *generate_all_fcoeffs(DDDnode *, int, PolyType );
  FCoeffList *generate_all_fcoeffsR(DDDnode *, int, HasherTree *, PolyType );
  double GetFCoeffValueR(DDDnode *, HasherTree *); 
  void PreFCoeffCalculationR(DDDnode * P);
  int FCoeffCalcSign(DDDnode * Parent, DDDnode * Child); 
  double CalcFCoeffCubeValue(char *set, double error);

};


class DDDcomputed1 : public Comparable {

  DDDnode *f, *g;
  integer type;

public:
  
  DDDcomputed1(integer i, DDDnode *x, DDDnode *y)
    { f    = MaxNode( x,y ); 
      g    = MinNode( x,y ); 
      type = i;}

  DDDcomputed1( DDDcomputed1 *p )
    { f    = p->f;
      g    = p->g;
      type = p->type;
    }
  ~DDDcomputed1() {;}

// utility functions
  DDDnode* MaxNode( DDDnode* P, DDDnode* Q ) {   //Returns the larger of two
    return ((ulong)P > (ulong)Q)? P:Q;
  }

  DDDnode* MinNode( DDDnode* P, DDDnode* Q ) {  //Returns the smaller of two
    return ((ulong)P < (ulong)Q)? P:Q;
  }

  unsigned Hash( unsigned n=0 ){
    /*return( ((unsigned) type) ^ ((unsigned) f << 5)
       ^ ((unsigned) g << 11) ^ ( n << 1 )) % DDD_PRIME; */
       return ((ulong) type + (ulong) f + (ulong) g)%DDD_PRIME;
  }

  integer isa() { return DDDCOMPUTED1;}
  integer Compare(Comparable *);    
};


class DDDcomputed2 : public Comparable {

  DDDnode *f;
  integer type, var;

public:
  
  DDDcomputed2(integer i, DDDnode *x, integer j)
    { f    = x; 
      type = i; 
      var  = j; }
  DDDcomputed2( DDDcomputed2 *p )
    { f    = p->f;
      
      type = p->type;
    }
  ~DDDcomputed2() {;}

  unsigned Hash( unsigned n=0 ){
    /*return( ((unsigned) type )
       ^ ((unsigned) f   << 7)
       ^ ((unsigned) var << 11)
       ^ ( n << 8 )) % DDD_PRIME;*/
      return ((ulong) type + (ulong) f + (ulong) var)%DDD_PRIME;
  }

  integer isa() { return DDDCOMPUTED2;}
  integer Compare(Comparable *);
};

class DDDcomputed3 : public Comparable {

  DDDnode *f, *g;
  integer type;

public:
  
  DDDcomputed3(integer i, DDDnode *x, DDDnode *y)
    { f    = x; 
      g    = y;
      type = i;}

  DDDcomputed3( DDDcomputed3 *p )
    { f    = p->f;
      g    = p->g;
      type = p->type;
    }
  ~DDDcomputed3() {;}

// utility functions

  unsigned Hash( unsigned n=0 ){
    /*return( ((unsigned) type) 
       ^ ((unsigned) f   << 5)
       ^ ((unsigned) g   << 11)
       ^ ( n << 1 )) % DDD_PRIME;*/
       return  ( (ulong) type + (ulong) f + (ulong) g) % DDD_PRIME;
  }

  integer isa() { return DDDCOMPUTED3;}
  integer Compare(Comparable *);    
};

#endif //DDD_BASE_H

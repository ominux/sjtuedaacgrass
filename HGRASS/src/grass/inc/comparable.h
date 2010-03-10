/*
 *    $RCSfile: comparable.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

#ifndef COMPARABLE_H
#define COMPARABLE_H

#include "object.h"

class Comparable : public Object {

public:

  virtual ~Comparable() {};
  virtual boolean isComparable() { return TRUE;} 
  virtual integer isa()          { return COMPARABLE;} 
     
  // Return random yet deterministic int based on the object and parameter i
  virtual unsigned Hash(unsigned i=0)     { return i; }

  // If this object is greater than x return a positive value
  // else if equal return zero
  // else return a negative value
  virtual integer Compare(Comparable *) { return 0; }
  integer Compare(Comparable &a) { return Compare(&a); }

  boolean operator< ( Comparable &x) { return Compare(&x)< 0;}
  boolean operator<=( Comparable &x) { return Compare(&x)<=0;}
  boolean operator==( Comparable &x) { return Compare(&x)==0;}
  boolean operator!=( Comparable &x) { return Compare(&x)!=0;}
  boolean operator> ( Comparable &x) { return Compare(&x)> 0;}
  boolean operator>=( Comparable &x) { return Compare(&x)>=0;}
};

#endif

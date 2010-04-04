#ifndef OBJECT_H
#define OBJECT_H

/*
 *    $RCSfile: object.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

////////////////////////////////////////////////////////////////////////////
//
//  object.h : Object class 
//
////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdlib.h>

#include "stddefs.h"
#include "classdef.h"

//////////////////////////////////////////////////////////////

//
// Object is a virtual base class
//

class Object
{   
public:
  	Object() {}
  	virtual ~Object() {}

  	// Print is, by convention, expected to print debugging info.
  	// For pretty printing redefine the << operator.
  	virtual void Print(std::ostream &out = std::cout)
  	{ out << "ERROR: write a Print funct!\n";}

  	virtual void Scan(std::istream &in = std::cin)
  	{ 	if (in.eof()) /* just uses in */ \
		std::cout << "ERROR: write a Read funct!\n";}
     
  	virtual boolean isComparable() { return FALSE;} 
  	virtual boolean isSticky()     { return FALSE;} 
  	virtual integer isa()          { return OBJECT;}
  	virtual boolean Flush()     	{ return TRUE; }
  	// This is more general than a constructor since constructors
  	// are not virtual 
  	virtual Object *Copy()
  	{ std::cerr << "Object has no Copy() member\n"; return(NULL); }

};

class DoubleObject : public Object {

    double num;

    public:
    DoubleObject( double val ) { num = val; }
    ~DoubleObject() {;}
    double get_value(void) { return num; }
};

inline std::istream&
operator>>(std::istream &in,Object *o)
{
  o->Scan(in);
  return(in);
}

inline std::istream&
operator>>(std::istream &in,Object &o)
{
  o.Scan(in);
  return(in);
}

inline std::ostream&
operator<<(std::ostream &out, Object &o)
{
  o.Print(out);
  return out;
}

inline std::ostream&
operator<<(std::ostream &out, Object *o)
{
  o->Print (out);
  return out;
}

#endif // OBJECT_H


#ifndef TNODE_H
#define TNODE_H

/*
 *    $RCSfile: tnode.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

#include <iostream>
#include "comparable.h"
#include "classdef.h"

#define MARK_BIT (unsigned long) 0x80000000
#define POP_BIT (unsigned long) 0x40000000
#define REF_BIT (unsigned long) 0xc0000000

class Tnode : public Comparable
{
protected:
	Tnode        		*left;
	Tnode           	*right;
	unsigned long    reference;
	//  short            	index;

	void    SetLeft(Tnode *l)  { left  = l;}
	void    SetRight(Tnode *r) { right = r;}

public:
	Tnode(): left(0), right(0), reference(0) {}

	~Tnode() {}

	Tnode*  Left()          { return left;}
	Tnode*  Right()         { return right;}
	Tnode*  One()           { return left;}
	Tnode*  Zero()          { return right;}
	Tnode*  True()          { return left;}
	Tnode*  False()         { return right;}

	virtual void    Reference()     { reference++;          }
	virtual void    UnReference()   { reference--;          }
	virtual unsigned NumReferences() { return reference & ~REF_BIT; }

	void    Mark()          { reference |= MARK_BIT;    }
	void    UnMark()        { reference &= ~MARK_BIT;    }
	boolean Marked()        { return reference & MARK_BIT;}

	void    PushMark()		  { reference &= ~POP_BIT;}
	void    PopMark()		      { reference |= POP_BIT;}
	boolean    PopMarked()		      { return reference & POP_BIT;}
	void    ZeroReference()     {reference &= REF_BIT;}

	virtual void Print(std::ostream& out = std::cout) { out << "Tnode\n"; } 
};

inline std::ostream& operator << (std::ostream &out, Tnode& f)
{ f.Print(out); return out; }

inline std::ostream& operator << (std::ostream &out, Tnode* f)
{ out << "(ptr->)"; f->Print(out); return out; }

#endif // TNODE_H


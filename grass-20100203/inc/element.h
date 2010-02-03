#ifndef _ELEMENT
#define _ELEMENT

class Element{
public:
	Element() {}	// null implementation
	virtual ~Element() {}
	virtual complex_t elem_value(double) = 0;
};

#endif

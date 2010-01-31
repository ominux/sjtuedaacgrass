/*	cfdd_global.h
 *	Defines globals for the CFDD package.
 *	05-16-04, G.Shi
 */

#ifndef	CFDD_GLOBAL_H
#define	CFDD_GLOBAL_H

#include	<limits.h>
#include	"globals.h"

/*#define 	PI 			3.14159265358979323846			
#define		DBL_MAX		1e20

// Edge Types.
enum EdgeType	{Y=0, Z, CC, VC, CS, VS};

// A structure for edge
struct Edge
{
	char	*name;	// The edge name.
	int		node1;
	int		node2;		// The two nodes this edge connects.
	int		type;		// The type of this edge.
	char 	*pname;		// The pairing edge name.
	int		pnum;		// The edge number of the pairing edge.
	double	value;		// value (R/L/C/Gain) read from netlist
	int		lumped;		// 1 = lumped, 0 = non-lumped
	int		index;		// Index of edges after Y/Z lumping
	int		edgeindex;
};

// A structure for symbol
struct SymbNode
{
	char		*name;		// Symbol name
	double		value;		// Symbol value
	double		imag;		// Imaginary part for a composite symbol
	Edge		*edge;		// Points to the EList in Enumerator
	SymbNode	*next;		// The next symbol in array
	SymbNode	*nextP;		// The next symbol in parallel
};*/

#endif


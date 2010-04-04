/**************************************************************************************
*        Symbolic Circuit Analysis With TPDDs (Tree Pair Decision Diagram)
* tpdd_sturct.h
* Struct Defintions for TPDD.
*
* CHEN Weiwei		GShi	06/03/31
* School of Microelectronics, Shanghai Jiaotong Univ.
* {chenweiwei, shiguoyong}@ic.sjtu.edu.cn
**************************************************************************************/

#ifndef	TPDDSTRUCT_H
#define TPDDSTRUCT_H

#include "tpdd_global.h"

/** 
	A structure for storing a node.
*/
typedef struct Node_g
{
	/// Node_g number.
		int	 	num;		
	// To form a linked list.
		Node_g	*next;		
}Node_g;

/** 
	A structure for storing an edge.
*/
typedef struct Edge_g
{
	/// Name of the edge
	char	*name;		
	
	/** 
		The edge is derived from this node.
		Node number is stored in the node list.
	*/
	Node_g	*node1;

	/** 
		The edge is ended to this node.
		Node number is stored in the node list.
	*/
	Node_g	*node2;		

	/// the type of this edge (Y/Z/VC/VS/CC/CS).
	int		type;

	/// Name of the pairing edge
	char	*pname;	
	
	/// Value of the element
	double	value;		

	/// Variable name for the value (not used now)
	char	*var_name;

	/// Make a linked list
	Edge_g	*next;		
}Edge_g;

/**
 A structure for edge
*/
struct Edge
{
	/// The edge name.
	char	*name;
	
	/** 
		The number of node1.
		The edge is derived from this node.
	*/
	int		node1;

	/** 
		The number of node2.
		The edge is ended to this node.
	*/
	int		node2;		// The two nodes this edge connects.

	/// The type of this edge.	
	int		type;		

	/// The pairing edge name.
	char 	*pname;	
	
	/// The edge number of the pairing edge.
	int		pnum;	
	
	/// value (R/L/C/Gain) read from netlist.
	double	value;		

	/// the lumped flag. 1 = lumped, 0 = non-lumped.
	int		lumped;		

	/// Index of edges after Y/Z lumping.
	int		index;		

	/// Index of edges.
	int		edgeindex;
};

/**
 A structure for symbol
 */
typedef struct SymbNode
{
	/// the index of the non-lumped symbol. 
	int			ExpSymbIndex;

	/** 
		the flag to identify whether the symbol is a source or not.
		1-is a source, 0-is not a source.
	*/
	int			isSrc;

	/// the Symbol name.
	char		*name;		

	/// the Symbol value.
	double		value;	
	
	/// Imaginary part for a composite symbol.
	double		imag;		

	/// Points to the EList in Enumerator.
	Edge		*edge;	
	
	/// the pointer to the pairedge if the symbol is a dependent source.
	Edge		*pairedge;

	/// The next symbol in array.
	SymbNode	*next;		

	// The next symbol in parallel.
	SymbNode	*nextP;		

	double appearTimes;
}SymbNode;


/**
 The sturcture to record the reduced graphs
*/
 typedef struct EdgeInfo  
{
	/** 
		The number of node1.
		The edge is derived from this node.
	*/
	 short node1		: 16;

	/** 
		The number of node2.
		The edge is ended to this node.
	*/
	 short node2		:16;

	/** 
		the selected mark.
		1-the edge is selected in the reduced graph. 
		0-the edge is removed in the reduced graph.
	*/
	 short mark			:1;

	/// the index of the edge in the edge list.
	 int edgeindex	:30;

	/** 
		the flag to identify whether the symbol is a source or not.
		1-is a source, 0-is not a source.
	*/
	 short isSrc		:1;
}EdgeInfo;

/**
	The structure used for symbol sorting.
 */
typedef struct stag
{
	/// the index of the symbol.
	int id;

	/// the random index assigned to the symbol.
	int rand;
}stag;

/**
	the structure for comparing the edges.
	used in subtree::completeCompareTree().
*/
typedef struct edgeComp
{
	/// the index of the edge in the edge list.
	int edgeNo;

	/** 
		the direction of the edge.
		1   --  positive. the edge is derived from node1 and ended to node2.
		-1  --  negative. the edge is dervied from node2 and ended to node1.
	*/
	int direct;

	/// the pointer to the next edge to compare.
	edgeComp *pNext;
}edgeComp;

/**
	The sructure for comparing the vertices.
	used in subtree::completeCompareTree().
*/

typedef struct vertexComp
{
	/// the index of the vertex.
	int vertexNo;

	/// the number of the edges connected to this node.
	int edgeSum;

	/// the pointer to the next edge to compare.
	edgeComp *pedge;
}vertexComp;



#endif


/* ../CFDD2/Enumerator.h
 * The Enumerator class works as the enumeration engine. 
 * Its key method is Minty(), which reads a graph file, and enumerates
 * the product terms in sequence.
 * GShi, 04/06/04
 */

#ifndef		ENUMERATOR_H
#define		ENUMERATOR_H


#include	"ToGraph.h"
#include	"SDD_Aid.h"
#include	"..\inc\dddbase.h"		// For accessing DDD.
#include	"cfdd_global.h"
#include	<iostream>

class 	Enumerator{

private:
	int		E, N;		// Num of edges and nodes
	int		term_cnt;	// Count of terms
	int		symb_cnt;	
	// Total num of symbols (count multiplicitty) in all terms.
	Edge	*EList;		// A list of edges
	int		*ZList;		// A list of Z edge numbers
	int		Nsymb;		// Num of symbols
	SymbNode	*SymbList;	  // A list of symbol nodes.
	SymbNode	**symbarray;  // An array of SymbNode pointers.

	DDDmanager	*ddd;	
	SDD_Aid		*sdd_aid;	// For SDD manipulation
	DDDnode		*sdd_root;	// root of the SDD
	DDDnode *ddd_root, *ddd_curr;

	// Stacks:
	int
		**NStackL, 	// Size [E][N], stores nodeL[N] of kth graph-copy.
		**NStackR, 	// Size [E][N], stores nodeR[N] of kth graph-copy.
		**EStack;  	// Size [E][E], stores edge marks of kth graph-copy.
		// Note the number of graph-copies in stack will not exceed the
		// number of edges.
	int	*E0Stack;	// Size [E].	
	// The number of the first unprocessed edge in the graph-copy.
	DDDnode **DDStack;	// Size [E]. Each points to a sub-SDD.
	int	stk_cnt;	

public:
	Enumerator();
	Enumerator(int n_edge, int n_node, Edge_g *edge_list); 
	~Enumerator();

	// E = num of edges, N = num of nodes.
	int	 ReadGraph(char *graph_file);
	int	 CreateEdgeList(Edge_g *edge_list);
	void InitSDD_Aid();		// Initilaize SDD_Aid.
	void AddSymbol(Edge *p_edge);
	void SetSymbValue(double freq);
	int	 Minty();			// Returns the total number of terms.

	inline int	 GetNumEdge() 		{ return E; }
	inline int	 GetNumNode() 		{ return N; }
	inline int	 GetNsymb() 		{ return Nsymb; }
	inline int   GetTotalSymb()		{ return symb_cnt; }
	inline DDDnode *GetSDD()		{ return sdd_root; }

	inline char	*GetSymbName(int index) \
				{ return  symbarray[index-1]->name; }
	inline double	GetValue(int index)  \
				{ return  symbarray[index-1]->value; }
	inline double	GetImaginary(int index)  \
				{ return  symbarray[index-1]->imag; }
	// The indexes of "X", "+" and "-" symbols.
	inline int		XIndex()		{ return Nsymb; }		// "X" (unknown)
	inline int		PIndex()		{ return Nsymb-1; }		// "+" (plus)
	inline int		MIndex()		{ return Nsymb-2; }		// "-" (minus)
	
	void PrintZList();
	void MakeSymbList();
	void PrintSymbList();

	void ExtractSets()	{  sdd_aid->PrintSDDSets(sdd_root, cout); }
	void CheckSDDInfo()	{  sdd_aid->PrintSDDInfo(sdd_root); }
	void PrintSDD()		{  sdd_aid->PrintSDD(sdd_root); }
	void GarbageCollectSDD()	{ sdd_aid->GarbageCollect(sdd_root); }
	void PrintGraph(std::ostream &out);

private:
	void SortEdge();
	void SetPairing();
	// Check whether a loop is formed by coloring the edge (n1, n2).
	int	 CheckLoop(int *node, int n1, int n2, int &cmp_cnt);
	void PushStacks(int *edge, int *nodeL, int *nodeR,
					int e0, DDDnode *DDcurr);
	void PopStacks(int *edge, int *nodeL, int *nodeR,
				int &ccntL, int &ccntR, int &e0, DDDnode *&DDcurr);
	void PrintTree(int *treeL, int *treeR);
	void PrintTerm(int *treeL, int *treeR, int *edge);
	int  GetSign(int *treeL, int *treeR, int *edge);
	int  CountEdge(int *edge);
	int  CheckTree(int *edge, int *nodeL, int *nodeR, 
				int *treeL, int *treeR);
};
#endif


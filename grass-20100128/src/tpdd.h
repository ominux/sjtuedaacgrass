/**************************************************************************************
*        Symbolic Circuit Analysis With TPDDs (Tree Pair Decision Diagram)
* tpdd.h
* Class Definition for tpdd
* tpdd is the class for Tree Pair Decision Diagram.
*
* CHEN Weiwei		GShi	06/03/31
* School of Microelectronics, Shanghai Jiaotong Univ.
* {chenweiwei, shiguoyong}@ic.sjtu.edu.cn
**************************************************************************************/


#ifndef TPDD_H
#define TPDD_H

#include "ToGraph.h"
#include "tpdd_struct.h"
#include "tpddnode.h"
#include "tnode.h"
#include "comparable.h"
#include "hasher.h"

#include<map>
#include <vector>
#include <string>

/**
	The class of tree pair decision diagram (TPDD).
	It is proper to rename this class as graph reduction decision diagram (GRDD).
*/

class tpdd : public Object
{
private:
	//the order number of sensitivity element designated

		int sensitivity_tag;
	/// the number of the nodes in the graph of the circuit.
		int N;							

	/// the number of the edges in the graph of the circuit.
		int E;							

	/// the number of the VS edges in the graph of the circuit.
		int VsN;						

	/// the number of the VC edges in the graph of the circuit.
		int VcN;					
	
	/// the number of the CS edges in the graph of the circuit.
		int CsN;						

	/// the number of the CC edges in the graph of the circuit.
		int CcN;						

	/// the number of the dependent sources in the graph of the circuit.
		int DpenSrcN;					

	/// the number of the lumped symbols in the graph of the circuit.
		int SymbN;						
	
	/// the total number of the symbols in the graph of the circuit.
		int TotalSymbN;

	/// the number of the terms in the symbolic expression of the transfer funcion of the circuit. 
		double TermN;

	/// the list of the edges in the graph of the circuit.
		Edge *Elist;			
	
	/// the list of the lumped symbols in the graph of the circuit.
		SymbNode **Slist;				
	
	/// the list of all the symbols in the graph of the circuit.
		SymbNode **SNoLumplist;


		SymbNode **SExpList;
	
		SymbNode *SymbListTemp;


	/// the root node of the tree pair decision diagram.
		tpddnode * pTpddRoot;			

	/// the 1-terminal node of the tree pair decision diagram.
		tpddnode * pOne;

	/// the 0-terminal node of the tree pair decision diagram.
		tpddnode * pZero;
		

		int *PathArray;
		int StackTop;
		int TermSign;

	/// the number of the tpdd nodes after Reduce.
		int TotalRNodeNum;					
	
	/// the list for TPDD reduce.	
		tpddnode ** ReduceList;

	/// the list to collect all the TPDD nodes.
		std::vector<tpddnode*> NodeList;

	/// the 1-terminal node of the decision diagram for the coefficients of the s-variable.
		scoeffNode *pSCoeffOne;
	
	/// the 0-terminal node of the decision diagram for the coefficients of the s-variable.
		scoeffNode *pSCoeffZero;
	
	/// the 1-terminal node of the expanded tpdd decision diagram (no lumped symbols).
		sexpTpddNode *pSExpOne;

	/// the 0-terminal node of the expanded tpdd decision diagram (no lumped symbols).
		sexpTpddNode *pSExpZero;

		double* Vlist;

		// Total number of nodes
		int TotalNodes_;

		// Numbers used to trace tpdd paths
		int minusN;
		int plusN;

		// Numbers for statistics
		int TotalSExpTpddNodeNum;
		int spanningN;
		int totalSubNodes;
		int subTreeSharingTimes;
		int tpddNodeSharingTimes;
		int totalTpddNodes;
		int reduceNodeNum;
		// the number of the nodes of the decision diagram of the coefficients of the s-variable.
		int TotalSCoeffNodeNum;
		// Flags
		int *PermMark;
		int *NodeMark;
		int MaxNumNodes;
	
public:	
	/// the root node of the expanded tpdd decision diagram (no lumped symbols).
	sexpTpddNode *pExpTpddRoot;

	/// the unique table for the reduced graphs
	HasherTree** uniqSubTree;

	/// the unique table for the tpdd nodes.
	HasherTree*** uniqTpdd;

	/// the unique table for the nodes of the coefficients of s-variables.
	HasherTree*** uniqScoeff;

	/// the unique table for the expandded tpdd nodes.
	HasherTree*** uniqSexpNode;

public:
	/**
		The default contructor of tpdd.
	*/
	tpdd();

	/**
		The constructor of tpdd with parameters.
		@param nn the number of the nodes in the graph of the circuit to analysis.
		@param en the number of the edges in the graph of the circuit to analysis.
		@param edge_list the list of the edges  in the graph of the circuit to analysis.
	*/
	tpdd(int nn,  int en, Edge_g * edge_list);

	/**
		The destructor of tpdd.
	*/
	~tpdd();

		inline int GetSensitivity_tag() {return sensitivity_tag;}
		
		inline void SetSensitivity_tag(int Sen_tag) {sensitivity_tag = Sen_tag;}
	/**
		Get the number of the edges in the graph of the circuit to analysis.
	*/
	inline int GetNumEdges() {return E;}

	/**
		Get the number of the nodes in the graph of the circuit to analysis.
	*/
	inline int GetNumNodes() {return N;} 

	/**
		Get the number of the lumped symbols in the graph of the circuit to analysis.
	*/
	inline int GetNumSymbol() {return SymbN;}

	/**
		Get the number of the voltage controlled sources in the circuit to analysis.
	*/
	inline int GetNumVS() {return VsN;}
	
	/**
		Get the number of the voltage controlling sources in the circuit to analysis.
	*/
	inline int GetNumVC() {return VcN;} 

	/**
		Get the number of the current controlled sources in the circuit to analysis.
	*/
	inline int GetNumCS() {return CsN;}

	/**
		Get the number of the current controlling sources in the circuit to analysis.
	*/
	inline int GetNumCC() {return CcN;}

	/**
		Get the number of the dependent sources in the circuit to analysis.
	*/
	inline int GetNumDependentSource() {return DpenSrcN;}
//	inline unsigned int GetNumTerms() {return TermN;}

	/**
		Get the number of the terms in the transfer function (TF).
	*/
	inline double GetNumTerms() {return TermN;}

	/**
		Get the number of the tpdd nodes after Reduce.
	*/
	inline int GetNumReduceNodes() {return TotalRNodeNum;}

	/**
		Get the root of the tree pair decision diagram.
	*/
	inline tpddnode* GetRootNode() {return pTpddRoot;}

	/**
		Get the value of the symbol with index <I> index</I>.
		<BR>If the symbol is a admittance/impedance, the value should be the value of this admittance/impedance.
		<BR>If the symbol is a source, the value should be the gain of this source.
		@param index the index of the symbol whose value should be returned.
		@return the value returned is the real part of the symbol value under certain frequency.
	*/
	inline double	GetValue(int index) { return  Slist[index]->value; }
	
	inline double   GetEvaluateValue(int index) {return Vlist[index];}

	/**
		Get the name of the symbol with index <I> index</I>.
		@param index the index of the symbol whose name should be returned.
	*/
	inline char	*GetSymbName(int index) { return  Slist[index]->name; }

	/**
		Get the value of the symbol with index <I> index</I>.
		@param index the index of the symbol whose name should be returned.
		@return the value returned is the imaginary part of the symbol value under certain frequency.
	*/
	inline double	GetImaginary(int index) { return  Slist[index]->imag; }

	/**
		Get the symbol list Slist.
		Each entry in the Slist is a pointor to a SymbNode.
	*/
	inline SymbNode ** GetSlist() {return Slist;}

	int Determine_tag(char * name);
	void Modify_value(double new_value, int tag, char * name);
	double Search_value(int tag, char *name);
	void Element_conj(double freq);
	inline void SetValue(int index, double value){Slist[index]->value = value; Slist[index]->imag = 0.0;}
	inline void SetImaginary(int index, double value) {Slist[index]->imag = value;}

	inline tpddnode *GetpOne() {return pOne;}
	inline tpddnode *GetpZero() {return pZero;}
	void ModifyStructure(int tag, char * name);
	void ModifyStructureR(tpddnode *P, int tag, char * name);
//	inline double GetImaginary(int index) {return Slist[index]->imag;}
	/**
		The function to create the edge list.
	*/
	int CreateEdgeList(Edge_g * edge_list);

	/**
		The function to create the symbol list.
		<BR><I>Slist</I> is the list for the lumped symbols.
		<BR><I>SNoLumplist</I> is the list for the non-lumpled symbols.
	*/
	int CreateSymbList();

	/**
		The function to add symbols into the symbol list Slist.
		The symbols could be lumped are managed in this function.
	*/
	void AddSymbol(Edge *p_edge);

	/**
		The function to sort the symbols before constructing a TPDD.
		Heuristic ordering are applied in this function.
		<BR><EM><B>Not implemented perfectly yet!</B><EM>
	*/
	void SortSymbList();

	/**
		The function to set the paired edges. If the two edges are related to one dependent source, 
		they should be the paired edges with each other.
	*/
	void SetPairing();

	/**
		Set the value of the symbol under certain frequency.
		<BR> R : value = 1/R;
		<BR> G : value = G;
		<BR> C : value = 0 + C * 2 * pi * freq * j;
		<BR> L : value = 0 - 1/(L * 2 * pi * freq)j;
		<BR> sources: value = the gain of the sources;
		@param freq the frequency.
	*/
	void SetSymbValue(double freq);
	
	/**
		The function to initialate the left graph for further graph reduction.
		The left graph should remove all the edges with type VC.
	*/
	EdgeInfo* InitialLTree(int &signL);

	/**
		The function to initialate the right graph for further graph reduction.
		The left graph should remove all the edges with type CS.
	*/
	EdgeInfo* InitialRTree(int &signR);

	/**
		The function to initialate the root node of TPDD.
	*/
	void InitialRoot();

	/**
		The function to create the left reduced graph of current tpdd node
		The rules of graph reduction should be applied in this function.
		@param SymbolIndex the index of the symbol to manage in this function.
		(we may reduce the edges with related with this symbol in this graph.)
		@param type the type of reduction. 0 - select this symbol; 1 - omit this symbol.
		@param pParent the parent node of this tpdd node.
		@param NumEdges the number of the edges in the left reduced graph. 
		This value is calculated before the graph is reduced.
		@param NumNodes the number of the nodes in the left reduced graph. 
		This value is calculated before the graph is reduced.
		@param LeftNodes the nodes in the reduced graph. the value is calculated after the graph is reduced.
		@param singL the sign of the reduced graph.
	*/
	EdgeInfo* CreateLTree(int SymbolIndex, int type, tpddnode * pParent, int NumEdges, int NumNodes, int &LeftNodes, int &signL);
	
	/**
		The function to create the right reduced graph of current tpdd node
		The rules of graph reduction should be applied in this function.
		@param SymbolIndex the index of the symbol to manage in this function.
		(we may reduce the edges with related with this symbol in this graph.)
		@param type the type of reduction. 0 - select this symbol; 1 - omit this symbol.
		@param pParent the parent node of this tpdd node.
		@param NumEdges the number of the edges in the right reduced graph. 
		This value is calculated before the graph is reduced.
		@param NumNodes the number of the nodes in the right reduced graph. 
		This value is calculated before the graph is reduced.
		@param RightNodes the nodes in the reduced graph. the value is calculated after the graph is reduced.
		@param singL the sign of the reduced graph.
	*/
	EdgeInfo* CreateRTree(int SymbolIndex, int type, tpddnode * pParent, int NumEdges, int NumNodes, int &RightNodes, int &signR);
	
	/**
		The function to create a new tpdd node.
		Always used to create the children nodes of the current tpdd node.
		We will create the left reduce graph and the right reduced graph in this function.
		@param SymbolIndex the index of the symbol for the tpdd node to create. 
		@param the type of reduction. 0 - select this symbol; 1 - omit this symbol.
		@param pParent the parent node of this tpdd node.
	*/
	tpddnode* CreateNode(int SymbolIndex, int type, tpddnode * pParent);

	/**
		Create the TPDD recursivly . 
		Use Minty's algorithm to enumerate all the valid tree pairs.
		<EM>May have bugs in this function!</EM>
	*/
	void MintySpanR();
	
	/**
		Recursivly create the subTPDD pointed by the open edge of current tpdd node. 
		Use Minty's algorithm to enumerate all the valid tree pairs.
		@param pParent the pointer to the current tpdd node.
	*/
	tpddnode* MintySpanO(tpddnode * pParent);
	
	/**
		Recursivly create the subTPDD pointed by the short edge of current tpdd node. 
		Use Minty's algorithm to enumerate all the valid tree pairs.
		@param pParent the pointer to the current tpdd node.
	*/
	tpddnode* MintySpanS(tpddnode * pParent);
	
	/**
		Create the TPDD in a non-recursive fashion. 
		Use Minty's algorithm to enumerate all the valid tree pairs.
		A stack is used to create the TPDD in a non-recursive fashion.	
	*/
	void MintySpan();
	
	/**
		Create the TPDD in a non-recursive fashion.
		Use Approximate Analysis methods to generate the transfer function.
		The method we use for appximate analysis is to generate the terms with lower order of s-variables.
		In other words, if the correct form of a polynorminal is 
		<BR>P(n) = a0 + a1 * s + a2 * s ^ 2 + ... + an * s ^ n
		<BR>When performing approximate analysis, we just generate the terms with lower order of s-variables.
		<BR> for example we just generate P(m) = a0 + a1 * s + a2 * s ^ 2 + ... + am * s ^ m, where n << m.
		<BR> Since the construction of TPDD is in a depth-first fashion, it is easy to calcualte the order
		of the s-variable of the valid trees. Early failure detection should be used in this function. However, since we
		calculate the order of the valid trees, we should not lump the parallel symbols in the circuit. Thus, the number of 
		the symbols increase greatly and lead to low efficiency of TPDD construction. Efforts are made for efficient 
		approximate analysis!!!!
		<B><EM>The function is under construction!</EM></B>	
	*/
	void ApproximateMinthSpan();

	/**
		Create the TPDD in a non-recursive fashion. 
		Use Minty's algorithm to enumerate all the valid tree pairs.
		Reduce is performed during the TPDD contructions while reduce is perfomed after the TPDD contruction 
		in MintySpan().
		<EM>The efficiency is not ideal!</EM>
	*/
	void MintySpanSR();

	/**
		Get the total number of nodes in the reduced graph.
		@param pEdgeInfo the pointor to the array representing the reduced graph.
		@param e_size the number of the edges in the reduced graph.
	*/
	int GetNodeSize(EdgeInfo *pEdgeInfo, int e_size);
	
	/**
		Check whether the a valid tree is generated or not
		@param pEdgeInfo the pointer to the reduced graph from which the valid tree is generated.
		@param e_size the number of the edges in the reduced graph.
		@param n_size the number of the nodes in the reduced graph.
	*/
	int CheckValidTree(EdgeInfo *pEdgeInfo, int e_size, int n_size);
	
	/**
		Detect if a valid tree could be generated from this reduced graph.
		@param pEdgeInfo the pointer to the reduced graph from which the valid tree is generated.
		@param e_size the number of the edges in the reduced graph.
		@param n_size the number of the nodes in the reduced graph.
	*/
	int DetectTree(EdgeInfo *pEdgeInfo, int e_size, int n_size);

	/**
		Get the permuation number of this reduced graph.
		The number is used to determine the sign of the reduced graph generated by current reduced graph.
		@param pEdgeInfo the pointer to the current reduced graph from which the valid tree is generated.
		@param e_size the number of the edges in the reduced graph.
		@param n_size the number of the nodes in the reduced graph.
	*/
	int GetPermNum(EdgeInfo *pEdgeInfo, int e_size, int n);

	/**
		Print the infos of the symbols in the circuit.
	*/
	void PrintSymbols();

	//! Change the value of a specific component
	//! Li Ji
	//! 2008-10-29 22:46
	double change_value(const std::string &, double);
	//! 2009-03-04
	void get_name_ptrvalue_map(std::map<std::string, double *> &);

	/**
		Print the infos of the graph converted by the circuit to analysis.
		@param out the ostream object for console output.
	*/
	void PrintGraph(std::ostream &out);

	/**
		Print all the nodes in the TPDD.
		@param out the ostream object for console output.
	*/
	void PrintAllNodes(std::ostream &out);

	/**
		Recusively Print all the nodes in the TPDD.
		@param out the ostream object for console output.
		@param p the root of the subTPDD to be printed.
	*/
	void PrintAllNodesR(std::ostream &out, tpddnode *p);

	/**
		Unmark all the nodes in TPDD.
	*/
	void UnMarkAllNodes(Tnode* p, int type);

	/**
		Recusively unmark all the nodes in TPDD.
		@param p the root of the subTPDD to be unmarked.
	*/
	void UnMarkNodeR(tpddnode *p);

	/**
		Print all the terms in the TPDD.
		@param out the ostream object for console output.
	*/
	void PrintPaths(std::ostream &out);

	/**
		Print all the terms in the TPDD.
		@param out the ostream object for console output.
		@param p the root of the subTPDD whose terms to be printed.
	*/
	void PrintPathsR(std::ostream &out, tpddnode *p);

	/**
		Print the transfer function of the circuit.
		<BR><PRE>		The TPDD should be :		</PRE>
	    <PRE>			 root(X)					</PRE>				
        <PRE>			  /  \						</PRE>
        <PRE>			num   den					</PRE>
        <PRE>			/\     /\					</PRE>
		<PRE>	      ...... ......					</PRE>
		<BR>The transfer function (TF) will be:
        <BR><UL>		    num
				TF =   - ---------
		<BR><UL>            den
		@param out the ostream object for console output.
	*/
	void PrintTransferFunc(std::ostream &out);

	/**
		Recursively Print the transfer function of the circuit.
		@param out the ostream object for console output.
		@param p the root of the subTPDD whose transfer function to be printed.
	*/
	void PrintTransferFuncR(std::ostream &out, tpddnode *p);

	/**
		Calculate the number of the terms of TODD.
	*/
	void CalculateTerms();

	//int CalculateTermsR(tpddnode *p);
	/**
		Recursively calculate the number of the terms of TODD
		@param p the root of the subTPDD whose terms to be calculated.
	*/
	double CalculateTermsR(tpddnode *p);

	/**
		The function to share the reduce graphs.
		We use a unqiue table to perform the hash scheme in this function.
		@param p the point to the reduced graph to hash.
		@return the hashed reduced graph. If the graph is not in the unique table, it will be put into
		the unique table and it will be returned directly; If the graph is already in the unique table, 
		the graph will be released and the existing one will be returned.
	*/
	subtree *ShareSubTree(subtree* &p);

	/**
		The function to share the tpdd nodes
		We use a unqiue table to perform the hash scheme in this function.
		@param p the point to the tpdd node to hash.
		@param tag a reference parameter to tell if the node is hashed or not. 1 - hashed; 0 - not hashed. Although
		this is a parameter of the function, since it is a refecence, we change its value in the function and it plays
		as a return value of the function.
		@return the hashed tpdd node. If the tpdd node is not in the unique table, it will be put into
		the unique table and it will be returned directly; If the tpdd node is already in the unique table, 
		the tpdd node will be released and the existing one will be returned.
	*/
	tpddnode *ShareTpddNode(tpddnode *&p, int& tag);

	/**
		Remove all the tpdd node in the unique table
	*/
	void RemoveshareTpddNode(tpddnode *p);

	/**
		Get the number of the valide edges in the reduced graph.
		The valid edges should not be shelf-looped.
		If several edges have the same end nodes, the number of the valid edges 
		of this group of edges should be 1.
		@param pe the pointer to the reduced graph.
		@param En the number of the edges in the reduced graph.
		@return int number of valid edges.
	*/
	int  GetValidEdges(EdgeInfo *pe, int En);

	/**
		Collect all the nodes in the TPDD into the vector ReducedList for TPDD reduce.
	*/
	void CollectNodes();

	/**
		Recursively collect all the nodes in the TPDD into the vector ReducedList for TPDD reduce.
		@param p the root of the subTPDD whose nodes will be collected.
	*/
	void CollectNodesR(tpddnode * p);

	/**
		TPDD reduce.
		After reducion, there will not be isomorphic subgraphs in TPDD.
		isomorphic subgraphs is generated due to the isomorphic reduced graphs.
	*/
	void Reduce();

	/**
		Garbage collection.
		When performing TPDD reduce, the tpdd nodes belonging to the isomorphic reduced graphs
		should be released.
	*/
	void GC();

	/**
		Garbage collection in the unique table.
		Garbage nodes will be released.
		@param i the first dimension of the unique table.
		@param j the second dimension of the unique table.
	*/
	void doGC(int i, int j);

	/**
		Set the label of all the tpdd nodes.
		The label is used in TPDD reduce.
		@param ReduceHTree the unique table for TPDD reduction. The number of the nodes with the same label
		should always be 1 for a reduced TPDD.
	*/
	void SetNodeLabel( HasherTree ***ReduceHTree);

	/**
		Recursively set the label of all the tpdd nodes.
		The label is used in TPDD reduce.
		@param ReduceHTree the unique table for TPDD reduction. The number of the nodes with the same label
		should always be 1 for a reduced TPDD.
		@param p the root of the subTPDD whose nodes' lables are to be set.
		@param level the entry of the unique table.
	*/
	void SetNodeLabelR(tpddnode *p, HasherTree **ReduceHTree, int level);
	
//  Reduce functions during spanning
	/**
		Collect all the nodes in the TPDD into the vector ReducedList for TPDD reduce.
		<B>The TPDD reduce is performed during the construction of TPDD.</B>
	*/
	void CollectNodesS();

	/**
		Recursively collect all the nodes in the TPDD into the vector ReducedList for TPDD reduce.
		<B>The TPDD reduce is performed during the construction of TPDD.</B>
		@param p the root of the subTPDD whose nodes will be collected.
	*/
	void CollectNodesRS(tpddnode * p);

	/**
		TPDD reduce.
		<B>The TPDD reduce is performed during the construction of TPDD.</B>
		After reducion, there will not be isomorphic subgraphs in TPDD.
		isomorphic subgraphs is generated due to the isomorphic reduced graphs.

	*/
	void ReduceS();

	/**
		Garbage collection in the unique table.
		Garbage nodes will be released.
		<B>The Garbage Collection is performed during the construction of TPDD.</B>
		
	*/
	void GCS();

	/**
		Garbage collection in the unique table.
		Garbage nodes will be released.
		<B>The Garbage Collection is performed during the construction of TPDD.</B>
		@param i the first dimension of the unique table.
		@param j the second dimension of the unique table.
	*/
	void doGCS(int i, int j);

	/**
		Set the label of all the tpdd nodes.
		The label is used in TPDD reduce.
		<B>The TPDD reduce is performed during the construction of TPDD.</B>
		@param ReduceHTree the unique table for TPDD reduction. The number of the nodes with the same label
		should always be 1 for a reduced TPDD.
	*/
	void SetNodeLabelS( HasherTree ***ReduceHTree);

	/**
		Recursively set the label of all the tpdd nodes.
		The label is used in TPDD reduce.
		<B>The TPDD reduce is performed during the construction of TPDD.</B>
		@param ReduceHTree the unique table for TPDD reduction. The number of the nodes with the same label
		should always be 1 for a reduced TPDD.
		@param p the root of the subTPDD whose nodes' lables are to be set.
		@param level the entry of the unique table.
	*/
	void SetNodeLabelRS(tpddnode *p, HasherTree **ReduceHTree, int level);


	tpddnode *ShareTpddNodeS(tpddnode *&p, int& tag);

// Operations for extracting the coefficients of S-variable
	/**
		Reference all the nodes in TPDD.
	*/
	void ReferenceAll();

	/**
		Recursively reference all the nodes in TPDD.
	*/
	void ReferenceR(tpddnode *p);

	/**
		UnReference all the nodes in TPDD.	
	*/
	void UnReferenceAll(Tnode*p, int type);

	/**
		Recursively UnReference all the nodes in TPDD.	
	*/
	void UnReferenceR(tpddnode *p);

	
	void scoeff();

	
	void scoeffR(tpddnode * p);

	/**
		Extract the coeffcients of the s-variables in the transfer function.
		The symbols should not be lumped for s-coeffcients extraction.
	*/
	void scoeffNew();

	/**
		Recursively Extract the coeffcients of the s-variables in the transfer function.
		The transfer function is s-variable coefficients extracted. That means the TF is
	    <BR><UL>					a0 + a1 * s + a2 * s^2 + ... + am * s^m </UL> 
				TF =    --------------------------------------------------------
		<BR><UL>				    b0 + b1 * s + b2 * s^2 + ... + bn * s^n</UL>

		The approximate poles/zeros could be achieved by the coefficients of the s-variables, e.g.
		<BR><PRE>  pole_i = -bi / bi+1 </PRE>
		<PRE> zero_i = -ai / ai+1 </PRE>
		@param p the root of the expanded subTPDD whose coeffcients of s-variables should be extracted.
	*/
	void scoeffRNew(sexpTpddNode * p);

	/**
		Print all the coeffcients of the s-variables with different orders.
		@param out the ostream object for console output.
	*/
    void PrintScoeffs(std::ostream &out);

	/**
		Print all the coeffcients of the s-variables with different orders.
		@param out the ostream object for console output.
		@param pscoeffNode the pointer to the nodes containing the info of the coefficients of s-variable.
		@param iSign the sign of the coefficients.
	*/
	void PrintScoeffPath(std::ostream &out, scoeffNode *pscoeffNode, int iSign = 1);

	/**
		Recursively Print all the coeffcients of the s-variables with different orders.
		@param out the ostream object for console output.
		@param pscoeffNode the pointer to the root node whose descendants contain the info of the coefficients of s-variable.
	*/
	void PrintScoeffPathR(std::ostream &out, scoeffNode *pscoeffNode);

	/**
		The function to share the scoeffNode.
		We use a unqiue table to perform the hash scheme in this function.
		@param p the point to the scoeffNode to hash.
		@param tag a reference parameter to tell if the node is hashed or not. 1 - hashed; 0 - not hashed. Although
		this is a parameter of the function, since it is a refecence, we change its value in the function and it plays
		as a return value of the function.
		@return the hashed scoeffNode. If scoeffNode is not in the unique table, it will be put into
		the unique table and it will be returned directly; If the scoeffNode is already in the unique table, 
		the scoeffNode will be released and the existing one will be returned.
	*/
	scoeffNode *ShareScoeffNodeNode(scoeffNode *&p, int& tag);

	/**
		Expand the TPDD nodes whose symbol is lumped.
		<BR><EM><B>x is a symbol with parallaled edges a, b, c.</B></EM>
		The node x will be expanded with nodes x, a, b, c in the expanded TPDD. Below is an example 
		for the TPDD is expansion.
		<BR><PRE>   x -> a -> b -> c 
		(ss: sign attached to the short edge; so: sign attached to the open edge)			</PRE>
		<PRE>				   x				</PRE>
		<PRE>			signs / \ signo			</PRE>
		<PRE>				 s   o				</PRE>   
		<PRE>				/ \ / \				</PRE>
		<PRE>			  ...........			</PRE>
		<PRE> expand =>							</PRE>
		<PRE>				  x					</PRE>
		<PRE>			  ss / \ 1				</PRE>
		<PRE>				/    a				</PRE>
		<PRE>			   /    / \ 1			</PRE>
		<PRE>			  |  ss/    b			</PRE>
		<PRE>			  |   /    / \ 1		</PRE>
		<PRE>			  |  / ss-/   c			</PRE>
		<PRE>			  | /  -/ ss-/  \ so	</PRE>
		<PRE>			  |/ -/ --/      o		</PRE>
		<PRE>			   s            / \		</PRE>
        <PRE>			  / \	      ......	</PRE>
		<PRE>			 .......				</PRE>

	*/
	void sexpand();

	/**
		Recursively expand the TPDD nodes whose symbol is lumped.
		@param p the root of the subTPDD to be expanded.
	*/
	sexpTpddNode *sexpandR(tpddnode* p);

	/**
		The function to share the sexpTpddNode.
		We use a unqiue table to perform the hash scheme in this function.
		@param p the point to the sexpTpddNode to hash.
		@param tag a reference parameter to tell if the node is hashed or not. 1 - hashed; 0 - not hashed. Although
		this is a parameter of the function, since it is a refecence, we change its value in the function and it plays
		as a return value of the function.
		@return the hashed sexpTpddNode. If sexpTpddNode is not in the unique table, it will be put into
		the unique table and it will be returned directly; If the sexpTpddNode is already in the unique table, 
		the sexpTpddNode will be released and the existing one will be returned.
	*/
	sexpTpddNode *ShareSExpTpddNodeS(sexpTpddNode *&p, int& tag);

	/**
		Clean the sexpTpddNodes.
		Since the expanded TPDD nodes are not in the unqiue table. We should release them manually.
	*/
	void cleanSExpTpddNodes();

	/**
		Recursively Clean the sexpTpddNodes.
	*/
	void cleanSExpTpddNodesR(sexpTpddNode *&p);

	void calcScoeffTermN();

	void calcScoeffTermNR(scoeffNode* p);

	void calcSymbAppearTimes();

	void calcSymbAppearTimesR(tpddnode* p);

	void MarkAllNodes(Tnode* p, int type);

	void clacNodeReference(Tnode* p, int type);

	void getMostReferencedNode(Tnode* p, int type);

	// Set and Return total number of nodes
	void TotalNodes(int tn) {TotalNodes_ = tn;}
	int TotalNodes() const {return TotalNodes_;}

	int get_minusN() const {return minusN;}
	int get_plusN() const {return plusN;}

	int get_TotalSExpTpddNodeNum() const {return TotalSExpTpddNodeNum;}
	int get_totalTpddNodes() const {return totalTpddNodes;}
	int get_spanningN() const {return spanningN;}
	int get_totalSubNodes() const {return totalSubNodes;}
	int get_subTreeSharingTimes() const {return subTreeSharingTimes;}
	int get_tpddNodeSharingTimes() const {return tpddNodeSharingTimes;}
	int get_reduceNodeNum() const {return reduceNodeNum;}
	int get_TotalSCoeffNodeNum() const {return TotalSCoeffNodeNum;}
};


#endif


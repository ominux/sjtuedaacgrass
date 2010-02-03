/**************************************************************************************
*        Symbolic Circuit Analysis With TPDDs (Tree Pair Decision Diagram)
* subtree.h
* Class Definition for subtree.
* subtree is the class to represent one tree of the tree pair.
*
* CHEN Weiwei		GShi	06/03/31
* School of Microelectronics, Shanghai Jiaotong Univ.
* {chenweiwei, shiguoyong}@ic.sjtu.edu.cn
**************************************************************************************/


#ifndef SUBTREE_H
#define SUBTREE_H

#include "comparable.h"
#include "tpdd_struct.h"
#include "class_id.h"

/**
	The class for the reduced graph for graph reduce process.
	It is proper to name this class subReduceGraph.
*/

class subtree : public Comparable
{	
private:
/*	short EdgeNum   : 8;
	short NodeNum   : 14;
	short sign		: 2;*/
	/// the number of the edges in the reduced graph.
	short EdgeNum   ;

	/// the number of the nodes in the reduced graph.
	short NodeNum   ;

	/** 
		the sign of the reduced graph.
		the sign of the node equals 
		<BR>
		signL(sign of theleft reduced graph) * signR(sign of the right reduced graph).
	*/
	short sign		;

	/// the pointer to the array recording the edge infos in the reduced graph.
	EdgeInfo *pTree;

public:
	/**
		The default constructor of subtree.
	*/
	subtree(){};

	/**
		The constructor of a subtree.
		@param pe the pointer to the array recording the edge infos in the reduced graph.
		@param En the number of the edges in the reduced graph.
		@param Nn the number of the nodes in the reduced graph.
		@param s  the sign of the reduced graph.
	*/
	subtree(EdgeInfo *pe, int En, int Nn, int s);

	/**
		The destructor of the subtree.
	*/
	~subtree();

	/**
		Get the pointer to the array recording the edge infos in the reduced graph.
		@return EdgeInfo* pTree.
	*/
	inline EdgeInfo * GetSubTree() {return pTree;}

	/**
		Get the the number of the edges in the reduced graph.
		@return int EdgeNum.
	*/
	inline int GetNumEdges() {return EdgeNum;}

	/**
		Get the the number of the nodes in the reduced graph.
		@return int NodeNum.
	*/
	inline int GetNumNodes() {return NodeNum;}

	/**
		Get the sign of the reduced graph.
		@return int sign.
	*/
	inline int GetSign() {return sign;}
//	inline short GetTag() {return tag;}

	/**
		Set the pointer to the array recording the edge infos in the reduced graph.
		@param pe the pointer to the array recording the edge infos in the reduced graph.
	*/
	inline void SetSubTree(EdgeInfo *pe) {pTree = pe;}

	/**
		Set the number of the edges in the reduced graph.
		@param En the number of the edges in the reduced graph.
	*/
	inline void SetNumEdges(int En) {EdgeNum = En;}

	/**
		Set the number of the nodes in the reduced graph.
		@param Nn the number of the nodes in the reduced graph.
	*/
	inline void SetNumNodes(int Nn) {NodeNum = Nn;}

	/**
		Set the sign of the reduce graph.
		@param s the sign of the reduced graph.
	*/
	inline void SetSign(int s) {sign = s;}
//	inline void SetTag(int t) {tag = t;}

	/**
		The function to identify the type of this class
		@return SUBTREE
	*/
	virtual integer isa() {return SUBTREE;}


	/**
	    The function to get the hash value.
		The value is determined by all the edges in the reduced graph.
	*/
	virtual unsigned Hash(unsigned i = 0);

	/**
		The function to compare two subtrees with the same hash value.
		@param p: the subtree with the same hash value with this subtree.
	*/
	virtual integer Compare(Comparable *p);

	/**
		 The function to get the entry of the hash table.
		 the entry is determined by the number of the edges and nodes in the reduced graph.
	*/
	unsigned HashTreeID();

	/**
		The function to compare two subtrees.
		@param pSubTree the subtree to be compared with the subtree pointed by this.
		@return 1 the two subtrees are identical.
		@return 0 the two subtrees are different.
	*/
	int compareTree(subtree * pSubTree);

	/**
		The function to compare two subtrees isomorphic or not.
		@param pSubTree the subtree to be compared with the subtree pointed by this.
		@param edgeNum the number of the edges.
		@return 1 the two subtrees are isomorphic.
		@return 0 the two subtrees are not isomorphic.

		Two reduced graphs are isomorphic if and only if they have the same topologies.
		(The index of the nodes may be different but the connection of the edges should be the same.)

	  <BR>	The complexity of this function is O(E*N).
	  <BR>	E = the number of the edges.
	  <BR>	N = the number of the nodes.
	  <BR><PRE><I><EM> graph1 and graph2 is isomorphic.	</EM></I></PRE>
      <PRE>     v1                         v1			</PRE>
      <PRE>     /\                         /\			</PRE>
      <PRE>    /  \                       /  \			</PRE>
      <PRE>e1 /    \  e3              e2 /    \  e3		</PRE>
      <PRE>  /      \                   /      \		</PRE>	
      <PRE> v2 ----- v3                v2 ----- v3		</PRE>
      <PRE>      e2                         e1			</PRE>
      <PRE>    graph1                     graph2		</PRE>

	  <BR><PRE><I><EM> graph1 and graph2 is not isomorphic.	</EM></I></PRE>
      <PRE>     v1                         v1			</PRE>
      <PRE>     /\                         /|\			</PRE>
      <PRE>    /  \                       / | \			</PRE>
      <PRE>e1 /    \  e3              e2 / /e1 \  e3	</PRE>
      <PRE>  /      \                   /_/     \		</PRE>	
      <PRE> v2 ----- v3                v2      v3		</PRE>
      <PRE>      e2                         			</PRE>
      <PRE>    graph1                     graph2		</PRE>
	*/
	int completeCompareTree(subtree * pSubTree, int edgeNum);

	/**
		Get the number of the valide edges in the reduced graph.
		The valid edges should not be shelf-looped.
		If several edges have the same end nodes, the number of the valid edges 
		of this group of edges should be 1.
		@return int number of valid edges.
	*/
	int GetValidEdges();

	/**
		Get the index of the first valid edge in the reduced graph.
		This function is designed to assist the function compareTree.
		@return the index of the first valid edge.
	*/
	int GetValidEdgeId();

	/**
		Get the index of the first valid edge in the reduced graph with directions.
		This function is designed to assist the function compareTree.
		@return the index of the first valid edge, position direction with positive sign and vice versa.
	*/
	int GetValidEdgeIndex();
};


/*class treetest : public Comparable
{
	short tag;
	short EdgeNum;
	short NodeNum;
	short sign;
	EdgeInfo *pTree;
	
};*/

#endif



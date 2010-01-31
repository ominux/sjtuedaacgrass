/**************************************************************************************
*        Symbolic Circuit Analysis With TPDDs (Tree Pair Decision Diagram)
* tpdd.cpp
* member functions for class tpdd.
* tpdd is the class for TPDD.
*
* CHEN Weiwei		GShi	06/03/31
* School of Microelectronics, Shanghai Jiaotong Univ.
* {chenweiwei, shiguoyong}@ic.sjtu.edu.cn
**************************************************************************************/
#include "tpdd.h"

#include "tpdd_global.h"
#include "utility.h"
#include <map>
#include <utility>
#include <iostream>
#include <iomanip>
#include <stack>
#include <string>
#include "option.h"
using namespace std;

// int *PermMark;
// int *NodeMark;
// int MaxNumNodes;

// extern int spanningN;
extern int compare (const void*, const void*);
extern int compare_g (const void*, const void*);
extern int compare_l (const void*, const void*);
// extern int My_ZRand;
// extern int subTreeSharingTimes;
// extern int tpddNodeSharingTimes;
// extern int totalTpddNodes;
// extern int totalSubNodes;
// extern int reduceNodeNum;
// extern int TotalSExpTpddNodeNum;

//extern char *RunStats();

tpdd::tpdd() :
 	Object(), TotalNodes_(0), minusN(0), plusN(0), TotalSExpTpddNodeNum(0),
	spanningN(0), totalSubNodes(0), subTreeSharingTimes(0),
 	tpddNodeSharingTimes(0), totalTpddNodes(0), reduceNodeNum(0),
	PermMark(0), NodeMark(0), MaxNumNodes(0)
{
	SymbN				= 0;
	TermN				= 0;
	pTpddRoot			= NULL;
	Elist				= NULL;
	Slist				= NULL;
	SExpList			= NULL;
	N					= 0;
	E					= 0;
	VsN					= 0;
	VcN					= 0;
	CsN					= 0;
	CcN					= 0;
	DpenSrcN			= 0;
	StackTop			= 0;

	pOne				= NULL;
	pZero				= NULL;
	PathArray			= NULL;
	TotalRNodeNum		= 0;
	TotalSCoeffNodeNum	= 0;

	pSCoeffOne			= NULL;
	pSCoeffZero			= NULL;

	pExpTpddRoot		= NULL;
	pSExpOne			= NULL;
	pSExpZero			= NULL;
}

tpdd::tpdd(int nn, int en, Edge_g *edge_list) :
 	Object(), TotalNodes_(0), minusN(0), plusN(0), TotalSExpTpddNodeNum(0),
	spanningN(0), totalSubNodes(0), subTreeSharingTimes(0),
	tpddNodeSharingTimes(0), totalTpddNodes(0), reduceNodeNum(0),
	PermMark(0), NodeMark(0), MaxNumNodes(0)
{
	int			i, j;


	N				= nn;
	E				= en;
	SymbN			= 0;
	TermN			= 0;
	Elist			= NULL;
	Slist			= NULL;
	SymbListTemp	= NULL;
	SExpList		= NULL;
	VsN				= 0;
	VcN				= 0;
	CsN				= 0;
	CcN				= 0;
	DpenSrcN		= 0;
	StackTop		= 0;
	pOne			= new tpddnode(-1, 1, 1, NULL, NULL);
	pZero			= new tpddnode(-2, 1, 1, NULL, NULL);
	PathArray		= NULL;
	
	pExpTpddRoot			= NULL;


//	pOne->sCoeffList		= new vector<scoeffNode*>;
//	pOne->sPowerList		= new vector<int>;
//	pOne->sCoeffList->push_back(pSCoeffOne);
//	pOne->sPowerList->push_back(0);

//	pZero->sCoeffList		= new vector<scoeffNode*>;
//	pZero->sPowerList		= new vector<int>;

	

	uniqSubTree				= new HasherTree*[SUBTREE_PRIME];
//	uniqTpdd				= new HasherTree*[TPDD_PRIME];
	uniqTpdd				= new HasherTree**[TPDD_PRIME_LEVEL];
	uniqScoeff				= new HasherTree**[TAG_PRIME1];
	uniqSexpNode			= new HasherTree**[TAG_PRIME1];


	for(i = 0; i < SUBTREE_PRIME; i ++)
	{
		uniqSubTree[i] = NULL;
	}

	for(i = 0; i < TAG_PRIME1; i++)
	{
		uniqSexpNode[i] = new HasherTree*[TAG_PRIME2];
		for(j = 0; j < TAG_PRIME2; j ++)
			uniqSexpNode[i][j] = NULL;
	}/**/

	for(i = 0; i < TPDD_PRIME_LEVEL; i ++)
	{
		uniqTpdd[i] = new HasherTree*[TPDD_PRIME];
		for(j = 0; j < TPDD_PRIME; j ++)
			uniqTpdd[i][j] = NULL;
	}
	
#ifdef HASHSCOEFFNODE 
	for(i = 0; i < TAG_PRIME1; i ++)
	{
		uniqScoeff[i] = new HasherTree*[TAG_PRIME2];
		for(j = 0; j < TAG_PRIME2; j ++)
			uniqScoeff[i][j] = NULL;
	}
#endif

	if ( ! CreateEdgeList(edge_list) )
	{
		cerr << "\nEnumerator(): Failed to create Edge List." 
	  		 << "\nProgram terminated." << endl;
		exit(1);
	}
	if ( !CreateSymbList())
	{
		cerr << "\nEnumerator(): Failed to create Symbol List." 
	  		 << "\nProgram terminated." << endl;
		exit(1);
	}

  	srand((integer) time((time_t*) NULL));
 	// My_ZRand = rand();

	PermMark = new int[N];
	NodeMark = new int[N];

}

tpdd::~tpdd()
{
	int i, j;

	if (PermMark)
	{
		delete[] PermMark;
		PermMark = 0;
	}
	if (NodeMark)
	{
		delete[] NodeMark;
		NodeMark = 0;
	}

#ifdef CALC_SCOEFFICIENTS
	cleanSExpTpddNodes();
#endif

	if(Elist)
	{
		delete[] Elist;
		Elist = 0;
	}

	if(pOne)
	{
		delete pOne;
		pOne = 0;
	}

	if(pZero)
	{
		delete pZero;
		pZero = 0;
	}

	if(PathArray)
	{
		delete[] PathArray;
		PathArray = 0;
	}

	if (Slist)
	{
		SymbNode	*pNode1, *pNode2;
		for (i = 0; i < SymbN; i++)
		{
			pNode2 = Slist[i]->nextP;
			while (pNode2)
			{
				pNode1 = pNode2;
				pNode2 = pNode2->next;
				delete[] pNode1->name;
				delete pNode1;
			}
			delete[] Slist[i]->name;
			delete Slist[i];
			Slist[i] = 0;
		}
		delete [] Slist;
		Slist = 0;
	}
//	delete [] SExpList;

	if(uniqSubTree)
	{
		for(i = 0 ; i < SUBTREE_PRIME ; i ++ )
		{
			if(!uniqSubTree[i])
				continue;
			uniqSubTree[i]->SetDelete(TRUE,FALSE);    
			delete uniqSubTree[i];
			uniqSubTree[i] = 0;
		}
		delete []uniqSubTree;
		uniqSubTree = 0;
	}

	if(uniqTpdd)
	{
		for(i = 0 ; i < TPDD_PRIME_LEVEL ; i ++ )
		{
			if(!uniqTpdd[i])
				continue;
			for(j = 0; j < TPDD_PRIME; j ++)
			{
				if(!uniqTpdd[i][j])
					continue;
				uniqTpdd[i][j]->SetDelete(TRUE,FALSE);
				delete uniqTpdd[i][j];
				uniqTpdd[i][j] = 0;
			}
			delete []uniqTpdd[i];
			uniqTpdd[i] = 0;
		}
		delete []uniqTpdd;
		uniqTpdd = 0;
	}

	if(uniqSexpNode)
	{
		for(i = 0; i < TAG_PRIME1; i++)
		{
			if(!uniqSexpNode[i])
				continue;
			for(j = 0; j < TAG_PRIME2; j ++)
			{
				if(!uniqSexpNode[i][j] )
					continue;
				uniqSexpNode[i][j]->SetDelete(TRUE, FALSE);
				delete uniqSexpNode[i][j];
				uniqSexpNode[i][j] = 0;
			}
			delete []uniqSexpNode[i];
			uniqSexpNode[i] = 0;
		}
		delete []uniqSexpNode;
		uniqSexpNode = 0;
	}


	NodeList.clear();



#ifdef HASHSCOEFFNODE 
	for(i = 0; i < TAG_PRIME1; i ++)
	{
		if(uniqScoeff[i])
		{
			if(!uniqScoeff[i])
				continue;
			for(j = 0; j < TAG_PRIME2; j ++)
			{
				if(!uniqScoeff[i][j])
					continue;
				uniqScoeff[i][j]->SetDelete(TRUE, FALSE);
				delete uniqScoeff[i][j];
				uniqScoeff[i][j] = 0;
			}
			delete[] uniqScoeff[i];
			uniqScoeff[i] = 0;
		}
	}

	delete[] uniqScoeff;
	uniqScoeff = 0;
#endif

#ifdef CALC_SCOEFFICIENTS
#if 0
	delete pSCoeffOne;
	pSCoeffOne = 0;
	delete pSCoeffZero;
	pSCoeffZero = 0;
#endif
	// pSCoeffOne and pSCoeffZero will be deleted
	// when deleting pSexpOne and pSExpZero.
	delete pSExpOne;
	pSExpOne = 0;
	delete pSExpZero;
	pSExpZero = 0;
#endif
}

int tpdd::CreateEdgeList(Edge_g * edge_list)
{
	if ( !edge_list )
	{
		cerr << "\nNo graph edge list available.";
		return 0;
	}
	
	if (E > 0)
		Elist = new Edge[E];
	else
	{
		cerr << "\nError in CreateEdgeList(): No edge in graph.";
		return 0;
	}
	
	Edge_g 	*edge;
	int		e = 0;
	int		max_n = 0;		// For checking the max node num.
	
	for(edge = edge_list; edge; edge = edge->next)
	{
		Elist[e].name = CopyStr( ToUpper(edge->name) ); 
		Elist[e].node1 = edge->node1->num;
  		Elist[e].node2 = edge->node2->num;
		Elist[e].lumped = 0;	// Not lumped
		Elist[e].edgeindex = e;
  		
		// Check syntax.
		if (Elist[e].node1 == Elist[e].node2)
		{
			cout << "\nError in CreateEdgeList(): Edge #" << e 
				 << " (" << Elist[e].name << ") "
				 << "is a self-loop.";
			delete [] Elist;
			Elist = 0;
			return 0;		// fail
		}

		// Make sure that the name of a pairing edge is set.
		if ( (edge->type >= CC) && (edge->pname == NULL) )
		{
			cerr << "\nThe name for one pairing edge is missing.";
		}
		Elist[e].type = edge->type;
		
		switch (edge->type)
		{
		case Z:
		case Y:
			Elist[e].value = edge->value;
			// cout << "edge value is : " << edge->value << endl;
			break;
		case VS:
			Elist[e].pname = CopyStr( ToUpper(edge->pname) );
			Elist[e].value = edge->value;
			VsN ++;
			break;
		case CS:
			Elist[e].pname = CopyStr( ToUpper(edge->pname) );
			Elist[e].value = edge->value;
			CsN ++;
			break;
		case VC:	
			Elist[e].pname = CopyStr( ToUpper(edge->pname) );
			VcN ++;
			break;
		case CC:
			Elist[e].pname = CopyStr( ToUpper(edge->pname) );
			CcN ++;
			break;
		default:
			cerr << "\nThe type of edge #" << e 
				 << " (" << Elist[e].name << ") "
				 << "incorrect.";
			delete [] Elist;
			Elist = 0;
			return 0;		// fail	
		}		
		// Keep the max node num.
		if (max_n < Elist[e].node1)		max_n = Elist[e].node1;
		if (max_n < Elist[e].node2)		max_n = Elist[e].node2;
		
		++e;	// next edge
	}

	// Nodes are supposed to be numbered from 0 ... N-1.
	// So check whether (max_n > N-1). 
	if (max_n > N-1)
	{
		cout << "\nNode number exceeded N-1. "
			 << "Check the node numbers in the graph file.";
		delete [] Elist;
		Elist = 0;
		return 0;		// fail
	}

	SetPairing();
	return 1;		// success
}

void tpdd::SetPairing()
{
	int e1;

	for (int e = 0; e < E; e++)
	{
		string	pname;
		if ( Elist[e].type > 1)		// For those dependent types.
		{
			pname = Elist[e].pname;
			e1 = 0;
			// Look for the pairing edge
			while ( pname.compare(Elist[e1++].name) );
			// Make a link.
			Elist[e].pnum = --e1;
		}
	}
}

int tpdd::CreateSymbList()
{	
	// Need an array for edge marks

	int	*edge_mark = new int[E];

	int edge = 0, e = 0, j = 0, m = 0;		

	for (e = 0; e < E; e++)
		edge_mark[e] = 0;

	// Traverse the Edge List.

	// Skip the first two edges which are a pair of I/O.
	for (e = 2; e < E; e++)
	{
		// Skip the marked edge.
		if ( edge_mark[e] )		continue;

		switch (Elist[e].type)
		{
		case  Y:
		case  Z:
			edge = e;
			break;
		case  CC:
		case  VC:
			edge = Elist[e].pnum;	// symbol with the pairing edge
			edge_mark[edge] = 1;	// mark the pairing edge
			DpenSrcN ++;
			break;
		case  CS:
		case  VS:
			edge = e;						// symbol with this edge
			edge_mark[ Elist[e].pnum ] = 1;	// mark the pairing edge
			DpenSrcN ++;	
			break;
		default:
			break;
		}
		AddSymbol( &Elist[edge]);
	}
	// Add the remaining one symbols.
	// Add "+" and "-" to the high end of SymbList[].
	SymbNode	*pNode = SymbListTemp, *pNewNode;
//	while (pNode->next)		pNode = pNode->next;

	// Add the X symbol (the highest).
	if (Elist[0].type == CS || Elist[0].type == VS)
		e = 0;
	else
		e = 1;

	SymbN += 1;		// Added three symbols more.
	Elist[e].index = SymbN;

	pNewNode = new SymbNode;
	pNewNode->name  = Elist[e].name;
	pNewNode->value = Elist[e].value;
	pNewNode->edge  = &Elist[e];
	pNewNode->nextP = NULL;
	pNewNode->next  = NULL;
//	pNewNode->edgeindex = e;
//	pNode->next = pNewNode;
	pNewNode->next = pNode;
	pNode = pNewNode;

	if(Elist[e].type > 1)
	{
		pNewNode->isSrc = 1;
		pNewNode->pairedge = &Elist[pNewNode->edge->pnum];
		DpenSrcN ++;
	}
	else
	{
		pNewNode->isSrc = 0;
		pNewNode->pairedge = NULL;
	}
	// Make a symbol array for quick access.
//	symbarray = new (SymbNode*)[Nsymb];
	Slist = new SymbNode*[SymbN];
//	pNode = SymbListTemp;
	
	for (int i = 0; i < SymbN; i++)
	{
		Slist[i] = pNode;
		pNode = pNode->next;
	}

	SortSymbList();

	SymbNode *pParaNode;
	TotalSymbN = 0;

	pNode = SymbListTemp;

/*	while(pNode)
	{	
		TotalSymbN ++;
		
		pParaNode = pNode->nextP;

		if(pParaNode)
		{
			pParaNode = pParaNode->nextP;
		}

		while(pParaNode)
		{
			TotalSymbN ++;
			pParaNode = pParaNode->nextP;
		}
		pNode = pNode->next;
	}*/

	for (int i = 0; i < SymbN; i++)
	{
		pParaNode = Slist[i]->nextP;
		TotalSymbN ++;

		if(pParaNode)
		{
		//	TotalSymbN ++;
		//	j ++;
			pParaNode = pParaNode->nextP;
		}

		while(pParaNode)
		{
			TotalSymbN ++;
			pParaNode					= pParaNode->nextP;
			j ++;
		}  m = i;
	}

	m ++;
	SExpList = new SymbNode*[TotalSymbN];
	for (int i = 0; i < SymbN; i++)
	{
		SExpList[i] = Slist[i];
		SExpList[i]->ExpSymbIndex = i;
	}

	j = SymbN;
	for (int i = 0; i < SymbN; i++)
	{
		pParaNode = Slist[i]->nextP;

		if(pParaNode)
		{
		//	SExpList[j]					= pParaNode;
		//	SExpList[j]->isSrc			= Slist[i]->isSrc;
		//	SExpList[j]->pairedge       = Slist[i]->pairedge;
		//	SExpList[j]->ExpSymbIndex	= j;
			pParaNode					= pParaNode->nextP;
		//	j ++;
		}/**/

		while(pParaNode)
		{
			SExpList[j]					= pParaNode;
			SExpList[j]->ExpSymbIndex	= j;
			SExpList[j]->isSrc          = 0;
			SExpList[j]->pairedge       = NULL;
			pParaNode					= pParaNode->nextP;
			j ++;
		}
	}

/*	for(i = 0; i < TotalSymbN; i ++)
	{
		cout << "symbol " << i << "  " << SExpList[i]->name << " " << SExendl;
	}*/

//	delete []Slist;
	cout << "total symbol number : " << j << endl;
	Slist = SExpList;

	SNoLumplist = new SymbNode*[TotalSymbN];
	j = 0;
	for (int i = 0; i < SymbN; i++)
	{
		if(Slist[i]->name[0] != '^')
			SNoLumplist[j] = Slist[i];
		else
		{
			SNoLumplist[j]					= Slist[i]->nextP;
			SNoLumplist[j]->isSrc			= Slist[i]->isSrc;
		    SNoLumplist[j]->pairedge		= Slist[i]->pairedge;
		}
		j ++;
	}
	for (int i = 0; i < SymbN; i++)
	{
		pParaNode = Slist[i]->nextP;

		if(pParaNode)
		{
			pParaNode					= pParaNode->nextP;
		}

		while(pParaNode)
		{
			SNoLumplist[j]					= pParaNode;
			SNoLumplist[j]->ExpSymbIndex	= j;
			SNoLumplist[j]->isSrc           = 0;
			SNoLumplist[j]->pairedge        = NULL;
			pParaNode					    = pParaNode->nextP;
			j ++;
		}
	}

	for(int i = 0; i < TotalSymbN; i ++)
	{
		cout << SNoLumplist[i]->name << " " << SNoLumplist[i]->edge->node1 << " " 
			<< SNoLumplist[i]->edge->node2 <<endl;
	}/**/

	delete[] edge_mark;
	edge_mark = 0;
	return 1;
}

void tpdd::AddSymbol(Edge *p_edge)
{
	// Always point at the tail of symblist. 
	// Use static to maintain the tail pointer.
	static SymbNode *pTail = NULL;  

	// First check whether this edge is in parallel to a symbol
	// already in the symblist.
	//cerr << "\nAdding edge - " << p_edge->name;
	SymbNode *pNode = SymbListTemp, *pNewNode;

	// Only lumps Y/Z edges.
	if (p_edge->type == Y || p_edge->type == Z)
	{
		// Check the parallelism.
		while (pNode)
		{
	//cout << "\nFile: " << __FILE__ << ", Line: " << __LINE__;
			// Skip those non-Y/Z edges.
			if (pNode->edge->type > 1) 
			{
				pNode = pNode->next;  
				continue;
			}

		/**/	if ( (p_edge->node1 == pNode->edge->node1 && 
				  p_edge->node2 == pNode->edge->node2) ||
		     	 (p_edge->node1 == pNode->edge->node2 && 
				  p_edge->node2 == pNode->edge->node1) )
			{
				//cerr << "\nDetected a parallel edge.";
				p_edge->lumped = 1;  	// This edge is lumped.

				// If lumping the first parallel edge, the
				// existing symbol is modified to be a composite symbol.
				if (!pNode->nextP)
				{
					//cerr << "\n !! The first lumping edge: " << p_edge->name;
					// Make a new node.
					pNewNode = new SymbNode;
					// Copy the 1st node to the 2nd.
					*pNewNode = *pNode;

					// Modify the symbol name of the composite symbol.
					char *new_name = new char[strlen(pNode->name)+2];
					sprintf(new_name, "^%s", pNode->name);
					pNode->name  = CopyStr(new_name);
					pNode->value = 0.0;		// unknown now
					// Should keep pNode->edge for parallelism checking.

					// Make connection.
					pNode->nextP = pNewNode;
					pNewNode->next  = NULL;
					pNewNode->nextP = NULL;
					delete[] new_name;
					new_name = 0;
				}

				// Go to the tail of the parallel symbol list.
				while (pNode->nextP)	pNode = pNode->nextP;
	//cout << "\nFile: " << __FILE__ << ", Line: " << __LINE__;
				// Add the new symbol node to the tail of parallel list.
				pNewNode = new SymbNode;
				pNode->nextP = pNewNode;

				pNewNode->name  = p_edge->name;
				pNewNode->value = p_edge->value;
				// cout << "value is : " << p_edge->value << endl;
				pNewNode->edge  = p_edge;
				pNewNode->next  = NULL;
				pNewNode->nextP = NULL;

				//cerr << "\nxxx A lumped edge added.";
				return;
			} // if 

			pNode = pNode->next;
		} // while
	}

	// Add the new symbol node to the tail of parallel list.
	++SymbN;		// Increase Nsymb
	// This edge is not lumped now.
	p_edge->lumped = 0;
	p_edge->index  = SymbN;		// Assign the index to this edge.

	pNewNode = new SymbNode;
	// Link to the symbol list.
	if (!SymbListTemp)	// The first symbol node.
		SymbListTemp = pTail = pNewNode;
	else			// The second on.
	{
		pTail->next = pNewNode,  pTail = pTail->next;  // Update the tail.
		//pNewNode->next = SymbListTemp;
		//SymbListTemp = pNewNode;
	}

	pNewNode->name  = p_edge->name;
	pNewNode->value = p_edge->value;
	pNewNode->edge  = p_edge;
	// cout << "value is : " << p_edge->value << endl;
	pNewNode->next  = NULL;
	pNewNode->nextP = NULL;
//	pNewNode->edgeindex = edgeid;
	if(p_edge->type > 1)
	{
		pNewNode->isSrc = 1;
		pNewNode->pairedge = &Elist[pNewNode->edge->pnum];
	}
	else
	{
		pNewNode->isSrc = 0;
		pNewNode->pairedge = NULL;
	}
	//cerr << "\n--- New symbol node added.";
	return;
}
void tpdd::Modify_value(double value, int tag, char *name)
{
//	cout << "value is : " << value << "tag is : " << tag << "name is : " << name <<endl;
	SymbNode *pNode;
	if(strcmp(Slist[tag]->name, name) == 0)
	{
		Slist[tag]->value = value;
	//	cout << "no lump.. " << endl;
	}
	else
	{
		pNode = Slist[tag]->nextP;
		for(; pNode; pNode = pNode->nextP)
		{
//			cout << "initial value is : " << pNode->value << endl;
			if(strcmp(pNode->name,name) == 0)
			{
				pNode->value = value;
				break;
			}
		}


	}
		
	pNode = Slist[tag]->nextP;
}
double tpdd::Search_value(int tag, char * name )
{
	double intial_value = 0.0;
        SymbNode *pNode;
        if(strcmp(Slist[tag]->name, name) == 0)
        {
		intial_value = Slist[tag]->value;
        }
	else
        {
                pNode = Slist[tag]->nextP;
                for(; pNode; pNode = pNode->nextP)
                {
                        if(strcmp(pNode->name,name) == 0)
                        {
				intial_value = pNode->value;
                                break;
                        }
                }


        }
	return intial_value;
}
void tpdd::Element_conj(double freq)
{
	SymbNode *pNode;
	double real = 0;
	double imag = 0;
	double value = 0;
	
	
	for (int i = 0; i < SymbN; i++)
        {
		cout << "i is " << i << endl;
		if((Slist[i]->nextP == NULL)&&(Slist[i]->edge->name[0] == 'C'))
		{
			Slist[i]->value = (-1)*Slist[i]->value;
			// cout << "value is : " << Slist[i]->value << endl;
		}
		else if((Slist[i]->nextP == NULL)&&(Slist[i]->edge->name[0] == 'L'))
		{
			Slist[i]->value = (-1)*Slist[i]->value;
			// cout << "value is : " << Slist[i]->value << endl;
                }
		else if ((pNode = Slist[i]->nextP))    // Has lumped edges.
                {
                        real = imag = 0;                // initialize
                        for (; pNode; pNode = pNode->nextP)
                        {
                                value = pNode->value;

                                switch (pNode->edge->name[0])
                                {
                                case 'R':
                                        real += (value == 0? DBL_MAX : 1/value);
                                        break;
                                case 'G':
                                        real += value;
                                        break;
                                case 'L':
					
					imag -= (value == 0? DBL_MAX : 1/(2*PI*freq*value) );
                                        break;
                                case 'C':
                                        imag += (2*PI*freq*value);
                                        break;
                                default:
                                        break;
                                }
                        }
                        // Store the real and imag values.
			cout << "before is : " << imag;
                        Slist[i]->value = real;
                        Slist[i]->imag  = (-1)*imag;
			cout << "after is : " << Slist[i]->imag;
                }
        }
}
void tpdd::SetSymbValue(double freq)
{
	SymbNode	*pNode;
	double		real = 0, imag = 0;		
	double		value;

	for (int i = 0; i < SymbN; i++)
	{
		if ((pNode = Slist[i]->nextP))	// Has lumped edges.
		{
			real = imag = 0;		// initialize
			for (; pNode; pNode = pNode->nextP)
			{
				value = pNode->value;

				switch (pNode->edge->name[0])
				{
				case 'R':
					real += (value == 0? DBL_MAX : 1/value);
					break;
				case 'G':
					real += value;
					break;
				case 'L':
					imag -= (value == 0? DBL_MAX : 1/(2*PI*freq*value) );
					break;
				case 'C':
					imag += (2*PI*freq*value);
					break;
				default:
					break;
				}
			}
			// Store the real and imag values.
			Slist[i]->value = real;
			Slist[i]->imag  = imag;
		}
	}
}

void tpdd::SortSymbList()
{
#if 0	
	int i, n1, n2, j, tmp;

	int* sdegree = new int[N];
	int* rdegree = new int[N];
	memset(sdegree, 0, N * sizeof(int));
	memset(sdegree, 0, N * sizeof(int));
	stag *s_array = new stag[SymbN];
	qsort(Slist + 1, SymbN - 1, sizeof(SymbNode*), compare);

	int *marked = new int[2 * SymbN];
	
	SymbNode **stemp = new SymbNode*[SymbN];

	memset(marked, 0, 2 * SymbN * sizeof(int));

	delete[] rdegree;
	rdegree = 0;
	delete[] sdegree;
	sdegree = 0;
	delete[] s_array;
	s_array = 0;
	delete[] stemp;
	stemp = 0;
	delete[] marked;/**/
	marked = 0;/**/
#endif	
}

void tpdd::PrintSymbols()
{
	if (SymbN == 0)
	{
		cout << "\nEmpty symbol list.";
		return;
	}
	cout << "\n  ";

	cout << setiosflags(ios::right)
		 << setw(10) << "Index"
		 << setw(10) << "Symbol"
		 << setw(10) << "IsSrc"
		 << setw(15) << "Value"  
		 << setw(10) << "Type"
		 << setw(15) << "Composite"
		 << endl;

	SymbNode	*pNode, *pNode2;
	int		n = 1;		// Count the index.

	for (n = 0; n < SymbN; n ++)
	{
		pNode = Slist[n];

		cout << setiosflags(ios::right)
			 << setw(10) << n
			 << setw(10) << pNode->name 
			 << setw(10) << (pNode->isSrc ? "T" : "F")
			 << setw(15) << pNode->value
			 << setw(10) << pNode->edge->type << " " 
						 << (pNode->isSrc ? pNode->pairedge->type : pNode->edge->type);
			 /*<< setw(10) << pNode->edge->node1
			 << setw(10) << pNode->edge->node2;*/

		if ((pNode2 = pNode->nextP))
		{
			cout << setw(15) << "(C)";
			cout << endl << "\t\t   x";
			// Print the lumped symbols
			while (pNode2)
			{
				cout << "->" << pNode2->name;
				pNode2 = pNode2->nextP;
			}
		}
		cout << endl;
	}
}

void tpdd::PrintGraph(ostream &out)
{
	out << "E = " << E << ", N = " << N << endl;
	out << setiosflags(ios::right)
		 << setw(10) << "edge#"
		 << setw(10) << "edge"
		 << setw( 5) << "n1#"
		 << setw( 5) << "n2#"
		 << setw( 6) << "type"
		 << setw(10) << "pair"
		 << setw(20) << "value"
		 << endl;

	for (int e = 0; e < E; e++)
	{
		out << setiosflags(ios::right)
		     << setw(10) << e
		     << setw(10) << Elist[e].name 
			 << setw( 5) << Elist[e].node1
			 << setw( 5) << Elist[e].node2
			 << setw( 6);
	
		switch (Elist[e].type)
		{
			case Y:		
				out << "Y"	<< "          "
					 << setw(20) << Elist[e].value;	
				break;
			case Z:		
				out << "Z"	<< "          "
				  	 << setw(20) << Elist[e].value;	
				break;
			case CC:	
				out << "CC";	
				out << "  ->" << setw(6)  
				     << Elist[Elist[e].pnum].name;
				break;
			case VC:	
				out << "VC";	
				out << "  ->" << setw(6)  
				     << Elist[Elist[e].pnum].name;
				break;
			case CS:	
				out << "CS";	
				out << "  <-" << setw(6)  
				     << Elist[Elist[e].pnum].name
				  	 << setw(20) << Elist[e].value;	
				break;
			case VS:	
				out << "VS";	
				out << "  <-" << setw(6)  
				     << Elist[Elist[e].pnum].name
				  	 << setw(20) << Elist[e].value;	
				break;
			default:
				break;
		}
		out << endl;
	}
}

EdgeInfo* tpdd::InitialLTree(int &signL)
{
	EdgeInfo* pNewLTree;
	int i, j = 0;

	pNewLTree = new EdgeInfo[SymbN + DpenSrcN - VcN];
	signL = 1;

#ifndef SPAN_NON_REC_APPROXIMATE
	for(i = 0; i < E; i ++)
	{
		if(!Elist[i].lumped  && Elist[i].type != VC)
		{
			pNewLTree[j].node1 = Elist[i].node1;
			pNewLTree[j].node2 = Elist[i].node2;
			if(Elist[i].type !=	VC)
			{
				pNewLTree[j].mark = 0;
			}
			else
			{
				pNewLTree[j].mark = 1;
			}
			pNewLTree[j].edgeindex = i;
			pNewLTree[j].isSrc	   = (Elist[i].type > 1 ? 1 : 0);
			j ++;
		}
	}
#else
	for(i = 0; i < E; i ++)
	{
		if(Elist[i].type != VC)
		{
			pNewLTree[j].node1 = Elist[i].node1;
			pNewLTree[j].node2 = Elist[i].node2;
			if(Elist[i].type !=	VC)
			{
				pNewLTree[j].mark = 0;
			}
			else
			{
				pNewLTree[j].mark = 1;
			}
			pNewLTree[j].edgeindex = i;
			pNewLTree[j].isSrc	   = (Elist[i].type > 1 ? 1 : 0);
			j ++;
		}
	}
#endif
	return pNewLTree;
}
	
EdgeInfo* tpdd::InitialRTree(int &signR)
{
	EdgeInfo* pNewRTree;
	int i, j = 0;
	signR = 1;

	pNewRTree = new EdgeInfo[SymbN + DpenSrcN - CsN];

#ifndef SPAN_NON_REC_APPROXIMATE
	for(i = 0; i < E; i ++)
	{
		if(!Elist[i].lumped  && Elist[i].type !=CS)
		{
			pNewRTree[j].node1 = Elist[i].node1;
			pNewRTree[j].node2 = Elist[i].node2;
			if(Elist[i].type !=	CS)
			{
				pNewRTree[j].mark = 0;
			}
			else
			{
				pNewRTree[j].mark = 1;
			}
			pNewRTree[j].edgeindex = i;
			pNewRTree[j].isSrc	   = (Elist[i].type > 1 ? 1 : 0);
			j ++;
		}
	}
#else
	for(i = 0; i < E; i ++)
	{
		if(Elist[i].type !=CS)
		{
			pNewRTree[j].node1 = Elist[i].node1;
			pNewRTree[j].node2 = Elist[i].node2;
			if(Elist[i].type !=	CS)
			{
				pNewRTree[j].mark = 0;
			}
			else
			{
				pNewRTree[j].mark = 1;
			}
			pNewRTree[j].edgeindex = i;
			pNewRTree[j].isSrc	   = (Elist[i].type > 1 ? 1 : 0);
			j ++;
		}
	}
#endif
	return pNewRTree;
}

void tpdd::InitialRoot()
{
	EdgeInfo *pLTree, *pRTree;
	subtree *pSLTree, *pSRTree, *pSubLeftTree, *pSubRightTree;

	int signL = 1; 
	int signR = 1;
	int isHash;
	
	pLTree = InitialLTree(signL);
	pRTree = InitialRTree(signR);

	pSLTree = new subtree(pLTree, SymbN + DpenSrcN - VcN, N, signL);
	pSRTree = new subtree(pRTree, SymbN + DpenSrcN - CsN, N, signR);

	pSubLeftTree	= ShareSubTree(pSLTree);
	pSubRightTree	= ShareSubTree(pSRTree);
	pTpddRoot = new tpddnode(0, 1, 1, pSubLeftTree, pSubRightTree); 

	ShareTpddNode(pTpddRoot, isHash);
	
}


//********************************************************************************/
//	Create Left Tree of the Child node.
//	type = 0 --> short;
//	type = 1 --> out;
//********************************************************************************/

EdgeInfo* tpdd::CreateLTree(int SymbolIndex, int type, tpddnode * pParent, int NumEdges, int NumNodes, int &LeftNodes, int &signL)
{
	EdgeInfo *pNewLTree, *pCurEdgeInfo;
	Edge *pCurEdge, *pCurPairEdge;
	signL = 1;

/***************************************************************************************************************/
//   S... : Controlling sources    Sn1: node1 of Controlling Sources  Sn2: node2 of Controlling Sources
//   C... : Controlled sources	   Cn1: node1 of Controlled Sources   Cn2: node2 of Controlled Sources
//								   Se: Controlling sources' edges     Ce: Controlled sources' edges
/***************************************************************************************************************/

	int isSrc = 0;
	int Stype = 0, Ctype = 0;
	int i = 0, j = 0;
	int tmp = 0;
	int Sn1 = 0, Sn2 = 0, Cn1 = 0, Cn2 = 0, Se = 0, Ce = 0, e = 0, n1 = 0, n2 = 0;
	
	pNewLTree = new EdgeInfo[NumEdges];
	pCurEdge = Slist[SymbolIndex]->edge;

	isSrc = Slist[SymbolIndex]->isSrc;
	LeftNodes = NumNodes;

	// Preprocessing 
	// Prepare the exchange nodes, the operating edges
	signL = 1;
	if(isSrc)
	{	
		pCurPairEdge = Slist[SymbolIndex]->pairedge;
		Stype = pCurEdge->type;
		Ctype = pCurPairEdge->type;
		Se = pCurEdge->edgeindex;
		Ce = pCurPairEdge->edgeindex; 
		for(i = 0; i < pParent->GetNumLeftEdges(); i ++)
		{
			if(pParent->GetLeftTree()[i].edgeindex == Se)
				break;
		}
		Sn1 = pParent->GetLeftTree()[i].node1;  //pCurEdge->node1;
		Sn2 = pParent->GetLeftTree()[i].node2;
		for(i = 0; i < pParent->GetNumLeftEdges(); i ++)
		{
			if(pParent->GetLeftTree()[i].edgeindex == Ce)
				break;
		}
		Cn1 = pParent->GetLeftTree()[i].node1;
		Cn2 = pParent->GetLeftTree()[i].node2;

		if(Stype == VS && Sn1 == Sn2)
		{
			delete[] pNewLTree;
			pNewLTree = 0;
			return NULL;
		}

		if(Sn1 > Sn2)
		{
			tmp = Sn2;
			Sn2 = Sn1;
			Sn1 = tmp;
			signL = signL * -1;
		}
	}
	else
	{
		e = pCurEdge->edgeindex;
		for(i = 0; i < pParent->GetNumLeftEdges(); i ++)
		{
			if(pParent->GetLeftTree()[i].edgeindex == e)
				break;
		}
		n1 = pParent->GetLeftTree()[i].node1;
		n2 = pParent->GetLeftTree()[i].node2;
	}
	
	if(type == 0)						// select this symbol
	{
		if(!isSrc)						// Y/Z edges
		{	
			if(n1 == n2)
			{
				delete []pNewLTree;
				pNewLTree = 0;
				return NULL;
			}
			if(n1 > n2)
			{
				tmp = n2;
				n2 = n1;
				n1 = tmp;
				signL = signL * -1;
			}

			j = 0;
			for(i = 0; i < pParent->GetNumLeftEdges(); i ++)
			{
				pCurEdgeInfo = &pParent->GetLeftTree()[i];
				if(pCurEdgeInfo->edgeindex == e)
				{
					if(GetPermNum(pParent->GetLeftTree(), pParent->GetNumLeftEdges(), n2))
					{
						signL = signL * -1;
					}
					continue;
				}
				if(pCurEdgeInfo->node1 == n2)
					pNewLTree[j].node1 = n1;
				else
					pNewLTree[j].node1 = pCurEdgeInfo->node1;

				if(pCurEdgeInfo->node2 == n2)
					pNewLTree[j].node2 = n1;
				else
					pNewLTree[j].node2 = pCurEdgeInfo->node2;

				pNewLTree[j].edgeindex = pCurEdgeInfo->edgeindex;
				pNewLTree[j].isSrc	   = pCurEdgeInfo->isSrc;
				pNewLTree[j].mark = 0;

				j ++;
			}	
			LeftNodes = NumNodes -1;
		}
		else
		{
			if(Stype == CS && Sn1 == Sn2)				// There exist loops, spanning failure
			{
				delete []pNewLTree;
				pNewLTree = 0;
				return NULL;
			}

			j = 0;
			for(i = 0; i < pParent->GetNumLeftEdges(); i ++)
			{
				pCurEdgeInfo = &pParent->GetLeftTree()[i];
				if(pCurEdgeInfo->edgeindex == Se || pCurEdgeInfo->edgeindex == Ce)
				{
					if(pCurEdgeInfo->edgeindex == Se && (GetPermNum(pParent->GetLeftTree(), pParent->GetNumLeftEdges(), Sn2)))
					{
						signL = signL * -1;
					}
					continue;
				}
				if(pCurEdgeInfo->node1 == Sn2)
					pNewLTree[j].node1 = Sn1;
				else
					pNewLTree[j].node1 = pCurEdgeInfo->node1;

				if(pCurEdgeInfo->node2 == Sn2)
					pNewLTree[j].node2 = Sn1;
				else
					pNewLTree[j].node2 = pCurEdgeInfo->node2;

				pNewLTree[j].edgeindex = pCurEdgeInfo->edgeindex;
				pNewLTree[j].isSrc = pCurEdgeInfo->isSrc;
				pNewLTree[j].mark = 0;
				j ++;
			}
			LeftNodes = NumNodes - 1;
		}
	}
	else						// omit this symbol
	{
		if(!isSrc)				// open Y/Z edges
		{
			j = 0;
			for(i = 0; i < pParent->GetNumLeftEdges(); i ++)
			{
				pCurEdgeInfo = &pParent->GetLeftTree()[i];
				if(pCurEdgeInfo->edgeindex == e)
					continue;
				
				pNewLTree[j].node1 = pCurEdgeInfo->node1;
				pNewLTree[j].node2 = pCurEdgeInfo->node2;
				
				pNewLTree[j].edgeindex = pCurEdgeInfo->edgeindex;
				pNewLTree[j].isSrc = pCurEdgeInfo->isSrc;
				pNewLTree[j].mark = 0;

				j ++;
			}	
		}
		else
		{
			j = 0;
			
			if(Ctype == CC && Cn1 == Cn2)
			{
				delete[] pNewLTree;
				pNewLTree = 0;
				return NULL;
			}

			switch(Stype)
			{
			case VS:				
				switch(Ctype)
				{
				case VC:							// short VS, open VC
					for(i = 0; i < pParent->GetNumLeftEdges(); i ++)
					{
						pCurEdgeInfo = &pParent->GetLeftTree()[i];
						if(pCurEdgeInfo->edgeindex == Se)
						{
						//	if((Sn2 & 0x1))
							if(GetPermNum(pParent->GetLeftTree(), pParent->GetNumLeftEdges(), Sn2))
							{
								signL = signL * -1;
							}
							continue;
						}
						if(pCurEdgeInfo->node1 == Sn2)
							pNewLTree[j].node1 = Sn1;
						else
							pNewLTree[j].node1 = pCurEdgeInfo->node1;

						if(pCurEdgeInfo->node2 == Sn2)
							pNewLTree[j].node2 = Sn1;
						else
							pNewLTree[j].node2 = pCurEdgeInfo->node2;

						pNewLTree[j].edgeindex = pCurEdgeInfo->edgeindex;
						pNewLTree[j].isSrc = pCurEdgeInfo->isSrc;
						pNewLTree[j].mark = 0;
						j ++;

					}
					LeftNodes = NumNodes - 1;
					break;
				case CC:						// short VS, short CC	
					if(Cn1 > Cn2)
					{
						tmp = Cn2;
						Cn2 = Cn1;
						Cn1 = tmp;
						signL = signL * -1;
					}
					if(Cn1 == Sn1 && Cn2 == Sn2)
					{
						delete[] pNewLTree;
						pNewLTree = 0;
						signL = 1;
						return NULL;
					}
					else
						LeftNodes = NumNodes - 2;

					if(Sn2 == Cn2)
					{
						if(Sn1 < Cn1)
						{
							n1 = Sn1;
							n2 = Sn1;
						}
						else
						{
							n1 = Cn1;
							n2 = Cn2;
						}
					}
					else if(Sn2 == Cn1)
					{
						n1 = Sn1;
						n2 = Sn1;
					}
					else if(Sn1 == Cn2)
					{
						n1 = Cn1;
						n2 = Cn1;
					}
					else
					{
						n1 = Sn1;
						n2 = Cn1;
					}

					for(i = 0; i < pParent->GetNumLeftEdges(); i ++)
					{
						pCurEdgeInfo = &pParent->GetLeftTree()[i];
						if(pCurEdgeInfo->edgeindex == Se) 
						{
							if(GetPermNum(pParent->GetLeftTree(), pParent->GetNumLeftEdges(), Sn2))
								signL = signL * -1;
							continue;
						}
						else if(pCurEdgeInfo->edgeindex == Ce)
						{
							if(GetPermNum(pParent->GetLeftTree(), pParent->GetNumLeftEdges(), Cn2))
								signL = signL * -1;
							continue;
						}
						if(pCurEdgeInfo->node1 == Sn2 || pCurEdgeInfo->node1 == Sn1)
							pNewLTree[j].node1 = n1;
						else
							pNewLTree[j].node1 = pCurEdgeInfo->node1;

						if(pCurEdgeInfo->node2 == Sn2 || pCurEdgeInfo->node2 == Sn1)
							pNewLTree[j].node2 = n1;
						else
							pNewLTree[j].node2 = pCurEdgeInfo->node2;

						pNewLTree[j].edgeindex = pCurEdgeInfo->edgeindex;
						pNewLTree[j].isSrc = pCurEdgeInfo->isSrc;
						pNewLTree[j].mark = 0;
						j ++;

					}

					for(i = 0; i < NumEdges; i ++)
					{
						pCurEdgeInfo = &pNewLTree[i];
						if(pCurEdgeInfo->node1 == Cn2 || pCurEdgeInfo->node1 == Cn1)
							pCurEdgeInfo->node1 = n2;
						
						if(pCurEdgeInfo->node2 == Cn2 || pCurEdgeInfo->node2 == Cn1)
							pCurEdgeInfo->node2 =  n2;
					}

					break;
				default:
					delete[] pNewLTree;
					pNewLTree = 0;
					return NULL;
				}
				break;
			case CS:							
				switch(Ctype)
				{
				case VC:								// open CS, open VC
					for(i = 0; i < pParent->GetNumLeftEdges(); i ++)
					{
						pCurEdgeInfo = &pParent->GetLeftTree()[i];
						if(pCurEdgeInfo->edgeindex == Se)
							continue;
						
						pNewLTree[j].node1 = pCurEdgeInfo->node1;
						pNewLTree[j].node2 = pCurEdgeInfo->node2;
						pNewLTree[j].edgeindex = pCurEdgeInfo->edgeindex;
						pNewLTree[j].isSrc = pCurEdgeInfo->isSrc;
						pNewLTree[j].mark = 0;
						j ++;
					}
					signL = 1;
					break;
				case CC:								// open CS, short CC
					if(Cn1 > Cn2)
					{
						tmp = Cn2;
						Cn2 = Cn1;
						Cn1 = tmp;
						signL = -1;
					}
					else
					{
						signL = 1;
					}

					for(i = 0; i < pParent->GetNumLeftEdges(); i ++)
					{
						pCurEdgeInfo = &pParent->GetLeftTree()[i];
						if(pCurEdgeInfo->edgeindex == Se || pCurEdgeInfo->edgeindex == Ce)
						{
							if(pCurEdgeInfo->edgeindex == Ce && (GetPermNum(pParent->GetLeftTree(), pParent->GetNumLeftEdges(), Cn2)))
								signL = signL * -1;
							continue;
						}
						if(pCurEdgeInfo->node1 == Cn2)
							pNewLTree[j].node1 = Cn1;
						else
							pNewLTree[j].node1 = pCurEdgeInfo->node1;

						if(pCurEdgeInfo->node2 == Cn2)
							pNewLTree[j].node2 = Cn1;
						else
							pNewLTree[j].node2 = pCurEdgeInfo->node2;

						pNewLTree[j].edgeindex = pCurEdgeInfo->edgeindex;
						pNewLTree[j].isSrc = pCurEdgeInfo->isSrc;
						pNewLTree[j].mark = 0;
						j ++;

					}
					LeftNodes = NumNodes - 1;
					break;
				default:
					delete[] pNewLTree;
					pNewLTree = 0;
					return NULL;
				}
				break;
			default:
				delete[] pNewLTree;
				pNewLTree = 0;
				return NULL;
			}
		}
	}

	return pNewLTree;
}

EdgeInfo* tpdd::CreateRTree(int SymbolIndex, int type, tpddnode * pParent, int NumEdges, int NumNodes, int &RightNodes, int& signR)
{
	EdgeInfo *pNewRTree, *pCurEdgeInfo;
	Edge *pCurEdge, *pCurPairEdge;

/***************************************************************************************************************/
//   S... : Controlling sources    Sn1: node1 of Controlling Sources  Sn2: node2 of Controlling Sources
//   C... : Controlled sources	   Cn1: node1 of Controlled Sources   Cn2: node2 of Controlled Sources
//								   Se: Controlling sources' edges     Ce: Controlled sources' edges
/***************************************************************************************************************/

	int isSrc = 0;
	int Stype = 0, Ctype = 0;
	int i = 0, j = 0;
	int	Sn1 = 0, Sn2 = 0, Cn1 = 0, Cn2 = 0, Se = 0, Ce = 0, e = 0, n1 = 0, n2 = 0;
	int tmp = 0;
	
	pNewRTree = new EdgeInfo[NumEdges];
	pCurEdge = Slist[SymbolIndex]->edge;

	isSrc = Slist[SymbolIndex]->isSrc;
	RightNodes = NumNodes;
	signR = 1;

	// Preprocessing 
	// Prepare the exchange nodes, the operating edges
	if(isSrc)
	{	
		pCurPairEdge = Slist[SymbolIndex]->pairedge;
		Stype = pCurEdge->type;
		Ctype = pCurPairEdge->type;
		Se = pCurEdge->edgeindex;
		Ce = pCurPairEdge->edgeindex; 
		
		for(i = 0; i < pParent->GetNumRightEdges(); i ++)
		{
			if(pParent->GetRightTree()[i].edgeindex == Se)
				break;
		}
		Sn1 = pParent->GetRightTree()[i].node1;  //pCurEdge->node1;
		Sn2 = pParent->GetRightTree()[i].node2;
		for(i = 0; i < pParent->GetNumRightEdges(); i ++)
		{
			if(pParent->GetRightTree()[i].edgeindex == Ce)
				break;
		}
		Cn1 = pParent->GetRightTree()[i].node1;
		Cn2 = pParent->GetRightTree()[i].node2;

		if(Cn1 > Cn2)
		{
			tmp = Cn2;
			Cn2 = Cn1;
			Cn1 = tmp;
			signR = signR * -1;
		}

		if(Ctype == CC && Cn1 == Cn2)				// There exist loops, spanning failure
		{
			delete []pNewRTree;
			pNewRTree = 0;
			return NULL;
		}
	}
	else
	{
		e = pCurEdge->edgeindex;
		for(i = 0; i < pParent->GetNumRightEdges(); i ++)
		{
			if(pParent->GetRightTree()[i].edgeindex == e)
				break;
		}
		n1 = pParent->GetRightTree()[i].node1;  //pCurEdge->node1;
		n2 = pParent->GetRightTree()[i].node2;
	}

	if(type == 0)						// select this symbol
	{
		RightNodes = NumNodes - 1;
		if(!isSrc)						// Y/Z edges
		{	
			if(n1 == n2)
			{
				delete []pNewRTree;
				pNewRTree = 0;
				return NULL;
			}
			if(n1 > n2)
			{
				tmp = n2;
				n2 = n1;
				n1 = tmp;
				signR = signR * -1;
			}
			j = 0;
			for(i = 0; i < pParent->GetNumRightEdges(); i ++)
			{
				pCurEdgeInfo = &pParent->GetRightTree()[i];
				if(pCurEdgeInfo->edgeindex == e)
				{
					if(GetPermNum(pParent->GetRightTree(), pParent->GetNumRightEdges(), n2))
						signR = signR * -1;
					continue;
				}

				if(pCurEdgeInfo->node1 == n2)
					pNewRTree[j].node1 = n1;
				else
					pNewRTree[j].node1 = pCurEdgeInfo->node1;

				if(pCurEdgeInfo->node2 == n2)
					pNewRTree[j].node2 = n1;
				else
					pNewRTree[j].node2 = pCurEdgeInfo->node2;
				pNewRTree[j].edgeindex = pCurEdgeInfo->edgeindex;
				pNewRTree[j].isSrc = pCurEdgeInfo->isSrc;
				pNewRTree[j].mark = 0;

				j ++;
			}	
		}
		else
		{
			if(Ctype == VC && Cn1 == Cn2)
			{
				delete []pNewRTree;
				pNewRTree = 0;
				return NULL;
			}		
			j = 0;
			for(i = 0; i < pParent->GetNumRightEdges(); i ++)
			{
				pCurEdgeInfo = &pParent->GetRightTree()[i];
				if(pCurEdgeInfo->edgeindex == Se || pCurEdgeInfo->edgeindex == Ce)
				{
					if(GetPermNum(pParent->GetRightTree(), pParent->GetNumRightEdges(), Cn2) && pCurEdgeInfo->edgeindex == Ce)
					{
						signR = signR * -1;
					}
					continue;
				}
				if(pCurEdgeInfo->node1 == Cn2)
					pNewRTree[j].node1 = Cn1;
				else
					pNewRTree[j].node1 = pCurEdgeInfo->node1;

				if(pCurEdgeInfo->node2 == Cn2)
					pNewRTree[j].node2 = Cn1;
				else
					pNewRTree[j].node2 = pCurEdgeInfo->node2;

				pNewRTree[j].edgeindex = pCurEdgeInfo->edgeindex;
				pNewRTree[j].isSrc = pCurEdgeInfo->isSrc;
				pNewRTree[j].mark = 0;
				j ++;
			}
		}
	}
	else
	{
		if(Stype == VS && Sn1 == Sn2)
		{
			delete[] pNewRTree;
			pNewRTree = 0;
			return NULL;
		}
		if(!isSrc)				// open Y/Z edges
		{
			j = 0;
			for(i = 0; i < pParent->GetNumRightEdges(); i ++)
			{
				pCurEdgeInfo = &pParent->GetRightTree()[i];
				if(pCurEdgeInfo->edgeindex == e)
					continue;
				
				pNewRTree[j].node1 = pCurEdgeInfo->node1;
				pNewRTree[j].node2 = pCurEdgeInfo->node2;
				
				pNewRTree[j].edgeindex = pCurEdgeInfo->edgeindex;
				pNewRTree[j].isSrc = pCurEdgeInfo->isSrc;
				pNewRTree[j].mark = 0;

				j ++;
			}	
		}
		else{
			j = 0;
			switch(Stype)
			{
			case VS:				
				switch(Ctype)
				{
				case VC:							// short VS, open VC
					if(Sn1 > Sn2)
					{
						tmp = Sn2;
						Sn2 = Sn1;
						Sn1 = tmp;
						signR = -1;
					}
					else
					{
						signR = 1;
					}
					for(i = 0; i < pParent->GetNumRightEdges(); i ++)
					{
						pCurEdgeInfo = &pParent->GetRightTree()[i];
						if(pCurEdgeInfo->edgeindex == Se || pCurEdgeInfo->edgeindex == Ce)
						{
							if(GetPermNum(pParent->GetRightTree(), pParent->GetNumRightEdges(), Sn2) && pCurEdgeInfo->edgeindex == Se)
								signR = signR * -1;
							continue;
						}
						if(pCurEdgeInfo->node1 == Sn2)
							pNewRTree[j].node1 = Sn1;
						else
							pNewRTree[j].node1 = pCurEdgeInfo->node1;

						if(pCurEdgeInfo->node2 == Sn2)
							pNewRTree[j].node2 = Sn1;
						else
							pNewRTree[j].node2 = pCurEdgeInfo->node2;

						pNewRTree[j].edgeindex = pCurEdgeInfo->edgeindex;
						pNewRTree[j].isSrc = pCurEdgeInfo->isSrc;
						pNewRTree[j].mark = 0;
						j ++;

					}
					RightNodes = NumNodes - 1;
					break;
				case CC:						// short VS, short CC
					if(Sn1 > Sn2)
					{
						tmp = Sn2;
						Sn2 = Sn1;
						Sn1 = tmp;
						signR = signR * -1;
					}
					if(Cn1 == Sn1 && Cn2 == Sn2)
					{
						delete[] pNewRTree;
						pNewRTree = 0;
						signR = 1;
						return NULL;
						RightNodes = NumNodes - 1;
					}
					else
						RightNodes = NumNodes - 2;

					if(Sn2 == Cn2)
					{
						if(Sn1 < Cn1)
						{
							n1 = Sn1;
							n2 = Sn1;
						}
						else
						{
							n1 = Cn1;
							n2 = Cn2;
						}
					}
					else if(Sn2 == Cn1)
					{
						n1 = Sn1;
						n2 = Sn1;
					}
					else if(Sn1 == Cn2)
					{
						n1 = Cn1;
						n2 = Cn1;
					}
					else
					{
						n1 = Sn1;
						n2 = Cn1;
					}

					for(i = 0; i < pParent->GetNumRightEdges(); i ++)
					{
						pCurEdgeInfo = &pParent->GetRightTree()[i];
						if(pCurEdgeInfo->edgeindex == Se)
						{
							if(GetPermNum(pParent->GetRightTree(), pParent->GetNumRightEdges(), Sn2))
								signR = signR * -1;
							continue;
						}
						else if(pCurEdgeInfo->edgeindex == Ce)
						{
						//	if(Cn2 & 0x1)
							if(GetPermNum(pParent->GetRightTree(), pParent->GetNumRightEdges(), Cn2))
								signR = signR * -1;
							continue;
						}
						if(pCurEdgeInfo->node1 == Sn2 || pCurEdgeInfo->node1 == Sn1)
							pNewRTree[j].node1 = n1;
						else
							pNewRTree[j].node1 = pCurEdgeInfo->node1;

						if(pCurEdgeInfo->node2 == Sn2 || pCurEdgeInfo->node2 == Sn1 )
							pNewRTree[j].node2 = n1;
						else
							pNewRTree[j].node2 = pCurEdgeInfo->node2;

						pNewRTree[j].edgeindex = pCurEdgeInfo->edgeindex;
						pNewRTree[j].isSrc = pCurEdgeInfo->isSrc;
						pNewRTree[j].mark = 0;
						j ++;

					}

					for(i = 0; i < NumEdges; i ++)
					{
						pCurEdgeInfo = &pNewRTree[i];
						if(pCurEdgeInfo->node1 == Cn2 || pCurEdgeInfo->node1 == Cn1)
							pCurEdgeInfo->node1 = n2;
						
						if(pCurEdgeInfo->node2 == Cn2 || pCurEdgeInfo->node2 == Cn1)
							pCurEdgeInfo->node2 = n2;
					}
					break;
				default:
					delete[] pNewRTree;
					pNewRTree = 0;
					return NULL;
				}
				break;
			case CS:							
				switch(Ctype)
				{
				case VC:								// open CS, open VC
					for(i = 0; i < pParent->GetNumRightEdges(); i ++)
					{
						pCurEdgeInfo = &pParent->GetRightTree()[i];
						if(pCurEdgeInfo->edgeindex == Ce)
							continue;
						
						pNewRTree[j].node1 = pCurEdgeInfo->node1;
						pNewRTree[j].node2 = pCurEdgeInfo->node2;
						pNewRTree[j].edgeindex = pCurEdgeInfo->edgeindex;
						pNewRTree[j].isSrc = pCurEdgeInfo->isSrc;
						pNewRTree[j].mark = 0;
						j ++;
					}
				    signR = 1;
					break;
				case CC:								// open CS, short CC
					for(i = 0; i < pParent->GetNumRightEdges(); i ++)
					{
						pCurEdgeInfo = &pParent->GetRightTree()[i];
						if(pCurEdgeInfo->edgeindex == Ce)
						{
						//	if(Cn2 & 0x1)
							if(GetPermNum(pParent->GetRightTree(), pParent->GetNumRightEdges(), Cn2))
								signR = signR * -1;
							continue;
						}
						if(pCurEdgeInfo->node1 == Cn2)
							pNewRTree[j].node1 = Cn1;
						else
							pNewRTree[j].node1 = pCurEdgeInfo->node1;

						if(pCurEdgeInfo->node2 == Cn2)
							pNewRTree[j].node2 = Cn1;
						else
							pNewRTree[j].node2 = pCurEdgeInfo->node2;

						pNewRTree[j].edgeindex = pCurEdgeInfo->edgeindex;
						pNewRTree[j].isSrc = pCurEdgeInfo->isSrc;
						pNewRTree[j].mark = 0;
						j ++;

					}
					RightNodes = NumNodes - 1;
					break;
				default:
					delete[] pNewRTree;
					pNewRTree = 0;
					return NULL;
				}
				break;
			default:
				delete[] pNewRTree;
				pNewRTree = 0;
				return NULL;
			}
		}
	}
	return pNewRTree;
}

tpddnode * tpdd::CreateNode(int SymbolIndex, int type, tpddnode * pParent)
{
	tpddnode * pNewNode;
	EdgeInfo *pLTreeEdge, *pRTreeEdge;
	subtree *pSLTree, *pSRTree, *pSubLeftTree, *pSubRightTree;
	SymbNode * pSymb = Slist[SymbolIndex - 1];
	int EnL, EnR, NnL, NnR, tag = 0, signL = 1, signR = 1, sign;

	if(SymbolIndex > SymbN)
		return NULL;

	if(pSymb->isSrc && pSymb->pairedge->type == CC)
		EnL = pParent->GetNumLeftEdges() - 2;
	else
		EnL = pParent->GetNumLeftEdges() - 1;

	if(pSymb->isSrc && pSymb->edge->type == VS)
		EnR = pParent->GetNumRightEdges() - 2;
	else
		EnR = pParent->GetNumRightEdges() - 1;

	if(Slist[SymbolIndex - 1]->edge->type == VS && type == 0)
		sign = -1;
	else
		sign = 1;


	pLTreeEdge	= CreateLTree(SymbolIndex - 1, type, pParent, EnL, pParent->GetNumLeftNodes(), NnL, signL);

 	if(!pLTreeEdge)
	{
		return pZero;
	}

	if(NnL > GetValidEdges(pLTreeEdge, EnL) + 1 || !DetectTree(pLTreeEdge, EnL, NnL))
	{
		delete[] pLTreeEdge;
		pLTreeEdge = 0;
		return pZero;
	}	

	if(NnL == 1)
	{
		if(CheckValidTree(pLTreeEdge, EnL, NnL))
			tag = 1;
		else
		{
			delete[] pLTreeEdge;
			pLTreeEdge = 0;
			return pZero;
		}
	}
	

	pRTreeEdge = CreateRTree(SymbolIndex - 1, type, pParent, EnR, pParent->GetNumRightNodes(), NnR, signR);

	if(!pRTreeEdge)
	{
		delete[] pLTreeEdge;
		pLTreeEdge = 0;
		return pZero;
	}

		if(NnR > GetValidEdges(pRTreeEdge, EnR) + 1 || !DetectTree(pRTreeEdge, EnR, NnR))
	{
		delete[] pLTreeEdge;
		pLTreeEdge = 0;
		delete[] pRTreeEdge;
		pRTreeEdge = 0;
		return pZero;
	}

	if(NnR == 1)
	{
		if(CheckValidTree(pRTreeEdge, EnR, NnR) && tag)
		{
			sign = signL * signR * sign;
			if(sign == -1 && type == 0)
			{
				pParent->SetSignS(sign);
			}
			else if (sign == -1 && type == 1)
			{
				pParent->SetSignO(sign);
			}
			delete[] pLTreeEdge;
			pLTreeEdge = 0;
			delete[] pRTreeEdge; 
			pRTreeEdge = 0;
			return pOne;
		}
		else
		{
			delete[] pLTreeEdge;
			pLTreeEdge = 0;
			delete[] pRTreeEdge;
			pRTreeEdge = 0;
			return pZero;
		}
	}


	sign = signL * signR * sign;

	pSLTree = new subtree(pLTreeEdge, EnL, NnL, signL);
	pSRTree = new subtree(pRTreeEdge, EnR, NnR, signR);

	pSubLeftTree	= ShareSubTree(pSLTree);
	pSubRightTree	= ShareSubTree(pSRTree);

	pNewNode = new tpddnode(SymbolIndex, 1, 1, pSubLeftTree, pSubRightTree);
	
	if(sign == -1 && type == 0)
	{
		pParent->SetSignS(sign);
	}
	else if (sign == -1 && type == 1)
	{
		pParent->SetSignO(sign);
	}
	return pNewNode;
}

int tpdd::GetNodeSize(EdgeInfo * pEdgeInfo, int e_size)
{
	int i, n1, n2, totalnodes;
	memset(NodeMark, 0, N * sizeof(int));
	
	for(i = 0; i < e_size; i ++)
	{
		n1 = pEdgeInfo[i].node1;
		n2 = pEdgeInfo[i].node2;
		NodeMark[n1] = 1;
		NodeMark[n2] = 1;
	}
	
	totalnodes = 0;

	for(i = 0; i < N; i ++)
	{
		if(NodeMark[i])
			totalnodes ++;
	}

	return totalnodes;
}

void tpdd::MintySpanR()
{
	tpddnode *pOpen, *pShort;
	if(N > E + 1)
	{
		pTpddRoot = NULL;
		return;
	}
	spanningN = 0;
	subTreeSharingTimes = 0;
	tpddNodeSharingTimes = 0;


	InitialRoot();
	
	pShort = MintySpanS(pTpddRoot);
	pOpen = MintySpanO(pTpddRoot);

	pTpddRoot->SetPShort(pShort);
	pTpddRoot->SetPOpen(pOpen);

}
	
tpddnode* tpdd::MintySpanO(tpddnode * pParent)
{
	tpddnode * pNewNode, *pOpen, *pShort, *pShareNode;
	int isHash;

	pNewNode = CreateNode(pParent->GetSymbIndex() + 1, 1, pParent);
	
	spanningN ++;
	if(pNewNode == pZero || pNewNode == pOne || !pNewNode)
		return pNewNode;

	pShareNode = ShareTpddNode(pNewNode, isHash);
	if(isHash)
	{
		return pShareNode;
	}
	
	pShort = MintySpanS(pNewNode);

	pOpen = MintySpanO(pNewNode);

	pNewNode->SetPShort(pShort);
	pNewNode->SetPOpen(pOpen);
	
	return pNewNode;

}
	
tpddnode* tpdd::MintySpanS(tpddnode * pParent)
{
	tpddnode * pNewNode, *pOpen, *pShort, *pShareNode;
	int isHash;
	pNewNode = CreateNode(pParent->GetSymbIndex() + 1, 0, pParent);

	spanningN ++;
	if(pNewNode == pZero || pNewNode == pOne || !pNewNode)
		return pNewNode;
	
	pShareNode = ShareTpddNode(pNewNode, isHash);
	if(isHash)
	{
		return pShareNode;
	}
	
	pShort = MintySpanS(pNewNode);

	pOpen = MintySpanO(pNewNode);
	
	pNewNode->SetPShort(pShort);
	pNewNode->SetPOpen(pOpen);

	return pNewNode;
}

void tpdd::PrintAllNodes(ostream &out)
{
	UnMarkAllNodes(pTpddRoot, 1);
	totalTpddNodes = 0;

	PrintAllNodesR(out, pTpddRoot);
}
	
void tpdd::PrintAllNodesR(ostream &out, tpddnode *p)
{
	if(p->Marked())
		return;
	p->Mark();

	p->PrintTpddNode(out);

	if(p->GetPShort())
		PrintAllNodesR(out, p->GetPShort());
	if(p->GetPOpen())
		PrintAllNodesR(out, p->GetPOpen());
}
	
void tpdd::UnMarkAllNodes(Tnode* p, int type)
{
//	UnMarkNodeR(pTpddRoot);

	stack<Tnode*> *S = new stack<Tnode*>;

	tpddnode* pTopNode;
	sexpTpddNode* peTopNode;


	S->push(p);

	if(type == 1)
	{
		pOne->UnMark();
		pZero->UnMark();
		while(!S->empty())
		{
			pTopNode = (tpddnode*)S->top();
			if(!pTopNode->Marked())
			{
				S->pop();
			}
			else
			{
				if(pTopNode->GetPShort()->Marked())
					S->push(pTopNode->GetPShort());
				else if(pTopNode->GetPOpen()->Marked())
					S->push(pTopNode->GetPOpen());
				else
				{
					pTopNode->UnMark();
				}
			}
		}
	}	
	else
	{
		pSExpOne->UnMark();
		pSExpZero->UnMark();
		while(!S->empty())
		{
			peTopNode = (sexpTpddNode*)S->top();
			if(!peTopNode->Marked())
			{
				S->pop();
			}
			else
			{
				if(peTopNode->GetPShort()->Marked())
					S->push(peTopNode->GetPShort());
				else if(peTopNode->GetPOpen()->Marked())
					S->push(peTopNode->GetPOpen());
				else
				{
					peTopNode->UnMark();
				}
			}
		}
	}
}
	
void tpdd::UnMarkNodeR(tpddnode *p)
{
	if(!p->Marked())
		return;

	p->UnMark();

	if(p->GetPShort())
	{
		UnMarkNodeR(p->GetPShort());
	}
	if(p->GetPOpen())
	{
		UnMarkNodeR(p->GetPOpen());
	}
}

int tpdd::CheckValidTree(EdgeInfo *pEdgeInfo, int e_size, int n_size)
{
	int i, edgeid;
	for(i = 0; i < e_size; i ++)
	{	
		edgeid = pEdgeInfo[i].edgeindex;
		if(Elist[edgeid].type == VS || Elist[edgeid].type == CC)
			return 0;
	}

	return 1;
}

void tpdd::PrintPaths(ostream &out)
{
	if(!PathArray && SymbN > 0)
	{
		ASSERT((PathArray = new int[SymbN]), "Memory Allocation Failure!");
	}
	else 
		return;


	
	if(pTpddRoot->GetSymbIndex() < 0)
		return;
	TermN	= 0;
	TermSign	= 1;
	minusN = 0;
	plusN = 0;

	PrintPathsR(out, pTpddRoot);

	delete[] PathArray;
	PathArray = 0;
}

void tpdd::PrintPathsR(ostream &out, tpddnode *p)
{
	int sign = 1, i;
	if(p->GetSymbIndex() == -1)
	{

#if 1
		for(i = 0; i < StackTop; i ++)
		{
			out << PathArray[i] << " ";
		}

		out << "term: sign " << TermSign << "       ";	
		if(TermSign < 0)
		{
			out << "-";
			minusN ++;
		}
		else
		{
			out << "+";
			plusN ++;
		}

		for(i = 0; i < StackTop; i ++)
		{
			if(Slist[PathArray[i]]->edge->type == Z)
			{ 
				out << "(";
				out << Slist[PathArray[i]]->edge->name;
				out << ").";
			}
			else
				out << Slist[PathArray[i]]->edge->name << ".";
		}
		out << endl;	
#else
	    if(TermSign < 0)
		{
			minusN ++;
		}
		else
		{
			plusN ++;
		}
#endif

		TermN ++;
		return;
	}
	
	if(p->GetSymbIndex()< 0)
		return;
	sign = TermSign;
	PathArray[StackTop ++]	= p->GetSymbIndex();

	TermSign *= p->GetSignS();	
	PrintPathsR(out, p->GetPShort());

	TermSign = sign * p->GetSignO();
	StackTop --;
	PrintPathsR(out, p->GetPOpen());
	TermSign = sign;
}

int tpdd::GetPermNum(EdgeInfo *pEdgeInfo, int e_size, int n)
{
	memset(PermMark, 0, N * sizeof(int));


	int i, permN = 0;

	for(i = 0; i < e_size; i ++)
	{
		if(pEdgeInfo[i].node1 < n && !PermMark[pEdgeInfo[i].node1])
		{
			PermMark[pEdgeInfo[i].node1] = 1;
			permN ++;
		}
		if(pEdgeInfo[i].node2 < n && !PermMark[pEdgeInfo[i].node2])
		{
			PermMark[pEdgeInfo[i].node2] = 1;
			permN ++;
		}
	}

	return (permN & 0x1);
}

subtree* tpdd::ShareSubTree(subtree *&p)
{
	int hashindex;
	subtree *q;


	hashindex = p->HashTreeID() % SUBTREE_PRIME;

	if(!uniqSubTree[hashindex])
	{
		uniqSubTree[hashindex] = new HasherTree();
	}

	if((q = (subtree*) uniqSubTree[hashindex]->Get(p)))
	{

		delete p;
		p = 0;
		subTreeSharingTimes ++;
		return q;
	}
	else
	{
		uniqSubTree[hashindex]->Put(*p, p);
	
	}

	return p;

}

tpddnode *tpdd::ShareTpddNode(tpddnode *&p, int& tag)
{
	int hashindex, level;
	tpddnode *q;
	level = p->GetSymbIndex() % TPDD_PRIME_LEVEL;
	hashindex = p->HashTreeID();
	 
	if(!uniqTpdd[level][hashindex])
	{
		uniqTpdd[level][hashindex] = new HasherTree();
	}
	if((q = (tpddnode *) uniqTpdd[level][hashindex]->Get(p)))
	{
		delete p;
		p = 0;
		tpddNodeSharingTimes ++;
		tag = 1;
		return q;
	}
	else
	{
		uniqTpdd[level][hashindex]->Put(*p, p);
	}
	
	tag = 0;
	return p;
}

void tpdd::RemoveshareTpddNode(tpddnode *p)
{
	int hashindex, level;
	level = p->GetSymbIndex() % TPDD_PRIME_LEVEL;
	hashindex = p->HashTreeID() % TPDD_PRIME;
	 
	uniqTpdd[level][hashindex]->Remove(p);
}

int tpdd::GetValidEdges(EdgeInfo * pTree, int En)
{

	int i, validedges = 0;
	for(i = 0; i < En;  i ++)
	{
		if(pTree[i].node1 ^ pTree[i].node2)
			validedges ++;
	}

	return validedges;
}


void tpdd::MintySpan()
{
	tpddnode *pCurNode, *pNewNode, *pHashNode;

	stack<tpddnode*> *S = new stack<tpddnode*>;

	totalTpddNodes = 0;
	tpddNodeSharingTimes = 0;
	int isHash = 0;
	
	if(!pTpddRoot)
		return;
	S->push(pTpddRoot);

	while(!S->empty())
	{
		if(spanningN % 100000 == 0)
		// if (1)
		{
			cout << "spanning times: " << spanningN << endl; 
		}
		pCurNode = S->top();

		if(!pCurNode->GetPShort())
		{
			spanningN ++;
			pNewNode = CreateNode(pCurNode->GetSymbIndex() + 1, 0, pCurNode);

	
			if(pNewNode == pZero || pNewNode == pOne || !pNewNode)
			{
				pCurNode->SetPShort(pNewNode);
				continue;
			}
			
			pHashNode = ShareTpddNode(pNewNode, isHash);
			if(!isHash) {
				S->push(pHashNode);
			}
			pCurNode->SetPShort(pHashNode);
		}
		else if(!pCurNode->GetPOpen())
		{
			spanningN ++;
			pNewNode = CreateNode(pCurNode->GetSymbIndex() + 1, 1, pCurNode);
	
			if(pNewNode == pZero || pNewNode == pOne || !pNewNode)
			{
				pCurNode->SetPOpen(pNewNode);
				continue;
			}
			
			pHashNode = ShareTpddNode(pNewNode, isHash);
			if(!isHash)
			{
				S->push(pHashNode);
			}
			pCurNode->SetPOpen(pHashNode);
		}
		else
		{
			totalTpddNodes ++;
			S->pop();
		}
	}

	delete S;
	S = 0;

	totalSubNodes = 0;

	if(uniqSubTree)
	{
		for(int i = 0 ; i < SUBTREE_PRIME ; i ++ )
		{
			if(!uniqSubTree[i])
				continue;
			totalSubNodes += uniqSubTree[i]->Count();
			uniqSubTree[i]->SetDelete(TRUE,FALSE);    
			delete uniqSubTree[i];
			uniqSubTree[i] = NULL;
		}
		delete []uniqSubTree;
		uniqSubTree = NULL;
	}
}

void tpdd::ApproximateMinthSpan()
{
	tpddnode *pCurNode, *pNewNode, *pHashNode;
	int isHash, i, topOrder = 0;

	int SymbNTmp = SymbN;
	SymbNode** SymblistTmp = Slist;
	Slist = SNoLumplist;
	SymbN = TotalSymbN;
		
	InitialRoot(); 
	
	stack<tpddnode*> *S = new stack<tpddnode*>;

	totalTpddNodes = 0;
	tpddNodeSharingTimes = 0;
	
	if(!pTpddRoot)
		return;
	S->push(pTpddRoot);

	while(!S->empty())
	{
		if(spanningN % 100000 == 0)
		{
			cout << "spanning times: " << spanningN << endl; 
		}
		pCurNode = S->top();

		if(!pCurNode->GetPShort())
		{
			spanningN ++;

			pNewNode = CreateNode(pCurNode->GetSymbIndex() + 1, 0, pCurNode);

			if(pCurNode->GetSymbIndex() < SymbN && Slist[pCurNode->GetSymbIndex()]->name[0] == 'C')
				topOrder ++;

//				cout << topOrder << endl;
			if(topOrder >= MAX_APPROX_ORDER && pNewNode != pZero)
			{
				pCurNode->SetPShort(pZero);
				if(pNewNode != pOne)
				{
					delete pNewNode;
					pNewNode = 0;
				}
				continue;
			}
	
			if(pNewNode == pZero || pNewNode == pOne || !pNewNode)
			{
				pCurNode->SetPShort(pNewNode);
				continue;
			}
			
			pHashNode = ShareTpddNode(pNewNode, isHash);
			if(!isHash)
			{
				S->push(pHashNode);
			}

			pCurNode->SetPShort(pHashNode);
		}
		else if(!pCurNode->GetPOpen())
		{
			spanningN ++;

			pNewNode = CreateNode(pCurNode->GetSymbIndex() + 1, 1, pCurNode);

			if(pCurNode->GetSymbIndex() < SymbN && Slist[pCurNode->GetSymbIndex()]->name[0] == 'C')
				topOrder --;
	
			if(pNewNode == pZero || pNewNode == pOne || !pNewNode)
			{
				pCurNode->SetPOpen(pNewNode);
				continue;
			}
			
			pHashNode = ShareTpddNode(pNewNode, isHash);
			if(!isHash)
			{
				S->push(pHashNode);
			}
			pCurNode->SetPOpen(pHashNode);
		}
		else
		{
			totalTpddNodes ++;
			S->pop();
		}
	}

	delete S;
	S = 0;

	totalSubNodes = 0;

	if(uniqSubTree)
	{
		for(i = 0 ; i < SUBTREE_PRIME ; i ++ )
		{
			if(!uniqSubTree[i])
				continue;
			totalSubNodes += uniqSubTree[i]->Count();
			uniqSubTree[i]->SetDelete(TRUE,FALSE);    
			delete uniqSubTree[i];
			uniqSubTree[i] = NULL;
		}
		delete []uniqSubTree;
		uniqSubTree = NULL;
	}

//	cout << "\nStats: " << RunStats();
	cout << endl;

	Reduce();
//	cout << "\nStats: " << RunStats();
	cout << endl;

	GC();

	ReferenceAll();


	SymbN = SymbNTmp;
	Slist = SymblistTmp;
}

void tpdd::CollectNodes()
{
	UnMarkAllNodes(pTpddRoot, 1);
	int i;
	ReduceList = new tpddnode*[SymbN + 2];
	for(i = 0; i < SymbN + 2; i ++)
		ReduceList[i] = NULL;

	ReduceList[0] = pZero;
	ReduceList[1] = pOne;

	pOne->Mark();
	pZero->Mark();

	CollectNodesR(pTpddRoot);
}

void tpdd::CollectNodesR(tpddnode *p)
{
	if(!p || p->Marked())
		return;

	p->Mark();
	p->SetNextPoint(NULL);

	tpddnode * q;
	q = ReduceList[p->GetSymbIndex() + 2];

	p->SetNextPoint(q);	
	ReduceList[p->GetSymbIndex() + 2] = p;
	CollectNodesR(p->GetPShort());
	CollectNodesR(p->GetPOpen());
}

void tpdd::Reduce()
{
	int i, j;
	tpddnode_tag *ptag;
	tpddnode *ps, *po;

	HasherTree *** pht = new HasherTree **[SymbN];

	reduceNodeNum  = 0;

	for(i = 0; i < SymbN; i ++)
	{
		pht[i] = new HasherTree*[TAG_PRIME2];
		for(j = 0; j < TAG_PRIME2; j ++)
			pht[i][j] = NULL;
	}

	pZero->GetTag()->setLRL(0, 0, 1);
	pOne->GetTag()->setLRL(0, 0, 2);
	
	NodeList.push_back(pZero);
	NodeList.push_back(pOne);

	TotalRNodeNum = NodeList.size();

	CollectNodes();
	
	SetNodeLabel(pht);

	if(pTpddRoot != NodeList[TotalRNodeNum - 1])
	{
		cout << "Reduce Error!!!" << endl;
		return;
	}

	for(i = 2; i < TotalRNodeNum; i ++)
	{
		ptag = NodeList[i]->GetTag();

		ps = NodeList[abs(ptag->label_left) - 1];
		po = NodeList[abs(ptag->label_right) - 1];
		NodeList[i]->SetPOpen(po);
		NodeList[i]->SetPShort(ps);
	}
	NodeList.clear();

	for(i = 0; i < SymbN; i ++)
	{
		if(pht[i])
		{
			for(j = 0; j < TAG_PRIME2; j ++)
			{
				if(pht[i][j])
				{
					pht[i][j]->SetDelete(FALSE, FALSE);
					delete pht[i][j];
					pht[i][j] = 0;
				}
			}
			delete[] pht[i];
			pht[i] = 0;
		}
	}
	delete[] pht;
	pht = 0;
}

void tpdd::GC()
{
	int i, j;
	cout << "\nGarbage collecting ... ..." << endl;
	for(i = TPDD_PRIME_LEVEL - 1; i >= 0; i --)
	{
		for(j = TPDD_PRIME - 1; j >= 0; j --)
		{
			if(!uniqTpdd[i][j])
				continue;
			if(uniqTpdd[i][j]->Count() != 0)  // unique[] is a list of HasherTrees
      			doGC(i, j);
		}
	}

}

void tpdd::doGC(int i, int j)
{
	tpddnode *p;

	HasherTree *unique_save = new HasherTree();

	Hasher_ForEach_Object(*uniqTpdd[i][j], key, object) {
		p = (tpddnode *) key;
		if (p->NumReferences()) {
			unique_save->Put(*p, p);	// *** Put ***
		} else {
			ASSERT(!p->NumReferences(), "deleting an unmarked node");
			delete p;	// Node released
			// This node is garbage collected.
		}
	} Hasher_End_ForEach;
	delete uniqTpdd[i][j];
	uniqTpdd[i][j] = unique_save;
}

void tpdd::SetNodeLabel(HasherTree ***ReduceHTree)
{
	int i;
	tpddnode * p;

	for(i = SymbN + 1; i > 1; i --)
	{
		p = ReduceList[i];
		while(p)
		{
			SetNodeLabelR(p, ReduceHTree[i - 2], i - 2);
			p = p->GetNextPoint();
		}
	}	
}

void tpdd::SetNodeLabelR(tpddnode *p, HasherTree **ReduceHTree, int level)
{
	int tagl, tagr, label;
	tagl = p->GetSignS() * p->GetPShort()->GetTag()->label;
	tagr = p->GetSignO() * p->GetPOpen()->GetTag()->label;

	if(abs(tagl) == 1 && abs(tagr) == 1)    // pShort == pOpen == pZero
	{
		p->GetTag()->setLRL(0, 0, 1);
		reduceNodeNum ++;
		return;
	}

	tpddnode_tag *pt = p->GetTag();
	tpddnode_tag *qt;

	pt->setLRL(tagl, tagr, 0);

	level = pt->HashId();

	if(!ReduceHTree[level])
	{
		ReduceHTree[level] = new HasherTree();
	}

	if((qt = (tpddnode_tag*) ReduceHTree[level]->Get(pt)))
	{
		label = qt->label;
		pt->setLRL(tagl, tagr, label);
		reduceNodeNum ++;
	}
	else
	{
		ReduceHTree[level]->Put(*pt, pt);
		NodeList.push_back(p);
		TotalRNodeNum = NodeList.size();
		pt->setLRL(tagl, tagr, TotalRNodeNum);
		p->Reference();
	}

	if(TotalRNodeNum % 10000 == 0)
	{
		cout << "Reduceing... " << TotalRNodeNum << endl;
	}
}

void tpdd::ReduceS()
{
	int i, j;
	tpddnode_tag *ptag;
	tpddnode *ps, *po;

	HasherTree *** pht = new HasherTree **[SymbN];

	for(i = 0; i < SymbN; i ++)
	{
		pht[i] = new HasherTree*[TAG_PRIME2];
		for(j = 0; j < TAG_PRIME2; j ++)
			pht[i][j] = NULL;
	}

	pZero->GetTag()->setLRL(0, 0, 1);
	pOne->GetTag()->setLRL(0, 0, 2);
	
	NodeList.push_back(pZero);
	NodeList.push_back(pOne);

	TotalRNodeNum = NodeList.size();

	CollectNodes();
	
	SetNodeLabelS(pht);

	if(pTpddRoot != NodeList[TotalRNodeNum - 1])
	{
		cout << "Reduce Error!!!" << endl;
		return;
	}

	for(i = 2; i < TotalRNodeNum; i ++)
	{
		ptag = NodeList[i]->GetTag();

		if(ptag->label_left)
		{
			ps = NodeList[abs(ptag->label_left) - 1];
		}
		else
		{
			ps = NULL;
		}
		if(ptag->label_right)
		{
			po = NodeList[abs(ptag->label_right) - 1];
		}
		else
		{
			po = NULL;
		}

		NodeList[i]->SetPOpen(po);
		NodeList[i]->SetPShort(ps);
	}
	

	for(i = 0; i < SymbN; i ++)
	{
		for(j = 0; j < TAG_PRIME2; j ++)
		{
			if(pht[i][j])
			{
				pht[i][j]->SetDelete(FALSE,FALSE);    
				delete pht[i][j];
				pht[i][j] = 0;
			}
		}
		if(pht[i])
		{
			delete[] pht[i];
			pht[i] = 0;
		}
	}
	NodeList.clear();
	delete[] pht;
	pht = 0;
}

void tpdd::GCS()
{
	int i, j;
	cout << "\nGarbage collecting ... ..." << endl;
	for(i = TPDD_PRIME_LEVEL - 1; i >= 0; i --)
	{
		for(j = TPDD_PRIME - 1; j >= 0; j --)
		{
			if(!uniqTpdd[i][j])
				continue;
			if(uniqTpdd[i][j]->Count() != 0)  // unique[] is a list of HasherTrees
      			doGCS(i, j);
		}
	}
}

void tpdd::doGCS(int i, int j)
{
	tpddnode *p;
	unsigned short ref;
 
  	HasherTree *unique_save = new HasherTree();
 
  	Hasher_ForEach_Object(*uniqTpdd[i][j], key, object) {
    	p = (tpddnode *) key;
		ref = p->NumReferences();
    	if (p->NumReferences()) {
      		unique_save->Put(*p, p);	// *** Put ***
    	} else {
      	//	ASSERT(!p->NumReferences(), "deleting an unmarked node");
			if (p->PopMarked())
			{
				delete p;// Node released
						 // This node is garbage collected.
			}
      	
      			
    	}
  	} Hasher_End_ForEach;
 
  	delete uniqTpdd[i][j];
  	uniqTpdd[i][j] = unique_save;
}

void tpdd::SetNodeLabelS(HasherTree ***ReduceHTree)
{
	int i;
	tpddnode * p;

	for(i = SymbN + 1; i > 1; i --)
	{
		p = ReduceList[i];
		while(p)
		{
			SetNodeLabelRS(p, ReduceHTree[i - 2], i - 2);
			p = p->GetNextPoint();
		}
	}	
}

void tpdd::SetNodeLabelRS(tpddnode *p, HasherTree **ReduceHTree, int level)
{
	int tagl = 0, tagr = 0, label;
	if(p->GetPShort())
	{
		tagl = p->GetSignS() * p->GetPShort()->GetTag()->label;
	}
	if(p->GetPOpen())
	{
		tagr = p->GetSignO() * p->GetPOpen()->GetTag()->label;
	}

	if(abs(tagl) == 1 && abs(tagr) == 1)    // pShort == pOpen == pZero
	{
		p->GetTag()->setLRL(0, 0, 1);
		p->UnReference();
		return;
	}

	tpddnode_tag *pt = p->GetTag();
	tpddnode_tag *qt;

	pt->setLRL(tagl, tagr, 0);

	level = pt->HashId();

	if(!ReduceHTree[level])
	{
		ReduceHTree[level] = new HasherTree();
	}

	if((qt = (tpddnode_tag*) ReduceHTree[level]->Get(pt)))
	{
		label = qt->label;
		pt->setLRL(tagl, tagr, label);
		p->UnReference();
	}
	else
	{
		ReduceHTree[level]->Put(*pt, pt);
		NodeList.push_back(p);
		TotalRNodeNum = NodeList.size();
		pt->setLRL(tagl, tagr, TotalRNodeNum);
	}
}

void tpdd::MintySpanSR()			// Reducing while spanning
{
	cout << "MintySpanSR()" << endl;
	stack<tpddnode*> S;

	tpddnode *pCurNode, *pNewNode, *pHashNode;
	int isHash, i, ifreduce = 0;

	if(!pTpddRoot)
		return;
	S.push(pTpddRoot);
	pTpddRoot->PushMark();
	pZero->Reference();
	pOne->Reference();
	pTpddRoot->Reference();

	MaxNumNodes = 20000;

	while(!S.empty())
	{
		if(spanningN % 100000 == 0)
		{
			cout << "spanning times: " << spanningN << endl; 
		}
		pCurNode = S.top();
		if(ifreduce)
		{
			if(!pCurNode->NumReferences())
			{
				S.pop();
				delete pCurNode;
				pCurNode = 0;
				continue;
			}
		}
		if(!pCurNode->GetPShort())
		{
			spanningN ++;
			pNewNode = CreateNode(pCurNode->GetSymbIndex() + 1, 0, pCurNode);
			if(pNewNode == pZero || pNewNode == pOne || !pNewNode)
			{
				pCurNode->SetPShort(pNewNode);
				continue;
			}
			pHashNode = ShareTpddNodeS(pNewNode, isHash);
			if(!isHash)
			{
				pHashNode->Reference();
				pHashNode->PushMark();
				S.push(pHashNode);
			}
			pCurNode->SetPShort(pHashNode);
		}
		else if(!pCurNode->GetPOpen())
		{
			spanningN ++;
			pNewNode = CreateNode(pCurNode->GetSymbIndex() + 1, 1, pCurNode);
			if(pNewNode == pZero || pNewNode == pOne || !pNewNode)
			{
				pCurNode->SetPOpen(pNewNode);
				continue;
			}
			pHashNode = ShareTpddNodeS(pNewNode, isHash);
			if(!isHash)
			{
				pHashNode->Reference();
				pHashNode->PushMark();
				S.push(pHashNode);
			}
			pCurNode->SetPOpen(pHashNode);
		}
		else
		{
			totalTpddNodes ++;
			pCurNode = S.top();
			pCurNode->PopMark();
			S.pop();

			if(!ifreduce)
				ifreduce = 1;

			
			if(totalTpddNodes % MaxNumNodes == 0)
			{
				
				ReduceS();
				GCS();/**/
				MaxNumNodes += TotalRNodeNum;
				cout << "Reducing ... ... " << TotalRNodeNum << endl;
				cout << "stack size : " << S.size() << endl;  
			}
		}
	}
	
	ReduceS();
	GCS();

	for(i = 0 ; i < SUBTREE_PRIME ; i ++ )
	{
		if(!uniqSubTree[i])
			continue;
		uniqSubTree[i]->SetDelete(TRUE,FALSE);    
		delete uniqSubTree[i];
		uniqSubTree[i] = NULL;
	}
	delete[] uniqSubTree;
	uniqSubTree = NULL;
}

void tpdd::CollectNodesS()
{
	UnMarkAllNodes(pTpddRoot, 1);
	int i;
	ReduceList = new tpddnode*[SymbN + 2];
	for(i = 0; i < SymbN + 2; i ++)
		ReduceList[i] = NULL;

	ReduceList[0] = pZero;
	ReduceList[1] = pOne;

	pOne->Mark();
	pZero->Mark();

	CollectNodesRS(pTpddRoot);
}

void tpdd::CollectNodesRS(tpddnode *p)
{
	if(!p || p->Marked())
		return;

	p->Mark();
	if(!p->NumReferences())
		return;

	p->SetNextPoint(NULL);

	tpddnode * q;
	q = ReduceList[p->GetSymbIndex() + 2];

	p->SetNextPoint(q);	
	ReduceList[p->GetSymbIndex() + 2] = p;
	CollectNodesRS(p->GetPShort());
	CollectNodesRS(p->GetPOpen());
}

tpddnode *tpdd::ShareTpddNodeS(tpddnode *&p, int& tag)
{
	int hashindex, level;
	tpddnode *q;
	level = p->GetSymbIndex() % TPDD_PRIME_LEVEL;
	hashindex = p->HashTreeID();
	 
	if(!uniqTpdd[level][hashindex])
	{
		uniqTpdd[level][hashindex] = new HasherTree();
	}
	if((q = (tpddnode *) uniqTpdd[level][hashindex]->Get(p)))		
	{
		if(q->NumReferences())
		{
			delete p;
			p = 0;
			tpddNodeSharingTimes ++;
			tag = 1;
			return q;
		}
		else
		{
			uniqTpdd[level][hashindex]->Remove(q);
			uniqTpdd[level][hashindex]->Put(*p, p);
		}

	}
	else
	{
		uniqTpdd[level][hashindex]->Put(*p, p);
	}
	
	tag = 0;
	return p;
}

void tpdd::CalculateTerms()
{
	if(!pTpddRoot)
		TermN = 0;

	pTpddRoot->Mark();
	UnMarkAllNodes(pTpddRoot, 1);

//	pZero->GetTag()->label = 0;
//	pOne->GetTag()->label = 1/1024;
	pZero->SetTermN(0);
	pOne->SetTermN(1.0);
	pZero->Mark();
	pOne->Mark();

	TermN = CalculateTermsR(pTpddRoot);

}
	
//int tpdd::CalculateTermsR(tpddnode *p)
double tpdd::CalculateTermsR(tpddnode *p)
{
/*	if(!p)
		return 0;
	if(p->Marked())
		return p->GetTag()->label;
	
	int terml, termr;
	terml = CalculateTermsR(p->GetPShort());
	termr = CalculateTermsR(p->GetPOpen());
	p->Mark();

	p->GetTag()->label = terml + termr;
	return terml + termr;*/
	if(!p)
		return 0;
	if(p->Marked())
		return p->GetTermN();
	
	double terml, termr;
	terml = CalculateTermsR(p->GetPShort());
	termr = CalculateTermsR(p->GetPOpen());
	p->Mark();

    p->SetTermN(terml + termr);
	return terml + termr;

}

void tpdd::PrintTransferFunc(ostream &out)
{
	if(!PathArray && SymbN > 0)
	{
		PathArray = new int[SymbN];
	}
	else 
		return;
	
	if(pTpddRoot->GetSymbIndex() < 0)
		return;

	

	if(pTpddRoot->GetPShort())
	{
		TermSign	= -1;
		cout << "The Numerator of the Transfer Function is: " << endl;
		StackTop = 0;
		PrintTransferFuncR(out, pTpddRoot->GetPShort());
	}

	if(pTpddRoot->GetPOpen())
	{
		TermSign	= 1;
		cout << "The Denominator of the Transfer Function is: " << endl;
		StackTop = 0;
		PrintTransferFuncR(out, pTpddRoot->GetPOpen());
	}

	delete[] PathArray;
	PathArray = NULL;
}
	
void tpdd::PrintTransferFuncR(ostream &out, tpddnode *p)
{
	int sign = 1, i;
	if(p->GetSymbIndex() == -1)
	{
		if(TermSign < 0)
		{
			out << "-";
		}
		else
		{
			out << "+";
		}

		for(i = 0; i < StackTop; i ++)
		{
			if(Slist[PathArray[i]]->edge->type == Z)
			{ 
				out << "(";
				out << Slist[PathArray[i]]->edge->name;
				out << ").";
			}
			else
				out << Slist[PathArray[i]]->edge->name << ".";
		}
		out << endl;	
		return;
	}
	
	if(p->GetSymbIndex()< 0)
		return;
	sign = TermSign;
	PathArray[StackTop ++]	= p->GetSymbIndex();

	TermSign *= p->GetSignS();	
	PrintTransferFuncR(out, p->GetPShort());

	TermSign = sign * p->GetSignO();
	StackTop --;
	PrintTransferFuncR(out, p->GetPOpen());
	TermSign = sign;
}

int tpdd::DetectTree(EdgeInfo *pEdgeInfo, int e_size, int n_size)
{
	int i,/* j,*/ nodes = 0;

	if(e_size == 0)
		return 1;

	short *mark = new short[N];

	memset(mark, 0, N * sizeof(short));
	
	for(i = 0; i < e_size; i ++)
	{
		if(!mark[pEdgeInfo[i].node1])
		{
			mark[pEdgeInfo[i].node1] = 1;
			nodes ++;
		}
		if(!mark[pEdgeInfo[i].node2])
		{
			mark[pEdgeInfo[i].node2] = 1;
			nodes ++;
		}
	}
	
	delete[] mark;
	mark = 0;
	if(nodes == n_size)
		return 1;

	return 0;
}

void tpdd::ReferenceAll()
{
	UnReferenceAll(pTpddRoot, 1);

	UnMarkAllNodes(pTpddRoot, 1);

	if(!pTpddRoot)
		return;
	ReferenceR(pTpddRoot);
}

void tpdd::ReferenceR(tpddnode *p)
{
	if(p->Marked())
		return;
	p->Mark();

	tpddnode *pNode;
	
	pNode = p->GetPShort();

	if(pNode != pOne && pNode != pZero)
	{
		pNode->Reference();
		ReferenceR(pNode);
	}

	pNode = p->GetPOpen();
	if(pNode != pOne && pNode != pZero)
	{
		pNode->Reference();
		ReferenceR(pNode);
	}

}

void tpdd::UnReferenceAll(Tnode* p, int type)
{
/*	UnMarkAllNodes(pTpddRoot, 1);

	if(!pTpddRoot)
		return;
	UnReferenceR(pTpddRoot);*/

	UnMarkAllNodes(pTpddRoot, 1);

	stack<Tnode*> *S = new stack<Tnode*>;

	tpddnode* pTopNode;
	sexpTpddNode* peTopNode;


	S->push(p);

	if(type == 1)
	{
		pOne->Mark();
		pZero->Mark();
		while(!S->empty())
		{
			pTopNode = (tpddnode*)S->top();
			if(pTopNode->Marked())
			{
				S->pop();
			}
			else
			{
				if(!pTopNode->GetPShort()->Marked())
					S->push(pTopNode->GetPShort());
				else if(!pTopNode->GetPOpen()->Marked())
					S->push(pTopNode->GetPOpen());
				else
				{
					pTopNode->Mark();
					pTopNode->ZeroReference();
				}
			}
		}
	}
	else
	{
		pSExpOne->Mark();
		pSExpZero->Mark();
		while(!S->empty())
		{
			peTopNode = (sexpTpddNode*)S->top();
			if(peTopNode->Marked())
			{
				S->pop();
			}
			else
			{
				if(!peTopNode->GetPShort()->Marked())
					S->push(peTopNode->GetPShort());
				else if(!peTopNode->GetPOpen()->Marked())
					S->push(peTopNode->GetPOpen());
				else
				{
					peTopNode->Mark();
					peTopNode->ZeroReference();
				}
			}
		}	
	}
}

void tpdd::UnReferenceR(tpddnode *p)
{
	if(p->Marked() || p == pOne || p == pZero)
		return;
	p->Mark();

	p->ZeroReference();

	UnReferenceR(p->GetPShort());

	UnReferenceR(p->GetPOpen());

}

void tpdd::scoeff()
{
/*	if(!pTpddRoot)
		return;

	stack<tpddnode*> S;
	tpddnode *pCurNode;

	TotalSCoeffNodeNum = 2;

	S.push(pTpddRoot);

	while(!S.empty())
	{
		pCurNode = S.top();

		if(!pCurNode->GetPShort()->sCoeffList)
		{
			S.push(pCurNode->GetPShort());
		}
		else if(!pCurNode->GetPOpen()->sCoeffList)
		{
			S.push(pCurNode->GetPOpen());
		}
		else
		{
			scoeffR(pCurNode);
			S.pop();
		}
	}*/
}

void tpdd::scoeffR(tpddnode *p)
{
}

void tpdd::PrintScoeffs(ostream &out)
{
//	vector<scoeffNode*> *pCoeff		= pTpddRoot->GetPShort()->sCoeffList;
//	vector<int> *pPower				= pTpddRoot->GetPShort()->sPowerList;

	// int i;

//	pCoeff							= pTpddRoot->GetPShort()->sCoeffList;
//	pPower							= pTpddRoot->GetPShort()->sPowerList;

	vector<scoeffNode*> *pCoeff		= pExpTpddRoot->pShort->sCoeffList;
	vector<int> *pPower				= pExpTpddRoot->pShort->sPowerList;

	out << "Powers of S variable: " << pPower->size() << endl;

	
	out << "Short " << endl;

	for(unsigned i = 0; i < pCoeff->size(); i ++)
	{
		out << "Powers of S variable: " << dec << (*pPower)[i] << endl;
		PrintScoeffPath(out, ((*pCoeff)[i]), pExpTpddRoot->signS);//
	}


	pCoeff				= pExpTpddRoot->pOpen->sCoeffList;
	pPower				= pExpTpddRoot->pOpen->sPowerList;

	out << "Powers of S variable: " << pPower->size() << endl;
	out << "Open " << endl;

	for(unsigned i = 0; i < pCoeff->size(); i ++)
	{
		out << "Powers of S variable: " << dec << (*pPower)[i] << endl;
		PrintScoeffPath(out, ((*pCoeff)[i]), pExpTpddRoot->signO);//
	}
	
}

void tpdd::PrintScoeffPath(ostream &out, scoeffNode *pscoeffNode, int iSign)
{
	if(!PathArray && SymbN > 0)
	{
		ASSERT((PathArray = new int[SymbN]), "Memory Allocation Failure!");
	}
	else 
		return;

	
	if(pscoeffNode->pNode->symbIndex < 0)
		return;
	TermN		= 0;
	TermSign	= iSign;
	StackTop	= 0;

	PrintScoeffPathR(out, pscoeffNode);

	delete[] PathArray;
	PathArray = NULL;
}

void tpdd::PrintScoeffPathR(ostream &out, scoeffNode *pscoeffNode)
{
	int sign = 1, i;
	if(pscoeffNode->pNode->symbIndex == -1)
	{

		for(i = 0; i < StackTop; i ++)
		{
			out << PathArray[i] << " ";
		}

		out << "term: sign " << TermSign << "       ";	
		if(TermSign < 0)
		{
			out << "-";
			minusN ++;
		}
		else
		{
			out << "+";
			plusN ++;
		}

		for(i = 0; i < StackTop; i ++)
		{
			if(Slist[PathArray[i]]->edge->type == Z)
			{ 
				out << "(";
				out << Slist[PathArray[i]]->edge->name;
				out << ").";
			}
			else
				out << Slist[PathArray[i]]->edge->name << ".";
		}
		out << endl;	

		TermN ++;
		return;
	}
	
	if(pscoeffNode->pNode->symbIndex< 0)
		return;
	sign = TermSign;
	PathArray[StackTop ++]	= pscoeffNode->pNode->symbIndex;

	TermSign *= pscoeffNode->pNode->signS;	
	PrintScoeffPathR(out, pscoeffNode->pShort);

	TermSign = sign * pscoeffNode->pNode->signO;
	StackTop --;
	PrintScoeffPathR(out, pscoeffNode->pOpen);
	TermSign = sign;
}

scoeffNode *tpdd::ShareScoeffNodeNode(scoeffNode *&p, int& tag)
{
#ifdef HASHSCOEFFNODE 
	int hashindex, level;
	scoeffNode *q;
	level = p->pNode->symbIndex % TAG_PRIME1;
	hashindex = p->HashTreeID();
	 

	if(!uniqScoeff[level][hashindex])
	{
		uniqScoeff[level][hashindex] = new HasherTree();
	}
	if((q = (scoeffNode *) uniqScoeff[level][hashindex]->Get(p)))
	{
		delete p;
		p = 0;
	//	tpddNodeSharingTimes ++;
		tag = 1;
		return q;
	}
	else
	{
		uniqScoeff[level][hashindex]->Put(*p, p);
	}
	p->label = TotalSCoeffNodeNum;

//	cout << TotalSCoeffNodeNum << endl;
	TotalSCoeffNodeNum ++;
	tag = 0;
	return p;	
#else
	TotalSCoeffNodeNum ++;
	return p;
#endif

}

void tpdd::sexpand()
{
#ifdef SPAN_NON_REC_APPROXIMATE
	int SymbNTmp = SymbN;
	SymbNode** SymblistTmp = Slist;
	Slist = SNoLumplist;
	SymbN = TotalSymbN;
#endif

	UnMarkAllNodes(pTpddRoot, 1);

	if(!pTpddRoot)
		return;

	pSExpOne			= new sexpTpddNode;
	pSExpZero			= new sexpTpddNode;

	pSExpOne->pTpddNode = pOne;
	pSExpOne->symbIndex = -1;
	pSExpOne->signO     = 1;
	pSExpOne->signS     = 1;
	pSExpOne->pOpen     = NULL;
	pSExpOne->pShort    = NULL;

	pSExpZero->pTpddNode = pZero;
	pSExpZero->symbIndex = -2;
	pSExpZero->signO     = 1;
	pSExpZero->signS     = 1;
	pSExpZero->pOpen     = NULL;
	pSExpZero->pShort    = NULL;


	TotalSExpTpddNodeNum  = 0;
	pExpTpddRoot = sexpandR(pTpddRoot);

#ifdef SPAN_NON_REC_APPROXIMATE
	SymbN = SymbNTmp;
	Slist = SymblistTmp;
#endif
}
	
sexpTpddNode* tpdd::sexpandR(tpddnode* p)
{

	// static int totalExpandedNodes = 0;

	if(p->Marked())
	{
		return p->pSExpNode;
	}
	else
	{
		p->Mark();
	}

	int tag;
	sexpTpddNode *pNewNode = new sexpTpddNode();
	sexpTpddNode *pSharedNode;

	pNewNode->pTpddNode = p;
	pNewNode->symbIndex = p->GetSymbIndex();
	pNewNode->signO     = p->GetSignO();
	pNewNode->signS		= p->GetSignS();

	pSharedNode = ShareSExpTpddNodeS(pNewNode, tag);

	if(tag)
	{
		p->pSExpNode = pSharedNode;
		return pSharedNode;
	}

	sexpTpddNode *pShort, *pOpen;

	if(p->GetPShort() == pOne)
	{
		pShort = pSExpOne;
	}
	else if(p->GetPShort() == pZero)
	{
		pShort = pSExpZero;
	}
	else
	{
		pShort = sexpandR(p->GetPShort());
	}


	if(p->GetPOpen() == pOne)
	{
		pOpen = pSExpOne;
	}
	else if(p->GetPOpen() == pZero)
	{
		pOpen = pSExpZero;
	}
	else
	{
		pOpen  = sexpandR(p->GetPOpen());
	}

	int signS, signO;
	SymbNode *pCurSymb;
	signS = p->GetSignS();
	signO = p->GetSignO();


	if(Slist[p->GetSymbIndex()]->name[0] == '^')
	{
		pSharedNode->signO = 1;
	//	totalExpandedNodes ++ ;
	//	cout << totalExpandedNodes << " " << Slist[p->GetSymbIndex()]->name << endl;
		pSharedNode->pShort = pShort;
		
		pCurSymb = Slist[p->GetSymbIndex()];

		pCurSymb = pCurSymb->nextP->nextP;

		pNewNode = new sexpTpddNode(pShort, pOpen, NULL, pCurSymb->ExpSymbIndex, p->GetSignS(), p->GetSignO());

		pSharedNode->pShort = pShort;
		pSharedNode->pOpen  = pNewNode;

		pCurSymb = pCurSymb->nextP;

		while(pCurSymb)
		{
			pNewNode = new sexpTpddNode(pShort, pSharedNode->pOpen, NULL, pCurSymb->ExpSymbIndex, p->GetSignS(), 1);
			TotalSExpTpddNodeNum ++;
			pSharedNode->pOpen = pNewNode;
			pCurSymb = pCurSymb->nextP;
		}

		p->pSExpNode = pSharedNode;
		return pSharedNode;

	}

	pSharedNode->pShort = pShort;
	pSharedNode->pOpen  = pOpen;

	p->pSExpNode = pSharedNode;
	return pSharedNode;
	
}

sexpTpddNode *tpdd::ShareSExpTpddNodeS(sexpTpddNode *&p, int& tag)
{
	int hashindex, level;
	sexpTpddNode *q;
	level = p->symbIndex % TAG_PRIME1;
	hashindex = p->HashTreeID();
	 
	if(!uniqSexpNode[level][hashindex])
	{
		uniqSexpNode[level][hashindex] = new HasherTree();
	}
	if((q = (sexpTpddNode *) uniqSexpNode[level][hashindex]->Get(p)))
	{
		delete p;
		p = 0;
	//	tpddNodeSharingTimes ++;
		tag = 1;
		return q;
	}
	else
	{
		uniqSexpNode[level][hashindex]->Put(*p, p);
	}
//	p->label = TotalSCoeffNodeNum ++;

	TotalSExpTpddNodeNum ++;

//	cout << TotalSExpTpddNodeNum << endl;
	tag = 0;/**/
	return p;
}

void tpdd::scoeffNew()
{
	if(!pExpTpddRoot)
		return;

	stack<sexpTpddNode*> S;
	sexpTpddNode *pCurNode;

	TotalSCoeffNodeNum = 2;

	// pSCoeffOne and pSCoeffZero will be deleted
	// by the destructors of pSExpOne and pSExpZero.
	pSCoeffOne		= new scoeffNode;
	pSCoeffZero		= new scoeffNode;

	pSCoeffOne->pNode		= pSExpOne;
	pSCoeffOne->pShort		= NULL;
	pSCoeffOne->pOpen		= NULL;
	pSCoeffOne->label		= 1; 
	pSCoeffOne->val_ptr     = new complex_t(1.0);

	pSCoeffZero->pNode		= pSExpZero;
	pSCoeffZero->pShort		= NULL;
	pSCoeffZero->pOpen		= NULL;
	pSCoeffZero->label		= 0;
	pSCoeffZero->val_ptr    = new complex_t(0.0);


	pSExpOne->sCoeffList		= new vector<scoeffNode*>;
	pSExpOne->sPowerList		= new vector<int>;
	pSExpOne->sCoeffList->push_back(pSCoeffOne);
	pSExpOne->sPowerList->push_back(0);

	pSExpZero->sCoeffList		= new vector<scoeffNode*>;
	pSExpZero->sPowerList		= new vector<int>;
	pSExpZero->sCoeffList->push_back(pSCoeffZero);
	pSExpZero->sPowerList->push_back(0);

	pSExpOne->Mark();
	pSExpZero->Mark();

	S.push(pExpTpddRoot);

	while(!S.empty())
	{
		pCurNode = S.top();

		if(!pCurNode->pShort->sCoeffList)
		{
			S.push(pCurNode->pShort);
		}
		else if(!pCurNode->pOpen->sCoeffList)
		{
			S.push(pCurNode->pOpen);
		}
		else
		{
			scoeffRNew(pCurNode);
			S.pop();
		}
	}
}

void tpdd::scoeffRNew(sexpTpddNode * p)
{
	vector<scoeffNode*> *pCoeffShort	= p->pShort->sCoeffList;
	vector<scoeffNode*> *pCoeffOpen		= p->pOpen->sCoeffList;
	
	vector<int> *pPowerShort			= p->pShort->sPowerList;
	vector<int> *pPowerOpen				= p->pOpen->sPowerList;

	p->sCoeffList						= new vector<scoeffNode*>;
	p->sPowerList						= new vector<int>;

	int i, j, tag;

	int shortSize, openSize;

	shortSize							= pPowerShort->size();
	openSize							= pPowerOpen->size();

	scoeffNode *pNewscoeffNode, *pSharescoeffNode;
	// scoeffNode *pZNode = NULL;
	// scoeffNode *pCNode = NULL;
	// scoeffNode *pLNode = NULL;
	// sexpTpddNode *pExpTpddNode;


	char eType = Slist[p->symbIndex]->name[0];
	if(eType == '^')
		eType = Slist[p->symbIndex]->name[1];

#if 1
    switch(eType)
	{
		case 'L':
				for(i = 0, j =0; i < shortSize || j < openSize;)
				{
					if(i < shortSize && j < openSize)
					{
						if((*pPowerShort)[i] < ((*pPowerOpen)[j] + 1))
						{
							p->sPowerList->push_back(((*pPowerShort)[i] - 1));

							pNewscoeffNode			= new scoeffNode;
							pNewscoeffNode->pNode	= p;
							pNewscoeffNode->pOpen	= pSCoeffZero;
							pNewscoeffNode->pShort	= (*pCoeffShort)[i];
							pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
							p->sCoeffList->push_back(pSharescoeffNode);
							i ++;
						}
						else if((*pPowerShort)[i] > ((*pPowerOpen)[j] + 1))
						{
							p->sPowerList->push_back((*pPowerOpen)[j]);

							pNewscoeffNode			= new scoeffNode;
							pNewscoeffNode->pNode	= p;
							pNewscoeffNode->pOpen	= (*pCoeffOpen)[j];
							pNewscoeffNode->pShort	= pSCoeffZero;
							pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
							p->sCoeffList->push_back(pSharescoeffNode);
							j ++;
						}
						else
						{
							p->sPowerList->push_back((*pPowerOpen)[j] );
							
							pNewscoeffNode			= new scoeffNode;
							pNewscoeffNode->pNode	= p;
							pNewscoeffNode->pOpen	= (*pCoeffOpen)[j];
							pNewscoeffNode->pShort	= (*pCoeffShort)[i];
							pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
							p->sCoeffList->push_back(pSharescoeffNode);
							i ++;
							j ++;
						}
					}
					else if(i < shortSize)
					{
						p->sPowerList->push_back(((*pPowerShort)[i] - 1));
							
						pNewscoeffNode			= new scoeffNode;
						pNewscoeffNode->pNode	= p;
						pNewscoeffNode->pOpen	= pSCoeffZero;
						pNewscoeffNode->pShort	= (*pCoeffShort)[i];
						pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
						p->sCoeffList->push_back(pSharescoeffNode);
						i ++;
					}
					else
					{
						p->sPowerList->push_back((*pPowerOpen)[j]);
								
						pNewscoeffNode			= new scoeffNode;
						pNewscoeffNode->pNode	= p;
						pNewscoeffNode->pOpen	= (*pCoeffOpen)[j];
						pNewscoeffNode->pShort	= pSCoeffZero;
						pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
						p->sCoeffList->push_back(pSharescoeffNode);
						j ++;
					}
				}
				break;
		case 'C':
				for(i = 0, j =0; i < shortSize || j < openSize;)
				{
					if(i < shortSize && j < openSize)
					{
						if((*pPowerShort)[i] < ((*pPowerOpen)[j] - 1))
						{
							p->sPowerList->push_back(((*pPowerShort)[i] + 1));

							pNewscoeffNode			= new scoeffNode;
							pNewscoeffNode->pNode	= p;
							pNewscoeffNode->pOpen	= pSCoeffZero;
							pNewscoeffNode->pShort	= (*pCoeffShort)[i];
							pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
							p->sCoeffList->push_back(pSharescoeffNode);
							i ++;
						}
						else if((*pPowerShort)[i] > ((*pPowerOpen)[j] - 1))
						{
							p->sPowerList->push_back((*pPowerOpen)[j]);

							pNewscoeffNode			= new scoeffNode;
							pNewscoeffNode->pNode	= p;
							pNewscoeffNode->pOpen	= (*pCoeffOpen)[j];
							pNewscoeffNode->pShort	= pSCoeffZero;
							pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
							p->sCoeffList->push_back(pSharescoeffNode);
							j ++;
						}
						else
						{
							p->sPowerList->push_back((*pPowerOpen)[j]);

							pNewscoeffNode			= new scoeffNode;
							pNewscoeffNode->pNode	= p;
							pNewscoeffNode->pOpen	= (*pCoeffOpen)[j];
							pNewscoeffNode->pShort	= (*pCoeffShort)[i];
							pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
							p->sCoeffList->push_back(pSharescoeffNode);
							i ++;
							j ++;
						}
					}
					else if(i < shortSize)
					{
						p->sPowerList->push_back(((*pPowerShort)[i] + 1));
						
						pNewscoeffNode			= new scoeffNode;
						pNewscoeffNode->pNode	= p;
						pNewscoeffNode->pOpen	= pSCoeffZero;
						pNewscoeffNode->pShort	= (*pCoeffShort)[i];
						pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
						p->sCoeffList->push_back(pSharescoeffNode);
						i ++;
					}
					else
					{
						p->sPowerList->push_back((*pPowerOpen)[j]);
							
						pNewscoeffNode			= new scoeffNode;
						pNewscoeffNode->pNode	= p;
						pNewscoeffNode->pOpen	= (*pCoeffOpen)[j];
						pNewscoeffNode->pShort	= pSCoeffZero;
						pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
						p->sCoeffList->push_back(pSharescoeffNode);
						j ++;
					}

				}
				break;
		default:  // R or G
			for(i = 0, j =0; i < shortSize || j < openSize;)
			{
					if(i < shortSize && j < openSize)
					{
						if((*pPowerShort)[i] < (*pPowerOpen)[j])
						{
							p->sPowerList->push_back((*pPowerShort)[i]);

							pNewscoeffNode			= new scoeffNode;
							pNewscoeffNode->pNode	= p;
							pNewscoeffNode->pOpen	= pSCoeffZero;
							pNewscoeffNode->pShort	= (*pCoeffShort)[i];
							pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
							p->sCoeffList->push_back(pSharescoeffNode);
							i ++;
						}
						else if((*pPowerShort)[i] > (*pPowerOpen)[j])
						{
							p->sPowerList->push_back((*pPowerOpen)[j]);

							pNewscoeffNode			= new scoeffNode;
							pNewscoeffNode->pNode	= p;
							pNewscoeffNode->pOpen	= (*pCoeffOpen)[j];
							pNewscoeffNode->pShort	= pSCoeffZero;
							pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
							p->sCoeffList->push_back(pSharescoeffNode);
							j ++;
						}
						else
						{
							p->sPowerList->push_back((*pPowerShort)[i]);
	
							pNewscoeffNode			= new scoeffNode;
							pNewscoeffNode->pNode	= p;
							pNewscoeffNode->pOpen	= (*pCoeffOpen)[j];
							pNewscoeffNode->pShort	= (*pCoeffShort)[i];
							pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
							p->sCoeffList->push_back(pSharescoeffNode);
							i ++;
							j ++;
						}
					}
					else if(i < shortSize)
					{
						p->sPowerList->push_back((*pPowerShort)[i]);

						pNewscoeffNode			= new scoeffNode;
						pNewscoeffNode->pNode	= p;
						pNewscoeffNode->pOpen	= pSCoeffZero;
						pNewscoeffNode->pShort	= (*pCoeffShort)[i];
						pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
						p->sCoeffList->push_back(pSharescoeffNode);
						i ++;
					}
					else
					{
						p->sPowerList->push_back((*pPowerOpen)[j]);
						
						pNewscoeffNode			= new scoeffNode;
						pNewscoeffNode->pNode	= p;
						pNewscoeffNode->pOpen	= (*pCoeffOpen)[j];
						pNewscoeffNode->pShort	= pSCoeffZero;
						pSharescoeffNode = ShareScoeffNodeNode(pNewscoeffNode, tag);
						p->sCoeffList->push_back(pSharescoeffNode);
						j ++;
					}

			}
			break;
#endif
	}
}

void tpdd::cleanSExpTpddNodes()
{	
	pSExpOne->visit_flag	= true;
	pSExpZero->visit_flag	= true;
	cleanSExpTpddNodesR(pExpTpddRoot);
}

void tpdd::cleanSExpTpddNodesR(sexpTpddNode *&p)
{
	if(p->visit_flag)
		return;
	
	char eType = Slist[p->symbIndex]->name[0];
	sexpTpddNode* q, *pcur;

	pcur = p;

	if(eType == '^')
	{
		q = p->pOpen;

		while(q->pTpddNode == NULL && q->pOpen)
		{
			p = q;
			q = q->pOpen;

			delete p;
			p = NULL;
		}
		
		pcur->pOpen = q;
	}
	pcur->visit_flag = true;

	
	cleanSExpTpddNodesR(pcur->pShort);
	cleanSExpTpddNodesR(pcur->pOpen);
}

void tpdd::calcScoeffTermN()
{
	pSCoeffOne->termN = 1;
	pSCoeffZero->termN = 0;

	// int i;

	for(unsigned i = 0; i < pExpTpddRoot->pShort->sCoeffList->size(); i ++)
	{
		calcScoeffTermNR((*(pExpTpddRoot->pShort->sCoeffList))[i]);
	}

	for(unsigned i = 0; i < pExpTpddRoot->pOpen->sCoeffList->size(); i ++)
	{
		calcScoeffTermNR((*(pExpTpddRoot->pOpen->sCoeffList))[i]);
	}
}

void tpdd::calcScoeffTermNR(scoeffNode* p)
{
	if(p->termN != -1)
		return;

	calcScoeffTermNR(p->pShort);
	calcScoeffTermNR(p->pOpen);

	p->termN = p->pShort->termN + p->pOpen->termN;
}

void tpdd::calcSymbAppearTimes()
{
	stack<sexpTpddNode*> *S = new stack<sexpTpddNode*>;
	int i;
	UnMarkAllNodes(pExpTpddRoot, 2);

	sexpTpddNode* pTopNode;

	pSExpOne->Mark();
	pSExpZero->Mark();

	pSExpOne->termN = 1;
	pSExpZero->termN = 0;

	pSExpOne->symbAppearTime = new double[TotalSymbN];
	pSExpZero->symbAppearTime = new double[TotalSymbN];

	memset(pSExpOne->symbAppearTime, 0, sizeof(double) * TotalSymbN);
	memset(pSExpZero->symbAppearTime, 0, sizeof(double) * TotalSymbN);

	S->push(pExpTpddRoot);

	while(!S->empty())
	{
		pTopNode = S->top();

		if(pTopNode->Marked())
		{
			S->pop();
		}
		else
		{
			if(!pTopNode->GetPShort()->Marked())
			{
				S->push(pTopNode->GetPShort());
			}
			else if(!pTopNode->GetPOpen()->Marked())
			{
				S->push(pTopNode->GetPOpen());
			}
			else
			{
				pTopNode->Mark();
				pTopNode->symbAppearTime = new double[TotalSymbN];
				memset(pTopNode->symbAppearTime, 0, sizeof(double) * TotalSymbN);

				pTopNode->termN = pTopNode->GetPShort()->termN + pTopNode->GetPOpen()->termN;

				for(i = 0; i < TotalSymbN; i ++)
				{
					pTopNode->symbAppearTime[i] = pTopNode->GetPShort()->symbAppearTime[i] 
						+ pTopNode->GetPOpen()->symbAppearTime[i]; 
				}
				pTopNode->symbAppearTime[pTopNode->symbIndex] = pTopNode->GetPShort()->termN;
			}
		}
	}

	for(i = 0; i < TotalSymbN; i ++)
	{
		cout << SNoLumplist[i]->name << "  " << pExpTpddRoot->symbAppearTime[i] << endl;
	}
}

void tpdd::calcSymbAppearTimesR(tpddnode* p)
{
	if(p->aT != -1)
		return;
	
	calcSymbAppearTimesR(p->GetPShort());
	calcSymbAppearTimesR(p->GetPOpen());

	p->aT = p->GetPShort()->aT + p->GetPOpen()->aT;

//	Slist[p->GetSymbIndex()]->appearTimes += p->GetPShort()->aT;

	if(p->GetPShort()->GetSymbIndex() >= 0)
		Slist[p->GetPShort()->GetSymbIndex()]->appearTimes += p->GetPShort()->GetPShort()->aT;

	if(p->GetPOpen()->GetSymbIndex() >= 0)
		Slist[p->GetPOpen()->GetSymbIndex()]->appearTimes += p->GetPOpen()->GetPShort()->aT;
}

void tpdd::MarkAllNodes(Tnode* p, int type)
{
	stack<Tnode*> *S = new stack<Tnode*>;

	tpddnode* pTopNode;
	sexpTpddNode* peTopNode;

	S->push(p);
	
	if(type == 1)
	{
		pOne->Mark();
		pZero->Mark();
		while(!S->empty())
		{
			
				pTopNode = (tpddnode*)S->top();
				if(pTopNode->Marked())
				{
					S->pop();
				}
				else
				{
					if(!pTopNode->GetPShort()->Marked())
						S->push(pTopNode->GetPShort());
					else if(!pTopNode->GetPOpen()->Marked())
						S->push(pTopNode->GetPOpen());
					else
					{
						pTopNode->Mark();
					}
				}
			}
	}
	else
	{	
		pSExpOne->Mark();
		pSExpZero->Mark();
		while(!S->empty())
		{
			peTopNode = (sexpTpddNode*)S->top();
			if(peTopNode->Marked())
			{
				S->pop();
			}
			else
			{
				if(!peTopNode->GetPShort()->Marked())
					S->push(peTopNode->GetPShort());
				else if(!peTopNode->GetPOpen()->Marked())
					S->push(peTopNode->GetPOpen());
				else
				{
					peTopNode->Mark();
				}
			}
		}	
	}
}

void tpdd::clacNodeReference(Tnode* p, int type)
{
	UnReferenceAll(p, type);
	UnMarkAllNodes(p, type);
	
	stack<Tnode*> *S = new stack<Tnode*>;
	tpddnode* pTopNode;
	sexpTpddNode* peTopNode;

	S->push(p);

	if(type == 1)
	{
		pOne->Mark();
		pZero->Mark();
		while(!S->empty())
		{
			pTopNode = (tpddnode* )S->top();
			if(pTopNode->Marked())
			{
				S->pop();
			}
			else
			{
				if(!pTopNode->GetPShort()->Marked())
					S->push(pTopNode->GetPShort());
				else if(!pTopNode->GetPOpen()->Marked())
					S->push(pTopNode->GetPOpen());
				else
				{
					pTopNode->Mark();
					pTopNode->GetPShort()->Reference();
					pTopNode->GetPOpen()->Reference();
				}
			}
		}
	}
	else
	{
		pSExpOne->Mark();
		pSExpZero->Mark();
		while(!S->empty())
		{
			peTopNode = (sexpTpddNode*)S->top();
			if(peTopNode->Marked())
			{
				S->pop();
			}
			else
			{
				if(!peTopNode->GetPShort()->Marked())
					S->push(peTopNode->GetPShort());
				else if(!peTopNode->GetPOpen()->Marked())
					S->push(peTopNode->GetPOpen());
				else
				{
					peTopNode->Mark();
					peTopNode->GetPShort()->Reference();
					peTopNode->GetPOpen()->Reference();
				}
			}
		}
	}
}

void tpdd::getMostReferencedNode(Tnode* p, int type)
{
	UnMarkAllNodes(p, type);
	
	stack<Tnode*> *S = new stack<Tnode*>;
	tpddnode *pTopNode = 0, *pMaxRefNode = 0;
	sexpTpddNode *peTopNode = 0, *peMaxRefNode = 0;
	unsigned maxRef = 0;

	S->push(p);

	if(type == 1)
	{
		pOne->Mark();
		pZero->Mark();

		while(!S->empty())
		{
			pTopNode = (tpddnode*) S->top();
			if(pTopNode->Marked())
			{
				S->pop();
			}
			else
			{
				if(!pTopNode->GetPShort()->Marked())
					S->push(pTopNode->GetPShort());
				else if(!pTopNode->GetPOpen()->Marked())
					S->push(pTopNode->GetPOpen());
				else
				{
					pTopNode->Mark();
					if(pTopNode->NumReferences() > maxRef)
					{
						maxRef = pTopNode->NumReferences();
						pMaxRefNode = pTopNode;
					}
				}
			}
		}
		cout << "maximum referenced node: " << Slist[pMaxRefNode->GetSymbIndex()]->name << endl;
	}
	else
	{
		pSExpOne->Mark();
		pSExpZero->Mark();

		while(!S->empty())
		{
			peTopNode = (sexpTpddNode* )S->top();
			if(peTopNode->Marked())
			{
				S->pop();
			}
			else
			{
				if(!peTopNode->GetPShort()->Marked())
					S->push(peTopNode->GetPShort());
				else if(!peTopNode->GetPOpen()->Marked())
					S->push(peTopNode->GetPOpen());
				else
				{
					peTopNode->Mark();

				//	cout << Slist[peTopNode->symbIndex]->name << "  " << peTopNode->NumReferences() << "  " << (void* )peTopNode<< endl;
					if(peTopNode->NumReferences() > maxRef)
					{
						maxRef = peTopNode->NumReferences();
						peMaxRefNode = peTopNode;
					}
				}
			}
		}
		cout << "maximum referenced node: " << Slist[peMaxRefNode->symbIndex]->name << endl;
	}

	

	cout << "maximum node reference time: " << maxRef << endl;

}

int tpdd::Determine_tag(char * name)
{
//	cout << "the symbol to be modified is : " << name << endl;
	char *name_tag = NULL;
	int list_tag;
	int i = 0;
	
	if(!pTpddRoot || pTpddRoot->GetSymbIndex() < 0)
	{
		
//		cout << "there is no root.." << endl;
		return -1;
	}

	SymbNode *pParaNode;
	for(i = 0; i < SymbN; i++)
	{
//		cout << i << "->" << Slist[i]->name <<endl;
		
		if(Slist[i]->name[0] !='^')
		{
			if(strcmp(Slist[i]->name, name) == 0)
			{
				name_tag = name;
				list_tag = i;
				SetSensitivity_tag(i);
//				cout << "the symbol is exist --no lump" << endl;
//				cout << "i is : " << i << endl;
				break;
			}
		}
		else
		{
			pParaNode = Slist[i]->nextP;
			while(pParaNode)
			{
		//		cout << "1" << endl;
				if(strcmp(pParaNode->name, name) == 0)
				{
		//			cout << "2" << endl;
					name_tag = name;
					list_tag = i;
					SetSensitivity_tag(i);
//					cout << "the symbol is exist--lump" << endl;
//					cout << "i is "  << i << endl;
					break;
		//			cout << "3" << endl;
				}
				pParaNode = pParaNode->nextP;
		//		cout << "4" << endl;
			}
	//		cout << "5" << endl;
		//	if(name_tag != NULL)cout <<" name_tag is : " << name_tag << endl; 
	//		cout << "name is : " << name << endl;
			if((name_tag != NULL) && ((strcmp(name_tag, name)) == 0))
			{
				cout << "the symbol has been searched.." << endl;
				break;
			}
	//		cout << "mark.." << endl;
		}
	
	}
//	cout <<  "find number is : " << i << endl;

	return i; 
}

void tpdd::ModifyStructure(int tag, char *name)
{
	cout << "in the process of modify structure" << endl;
	cout << "name is : " << name << " tag is : " << tag << endl;
	
	if(!pTpddRoot||pTpddRoot->GetSymbIndex() < 0)
	{
		cout << "there is no root... " << endl;
		return;
	}
	
	if(pTpddRoot->GetPShort()->GetSymbIndex() > tag)
	{
		cout << "\n no term of Numerator is exist.." << endl;
		pTpddRoot->SetPShort(pZero);	
	}
	if(pTpddRoot->GetPOpen()->GetSymbIndex() > tag)
	{
		cout << "\n no term of Denominator is exist.." << endl;
		pTpddRoot->SetPOpen(pZero);
	}
	
	UnReferenceAll(pTpddRoot,1);
	UnMarkAllNodes(pTpddRoot,1);
	ModifyStructureR(pTpddRoot, tag, name);
	cout << "end" << endl;
}
void tpdd::ModifyStructureR(tpddnode *p, int tag, char * name)
{
//	cout << "rescursive.." << endl;
	if(p->Marked())
		return;
	p->Mark();

	if(p->GetSymbIndex() == tag)
        {
                if(Slist[tag]->name[0] != '^')
                {
                        Slist[tag]->name[0] = 'R';

                        Slist[tag]->value = 1;
//			cout << ""
//cout << "**^^^  " << Slist[tag]->name[0] << "&^^^&&" << Slist[tag]->value << endl;  
              }
                else
                {
                        Slist[tag]->name[0] = 'R';
                        Slist[tag]->value = 1;
//			cout << "**  " << Slist[tag]->name[0] << "&&&" << Slist[tag]->value << endl;
                }
        }

	
	tpddnode *pNode;
	
	pNode = p->GetPShort();
	
	if((p->GetSymbIndex() < tag)&&(pNode->GetSymbIndex() > tag) )
	{
		p->SetPShort(pZero);
		pNode = p->GetPShort();
	}
	else if ((p->GetSymbIndex() < tag) && (pNode == pOne))
	{
		p->SetPShort(pZero);
		pNode = p->GetPShort();
	}
	
	if(pNode != pOne && pNode != pZero)
	{
		pNode->Reference();
		ModifyStructureR(pNode, tag, name);
	}
	
	pNode = p->GetPOpen();
	
	if((p->GetSymbIndex() < tag)&&(pNode->GetSymbIndex() > tag) )
        {
                p->SetPOpen(pZero);
                pNode = p->GetPOpen();
        }
        else if ((p->GetSymbIndex() < tag) && (pNode == pOne))
        {
                p->SetPOpen(pZero);
                pNode = p->GetPOpen();
        }
	
	if(p->GetSymbIndex() == tag)
	{
		p->SetPOpen(pZero);
		pNode = p->GetPOpen();
		cout << "right node is zero.." << endl;
		}
	if(pNode != pOne && pNode != pZero)
        {
                pNode->Reference();
                ModifyStructureR(pNode, tag, name);
        }

//	if(pNode->GetSymbIndex() == tag)
//	{
//		p->SetPOpen(pZero);
//		
//	}
}	

// Change the value of a specific component
// Li Ji
// 2008-10-29 22:46
double tpdd::change_value(const string &comp_name, double nv)
{
	double old_value = 0.0; // an error value in default
	if (SymbN == 0)
	{
		cout << "\nEmpty symbol list.\n" << endl;
		return 0.0; // an error value
	}
	for(int n=0; n < SymbN; ++n)
	{
		SymbNode *pNode = Slist[n];
		do
		{
			string name(pNode->name);
			if(name == comp_name)
			{
				old_value = pNode->value;
				cout << name << ".(" << pNode->value << ") <- " << nv << endl;
				pNode->value = nv;
				return old_value;
			}
		}while((pNode = pNode->nextP));
	}
	cout << "No " << comp_name << endl;
	return old_value;
}

// Li Ji
void tpdd::get_name_ptrvalue_map(map<string, double *> &name_pvalue_map)
{
	name_pvalue_map.clear();
	for (int i=0; i < SymbN; ++i)
	{
		SymbNode *pNode = Slist[i];
		while (pNode)
		{
			char c = *pNode->name;
			// Ignore voltage sources and composite components
			if (c!='V' && c != '^')
				name_pvalue_map.insert(make_pair(pNode->name, &pNode->value));
			pNode = pNode->nextP;
		}
	}
}

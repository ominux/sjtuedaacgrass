/**************************************************************************************
*        Symbolic Circuit Analysis With TPDDs (Tree Pair Decision Diagram)
* tpddnode.cpp
* Member functions for class tpddnode.
* tpddnode is the nodes in TPDD.
*
* CHEN Weiwei		GShi	06/03/31
* School of Microelectronics, Shanghai Jiaotong Univ.
* {chenweiwei, shiguoyong}@ic.sjtu.edu.cn
**************************************************************************************/
#include "tpddnode.h"
#include <vector>
#include <cstdio>
using std::ostream;
using std::endl;
using std::cerr;
using std::hex;
using std::vector;
// using std::printf;
// using std::fflush;

tpddnode::tpddnode(): Tnode()
{
	symbIndex	= -1;
	signS		= 1;
	signO		= 1;
	pShort		= NULL;
	pOpen		= NULL;
	pLTree		= NULL;
	pRTree		= NULL;
	visit_flag  = false;
	val_ptr		= NULL;
//	sCoeffList   = NULL;
//	sPowerList  = NULL;
	aT			= -1;
}	
		
tpddnode::tpddnode(int SymbI, int ss, int so, subtree * LT, subtree *RT): Tnode()
{
	symbIndex	= SymbI;
	signS		= ss;
	signO		= so;
	pShort		= NULL;
	pOpen		= NULL;
	pLTree		= LT;
	pRTree		= RT;
	visit_flag  = false;
	val_ptr		= NULL;
	aT			= -1;
//	sCoeffList   = NULL;
//	sPowerList  = NULL;
//	tag = 1;
}
		
tpddnode::~tpddnode()
{
	pLTree	= NULL;
	pRTree	= NULL;
	
	// int i;
	// scoeffNode *pScoeffNode;
/*	if(sCoeffList)
	{
#ifndef HASHSCOEFFNODE 
		for(i = 0; i < sCoeffList->size(); i ++)
		{
			pScoeffNode = (*sCoeffList)[i];
			delete pScoeffNode;
		}
		sCoeffList->clear();
#endif
		delete sCoeffList;
	}

	if(sPowerList)
	{
		sPowerList->clear();
		delete sPowerList;
	}*/
}

void tpddnode::PrintTpddNode(ostream &out)
{
	out << endl;
	out << "TPDD Node " << (void*) this << " symbol = " << symbIndex<< " " << hex << reference <<endl;
	if(pShort)
	{
		out << "Short (left) = " << pShort->GetSymbIndex() << " [" << (void *) pShort << "]" ;
	}
	else
	{
		out << "Short (left) = " << 0 << " [00000000]";
	}
	out << " : ";
	if(pOpen)
	{
		out << "Out (left) = " << pOpen->GetSymbIndex() << " [" << (void *) pOpen << "]";
	}
	else
	{
		out << "Out (left) = " << 0 << " [00000000]";
	}

	out << endl;
	out << " signS = " << signS << "  signO = " << signO;
	out << " : " << endl;
}

unsigned tpddnode::Hash(unsigned i)
{
//	int value = (((int)pLTree << 7)^ ((int)pRTree << 15) ^ (symbIndex < 1));
	long value = ((((long)pLTree << 3) + ((long)pRTree << 15)) ^ (~symbIndex));
	return (value ^ i) % TPDD_PRIME;
}

integer tpddnode::Compare(Comparable * p)
{
	tpddnode * n= (tpddnode *)p;

	ASSERT( p->isa() == TPDDNODE, "not a TPDD Node Object");
 
	if (( symbIndex ^ n->GetSymbIndex()) || ((long)pLTree ^ (long)n->GetLeftSubTree()) ||
		  ((long)pRTree ^ (long)n->GetRigthSubTree()))
	{
		if ((long)this > (long)n)
			return 1;
		if ((long)this < (long)n)
			return -1;
	}
	return 0;
}
		
unsigned tpddnode::HashTreeID()
{
//	return symbIndex;
	return ((((long)pLTree) ^ ((long)pRTree))) % TPDD_PRIME;
}

integer tpddnode_tag::Compare(Comparable *p)
{
	tpddnode_tag * n = (tpddnode_tag *) p;
	
	if((n->label_left ^ label_left) || (n->label_right ^ label_right))
	{
		if ((long)this > (long)n)
			return 1;
		else
			return -1;
	}

	return 0;
}

scoeffNode::~scoeffNode()
{
	delete val_ptr;
}


unsigned scoeffNode::Hash(unsigned i)
{
	return ((pShort->label << 3) ^ (pOpen->label << 7));
}

integer scoeffNode::Compare(Comparable *p)
{
	scoeffNode * n = (scoeffNode *) p;
	
	if((n->pShort->label ^ pShort->label) || (n->pOpen->label ^ pOpen->label) 
		|| n->pNode->signO ^ pNode->signO || n->pNode->signS ^ pNode->signS)
	{
		if ((long)this > (long)n)
			return 1;
		else
			return -1;
	}

	return 0;
}
	
unsigned scoeffNode::HashTreeID()
{
//	return abs(pShort->label * pNode->GetSignS() * TAG_PRIME1 + pOpen->label * pNode->GetSignO()) % TAG_PRIME2;
	return abs(pShort->label * pNode->signS * TAG_PRIME1 + pOpen->label * pNode->signO) % TAG_PRIME2;
}
			
complex_t scoeffNode::GetValue()
{
	return *val_ptr;
}
	
complex_t *scoeffNode::GetValueP()
{
	return val_ptr;
}

// int sexpTpddNode::cnt = 0;

sexpTpddNode::sexpTpddNode()
{
	// id = cnt++;
	// printf("sexpTpddNode constructor[%-3d]: this = %p\n", id, this);
	pOpen		= NULL;
	pShort		= NULL;
	pTpddNode	= NULL;
	sCoeffList   = NULL;
	sPowerList  = NULL;
	visit_flag	= false;
	symbAppearTime = NULL;
}

sexpTpddNode::sexpTpddNode(sexpTpddNode* pS, sexpTpddNode* pO, tpddnode* pT, int sn, short ss, short so)
{
	// id = cnt++;
	// printf("sexpTpddNode copy constructor[%-3d]: this = %p\n", id, this);
	pOpen		= NULL;
	pShort		= pS;
	pOpen		= pO;
	pTpddNode	= pT;
	symbIndex	= sn;
	signO		= so;
	signS		= ss;
	sCoeffList   = NULL;
	sPowerList  = NULL;
	visit_flag	= false;
	symbAppearTime = NULL;
}

sexpTpddNode::~sexpTpddNode()
{
	// printf("sexpTpddNode destructor[%-3d]: this = %p->", id, this);
	// fflush(stdout);
	if(sCoeffList)
	{
#ifndef HASHSCOEFFNODE 
		for (std::vector<scoeffNode*>::iterator it = sCoeffList->begin();
				 it != sCoeffList->end();
				 ++it)
		{
			// (*it)->print();
			delete *it;
		}
#endif
		delete sCoeffList;
		sCoeffList = 0;
	}

	if(sPowerList)
	{
		sPowerList->clear();
		delete sPowerList;
		sPowerList = 0;
	}

	if(symbAppearTime)
	{
		delete[] symbAppearTime;
		symbAppearTime = 0;
	}
	// printf("<-\n");
}

unsigned sexpTpddNode::Hash(unsigned i)
{
	return (long)pTpddNode;
//	return i;
}

integer sexpTpddNode::Compare(Comparable *p)
{
	sexpTpddNode * n = (sexpTpddNode *) p;
	if((long)this->pTpddNode ^ (long)n->pTpddNode || this->signS ^ n->signS || this->signO ^ n->signO 
		)//|| (!this->pTpddNode && (this->symbIndex ^ n->symbIndex)))
	{
		if ((long)this > (long)n)
			return 1;
		else
			return -1;
	}

	return 0;
}
	
unsigned sexpTpddNode::HashTreeID()
{
	return symbIndex % TAG_PRIME2;
}

/**************************************************************************************
*        Symbolic Circuit Analysis With TPDDs (Tree Pair Decision Diagram)
* subtree.cpp
* Member functions for class subtree.
* subtree is the class to represent one tree of the tree pair.
*
* CHEN Weiwei		GShi	06/03/31
* School of Microelectronics, Shanghai Jiaotong Univ.
* {chenweiwei, shiguoyong}@ic.sjtu.edu.cn
**************************************************************************************/

#include "subtree.h"
#include "tpdd_global.h"
#include "tpdd_struct.h"
#include "tpdd.h"
using std::cerr;

//extern int TotalNodes;
//extern int TotalEdges;
//extern int temp;
//extern int dt;
extern tpdd *mytpdd1;

subtree::subtree(EdgeInfo *pe, int En, int Nn, int s)
{
		pTree	= pe;
		EdgeNum = En;
		NodeNum = Nn;
		sign	= s;
//		tag = 0;
}

subtree::~subtree()
{
	if(pTree)
	{
		delete[] pTree;
	}
	pTree = NULL;
}

unsigned subtree::Hash(unsigned i)
{
	int j;
	unsigned result = 0;
/*	if(tag)
		return EdgeNum;*/

	for(j = 0; j < EdgeNum; j ++)
	{
		if(pTree[j].node1 ^ pTree[j].node2)
			result += (((pTree[j].node1 + 1) << 9) ^ ((pTree[j].node2 + 1) << 7)) * (pTree[j].edgeindex);
	}
	return ((result ^ i) % SUBTREE_PRIME);
}
	
integer subtree::Compare(Comparable *p)
{
	ASSERT( p->isa() == SUBTREE, "not a subtree Object");
 
	subtree *n = (subtree *)p;

	if(EdgeNum == n->GetNumEdges() && NodeNum == n->GetNumNodes())
	{
		return this->compareTree(n);
	}

	if((long)this > (long)n)
		return 1;
	else
		return -1;
}

unsigned subtree::HashTreeID()
{
	int i;
	unsigned result = 0;
	for(i = 0; i < EdgeNum; i ++)
		result ^= (pTree[i].edgeindex);

	return ((EdgeNum * mytpdd1->TotalNodes() + NodeNum) * result) % SUBTREE_PRIME;
}
	
int subtree::compareTree(subtree * pSubTree)
{
	int ne1, ne2, i;
	EdgeInfo *pC, *pS;

	ne1 = this->GetValidEdges();
	ne2 = pSubTree->GetValidEdges();

/*	if(ne1 != ne2)
	{
		if((int)this > (int)pSubTree)
			return 1;
		else
			return -1;
	}*/	

	if(NodeNum == 2 && ne1 == 1 && ne2 == 1)
	{
		if((this->GetValidEdgeIndex() == pSubTree->GetValidEdgeIndex()) && !pTree[this->GetValidEdgeId()].isSrc)
			return 0;
		else
			return ((long)this > (long)pSubTree ? 1 : -1);
	}

	for(i = 0; i < EdgeNum; i ++)
	{
		pC = &pTree[i];
		pS = &pSubTree->GetSubTree()[i];

		if((pC->edgeindex ^ pS->edgeindex) || (pC->node1 ^ pS->node1) || (pC->node2 ^ pS->node2))
		{
			if((long)this > (long)pSubTree)
				return 1;
			else
				return -1;

			/*	return completeCompareTree(pSubTree, EdgeNum);*/
		}
	}

/*	if(ne1 != ne2)
	{
		if((int)this > (int)pSubTree)
				return 1;
		else
				return -1;
	}*/

	return 0;
//	return completeCompareTree(pSubTree, EdgeNum);
//	
}

int subtree::GetValidEdges()
{
	int i, validedges = 0;
	for(i = 0; i < EdgeNum; i ++)
	{
		if(pTree[i].node1 ^ pTree[i].node2)
			validedges ++;
	}
	return validedges;
}

int subtree::GetValidEdgeId()
{
	int i;
	for(i = 0; i < EdgeNum; i ++)
	{
		if(pTree[i].node1 ^ pTree[i].node2)
			return i;
	}
	return -1;
}

int subtree::GetValidEdgeIndex()
{
	int i, j;
	for(i = 0; i < EdgeNum; i ++)
	{
		if(pTree[i].node1 ^ pTree[i].node2)
		{
			j = pTree[i].node1 > pTree[i].node2 ? 1 : -1;
			return pTree[i].edgeindex * j;
		}
	}
	return 0;
}

int subtree::completeCompareTree(subtree * pSubTree, int edgeNum)
{
	int i, j,/*k,*/ v, tag1, tag2, tag3;
	edgeComp* pNew, *pCur, *peSrc, *peDes;
	vertexComp* pvSrc, *pvDes;

	vertexComp* vSrc = new vertexComp[mytpdd1->TotalNodes()];
	vertexComp* vDes = new vertexComp[mytpdd1->TotalNodes()];
	int * mark = new int[mytpdd1->TotalNodes()];

//	memset(vSrc, 0, TotalNodes * sizeof(vertexComp));
//	memset(vDes, 0, TotalNodes * sizeof(vertexComp));

	for(i = 0; i < mytpdd1->TotalNodes(); i ++)
	{
		vSrc[i].edgeSum = 0;
		vSrc[i].pedge = NULL;
		vSrc[i].vertexNo = i;

		vDes[i].edgeSum = 0;
		vDes[i].pedge = NULL;
		vDes[i].vertexNo = i;
	}

	memset(mark, 0, mytpdd1->TotalNodes() * sizeof(int));

	for(i = 0; i < EdgeNum; i ++)
	{
		pNew = new edgeComp;
		pNew->direct = 1;
		pNew->edgeNo = pTree[i].edgeindex;
		v = pTree[i].node1;
		pNew->pNext = vSrc[v].pedge;
		vSrc[v].pedge = pNew;
		vSrc[v].edgeSum ++;

		pNew = new edgeComp;
		pNew->direct = -1;
		pNew->edgeNo = pTree[i].edgeindex;
		v = pTree[i].node2;
		pNew->pNext = vSrc[v].pedge;
		vSrc[v].pedge = pNew;
		vSrc[v].edgeSum ++;

		pNew = new edgeComp;
		pNew->direct = 1;
		pNew->edgeNo = pSubTree->pTree[i].edgeindex;
		v = pSubTree->pTree[i].node1;
		pNew->pNext = vDes[v].pedge;
		vDes[v].pedge = pNew;
		vDes[v].edgeSum ++;

		pNew = new edgeComp;
		pNew->direct = -1;
		pNew->edgeNo = pSubTree->pTree[i].edgeindex;
		v = pSubTree->pTree[i].node2;
		pNew->pNext = vDes[v].pedge;
		vDes[v].pedge = pNew;
		vDes[v].edgeSum ++;
	}

/*********************** Compare ***********************************/

	tag3 = 0;

	for(i = 0; i < mytpdd1->TotalNodes(); i ++)
	{
		pvSrc = &vSrc[i];
		if(!pvSrc->edgeSum)
			continue;
		
		tag2 = 0;

		for(j = 0; j < mytpdd1->TotalNodes(); j ++)
		{
			pvDes = &vDes[j];
			if(pvSrc->edgeSum == pvDes->edgeSum && !mark[pvDes->vertexNo])
			{
				peSrc = pvSrc->pedge;
				peDes = pvDes->pedge;
					
				tag1 = 0;

				while(peSrc)
				{
					if(peSrc->edgeNo == peDes->edgeNo && peSrc->direct == peDes->direct)
					{
						peSrc = peSrc->pNext;
						peDes = peDes->pNext;
						continue;
					}
					tag1 = 1;
					break;
				}

				if(!tag1)
				{
					mark[pvDes->vertexNo] = 1;
					tag2 = 1;
					break;
				}
			}
		}

		if(!tag2)
		{
			tag3 = 1;
			break;
		}
	}

	for(i = 0; i < mytpdd1->TotalNodes(); i ++)
	{
		pCur = vSrc[i].pedge;
		while(pCur)
		{
			vSrc[i].pedge = pCur->pNext;
			delete pCur;
			pCur = vSrc[i].pedge;
		}

		pCur = vDes[i].pedge;
		while(pCur)
		{
			vDes[i].pedge = pCur->pNext;
			delete pCur;
			pCur = vDes[i].pedge;
		}
	}

	delete[] vSrc;
	delete[] vDes;

	if(tag3)
	{
		if((long)this > (long)pSubTree)
		
			return 1;
		else
			return -1;
	}

	return 0;
}

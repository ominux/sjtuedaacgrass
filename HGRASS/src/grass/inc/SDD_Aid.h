/* SDD_Aid.h
 * Symbol Decision Diagram Aid -- An interface to DDDmanager.
 * GShi, 04/07/04, 05/03/04
 */

#ifndef		SDD_AID_H
#define		SDD_AID_H

#include	<iostream>
#include 	"..\inc\globals.h"
#include 	"..\inc\dddbase.h"
#include 	"cfdd_global.h"

//using namespace std;


class SDD_Aid
{
private:
	DDDmanager	*ddd; 	// Instantiated in Enumerator.h
	SymbNode	**symbarray;	// An array of SymbNode pointers
	int		Nsymb;
	int		num_terms;

public:
	SDD_Aid(int Nsymb, SymbNode **symbarray, DDDmanager *ddd);
	~SDD_Aid();
	DDDnode	*InsertSet(int	*set);

	void	GarbageCollect(DDDnode *root);

	void	PrintSet(int *set);
	void	PrintSDD(DDDnode* root)	{ ddd->PrintDDDTree(root); }
	void 	PrintSDDSets(DDDnode* P, std::ostream& out); 
	void 	PrintIndexR(DDDnode* P, int *head, int *tail, std::ostream& out);
	void	PrintSDDInfo(DDDnode *P) { ddd->statistic(P); }
};

#endif


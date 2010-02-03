/* Analysis.h
 * For AC analysis based on SDD (Symbol Decision Diagram).
 * GShi, 04/12/04
 */

#ifndef		ANALYSIS_H
#define		ANALYSIS_H

//#include	"..\inc\complex_t.h"
#include	"..\inc\globals.h"
#include	"..\inc\dddbase.h"

#include	<iostream>
#include	<limits.h>


#include	"cfdd_global.h"

#include	"Enumerator.h"
#include    "tpdd.h"

class Analysis
{

private:
	Enumerator	*CFE;
	tpdd		*TPE;

public:
	Analysis(Enumerator *cfe);
	Analysis(tpdd *tpe);
	~Analysis();
	
	// Evaluate the TF from SDD.
	complex_t	Evaluate(DDDnode *root, double freq);
	// Recursive evaluate a sub-SDD.
	complex_t	EvaluateR(DDDnode *P, double freq, HasherTree *ht);
	complex_t	EvaluateR2(DDDnode *P, double freq, bool visited);

	complex_t Evaluate(tpddnode *pRoot, double freq);
	complex_t	EvaluateR(tpddnode *P, double freq, bool visited);
	complex_t	EvaluateH(tpddnode *P, double freq, HasherTree *ht);
};

#endif


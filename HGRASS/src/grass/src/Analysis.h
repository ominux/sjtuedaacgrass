/**************************************************************************************
*        Symbolic Circuit Analysis With TPDDs (Tree Pair Decision Diagram)
* Analysis.h
* For AC analysis based on TPDD (Tree Pair Decision Diagram).
*
* CHEN Weiwei		GShi	06/03/31
* School of Microelectronics, Shanghai Jiaotong Univ.
* {chenweiwei, shiguoyong}@ic.sjtu.edu.cn
**************************************************************************************/

#ifndef		ANALYSIS_H
#define		ANALYSIS_H
#include<complex>

//#include	"complex_t.h"
#include	"globals.h"
//#include	"dddbase.h"

#include	<iostream>
#include	<limits.h>


#include	"tpdd_global.h"

//#include	"Enumerator.h"
#include	"Mosfet.h"
#include    	"tpdd.h"

struct E_value{
	complex_t num;
	complex_t den;
	};

/**
	The class for performing numerical analysis onto the Tree Pair Decision Diagram (TPDD). 
*/
class Analysis
{

private:
	/// the pointer to the TPDD to analysis.
	tpdd		*TPE;
	// Recursively call mark for re-traversing the s-expanded muti-root tree: Li Ji
	void UnMarkSExpTree(sexpTpddNode *);

public:

	/**
		The constructor of the Analysis class.
		@param tpe the pointer to the TPDD to analysis. 
	*/
	Analysis(tpdd *tpe);

	/**
		The destructor of the Analysis class.
	*/
	~Analysis(){};

	/**
		The function to evaluate the Transfer function (TF) from the TPDD.
	*/
	complex_t Evaluate(tpddnode *pRoot, double freq);
	
	E_value Evaluate_complex(tpddnode * pRoot, double freq);
	E_value Evaluate_complex_diff(tpddnode * pRoot, double symbol_value, char * symb_name,int tag, double freq);
	E_value Evaluate_complex_diff_factor(tpddnode * pRoot, double symbol_value, char * symb_name, int tag, double freq);	
	E_value Evaluate_complex_diff_factor_conj(tpddnode * pRoot, double symbol_value, char * symb_name, int tag, double freq);
/**
		Recursive evaluate a TPDD.
		Mark the TPDD nodes to avoid multiple nodes' evaluation. 
	*/
	complex_t	EvaluateR(tpddnode *P, double freq, bool visited);
	complex_t EvaluateR_diff(tpddnode *P, double freq, bool visited, int sentag);
	complex_t EvaluateR_diff_factor(tpddnode *P, double freq, bool visited, int sentag, char *name);
		/**
		Recursive evaluate a TPDD.
		Use a unique table (Hash table) to avoid multiple nodes' evaluation. 
	*/
	complex_t	EvaluateH(tpddnode *P, double freq, HasherTree *ht);

	/**
		Get the complex_t value of the transfer function (TF) of the TPDD with differrent frequency.
		The transfer function is s-variable coefficients extracted. That means the TF is
	    <BR><UL>					a0 + a1 * s + a2 * s^2 + ... + am * s^m </UL> 
				TF =    --------------------------------------------------------
		<BR><UL>				    b0 + b1 * s + b2 * s^2 + ... + bn * s^n</UL>
		The TF with s-variable coefficients extracted is built already.
		All of the s-variable coefficients is recorded in the vector named sCoeffList in the two children nodes 
		of the root of the sexpTpddNode in the TPDD.
		The order of the s-variable is recored in the vector named sPowerList in the two children nodes 
		of the root of the sexpTpddNode in the TPDD.
		@param freq the frequency of the s-variable.
	*/
	complex_t EvaluateSCoeffFreq(double freq);

	// Li Ji, 2009-03-04
	inline std::complex<double> stdEvaluateSCoeffFreq(double freq)
	{ 
		complex_t ret(EvaluateSCoeffFreq(freq));
		return std::complex<double>(ret.real(), ret.imag());
 	}

	/**
		Recursive evaluate a TPDD.
		Mark the TPDD nodes to avoid multiple nodes' evaluation. 
		The transfer function is s-variable extracted. That means the TF is
	    <BR><UL>					a0 + a1 * s + a2 * s^2 + ... + am * s^m </UL> 
				TF =    --------------------------------------------------------
		<BR><UL>				    b0 + b1 * s + b2 * s^2 + ... + bn * s^n</UL>
		@param P		the root of the sub-expanded-TPDD to be evaluated.
		@param visited	the flag to identify whether the node is evaluated or not.
	*/
	void EvaluateSCoeffR(sexpTpddNode *P, bool visited);

	/**
		The function to evaluate the Transfer function (TF) from the TPDD.
		The transfer function is s-variable extracted. That means the TF is
	    <BR><UL>					a0 + a1 * s + a2 * s^2 + ... + am * s^m </UL> 
				TF =    --------------------------------------------------------
		<BR><UL>				    b0 + b1 * s + b2 * s^2 + ... + bn * s^n</UL>
	*/
	void EvaluateSCoeff();
};

#endif


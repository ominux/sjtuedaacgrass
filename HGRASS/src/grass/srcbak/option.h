/**************************************************************************************
*        Symbolic Circuit Analysis With TPDDs (Tree Pair Decision Diagram)
* option.h
* Define Compile Options. (Decide the behavior of the main function)
*
* CHEN Weiwei		GShi	06/03/31
* School of Microelectronics, Shanghai Jiaotong Univ.
* {chenweiwei, shiguoyong}@ic.sjtu.edu.cn
**************************************************************************************/

#ifndef OPTION_H
#define OPTION_H

///	non-recusively construct the TPDD.
#define		SPAN_NON_REC
//#define 	STRUCTURE_MODIFY
//#define 	FREQUENCY_CONSTANT
//#define		TEST_CHARACTERISTIC
//#define 	ELEMENT_CONSTANT
/// perform the numerical analysis.
// #define		NUM_ANALYSIS
//numerical analysis when the value of one symbol is changed.
//#define		NUM_ANALYSIS_MUL
//calculate the value of differential
//#define		NUM_ANALYSIS_DIFFERENTIAL_SINGLE
//calculate the value of differential with the frequency varied
//#define 		NUM_ANALYSIS_DIFFERENTIAL_FREQ
/// display the staticistics of the TPDD.
#define		DISP_STATISTICS	 

/// print the info of the graph converted from the circuit.
// #define	PRINT_GRAPH_INFO

/// recusively construct the TPDD.
//#define		SPAN_REC

///	non-recusively construct the TPDD, perform reduce during TPDD construction.
//#define		SPAN_NON_REC_REDUCE

/// print all the nodes in TPDD.
//#define		PRINT_NODES

/// print all the terms in TPDD.
//#define		PRINT_TERMS

/// print transfer function of the TPDD.
//#define		PRINT_TRANS

/// extract the coefficients of the s-variable.
#define     CALC_SCOEFFICIENTS

/// print the coefficients of the s-variable with different orders.
//#define		PRINT_SCOEFFICIENTS

/// hash the scoeffNode.
//#define HASHSCOEFFNODE //

/// numerical analysis the transfer function with the coefficients of s-variable extracted.
#define SCOEFF_ANALYSIS

/// Approximate analysis
//#define SPAN_NON_REC_APPROXIMATE

//#define CALC_SIGNIFICANT_SYMBS

#define EXPTPDD

#define PRINT_TO_FILE

// Do NOT use parallel computing because of unknown bugs!!! 2009-06-24
// #define PARALLEL_COMPUTING
#define MADIMING
//#define CONSIDER_MATCHED
#endif

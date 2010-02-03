/**************************************************************************************
*        Symbolic Circuit Analysis With TPDDs (Tree Pair Decision Diagram)
* tpdd_global.h
* global definitons.
*
* CHEN Weiwei		GShi	06/03/31
* School of Microelectronics, Shanghai Jiaotong Univ.
* {chenweiwei, shiguoyong}@ic.sjtu.edu.cn
**************************************************************************************/

#ifndef TPDDG_H
#define TPDDG_H

#include "utility.h"

#define		SUBTREE_PRIME 203311
//#define		TPDD_PRIME 218833
//#define		TAG_PRIME1 4093
//#define		TAG_PRIME2 7873
#define		TAG_PRIME1 2183
#define		TAG_PRIME2 1007

#define 	PI 			3.14159265358979323846			

#ifndef DBL_MAX // Li Ji
#define		DBL_MAX		1e20
#endif // DBL_MAX

#define     MAX_APPROX_ORDER  20

// Edge Types.
enum EdgeType	{Y=0, Z, CC, VC, CS, VS};

#endif


#ifndef STDDEFS_H
#define STDDEFS_H

/*
 *    $RCSfile: stddefs.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

/*
 *
 * stddefs.h : standard header file
 * This file should be compatable with c and c++
 *
 */


/* The folowing is for portability between IBM PC's and SUN.
   'integer' is a 32 bit number. */
/* The current PC already has 32 bits for an integer. G.Shi */

#include <iostream>
#include <cstdlib>

#define BITS_PER_INTEGER 32
typedef int                integer;  
typedef unsigned int       uinteger;  
typedef unsigned int       boolean;  
typedef unsigned long       ulong;

#define UCSB            1

#ifndef TRUE
#define TRUE            ((boolean)1)
#endif
#ifndef FALSE
#define FALSE           ((boolean)0)
#endif
#define YES             1
#define NO              0
#define RAD_CODE        YES
#define MAX_CHARS	100
#define SAMPLE_NUM_SENS 63

#define NIL(X)   ((X)0)
#ifndef MAX
#define MAX(X,Y) (((X)>(Y))?(X):(Y))
#endif
#ifndef MIN
#define MIN(X,Y) (((X)>(Y))?(Y):(X))
#endif
#ifndef ABS
#define ABS(X) (((X)>0)?(X):(X)*-1)
#endif
#ifndef SWAP
#define SWAP(T,X,Y) {T S; S=X; X=Y; Y=S;}
#endif

//////////////////////////////////////////////////////////////
// c++ specific macros

#ifdef DEBUG
#define FATAL(x) { std::cerr << "fatal error in file "; \
           std::cerr << __FILE__ << " at line "; \
           std::cerr << __LINE__ << ", " << x << "\n"; \
           *((int*)0) = 42; \
           std::exit(-1); }
#else
#define FATAL(x) { std::cerr << "fatal error in file "; \
           std::cerr << __FILE__ << " at line "; \
           std::cerr << __LINE__ << ", " << x << "\n"; \
           std::exit(-1); }
#endif

#define ASSERT(exp, y) if (!(exp)) FATAL(y);

//////////////////////////////////////////////////////////////

#include <ctime>


//extern void srandom(int seed);
//extern long random(void);

#define SRANDOM() {\
	std::time_t seed = 0; \
	srand( std::time( &seed)); \
}

#endif

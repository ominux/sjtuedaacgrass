
/* globals.h
 *   Contains external (global) variable declarations that are
 *   referenced in multiple files.  globals.h can be included in
 *   in .c file where these variables are referenced, rather than 
 *   repeating extern declarations separately for each use.  Some 
 *   global variables may be declared in other header files, such as 
 *   tfa_in in parseControl.h, since the variable is closely tied to
 *   a single source file.  
 */

/*
 *    $RCSfile: globals.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

#ifndef GLOBALS_H
#define GLOBALS_H

/* common information needed by most of files */
#include "complex_t.h"
#include    <iostream>
#include    <fstream>
#include    <stdlib.h>
#include    <stdio.h>
#include    <assert.h>

#include "unixstd.h"
#include "stddefs.h"
#include "hasher.h"
#include "hashcach.h"


/* Circuit Type */
typedef enum {
    cLinear,
    cNonLinear
} CirType;

/* Define analysis modes: */
typedef enum {
    aAC,
    aDC,
    aTRANSIENT,
    aSENS
} AnalysisType;

/* device type definition */

typedef enum {
    dUNKNOWN,// unknown type
    dRES,    // resistor
    dCAP,    // capacitor
    dIND,    // inductor
    dVCCS,   // voltage controlled current source
    dVCVS,   // voltage controlled voltage source
    dCCVS,   // current controlled voltage source
    dCCCS,   // current controlled current source
    dVOL,    // independent volage
    dCUR,    // independent current
    dSWITCH, // switch
    dDIODE,  // diode
    dBJT,    // bipolar junction transistor
    dJFET,   // junction field effect transistor
    dMOSFET, // mosfet
    dMESFET  // mosfet
} DeviceType;

typedef enum {
    NUM,    // numerator
    DEN    // denominator
} PolyType;

/* Define TRANSIENT model types: */
#define BACK_EULER 0
#define TRAPEZOIDAL 1
#define GEARS 2

/* matrix expansion methods */
typedef enum {
    CLOSED_LOOP,
    EXPANSION,
    SUBSET
} ExpMethod;

/* type of a branch (R, C, L) */
typedef enum {
    ResPart,
    CapPart,
    IndPart,
    Unknown
} BType;

#define ElemPart BType

/* Global vairable declarations */

extern class Circuit *theCkt; // the top (main) ckt  
extern class SubCircuit *theCurCkt; // current ckt being passed
// These are originally defined in "yainit.cc".

#include "message.h"
#include "utility.h"

#endif /* ifndef  GLOBALS_H */

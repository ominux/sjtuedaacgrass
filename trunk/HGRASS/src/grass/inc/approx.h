#ifndef _APPROXIMATE_H
#define _APPROXIMATE_H
 
/*
 *    $RCSfile: approx.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

 /*
 ////////////////////////////////////////////////////
 //
 //  aproxiate.h : 
 //     head file for DDD approximation 
 //     ddd - determinant decision diagram
 //
 //  X. Sheldon Tan
 //  (c) Copyright the University of Iowa, 1997
 //
 //
 ///////////////////////////////////////////////////
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include "stddef.h"
#include "dtstruct.h"

extern void AfterApproximation(DDDmanager *, DDDnode *, double , double );

#define dWHITE (bool)0
#define dBLACK (bool)1

#define INF_WEIGHT 1e60
extern DDDstack *theTopoList;
extern DDDstack *theCurPath;

extern int UseSimpExp; // use the simplified expression 
extern int EnBranchElim; // enable branch elimination mechnasim
extern int UseSimpExp2; // use the simplified expression 

#endif


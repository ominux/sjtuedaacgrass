#ifndef UTILITY_H
#define UTILITY_H


/*
 *    $RCSfile: utility.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */

/*
** The header file for the utility functions
*/

#include	"globals.h"

extern char *CopyStr(const char *str);
extern char *ToUpper(char *str);
extern char *ToLower(char *str);
extern char *NextField(char **str);
extern double    TransValue(const char *value);
extern double    RandVar(double lbound, double ubound);
extern DeviceType WhichType(const char *keyword);
extern void Sort(int*, double*, int);

extern int ddd2coeff(int);
extern int coeff2ddd(int);

#endif //UTILITY_H


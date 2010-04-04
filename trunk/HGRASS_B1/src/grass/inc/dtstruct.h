#ifndef BASIC_STRUCT_H
#define BASIC_STRUCT_H

/*
 *    $RCSfile: dtstruct.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */


#include <stdio.h>
#include "dddbase.h"
 /*
 ////////////////////////////////////////////////////
 //
 //  dtstruct.h :
 //  head file some basic data structure  
 //  definition used in our system
 //    
 //  X. Sheldon Tan
 //  (c) Copyright the University of Iowa, 1997
 //
 //
 ///////////////////////////////////////////////////
 */


/* 
**    Some basic data definition (stack, queue ...) 
*/
 
////////////////////////////////////////////////
//Stack definition
///////////////////////////////////////////////

typedef struct _StackEle {
    int key;
    struct _StackEle *next;
} iSTACKELE;


class iStack {
    iSTACKELE *head;

    public:
    iStack() {head = NULL;}
    void Free();
    ~iStack(){Free();}
    void FreeR(iSTACKELE *list );
    int Pop();
    void Push(int val);
};

////////////////////////////////////////////////
//DDD Queue definition
///////////////////////////////////////////////

typedef struct _DDDmember {
    DDDnode    *ddd;
    struct _DDDmember *next;
} DDDmember;

class DDDqueue {

    DDDmember   *dddlist;
    DDDmember   *head;
    DDDmember   *end;
    int         num; // #element in queue

    public:
    DDDqueue() {dddlist=NULL;head=NULL;end=NULL;num=0;}
    ~DDDqueue();

    void Enquque(DDDnode *ddd);
    DDDnode * Dequque();
};

////////////////////////////////////////////////
//DDD Stack definition
///////////////////////////////////////////////

class DDDstack {

    DDDmember   *head;
    int         num; // #element in queue

    public:
    DDDstack() {head=NULL;num=0;}
    ~DDDstack(){};

    void DDDpush(DDDnode *ddd);
    DDDnode * DDDpop();
    int    isEmpty() { if(!head) return 1; else return 0;}
    int    StackSize() { return num; }
    void ClearStack() { head=NULL; num=0;}
};

#endif //BASIC_STRUCT_H

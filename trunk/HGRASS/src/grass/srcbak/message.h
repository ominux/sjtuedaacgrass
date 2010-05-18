
#ifndef MESSAGE_H
#define MESSAGE_H

/*
 *    $RCSfile: message.h,v $
 *    $Revision: 1.1 $
 *    $Date: 1999/04/29 19:16:38 $
 */


#include <iostream>
#include <cstdio>

#define FAT_ERROR    0        // fatal error
#define INT_ERROR    1        // internal error
#define IO_ERROR    2        // I/O file error
#define PARSE_ERROR 3        // parse error
#define SYN_ERROR     4        // syntex error

/* function interface delaratioins */
extern void print_error(int code, const char *mesg);
extern void error_mesg(int code, const char *mesg); 
extern void print_mesg(const char *mesg); 
extern void print_warn(const char *mesg); 
extern void print_raw(const char *mesg);


#define print_error(code, mesg) \
{ \
	std::cout <<"IN FILE: "<<__FILE__<<"  LINE: "<<__LINE__<<std::endl;\
    error_mesg(code,mesg); \
}

extern char _buf[256]; // used for buffering output message

#endif //MESSAGE_H


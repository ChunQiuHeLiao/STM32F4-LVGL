#ifndef __DEBUG_H
#define __DEBUG_H
#include"main.h"
#include"stdarg.h"


#define EN_DEBUG 


#ifdef EN_DEBUG
#define Debug_Printf(fmt,...)  printf("%s:%d:",__FILE__, __LINE__,##__VA_ARGS__)
#else 
#define Debug_Printf(fmt,...) ((void)0)



#endif
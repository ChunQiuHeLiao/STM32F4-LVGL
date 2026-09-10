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


extern TaskHandle_t debugHandle;


/*串口命令调试任务: cmd:value 例如  print_task_info:1 */
void Debug_Init();

#endif

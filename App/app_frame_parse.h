#ifndef __APP_FRAME_PARSE_H
#define __APP_FRAME_PARSE_H

#include"main.h"
#include"app_task.h"
#include"transfer_protocol.h"


extern RingBuffer_t uartRingBuf;
extern DataFrame_t uartDf;


void AppFrameParse_Init();

#endif

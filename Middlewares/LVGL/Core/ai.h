#ifndef __AI_H
#define __AI_H
#include "lvgl.h"
#include"Core/main_lib.h"

enum
{
    AI_INDEX_XUN_FEI_LITE=0, /*讯飞轻量化的AI*/
    AI_INDEX_BAI_DU_LITE, /*百度轻量化的AI*/
    AI_INDEX_BAI_DU_SPEED, /*百度高性能的AI*/
    AI_INDEX_BAI_DU_TINY /*百度超高性能的AI*/
};



typedef struct
{
    uint8_t isUserSend : 1;

    uint8_t isAISend : 1;
    uint8_t isSwitchAI : 1;

    uint8_t aiIndex; /*AI索引，即使用哪个AI。 AI_INDEX_xxx */
}AI_Handle;

extern AI_Handle aiHandle;

char* AI_GetResponse(const char* dialog);
#endif

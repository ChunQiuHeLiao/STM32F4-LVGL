#ifndef __IWDG_H
#define __IWDG_H
#include"main.h"


/* STM32F4 的 LSI 约 32KHz, 64 分频后约 500Hz(1计数=2ms),
 * RLR 最大 0xFFF, 所以看门狗最长时间约 8190ms。建议喂狗间隔 1~2s。 */
#define IWDG_MAX_TIMEOUT_MS (8000)


uint8_t IWDG_Init(uint16_t ms);
uint8_t IWDG_Reflesh();

#endif

#include"iwdg.h"

/*!
 * 独立看门狗(IWDG)
 * 本工程没有裁剪进 HAL 的 IWDG 驱动, 所以这里直接操作寄存器。
 * STM32F4 的 LSI 约 32KHz。
 *   分频 64  -> 计数频率 = 32000/64 = 500Hz -> 1 个计数 = 2ms
 *   RLR 最大 0xFFF(4095) -> 最长约 8190ms
 */


/// @brief 看门狗初始化
/// @param ms 喂狗最大时间(ms), 超过该时间不喂狗就复位
/// @return 0成功
uint8_t IWDG_Init(uint16_t ms)
{
    uint16_t reload;

    if(ms==0) ms=1000;
    if(ms>IWDG_MAX_TIMEOUT_MS) ms=IWDG_MAX_TIMEOUT_MS;

    reload = ms/2;              /* 500Hz -> 1计数=2ms */
    if(reload<2) reload=2;
    if(reload>0x0FFF) reload=0x0FFF;

    /* 1. 启动看门狗(第一次写 0xCCCC 才会真正启动) */
    IWDG->KR = 0x0000CCCCU;

    /* 2. 允许写 PR / RLR */
    IWDG->KR = 0x00005555U;
    IWDG->PR = 0x04U;           /* 64 分频 */
    IWDG->RLR = reload;

    /* 3. 喂一次狗, 把 RLR 装载进计数器 */
    IWDG->KR = 0x0000AAAAU;

    return 0;
}



/// @brief 喂狗，即重装载计数器的值
/// @return 0成功
uint8_t IWDG_Reflesh()
{
    IWDG->KR = 0x0000AAAAU;
    return 0;
}

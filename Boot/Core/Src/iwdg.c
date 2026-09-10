#include"iwdg.h"

static IWDG_HandleTypeDef iwdgTypedef={0};

/// @brief 看门狗喂狗最大时间(ms)
/// @param ms 毫秒(目前使用64分频，所以最多看门狗就6.56s)，建议可以用1 或 2秒
uint8_t IWDG_Init(uint16_t ms)
{
    iwdgTypedef.Instance=IWDG;
    iwdgTypedef.Init.Prescaler=IWDG_PRESCALER_64; //F103C8T6 LSI 40KHz -->625Hz
    iwdgTypedef.Init.Reload=ms*625/1000;
    HAL_StatusTypeDef ret=HAL_IWDG_Init(&iwdgTypedef);
    if(ret!=HAL_OK)
    {
        LOG_E(ret,"HAL_IWDG_Init");
    }

    return ret;
}



/// @brief 喂狗，即重装载值
/// @return 
uint8_t IWDG_Reflesh()
{
    HAL_StatusTypeDef ret=HAL_IWDG_Refresh(&iwdgTypedef);
    if(ret!=HAL_OK)
    {
        LOG_E(ret,"HAL_IWDG_Refresh");
    }

    return ret;
}
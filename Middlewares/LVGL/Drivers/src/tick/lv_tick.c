/**
 * @file lv_tick.c
 * Provide access to the system tick with 1 millisecond resolution
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_tick_private.h"
#include "../misc/lv_types.h"
#include "../core/lv_global.h"

/*********************
 *      DEFINES
 *********************/
#define state LV_GLOBAL_DEFAULT()->tick_state

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

LV_ATTRIBUTE_TICK_INC void lv_tick_inc(uint32_t tick_period)
{
    lv_tick_state_t * state_p = &state;

    state_p->sys_irq_flag = 0;
    state_p->sys_time += tick_period;
}
// #include"FreeRTOS.h"
// #include"task.h"
uint32_t lv_tick_get(void)
{
    //return xTaskGetTickCount(); 
    
    lv_tick_state_t * state_p = &state;

    if(state_p->tick_get_cb)
        return state_p->tick_get_cb();

    /*If `lv_tick_inc` is called from an interrupt while `sys_time` is read
     *the result might be corrupted.
     *This loop detects if `lv_tick_inc` was called while reading `sys_time`.
     *If `tick_irq_flag` was cleared in `lv_tick_inc` try to read again
     *until `tick_irq_flag` remains `1`.*/
    uint32_t result;
    do {
        state_p->sys_irq_flag = 1;
        result        = state_p->sys_time;
    } while(!state_p->sys_irq_flag); /*Continue until see a non interrupted cycle*/

    return result;
}

uint32_t lv_tick_elaps(uint32_t prev_tick)
{
    uint32_t act_time = lv_tick_get();

    /*If there is no overflow in sys_time simple subtract*/
    if(act_time >= prev_tick) {
        prev_tick = act_time - prev_tick;
    }
    else {
        prev_tick = UINT32_MAX - prev_tick + 1;
        prev_tick += act_time;
    }

    return prev_tick;
}

#include"utility.h"
void lv_delay_ms(uint32_t ms)
{
    // if(state.delay_cb) 
    // {
    //     state.delay_cb(ms);
    // }
    // else 
    // {
    //     uint32_t t = lv_tick_get();
    //     while(lv_tick_elaps(t) < ms) {
    //         /*Do something to no call `lv_tick_elaps` too often as it might interfere with interrupts*/
    //         volatile uint32_t i;
    //         volatile uint32_t x = ms;
    //         for(i = 0; i < 100; i++) {
    //             x = x * 3;
    //         }
    //     }
    // }
    uint32_t t1=HAL_GetTick();
    
    uint16_t times=ms/LV_DEF_REFR_PERIOD;
    uint8_t t=0;
    if(times==0) 
    {
        lv_timer_handler();
        return;
    }
    
    while (times--)
    {
        t=lv_timer_handler();
        delay_ms(t); 
    } 

    uint32_t dif_t=HAL_GetTick()-t1;
    // printf("ms:%d,dif_t:%d,%d\n",ms,dif_t,ms-dif_t);
    
    if((int)(ms-dif_t)>0) /*注意转有符号整型*/
    {
        // printf("enter\n");
        delay_ms(ms-dif_t);
    }

    // printf("dif t:%d,%d\n",HAL_GetTick()-t1,t);
}

void lv_tick_set_cb(lv_tick_get_cb_t cb)
{
    state.tick_get_cb = cb;
}

void lv_delay_set_cb(lv_delay_cb_t cb)
{
    state.delay_cb = cb;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

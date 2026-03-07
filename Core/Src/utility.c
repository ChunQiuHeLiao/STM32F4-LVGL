#include"utility.h"
#include"stdarg.h"
/*
注意：延迟函数部分 使用了就不能用HAL_Delay()
*/



uint8_t us_num=0; //达到1us所需要的时钟震荡次数，即72MHz Systick 需要经过72个周期才是1us

//填写自己芯片最大的时钟频率(72)。切记 使用了自定义的延迟函数后就不能使用
//官方提供的HAL_Delay()了，因为这个也是SysTick实现的
void delay_init(uint16_t sysclk)
{
    #if DELAY_MODE_SYSTICK==1 

    SysTick->CTRL=0;
    //时钟8分频
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8); 
    us_num=sysclk/8; //因为这里用了8分频
    #endif

    #if DELAY_MODE_TIMER==1
  // 1. 使能TIM7时钟
    RCC->APB1ENR |= DELAY_RCC_APB1ENR_TIMxEN;
    
    // 2. 配置TIM7预分频器
    // 假设系统时钟为84MHz，预分频为84-1，使计数器频率为1MHz(1us计数一次)
    DELAY_TIMx->PSC = sysclk - 1;
    
    // 3. 配置TIM7为单次模式(向上计数)
    DELAY_TIMx->CR1 &= ~TIM_CR1_DIR; // 向上计数
    DELAY_TIMx->CR1 |= TIM_CR1_OPM;  // 单脉冲模式
    
    // 4. 使能TIM7更新中断(可选)
    // DELAY_TIMx->DIER |= TIM_DIER_UIE;
    
    // 5. 配置NVIC(如果使用中断)
    // NVIC_EnableIRQ(TIM7_IRQn);
    // NVIC_SetPriority(TIM7_IRQn, 0);
    
    #endif
}


void delay_us(uint16_t time)
{
    if(time==0) return;
    if(time==1)time=2;

    #if DELAY_MODE_SYSTICK==1

    uint32_t temp=0;
    SysTick->LOAD=time*us_num; //设置重装载值
    SysTick->VAL=0x00; //清空计数器当前的值，计数器值为0后，会自动VAL=LOAD
    SysTick->CTRL|=0x01; //启动SysTick定时器。0位控制定时器启停

    //如果 定时器是启动的且计数器值还没有到0，则继续循环
    do
    {
        temp=SysTick->CTRL;
    }while((temp&0x01)&&!(temp&(1<<16)));
    
    SysTick->CTRL&=(~1); //关闭SysTick
    SysTick->VAL=0; //清空计数器值

    #endif



    //使用定时器延迟
    #if DELAY_MODE_TIMER==1
        // 1. 设置自动重载值
    DELAY_TIMx->ARR = time - 1; // 计数us次
    
    // 2. 清除更新中断标志
    DELAY_TIMx->SR = 0;
    
    // 3. 启动计数器
    DELAY_TIMx->CR1 |= TIM_CR1_CEN;
    // 4. 等待计数器停止(单次模式下计数完成后会自动停止)
    //printf("ti s=%d\n",num);
    while((DELAY_TIMx->CR1 & TIM_CR1_CEN) != 0);
    //printf("ti o=%d\n",num++);
    #endif
}

void delay_ms(uint16_t time)
{
    while (time--) delay_us(1000);
}



/// @brief 设置栈顶地址
/// @param addr 栈顶地址。栈顶地址为程序起始地址。
void SYS_SetMSP(uint32_t appBaseAddr)
{
    __set_MSP(*((volatile uint32_t*)appBaseAddr));
}


/// @brief 设置中断向量表
/// @param baseAddr 程序起始地址(一般0x80000000)
/// @param offset 偏移的地址。拍内衣地址必须是0x200的倍数，即512B的倍数
void SYS_Set_Vector_Table(uint32_t appBaseAddr,uint32_t offset)
{
  SCB->VTOR=appBaseAddr|(offset&(uint32_t)0xFFFFFE00); //0xFFFFFE00 是为了保证偏移地址一定是0x200倍数
}

JumpApp jumpApp;
/// @brief 跳转到指定地址的APP
/// @param appBaseAddr APP基地址
/// @return 1为FLASH里没有APP，2为栈顶地址不合法
uint8_t SYS_JumpApp(uint32_t appBaseAddr)
{
    if (((*(volatile uint32_t *)(appBaseAddr + 4)) & 0xFF000000) == 0x08000000) /* 判断FLASH里面是否有APP,有的话执行 */
    {
        if (((*(volatile  uint32_t *)appBaseAddr) & 0x2FFE0000) == 0x20000000)     /* 检查栈顶地址是否合法.可以放在内部SRAM共64KB(0x20000000) */
        {
            /* 用户代码区第二个字为程序开始地址(复位地址) */
            SYS_SetMSP(appBaseAddr);
            jumpApp=(JumpApp)(*((volatile uint32_t*)(appBaseAddr+4)));    
            jumpApp();
            
        }
        return 2;
    }
    return 1;
}




/* 内部帮手的实现 */
static void _print_int(int v)
{
    char buf[12], * p = buf + sizeof(buf);
    unsigned int u = (v < 0) ? -v : v;
    *--p = '\0';
    do { *--p = '0' + u % 10; } while (u /= 10);
    if (v < 0) *--p = '-';
    while (*p) putchar(*p++);
}

static void _print_str(const char* s)
{
    while (*s) putchar(*s++);
}

static void _print_float(float fv)
{
    int whole = (int)fv;
    int frac = (int)((fv - whole) * 100.0f + 0.5f);
    if (frac < 0)  frac = -frac;
    if (frac >= 100) frac = 99;
    _print_int(whole);
    putchar('.');
    putchar('0' + frac / 10);
    putchar('0' + frac % 10);
}


/*浮点只支持 %.2f */
void my_printf(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            switch (*++fmt) {
            case 'f': _print_float((float)va_arg(ap, double)); break;
            case 'd': _print_int(va_arg(ap, int));              break;
            case 's': _print_str(va_arg(ap, char*));          break;
            case '%': putchar('%');                             break;
            default:  putchar(*fmt);                            break;
            }
        }
        else {
            putchar(*fmt);
        }
        ++fmt;
    }
    va_end(ap);
}



/*自定义 Sprintf*/


/* 内部帮手：现在把字符写到 buf 里，并实时更新 *buf 指针 */
static void _print_int2(char** buf, int v)
{
    char tmp[12], * p = tmp + sizeof(tmp);
    unsigned int u = (v < 0) ? -v : v;
    *--p = '\0';
    do { *--p = '0' + u % 10; } while (u /= 10);
    if (v < 0) *--p = '-';
    while (*p) *(*buf)++ = *p++;
}

static void _print_str2(char** buf, const char* s)
{
    while (*s) *(*buf)++ = *s++;
}

static void _print_float2(char** buf, float fv)
{
    int whole = (int)fv;
    int frac = (int)((fv - whole) * 100.0f + 0.5f);
    if (frac < 0)  frac = -frac;
    if (frac >= 100) frac = 99;
    _print_int2(buf, whole);
    *(*buf)++ = '.';
    *(*buf)++ = '0' + frac / 10;
    *(*buf)++ = '0' + frac % 10;
}

/* 主接口：Sprintf  ----------------------------------------------------- */
uint8_t my_sprintf(char* buf, const char* fmt, ...)
{
    char* start = buf;
    va_list ap;
    va_start(ap, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            switch (*++fmt) {
            case 'f':                    /* %f 一律按 %.2f 处理 */
                _print_float2(&buf, (float)va_arg(ap, double));
                break;
            case 'd':
                _print_int2(&buf, va_arg(ap, int));
                break;
            case 's':
                _print_str2(&buf, va_arg(ap, char*));
                break;
            case '%':
                *buf++ = '%';
                break;
            default:                     /* 不认识就原样输出 */
                *buf++ = *fmt;
                break;
            }
        }
        else {
            *buf++ = *fmt;
        }
        ++fmt;
    }
    *buf = '\0';
    va_end(ap);
    return (uint8_t)(buf - start);   /* 返回写入长度（不含 '\0'） */
}




char strcat_buf[48] = { 0 };
/// @brief 拼接函数，默认用于常量。默认支持最大48字节
/// @return 返回拼接字符串的首地址
char* my_strcat(const char* str1, const char* str2)
{
    snprintf(strcat_buf, sizeof(strcat_buf), "%s%s", str1, str2);
    return strcat_buf;
}




/*--------------------调试用的--------------*/

typedef struct 
{
    const char* moduleName; /*模块名,MODULE_NAME_XXX 如 MODULE_NAME_SPI*/
    uint8_t errCode; /*错误编码*/
    const char* file; /*错误发生所在文件*/
    uint16_t line; /*错误发生所在的代码行号*/
}ErrorInfo_t;
ErrorInfo_t errInfo={0};


/*获取错误的信息*/
void Err_GetInfo(const char* moduleName,uint8_t errCode,const char* file,uint16_t line)
{
    errInfo.moduleName=moduleName;
    errInfo.errCode=errCode;
    errInfo.file=file;
    errInfo.line=line;
}

/// @brief 打印错误信息
/// @param file 打印函数所在的文件
/// @param line 打印函数所在行号
void Err_Print(const char* file,uint16_t line)
{
    printf("[%s,%d] module:%s,errCode:%d,file:%s,line:%d\n",file,line,\
        errInfo.moduleName,errInfo.errCode,errInfo.file,errInfo.line);
}


/*精简版*/
void PrintError(const char* file,uint16_t line,uint8_t errCode)
{
    printf("[%s,%d] errCode:%d\n",file,line,errCode);
}

/*-----------------------------------*/
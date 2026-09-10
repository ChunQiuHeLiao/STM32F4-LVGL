#include"utility.h"
#include"stm_flash.h"
/*
注意：延迟函数部分 使用了就不能用HAL_Delay()
*/


SysConfigInfo_t sysConfInfo={0};


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
    uint32_t MSPVal= *((volatile uint32_t*)appBaseAddr);
    uint32_t resetHandlerAddr= *((volatile uint32_t*)(appBaseAddr+4)); //复位函数的地址

    printf("%08x,%08x\r\n",MSPVal,resetHandlerAddr);

    // SYS_SetMSP(appBaseAddr);
    // jumpApp=(JumpApp)(*((volatile uint32_t*)(appBaseAddr+4)));    
    // jumpApp();
    
    
    if (((*(volatile uint32_t *)(appBaseAddr + 4)) & 0xFF000000) == 0x08000000) /* 判断FLASH里面是否有APP,有的话执行 */
    {
        if (((*(volatile  uint32_t *)appBaseAddr) & 0x2FFE0000) == 0x20000000)     /* 检查栈顶地址是否合法.可以放在内部SRAM共64KB(0x20000000) */
        {
            /* 用户代码区第二个字为程序开始地址(复位地址) */
            SYS_SetMSP(appBaseAddr);
            jumpApp=(JumpApp)(*((volatile uint32_t*)(appBaseAddr+4)));    
            jumpApp();
        }

        LOG_E(2,"MSP addr err:%08x\r\n", (*(volatile uint32_t*)(appBaseAddr)));
        return 2;
    }
    LOG_E(1,"reset handler addr err:%08x\r\n", (*(volatile uint32_t*)(appBaseAddr+4)));
    return 1;
}


/// @brief 软件复位
void Sys_SoftReset()
{
    //复位前清理，避免软件复位后 RCC/Flash 状态异常导致 Boot 时钟初始化失败(进 Error_Handler 死循环)
    //1. 等待 Flash 空闲(升级刚写完 Flash，需等 BSY 清零)
    while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);

    //2. 把系统时钟切回 HSI(关闭 PLL/HSE)，复位后 Boot 重新配置时钟时不会因 PLL 残留状态而失败
    // RCC->CR &= ~RCC_CR_PLLON;                      // 关 PLL
    // while((RCC->CR & RCC_CR_PLLRDY) != 0);          // 等 PLLRDY 清零
    // RCC->CFGR &= (uint32_t)(~RCC_CFGR_SW);          // SYSCLK = HSI
    // while((RCC->CFGR & RCC_CFGR_SWS) != 0);         // 等切回 HSI

    //3. 关闭所有普通中断，只屏蔽普通中断，不屏蔽 HardFault 等异常和不可屏蔽中断
    __disable_irq();

    //4. 开始复位
    __NVIC_SystemReset();
}





/// @brief 写入一个字节
/// @param rb 
/// @param data 
static int RingBuffer_WriteByte(RingBuffer_t* rb,uint8_t data)
{
    if(rb->cnt==rb->size) return RES_ERR;

    rb->data[rb->tail]=data;
    rb->tail=(rb->tail+1) % rb->size; 
    rb->cnt++;

    return RES_OK;
}

/// @brief 读取一个字节
/// @param rb 
/// @param data 读取的数据
static int RingBuffer_ReadByte(RingBuffer_t* rb,uint8_t* data)
{
    if(rb->cnt==0) return RES_ERR;

    *data=rb->data[rb->head];
    rb->head=(rb->head+1) % rb->size; 
    rb->cnt--;

    return RES_OK;
}



/// @brief 环形缓存区初始化
/// @param rb 
/// @param size 
/// @return 0成功，其它失败
int RingBuffer_Init(RingBuffer_t* rb,uint16_t size)
{
#ifdef USE_FreeRTOS
    rb->data =pvPortMalloc(sizeof(uint8_t)*size);
    if(rb->data==NULL)
    {
        LOG_E(RES_ERR,"pvPortMalloc");
        return RES_ERR;
    }

    rb->rwMutex=xSemaphoreCreateMutex();
    if(rb->rwMutex==NULL)
    {
        LOG_E(-1,"xSemaphoreCreateMutex");
        vPortFree(rb->data);
        return RES_ERR;
    }

#else
    rb->data =malloc(sizeof(uint8_t)*size);
    if(rb->data==NULL)
    {
        LOG_E(RES_ERR,"malloc");
        return RES_ERR;
    }
#endif

    rb->size=size;
    rb->cnt=0;
    rb->head=0;
    rb->tail=0;

    return RES_OK;
}



/// @brief 写入的数据大小
/// @param rb 
/// @param data 
/// @param size 
/// @return 返回写入的数据大小
int RingBuffer_Write(RingBuffer_t* rb,uint8_t* data,uint16_t size)
{
    if(rb->data==NULL) 
    {
        DEBUG_E(-1,"data is null");
        return -1;
    }

#ifdef USE_FreeRTOS
    if(rb->rwMutex==NULL)
    {
        DEBUG_E(-1,"rwMutex is null");
        return -1;
    }
#endif

    int ret=RES_OK;
    int writeByte=0; //写入缓存区的大小

#ifdef USE_FreeRTOS
    xSemaphoreTake(rb->rwMutex,portMAX_DELAY); //拿锁
#endif

    for(uint16_t i=0;i<size;i++)
    {
        ret=RingBuffer_WriteByte(rb,data[i]);
        if(ret!=RES_OK) 
        {
            DEBUG_E(ret,"RingBuffer_WriteByte full");
            break;
        }

        writeByte++;
    }

#ifdef USE_FreeRTOS
    xSemaphoreGive(rb->rwMutex); //放锁
#endif

    return writeByte;
}


/// @brief 读取缓存区数据
/// @param rb 
/// @param data 
/// @param size 
/// @return 返回实际读取的数据大小
int RingBuffer_Read(RingBuffer_t* rb,uint8_t* data,uint16_t size)
{
    if(rb->data==NULL) 
    {
        DEBUG_E(-1,"data is null");
        return -1;
    }

#ifdef USE_FreeRTOS
    if(rb->rwMutex==NULL)
    {
        DEBUG_E(-1,"rwMutex is null");
        return -1;
    }
#endif

    int ret=RES_OK;
    int readByte=0; //读取缓存区的大小

#ifdef USE_FreeRTOS
    xSemaphoreTake(rb->rwMutex,portMAX_DELAY); //拿锁
#endif

    for(uint16_t i=0;i<size;i++)
    {
        ret=RingBuffer_ReadByte(rb,&data[i]);
        if(ret!=RES_OK) 
        {
            DEBUG_E(ret,"RingBuffer_ReadByte empty");
            break;
        }

        readByte++;
    }

#ifdef USE_FreeRTOS
    xSemaphoreGive(rb->rwMutex); //放锁
#endif

    return readByte;
}


int RingBuffer_Peak(RingBuffer_t* rb,uint8_t* data,uint16_t size)
{

}


/// @brief 释放环形缓存区内存
/// @param rb 
void RingBuffer_DeInit(RingBuffer_t* rb)
{
    if(rb->data==NULL) 
    {
        DEBUG_E(-1,"data is null");
        return;
    }

#ifdef USE_FreeRTOS
    if(rb->rwMutex==NULL)
    {
        DEBUG_E(-1,"rwMutex is null");
        return;
    }
#endif

    #ifdef USE_FreeRTOS
    vPortFree(rb->data);
    vSemaphoreDelete(rb->rwMutex);
    rb->rwMutex=NULL;
    #else
    free(rb->data);
    rb->data=NULL;
    #endif
    rb->size=0;
    rb->cnt=0;
    rb->head=rb->tail=0;
}
















/*----------------工具部分--------------*/

/*-------------1.CRC 软件校验--------------*/

 
static const unsigned long crc32_tbl[256] = 
{
    0x00000000L, 0x77073096L, 0xEE0E612CL, 0x990951BAL, 0x076DC419L,
    0x706AF48FL, 0xE963A535L, 0x9E6495A3L, 0x0EDB8832L, 0x79DCB8A4L,
    0xE0D5E91EL, 0x97D2D988L, 0x09B64C2BL, 0x7EB17CBDL, 0xE7B82D07L,
    0x90BF1D91L, 0x1DB71064L, 0x6AB020F2L, 0xF3B97148L, 0x84BE41DEL,
    0x1ADAD47DL, 0x6DDDE4EBL, 0xF4D4B551L, 0x83D385C7L, 0x136C9856L,
    0x646BA8C0L, 0xFD62F97AL, 0x8A65C9ECL, 0x14015C4FL, 0x63066CD9L,
    0xFA0F3D63L, 0x8D080DF5L, 0x3B6E20C8L, 0x4C69105EL, 0xD56041E4L,
    0xA2677172L, 0x3C03E4D1L, 0x4B04D447L, 0xD20D85FDL, 0xA50AB56BL,
    0x35B5A8FAL, 0x42B2986CL, 0xDBBBC9D6L, 0xACBCF940L, 0x32D86CE3L,
    0x45DF5C75L, 0xDCD60DCFL, 0xABD13D59L, 0x26D930ACL, 0x51DE003AL,
    0xC8D75180L, 0xBFD06116L, 0x21B4F4B5L, 0x56B3C423L, 0xCFBA9599L,
    0xB8BDA50FL, 0x2802B89EL, 0x5F058808L, 0xC60CD9B2L, 0xB10BE924L,
    0x2F6F7C87L, 0x58684C11L, 0xC1611DABL, 0xB6662D3DL, 0x76DC4190L,
    0x01DB7106L, 0x98D220BCL, 0xEFD5102AL, 0x71B18589L, 0x06B6B51FL,
    0x9FBFE4A5L, 0xE8B8D433L, 0x7807C9A2L, 0x0F00F934L, 0x9609A88EL,
    0xE10E9818L, 0x7F6A0DBBL, 0x086D3D2DL, 0x91646C97L, 0xE6635C01L,
    0x6B6B51F4L, 0x1C6C6162L, 0x856530D8L, 0xF262004EL, 0x6C0695EDL,
    0x1B01A57BL, 0x8208F4C1L, 0xF50FC457L, 0x65B0D9C6L, 0x12B7E950L,
    0x8BBEB8EAL, 0xFCB9887CL, 0x62DD1DDFL, 0x15DA2D49L, 0x8CD37CF3L,
    0xFBD44C65L, 0x4DB26158L, 0x3AB551CEL, 0xA3BC0074L, 0xD4BB30E2L,
    0x4ADFA541L, 0x3DD895D7L, 0xA4D1C46DL, 0xD3D6F4FBL, 0x4369E96AL,
    0x346ED9FCL, 0xAD678846L, 0xDA60B8D0L, 0x44042D73L, 0x33031DE5L,
    0xAA0A4C5FL, 0xDD0D7CC9L, 0x5005713CL, 0x270241AAL, 0xBE0B1010L,
    0xC90C2086L, 0x5768B525L, 0x206F85B3L, 0xB966D409L, 0xCE61E49FL,
    0x5EDEF90EL, 0x29D9C998L, 0xB0D09822L, 0xC7D7A8B4L, 0x59B33D17L,
    0x2EB40D81L, 0xB7BD5C3BL, 0xC0BA6CADL, 0xEDB88320L, 0x9ABFB3B6L,
    0x03B6E20CL, 0x74B1D29AL, 0xEAD54739L, 0x9DD277AFL, 0x04DB2615L,
    0x73DC1683L, 0xE3630B12L, 0x94643B84L, 0x0D6D6A3EL, 0x7A6A5AA8L,
    0xE40ECF0BL, 0x9309FF9DL, 0x0A00AE27L, 0x7D079EB1L, 0xF00F9344L,
    0x8708A3D2L, 0x1E01F268L, 0x6906C2FEL, 0xF762575DL, 0x806567CBL,
    0x196C3671L, 0x6E6B06E7L, 0xFED41B76L, 0x89D32BE0L, 0x10DA7A5AL,
    0x67DD4ACCL, 0xF9B9DF6FL, 0x8EBEEFF9L, 0x17B7BE43L, 0x60B08ED5L,
    0xD6D6A3E8L, 0xA1D1937EL, 0x38D8C2C4L, 0x4FDFF252L, 0xD1BB67F1L,
    0xA6BC5767L, 0x3FB506DDL, 0x48B2364BL, 0xD80D2BDAL, 0xAF0A1B4CL,
    0x36034AF6L, 0x41047A60L, 0xDF60EFC3L, 0xA867DF55L, 0x316E8EEFL,
    0x4669BE79L, 0xCB61B38CL, 0xBC66831AL, 0x256FD2A0L, 0x5268E236L,
    0xCC0C7795L, 0xBB0B4703L, 0x220216B9L, 0x5505262FL, 0xC5BA3BBEL,
    0xB2BD0B28L, 0x2BB45A92L, 0x5CB36A04L, 0xC2D7FFA7L, 0xB5D0CF31L,
    0x2CD99E8BL, 0x5BDEAE1DL, 0x9B64C2B0L, 0xEC63F226L, 0x756AA39CL,
    0x026D930AL, 0x9C0906A9L, 0xEB0E363FL, 0x72076785L, 0x05005713L,
    0x95BF4A82L, 0xE2B87A14L, 0x7BB12BAEL, 0x0CB61B38L, 0x92D28E9BL,
    0xE5D5BE0DL, 0x7CDCEFB7L, 0x0BDBDF21L, 0x86D3D2D4L, 0xF1D4E242L,
    0x68DDB3F8L, 0x1FDA836EL, 0x81BE16CDL, 0xF6B9265BL, 0x6FB077E1L,
    0x18B74777L, 0x88085AE6L, 0xFF0F6A70L, 0x66063BCAL, 0x11010B5CL,
    0x8F659EFFL, 0xF862AE69L, 0x616BFFD3L, 0x166CCF45L, 0xA00AE278L,
    0xD70DD2EEL, 0x4E048354L, 0x3903B3C2L, 0xA7672661L, 0xD06016F7L,
    0x4969474DL, 0x3E6E77DBL, 0xAED16A4AL, 0xD9D65ADCL, 0x40DF0B66L,
    0x37D83BF0L, 0xA9BCAE53L, 0xDEBB9EC5L, 0x47B2CF7FL, 0x30B5FFE9L,
    0xBDBDF21CL, 0xCABAC28AL, 0x53B39330L, 0x24B4A3A6L, 0xBAD03605L,
    0xCDD70693L, 0x54DE5729L, 0x23D967BFL, 0xB3667A2EL, 0xC4614AB8L,
    0x5D681B02L, 0x2A6F2B94L, 0xB40BBE37L, 0xC30C8EA1L, 0x5A05DF1BL,
    0x2D02EF8DL
};
 

//分段CRC32校验
void CRC32_Init(CRC32_CTX *ctx)
{
    ctx->crc = 0xFFFFFFFFL;
}
 
void CRC32_Update(CRC32_CTX *ctx, const unsigned char *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        ctx->crc = (ctx->crc >> 8) ^ crc32_tbl[(ctx->crc & 0xFF) ^ *data++];
    }
}
 
void CRC32_Final(CRC32_CTX *ctx, uint32_t *finalCrcVal)
{
    // 标准 IEEE CRC32：取反即得到最终校验值，无需再做字节交换。
    // 若某些上位机工具要求小端字节序输出，应在打印/组包时处理，而不是在这里交换。
    ctx->crc ^= 0xFFFFFFFFUL;
    *finalCrcVal = ctx->crc;
}


//整段CRC32 校验
uint32_t CRC32_Calculate(uint8_t* data,uint16_t size)
{
    uint32_t crcVal=0;
    CRC32_CTX ctx={0};
    CRC32_Init(&ctx);
    CRC32_Update(&ctx,data,size);
    CRC32_Final(&ctx,&crcVal);

    return crcVal;
}   



/*-----------------2.CRC16校验-----------------*/

static const uint16_t crc_ta_8[256]={ /* CRC 字节余式表 */
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};
 
 
/*CRC16_Calculate;按字节计算CRC
  函数参数：uint8_t *ptr:指向发送缓冲区的首字节
            uint32_t len:要发送的总字节数
  函数返回值：uint16_t
  多项式采用CRC-CCITT 0x1021
*/
uint16_t CRC16_Calculate(uint8_t *ptr, uint32_t len)
{
    uint16_t  crc = 0xffff;
 
    while(len-- != 0)
    {
        uint16_t high = (unsigned int)(crc/256); //取CRC高8位
        crc <<= 8;
        crc ^= crc_ta_8[high^*ptr];
        ptr++;
    }
 
    return crc;
}





/*----------------系统配置部分-----------------*/



/// @brief 从FLASH中获取系统信息
/// @return 0成功
int SysConfInfo_Get()
{
    uint8_t ret=0;
    //1.先从存放系统配置的FLASH中 读取这部分数据
    STM_FLASH_ReadData(SYS_CFG_START_ADDR,(uint8_t*)&sysConfInfo,sizeof(SysConfigInfo_t));

    //2.判断是不是第一次运行程序，判断出厂标志是不是0xff,是就是出厂了
    if(sysConfInfo.factoryFlag==0xff) 
    {
        //printf("%d\r\n",sizeof(SysConfigInfo_t));
        memset(&sysConfInfo,0,sizeof(SysConfigInfo_t)); //第一次 全部清零

        sysConfInfo.factoryFlag=0;
        printf("first enter this exe\r\n");
        sysConfInfo.factoryFirmware.size=FACTOR_FIRMWARE_SIZE;
        sysConfInfo.factoryFirmware.crc32=FACTOR_FIRMWARE_CRC32;

        ret=STM_FLASH_Write(SYS_CFG_START_ADDR,(uint8_t*)&sysConfInfo,sizeof(SysConfigInfo_t));
        if(ret!=0)
        {
            LOG_E(ret,"write factory firmware info fail\r\n");
            return ret;
        }
    }

    LOG_I("----sys info----\r\n");

    LOG_I("update flag:%d\r\n",sysConfInfo.isUpdate);
    LOG_I("factory size:%lu\r\n",sysConfInfo.factoryFirmware.size);
    LOG_I("factory crc32:%08x\r\n",sysConfInfo.factoryFirmware.crc32);
    LOG_I("new firmware size:%lu\r\n",sysConfInfo.updateFireware.size);
    LOG_I("new firmware crc32:%08x\r\n",sysConfInfo.updateFireware.crc32);

    LOG_I("----sys info end----\r\n");


    return 0;
}



int SysConfInfo_Set()
{
    uint8_t ret=STM_FLASH_Write(SYS_CFG_START_ADDR,(uint8_t*)&sysConfInfo,sizeof(SysConfigInfo_t));
    if(ret!=0)
    {
        LOG_E(ret,"SysConfInfo_Set fail\r\n");
        return ret;
    }

    return 0;
}
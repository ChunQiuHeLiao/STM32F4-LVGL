该项目是基于STM32+LVGL+FreeRTOS实现的，里面有10个小的app。分别为设置(里面有连接WIFI和OTA固件升级功能)，日历、天气、文件浏览器、相册、视频(不支持播放音频)、音乐、摄影(暂未开始实现)、AI对话、录音。对应的功能我就不赘述了，想必大家也知道，里面的功能都是些基础功能。

使用的硬件有STM32F411RET6，触摸LCD(触摸芯片为xpt2046，显示驱动为ILI9341)，ESP8266，INMP441麦克风模块、MAX98357音频放大器 。

Core文件包含TM32外设驱动代码。

My_Drivers包含各模块驱动代码。

Boot为Bootloader程序

Middlewares是一些第三方库，包括FATFS，LVGL库。
- FreeRTOS目录为FreeRTOS源码
- FATFS/Core 目录包含基于 FATFS 库封装的文件系统操作接口
- VGL/Core 目录定义 各小app的句柄结构体；
- LVGL/SquareLine 目录存放由 SquareLine Studio 生成的 UI 界面描述代码；
- LVGL/mid 目录作为 UI 界面代码与业务逻辑的适配层，负责将 LVGL 界面事件与底层业务逻辑进行关联
- LVGL/Font目录为常用3000+汉字字体库

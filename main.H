/**********************************  Ver3.3 说明 ********************************************************************
  (1) 系统工作时钟频率可以在main.c中修改 SysClock赋值（单位Hz）。
      如：code long SysClock=11059200; 定义系统工作时钟频率为11059200Hz（也即11.0592MHz）		
      系统工作频率必须与实际工作频率（下载时选择的）一致，以免与定时相关的所有功能出现误差或错误。
	(2) 使用方法：
	        1，在工程中加载main.c文件和STC_BSP.lib库文件
					2，在main.c中选择包含以下头文件（如果要使用可选模块提供的函数和方法，就必须包含其头文件）	：
								#include "STC15F2K60S2.H"        //必须，"STC-B学习板"使用MCU指定的头文件
								#include "sys.H"                 //必须，sys（MySTC_OS调度程序）头文件
								#include "display.H"             //可选，display（显示模块）头文件。          
								#include "key.H"                 //可选，key（按键模块）头文件。
								#include "hall.H"                //可选，hall（霍尔传感器模块）头文件。
								#include "Vib.h"	               //可选，Vib（振动传感器模块）头文件。								 
								#include "beep.H"                //可选，beep（蜂鸣器模块）头文件。					
								#include "music.h"               //可选，music（音乐播放）头文件。	
								#include "adc.h"                 //可选，adc（热敏、光敏、导航按键、扩展接口ADC功能）头文件。	
								#include "uart1.h"               //可选，uart1（串口1通信）头文件。
								#include "uart2.h"               //可选，uart2（串口2通信）头文件。
							  #include "stepmotor.h"           //可选，步进电机      
                #include "DS1302.h"              //可选，DS1302实时时钟
                #include "M24C02.h"              //可选，24C02非易失性存储器
                #include "FM_Radio.h"	           //可选，FM收音机
                #include "EXT.h"                 //可选，EXT扩展接口（电子秤、超声波测距、旋转编码器、PWM输出控制电机快慢和正反转）
                #include "IR.h"	                 //可选，38KHz红外通信
								
					3，MySTC_Init()是sys初始化函数，必须执行一次；
					   MySTC_OS()是sys调度函数，应置于while（1）循环中；
					4，各可选模块如果选用，必须在使用模块其它函数和方法前执行一次模块所提供的驱动函数（设置相关硬件、并在sys中加载其功能调度）：
						    DisplayerInit();      //显示模块驱动 
								Key_Init();           //按键模块驱动
								BeepInit();	          //蜂鸣器模块驱动
								MusicPlayerInit();    //蜂鸣器播放音乐驱动
	              HallInit();           //霍尔传感器模块驱动                      
	              VibInit();            //振动传感器模块驱动          
	              AdcInit();            //模数转换ADC模块驱动（含温度、光照、导航按键与按键Key3、EXT扩展接口上的ADC）
	              StepMotorInit();      //步进电机模块驱动	                  
    	          DS1302Init();         //DS1302实时时钟驱动
	              FMRadioInit();	      //FM收音机驱动           
                EXTInit();            //扩展接口驱动（含电子秤、超声波测距、旋转编码器、PWM输出，但不含EXT上Uart2和与之相关应用）
	              Uart1Init();          //Uart1（串口1）驱动：USB上（与计算机通信）              
                Uart2Init();          //Uart2（串口2）驱动：485接口、或EXT扩展接口（多机通信、Uart方式模块如蓝牙模块） 
	              IrInit();             //38KHz红外通信模块驱动
								
							说明：有部分模块不需要驱动；驱动函数有些有参数。（具体见各模块头文件说明）
								
          5，sys和各模块共提供以下事件：
					      numEventSys1mS:              1mS事件                        （"1毫秒时间间隔到"事件）
					      enumEventSys10mS:            10mS事件                       （"10毫秒时间间隔到"事件）
				        enumEventSys100mS:           100mS事件                      （"100毫秒时间间隔到"事件）
								enumEventSys1S:              1S事件                         （"1秒时间间隔到"事件）
								enumEventKey:                按键事件                       （K1、K2、K3 三个按键有"按下"或"抬起"操作）
								enumEventHall:               霍尔传感器事件                 （霍尔传感器有"磁场接近"或"磁场离开"事件）
 								enumEventVib:                振动传感器事件                 （振动感器检测到"振动"事件）
								enumEventNav:                导航按键事件                   （导航按键5个方向、或按键K3 有"按下"或"抬起"操作）
								enumEventXADC:               扩展接口上完成一次ADC转换事件  （P1.0、P1.1采取到一组新数据）
								enumEventUart1Rxd:           Uart1收到了一个符合指定要求（数据包头匹配、数据包大小一致）的数据包
	              enumEventUart2Rxd:           Uart2收到了一个符合指定要求（数据包头匹配、数据包大小一致）的数据包   
	              enumEventIrRxd:              红外接收器Ir上收到一个数据包						
								
								对这些事件，应采用"回调函数"方法响应（即用sys提供的SetEventCallBack()设置用户回调函数），以提高系统性能。
								
					6，各可选模块提供的其它函数和具体使用方法请参见：
					      各模块头文件中的说明；
								main.c提供的推荐程序框架和部分示例；
								其它可能技术文档或应用示例
						
编写：徐成（电话18008400450）   2021年2月26日设计，2021年9月1日更新
**************************************************************************/

#include "STC15F2K60S2.H"        //必须。
#include "sys.H"                 //必须。
#include "displayer.H" 
#include "key.h"
#include "hall.h"
#include "Vib.h"
#include "beep.h"
#include "music.h"
#include "adc.h"
#include "uart1.h"
#include "uart2.h"
#include "stepmotor.h"
#include "DS1302.h"
#include "M24C02.h"
#include "FM_Radio.h"	
#include "EXT.h"
#include "IR.h"

code unsigned long SysClock=11059200L;         //必须。定义系统工作时钟频率(Hz)，用户必须修改成与实际工作频率（下载时选择的）一致
#ifdef _displayer_H_                          //显示模块选用时必须。（数码管显示译码表，用戶可修改、增加等） 
code char decode_table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x08,0x40,0x01, 0x41, 0x48, 
	              /* 序号:   0   1    2	   3    4	    5    6	  7   8	   9	 10	   11		12   13    14     15     */
                /* 显示:   0   1    2    3    4     5    6    7   8    9  (无)   下-  中-  上-  上中-  中下-   */  
	                       0x3f|0x80,0x06|0x80,0x5b|0x80,0x4f|0x80,0x66|0x80,0x6d|0x80,0x7d|0x80,0x07|0x80,0x7f|0x80,0x6f|0x80 };  
             /* 带小数点     0         1         2         3         4         5         6         7         8         9        */
#endif
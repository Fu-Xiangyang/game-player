/********************************************************************************
  * 文 件 名: main.c
  * 版 本 号: 初版
  * 修改作者: LC
  * 修改日期: 2022年04月12日
  * 功能介绍:          
  ******************************************************************************
  * 注意事项:
*********************************************************************************/

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "bsp_led.h"
#include "sys.h"
#include "bsp_usart.h"
#include "bsp_key.h"
#include "bsp_basic_timer.h"
#include "bsp_pwm.h"
#include "stdlib.h"
#include "string.h"
#include "bsp_dma.h"

#include "adc_key.h"
#include "Lcd.h"
#include "Gui.h"
#include "pic.h"
//#include "tiane.h"
//#include "tianee.h"
#include "gmx.h"

//#include "i2c.h"
//#include "eeprom.h"
//#include "dac_audio.h"
//#include "audio_source.h"


/************************************************
函数名称 ： main
功    能 ： 主函数
参    数 ： 无
返 回 值 ： 无
作    者 ： LC
*************************************************/
int main(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);  // 优先级分组

    /* 核心板 */
    systick_config();          					// 滴答定时器初始化
    led_gpio_config();   								// led初始化
    key_gpio_config(); 				 					// key初始化
    usart_gpio_config(115200U);  				// 串口0初始化

    /* 扩展板 */
    Lcd_Init();                // spi lcd初始化

    /* SPI LCD */
    LCD_Fill(0,0,LCD_W,LCD_H,COLOR_BLUE); // 深蓝色 背景
	
	LCD_ShowPicture(0,27,280,186,gImage_gmx);//绘制图片
    while(1);
}

	//LCD_ShowPicture((LCD_W-222) / 2,0,222,60,gImage_lCKFB);         // 显示图片
    //LCD_DrawRectangle(10,60 + 10,LCD_W -10,LCD_H -10,COLOR_BLACK);   // 绘制一个矩形
    //LCD_ShowChinese(15,60 + 10 + 5,"开始游戏",COLOR_WHITE,COLOR_BLUE,12,0);   // 显示12汉字字符串
    //LCD_ShowChinese(15,60 + 10 + 5 + 12,"开始游戏",COLOR_WHITE,COLOR_BLUE,16,0);    // 显示16汉字字符串
    //LCD_ShowChinese(15,60 + 10 + 5 + 12 + 16,"开始游戏",COLOR_WHITE,COLOR_BLUE,24,0);    // 显示24汉字字符串
    //LCD_ShowChinese(15,60 + 10 + 5 + 12 + 16 + 24 ,"开始游戏",COLOR_WHITE,COLOR_BLUE,32,0);   // 显示32汉字字符串
    //LCD_ShowString(15,60 + 10 + 5 + 12 + 16 + 24 + 32 ,"123abc",COLOR_WHITE,COLOR_BLUE,16,0);  // 显示16字符串
   //LCD_ShowString(15,60 + 10 + 5 + 12 + 16 + 24 + 32 +16,"123abc",COLOR_WHITE,COLOR_BLUE,24,0);  // 显示24字符串
   // LCD_ShowString(15,60 + 10 + 5 + 12 + 16 + 24 + 32 + 16 + 24,"123abc",COLOR_WHITE,COLOR_BLUE,32,0);  // 显示32字符串
	

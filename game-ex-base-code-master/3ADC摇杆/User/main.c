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
#include "i2c.h"
#include "eeprom.h"
#include "dac_audio.h"
#include "audio_source.h"



/* 计算整数的数字的位数 */
int Num_count(int num)
{
    int wei = 0;
		if(num == 0)
			wei = 1;
    while(num>0){
        wei++;
        num /= 10;
    }
    return wei;
}

/* 显示摇杆进度条 */
void RockerProgressBar(uint16_t startx,uint16_t starty,uint16_t value)
{
	  int temp;
		LCD_Fill(startx,starty + ((24 - 16)/2),50 + (value/24),starty + ((24 - 16)/2) + 16,COLOR_GREEN);   // 进度条绿色
		LCD_Fill(startx + (value/24),starty + ((24 - 16)/2),220,starty + ((24 - 16)/2) + 16,COLOR_WHITE);   // 进度条白色
	  /* 清除原来显示的值 */
	  LCD_Fill(startx,starty + ((24 - 16)/2) + 16,220 + 10,starty + ((24 - 16)/2) + 16 + 16,COLOR_BLUE);   // 蓝色
	  /* 显示当前值 */
	  temp = startx + (value/24);
	  if(temp > (220 - 30))
				temp = 220 - 30;
		LCD_ShowIntNum(temp,starty + ((24 - 16)/2) + 16,value,Num_count(value),COLOR_RED,COLOR_BLUE,16);    // 显示x的值 
}


/************************************************
函数名称 ： main
功    能 ： 主函数
参    数 ： 无
返 回 值 ： 无
作    者 ： LC
*************************************************/
int main(void)
{
    uint8_t keyTimes = 0;  // 开机次数
    uint16_t temp;  // 临时变量
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);  // 优先级分组

    /* 核心板 */
    systick_config();          					// 滴答定时器初始化
    led_gpio_config();   								// led初始化
    key_gpio_config(); 				 					// key初始化
    usart_gpio_config(115200U);  				// 串口0初始化

    /* 扩展板 */
    adc_config();              // 摇杆初始化   
    Lcd_Init();                // spi lcd初始化

    /* SPI LCD */
    LCD_Fill(0,0,LCD_W,LCD_H,COLOR_BLUE); // 深蓝色 背景
    LCD_ShowPicture((LCD_W-222) / 2,0,222,60,gImage_lCKFB);         // 显示图片
    LCD_DrawRectangle(10,60 + 10,LCD_W -10,LCD_H -10,COLOR_BLACK);   // 绘制一个矩形

     
    /* 先来获取一下遥感的位置 */
    adc_key_scan();
    /* x轴 进度条的长度最大为220 - 50 = 170, 然后值是从0-4095 共4096 平均一个像素点是24 */
    temp = 75;
    LCD_ShowString(15,temp,"X:",COLOR_WHITE,COLOR_BLUE,24,0);  // 显示16字符串
    RockerProgressBar(50,temp,adcXValue);

    /* y轴 */
    temp = temp + 32 + 5;
    LCD_ShowString(15,temp,"Y:",COLOR_WHITE,COLOR_BLUE,24,0);  // 显示16字符串
    RockerProgressBar(50,temp,adcYValue);

    while(1) 
    {
        /* 摇杆 */
        adc_key_scan(); // 摇杆扫描
        RockerProgressBar(50,75,adcXValue);  // x轴
        RockerProgressBar(50,75 + 16 + 16 + 5,adcYValue);  // y轴

    }
}

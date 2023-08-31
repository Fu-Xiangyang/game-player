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
#include "adc_bat.h"


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
//电池进度条显示
void RockerProgressBar(uint16_t startx,uint16_t starty,uint16_t value)
{
	  int temp;
		LCD_Fill(startx,starty + ((24 - 16)/2),startx + ((value-2500)/9),starty + ((24 - 16)/2) + 16,COLOR_GREEN);   // ?????????
		LCD_Fill(startx + ((value-2500)/9),starty + ((24 - 16)/2),220,starty + ((24 - 16)/2) + 16,COLOR_WHITE);   // ?????????
	  //清除数字
	  LCD_Fill(startx,starty + ((24 - 16)/2) + 16,220 + 10,starty + ((24 - 16)/2) + 16 + 16,COLOR_BLUE);   // ???
	  //显示数字
	  temp = startx + ((value-2500)/9);
	  if(temp > (220 - 30))
				temp = 220 - 30;
		LCD_ShowIntNum(temp,starty + ((24 - 16)/2) + 16,value,Num_count(value),COLOR_RED,COLOR_BLUE,16);    // ???x??? 
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
    uint8_t Times = 0;  // 次数
    uint16_t temp;  // 临时变量
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);  // 优先级分组

    /* 核心板 */
    systick_config();          					// 滴答定时器初始化
    led_gpio_config();   								// led初始化
    key_gpio_config(); 				 					// key初始化
    usart_gpio_config(115200U);  				// 串口0初始化

    /* 扩展板 */
    bat_io_init();             //电池管理初始化
    adc_config();              // 摇杆初始化   
    Lcd_Init();                // spi lcd初始化


    /* SPI LCD */
    LCD_Fill(0,0,LCD_W,LCD_H,COLOR_BLUE); // 深蓝色 背景
    LCD_ShowPicture((LCD_W-222) / 2,0,222,60,gImage_lCKFB);         // 显示图片
    LCD_DrawRectangle(10,60 + 10,LCD_W -10,LCD_H -10,COLOR_BLACK);   // 绘制一个矩形
    LCD_ShowString(20,75,"BAT VCC: 2.4-4.2V",COLOR_WHITE,COLOR_BLUE,24,0); // 显示16字符串
    
    while(1) 
    {
        temp = adc_bat_get_value();
        RockerProgressBar(20,95,temp);
        delay_1ms(100);
        
        if(++Times> 100) 
        {
            Times=0;
            BAT_EN_ONFF();//关机
        }
    }
}

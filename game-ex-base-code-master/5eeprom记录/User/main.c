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
    Lcd_Init();                // spi lcd初始化
    i2c_eeprom_init();         // eeprom初始化

    /* EEPROM */
    eeprom_buffer_read_timeout(&keyTimes,0x00,1);	  // 先读出来keyTimes
    keyTimes ++;
    eeprom_buffer_write_timeout(&keyTimes,0x00,1);	// 再写入keyTimes

    /* SPI LCD */
    LCD_Fill(0,0,LCD_W,LCD_H,COLOR_BLUE); // 深蓝色 背景
    LCD_ShowPicture((LCD_W-222) / 2,0,222,60,gImage_lCKFB);         // 显示图片
    LCD_DrawRectangle(10,60 + 10,LCD_W -10,LCD_H -10,COLOR_BLACK);   // 绘制一个矩形
    
    LCD_ShowString(15,75,"Power On Number:",COLOR_WHITE,COLOR_BLUE,16,0);
    LCD_ShowString(15,95,"EEPROM:",COLOR_WHITE,COLOR_BLUE,32,0); // 显示32字符串
    LCD_ShowIntNum(15 + 7*16,95,keyTimes,Num_count(keyTimes),COLOR_RED,COLOR_BLUE,32);
    
    
    while(1);
}

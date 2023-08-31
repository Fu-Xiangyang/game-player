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



/************************************************
函数名称 ： main
功    能 ： 主函数
参    数 ： 无
返 回 值 ： 无
作    者 ： LC
*************************************************/
int main(void)
{
    uint16_t temp;  // 临时变量
    uint8_t motor;
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);  // 优先级分组

    /* 核心板 */
    systick_config();          					// 滴答定时器初始化
    led_gpio_config();   								// led初始化
    key_gpio_config(); 				 					// key初始化
    usart_gpio_config(115200U);  				// 串口0初始化

    /* 扩展板 */
    //adc_config();              // 摇杆初始化   
    Lcd_Init();                // spi lcd初始化
    pwm_config(200,1000);      // 震动马达


    /* SPI LCD */
    LCD_Fill(0,0,LCD_W,LCD_H,COLOR_BLUE); // 深蓝色 背景
    LCD_ShowPicture((LCD_W-222) / 2,0,222,60,gImage_lCKFB);         // 显示图片
    LCD_DrawRectangle(10,60 + 10,LCD_W -10,LCD_H -10,COLOR_BLACK);   // 绘制一个矩形

    temp = 75;
    LCD_ShowString(15,temp,"Press KEY-A Motor Toggle!!",COLOR_WHITE,COLOR_BLUE,16,0);
    temp = temp+20;
    LCD_ShowChinese(15,temp,"震动马达：",COLOR_WHITE,COLOR_BLUE,16,0);    // 显示16汉字字符串

    while(1) 
    {
        /* 震动马达 */
        if(motor != pwm_motor_status)  
        {
            motor = pwm_motor_status;
            if(motor)// 震动马达状态开启 
            {
                Draw_Solid_Circle(15 + 100,95+8,8,COLOR_GREEN);  // 显示实心圆
            }
            else// 震动马达状态关闭 
            {
                Draw_Solid_Circle(15 + 100,95+8,8,COLOR_WHITE);  // 显示实心圆
                pwmMotorSetValue(0); // 关闭震动马达
            }
        }
        
        if(motor)// 震动马达状态开启 
        {
            for(temp=200;temp<1000;temp++)
            {
                pwmMotorSetValue(temp);
                delay_1ms(1);
            }
            for(;temp>200;temp--)
            {
                pwmMotorSetValue(temp);
                delay_1ms(1);
            }
            pwmMotorSetValue(0);
            delay_1ms(50);
        }
        delay_1ms(20);
        
    }
}

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
    uint8_t keyTimes = 0;  // 开机次数
    uint16_t temp;  // 临时变量
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);  // 优先级分组

    /* 核心板 */
    systick_config();          					// 滴答定时器初始化
    led_gpio_config();   								// led初始化
    key_gpio_config(); 				 					// key初始化
    usart_gpio_config(115200U);  				// 串口0初始化

    /* 扩展板 */
    //adc_config();              // 摇杆初始化   
    Lcd_Init();                // spi lcd初始化
    dac_audio_init(22000);     // dac音频初始化

    /* SPI LCD */
    LCD_Fill(0,0,LCD_W,LCD_H,COLOR_BLUE); // 深蓝色 背景
    LCD_ShowPicture((LCD_W-222) / 2,0,222,60,gImage_lCKFB);         // 显示图片
    LCD_DrawRectangle(10,60 + 10,LCD_W -10,LCD_H -10,COLOR_BLACK);   // 绘制一个矩形
     
    /**/
    temp = 75;
    LCD_ShowString(15,temp,"Press KEY-B Music Toggle!!",COLOR_WHITE,COLOR_BLUE,16,0);
    temp = temp + 20;
    LCD_ShowChinese(15,temp,"背景音乐：",COLOR_WHITE,COLOR_BLUE,16,0);    // 显示16汉字字符串
    Draw_Solid_Circle(15 + 100,temp + 8,8,COLOR_WHITE);  // 显示实心圆		

    while(1) 
    {
        /* 音频 */
        if(audio_status == 1)  // 开启音乐
        {
            audio_status = 3;
            Draw_Solid_Circle(15 + 100,temp + 8,8,COLOR_GREEN);  // 显示实心圆
            start_play_audio(audioSouce_Background,sizeof(audioSouce_Background),1); // 播放音乐 单次播放
        }else if(audio_status == 2) //状态关闭 
        {
            audio_status = 0;
          Draw_Solid_Circle(15 + 100,temp + 8,8,COLOR_WHITE);  // 显示实心圆
            stop_play_audio();
        }
    }
}

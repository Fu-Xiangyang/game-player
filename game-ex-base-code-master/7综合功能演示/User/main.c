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
void RockerProgressBar1(uint16_t startx,uint16_t starty,uint16_t value)
{
    int temp;

    if(value >= 2500 && value <= 4200) //2.4V
    {
        LCD_Fill(startx,starty + ((24 - 16)/2),startx + ((value-2500)/10),starty + ((24 - 16)/2) + 16,COLOR_GREEN);   // 进度条绿色
        LCD_Fill(startx + ((value-2500)/10),starty + ((24 - 16)/2),220,starty + ((24 - 16)/2) + 16,COLOR_WHITE);   // 进度条白色
        /* 清除原来显示的值 */
        LCD_Fill(startx,starty + ((24 - 16)/2) + 16,220 + 10,starty + ((24 - 16)/2) + 16 + 16,COLOR_BLUE);   // 蓝色
        /* 显示当前值 */
        temp = startx + ((value-2500)/10);
        if(temp > (220 - 30)) temp = 220 - 30;
        LCD_ShowIntNum(temp,starty + ((24 - 16)/2) + 16,value,Num_count(value),COLOR_RED,COLOR_BLUE,16);    // 显示x的值 
    }
    else if(value < 2500)
    {
        LCD_Fill(startx ,starty + ((24 - 16)/2),220,starty + ((24 - 16)/2) + 16,COLOR_WHITE);    // 进度条白色
        LCD_ShowIntNum(startx,starty + ((24 - 16)/2) + 16,value,Num_count(value),COLOR_RED,COLOR_BLUE,16);    // 显示x的值 
    }
    else //>4200
    {
        LCD_Fill(startx,starty + ((24 - 16)/2),startx + 220,starty + ((24 - 16)/2) + 16,COLOR_GREEN);  // 进度条绿色
        LCD_ShowIntNum(220-30,starty + ((24 - 16)/2) + 16,value,Num_count(value),COLOR_RED,COLOR_BLUE,16);    // 显示x的值 
    }

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
    uint16_t Times = 0;  // 次数
    uint8_t keyTimes = 0;// 开机次数
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
    pwm_config(200,1000);      // 震动马达
    i2c_eeprom_init();         // eeprom初始化
    dac_audio_init(22000);     // dac音频初始化

    /* EEPROM */
    eeprom_buffer_read_timeout(&keyTimes,0x00,1);	  // 先读出来keyTimes
    keyTimes ++;
    eeprom_buffer_write_timeout(&keyTimes,0x00,1);	// 再写入keyTimes

    /* SPI LCD */
    LCD_Fill(0,0,LCD_W,LCD_H,COLOR_BLUE); // 深蓝色 背景
    LCD_ShowPicture((LCD_W-222) / 2,0,222,60,gImage_lCKFB);         // 显示图片
    LCD_DrawRectangle(10,60 + 10,LCD_W -10,LCD_H -10,COLOR_BLACK);   // 绘制一个矩形
    LCD_ShowChinese(15,60 + 10 + 5,"开始游戏",COLOR_WHITE,COLOR_BLUE,12,0);   // 显示12汉字字符串
    LCD_ShowChinese(15,60 + 10 + 5 + 12,"开始游戏",COLOR_WHITE,COLOR_BLUE,16,0);    // 显示16汉字字符串
    LCD_ShowChinese(15,60 + 10 + 5 + 12 + 16,"开始游戏",COLOR_WHITE,COLOR_BLUE,24,0);    // 显示24汉字字符串
    LCD_ShowChinese(15,60 + 10 + 5 + 12 + 16 + 24 ,"开始游戏",COLOR_WHITE,COLOR_BLUE,32,0);   // 显示32汉字字符串
    LCD_ShowString(15,60 + 10 + 5 + 12 + 16 + 24 + 32 ,"123abc",COLOR_WHITE,COLOR_BLUE,16,0);  // 显示16字符串
    LCD_ShowString(15,60 + 10 + 5 + 12 + 16 + 24 + 32 +16,"123abc",COLOR_WHITE,COLOR_BLUE,24,0);  // 显示24字符串
    LCD_ShowString(15,60 + 10 + 5 + 12 + 16 + 24 + 32 + 16 + 24,"123abc",COLOR_WHITE,COLOR_BLUE,32,0);  // 显示32字符串
    LCD_ShowString(15,60 + 10 + 5 + 12 + 16 + 24 + 32 + 16 + 24 + 32,"EEPROM:",COLOR_RED,COLOR_BLUE,32,0);  // 显示32字符串
    LCD_ShowIntNum(15 + 7*16,60 + 10 + 5 + 12 + 16 + 24 + 32 + 16 + 24 + 32,keyTimes,Num_count(keyTimes),COLOR_RED,COLOR_BLUE,32);     // 显示EEPROM保存的数据 记录开机的次数

    pwmMotorSetValue(1000); // 打开震动马达  
    delay_1ms(500);  // 延迟3s
    pwmMotorSetValue(0); // 关震动马达 
    LCD_Fill(15,75,LCD_W -10,LCD_H -10,COLOR_BLUE); // 蓝色 背景
     
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

    temp = temp + 32 + 5;
    LCD_ShowString(15,temp+4,"VCC:",COLOR_WHITE,COLOR_BLUE,16,0);   // 显示16字符串
    RockerProgressBar1(50,temp,adc_bat_get_value());
    
    /* 震动马达 */
    temp = temp + 32 + 20;
    LCD_ShowChinese(15,temp,"震动马达：",COLOR_WHITE,COLOR_BLUE,16,0);    // 显示16汉字字符串
    /* 背景音乐：*/
    temp = temp + 16 + 20;
    LCD_ShowChinese(15,temp,"背景音乐：",COLOR_WHITE,COLOR_BLUE,16,0);    // 显示16汉字字符串
    Draw_Solid_Circle(15 + 100,temp + 8,8,COLOR_WHITE);  // 显示实心圆		

    while(1) 
    {

        /* 摇杆 */
        adc_key_scan(); // 摇杆扫描
        RockerProgressBar(50,75,adcXValue);  // x轴
        RockerProgressBar(50,75 + 16 + 16 + 5,adcYValue);  // y轴
        RockerProgressBar1(50,149,adc_bat_get_value());//显示电压
        
        /* 震动马达 如果用摇杆控制的话 就是1:4*/
        if(pwm_motor_status)  // 震动马达状态开启 
        {
            Draw_Solid_Circle(15 + 100,temp - 16 - 20 + 8,8,COLOR_GREEN);  // 显示实心圆
            pwmMotorSetValue(adcYValue / 4); // 设置震动马达
        }else  // 震动马达状态关闭 
        {
            Draw_Solid_Circle(15 + 100,temp - 16 - 20 + 8,8,COLOR_WHITE);  // 显示实心圆
            pwmMotorSetValue(0); // 关闭震动马达
        }

        /* 音频 */
        if(audio_status == 1)  // 开启音乐
        {
            audio_status = 3;
            Draw_Solid_Circle(15 + 100,temp + 8,8,COLOR_GREEN);  // 显示实心圆
            start_play_audio(audioSouce_Background,sizeof(audioSouce_Background),1); // 播放音乐 单次播放
        }else if(audio_status == 2) // 震动马达状态关闭 
        {
            audio_status = 0;
          Draw_Solid_Circle(15 + 100,temp + 8,8,COLOR_WHITE);  // 显示实心圆
            stop_play_audio();
        }

        delay_1ms(20);
        if(++Times > 500) 
        {
            Times=0;
            BAT_EN_ONFF();//电池关机
        }
    }
}

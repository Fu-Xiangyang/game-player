/********************************************************************************
  * �� �� ��: main.c
  * �� �� ��: ����
  * �޸�����: LC
  * �޸�����: 2022��04��12��
  * ���ܽ���:          
  ******************************************************************************
  * ע������:
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
�������� �� main
��    �� �� ������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� LC
*************************************************/
int main(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);  // ���ȼ�����

    /* ���İ� */
    systick_config();          					// �δ�ʱ����ʼ��
    led_gpio_config();   								// led��ʼ��
    key_gpio_config(); 				 					// key��ʼ��
    usart_gpio_config(115200U);  				// ����0��ʼ��

    /* ��չ�� */
    Lcd_Init();                // spi lcd��ʼ��

    /* SPI LCD */
    LCD_Fill(0,0,LCD_W,LCD_H,COLOR_BLUE); // ����ɫ ����
	
	LCD_ShowPicture(0,27,280,186,gImage_gmx);//����ͼƬ
    while(1);
}

	//LCD_ShowPicture((LCD_W-222) / 2,0,222,60,gImage_lCKFB);         // ��ʾͼƬ
    //LCD_DrawRectangle(10,60 + 10,LCD_W -10,LCD_H -10,COLOR_BLACK);   // ����һ������
    //LCD_ShowChinese(15,60 + 10 + 5,"��ʼ��Ϸ",COLOR_WHITE,COLOR_BLUE,12,0);   // ��ʾ12�����ַ���
    //LCD_ShowChinese(15,60 + 10 + 5 + 12,"��ʼ��Ϸ",COLOR_WHITE,COLOR_BLUE,16,0);    // ��ʾ16�����ַ���
    //LCD_ShowChinese(15,60 + 10 + 5 + 12 + 16,"��ʼ��Ϸ",COLOR_WHITE,COLOR_BLUE,24,0);    // ��ʾ24�����ַ���
    //LCD_ShowChinese(15,60 + 10 + 5 + 12 + 16 + 24 ,"��ʼ��Ϸ",COLOR_WHITE,COLOR_BLUE,32,0);   // ��ʾ32�����ַ���
    //LCD_ShowString(15,60 + 10 + 5 + 12 + 16 + 24 + 32 ,"123abc",COLOR_WHITE,COLOR_BLUE,16,0);  // ��ʾ16�ַ���
   //LCD_ShowString(15,60 + 10 + 5 + 12 + 16 + 24 + 32 +16,"123abc",COLOR_WHITE,COLOR_BLUE,24,0);  // ��ʾ24�ַ���
   // LCD_ShowString(15,60 + 10 + 5 + 12 + 16 + 24 + 32 + 16 + 24,"123abc",COLOR_WHITE,COLOR_BLUE,32,0);  // ��ʾ32�ַ���
	

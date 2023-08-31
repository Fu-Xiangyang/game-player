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
#include "i2c.h"
#include "eeprom.h"
#include "dac_audio.h"
#include "audio_source.h"



/* �������������ֵ�λ�� */
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

/* ��ʾҡ�˽����� */
void RockerProgressBar(uint16_t startx,uint16_t starty,uint16_t value)
{
	  int temp;
		LCD_Fill(startx,starty + ((24 - 16)/2),50 + (value/24),starty + ((24 - 16)/2) + 16,COLOR_GREEN);   // ��������ɫ
		LCD_Fill(startx + (value/24),starty + ((24 - 16)/2),220,starty + ((24 - 16)/2) + 16,COLOR_WHITE);   // ��������ɫ
	  /* ���ԭ����ʾ��ֵ */
	  LCD_Fill(startx,starty + ((24 - 16)/2) + 16,220 + 10,starty + ((24 - 16)/2) + 16 + 16,COLOR_BLUE);   // ��ɫ
	  /* ��ʾ��ǰֵ */
	  temp = startx + (value/24);
	  if(temp > (220 - 30))
				temp = 220 - 30;
		LCD_ShowIntNum(temp,starty + ((24 - 16)/2) + 16,value,Num_count(value),COLOR_RED,COLOR_BLUE,16);    // ��ʾx��ֵ 
}


/************************************************
�������� �� main
��    �� �� ������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� LC
*************************************************/
int main(void)
{
    uint8_t keyTimes = 0;  // ��������
    uint16_t temp;  // ��ʱ����
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);  // ���ȼ�����

    /* ���İ� */
    systick_config();          					// �δ�ʱ����ʼ��
    led_gpio_config();   								// led��ʼ��
    key_gpio_config(); 				 					// key��ʼ��
    usart_gpio_config(115200U);  				// ����0��ʼ��

    /* ��չ�� */
    adc_config();              // ҡ�˳�ʼ��   
    Lcd_Init();                // spi lcd��ʼ��

    /* SPI LCD */
    LCD_Fill(0,0,LCD_W,LCD_H,COLOR_BLUE); // ����ɫ ����
    LCD_ShowPicture((LCD_W-222) / 2,0,222,60,gImage_lCKFB);         // ��ʾͼƬ
    LCD_DrawRectangle(10,60 + 10,LCD_W -10,LCD_H -10,COLOR_BLACK);   // ����һ������

     
    /* ������ȡһ��ң�е�λ�� */
    adc_key_scan();
    /* x�� �������ĳ������Ϊ220 - 50 = 170, Ȼ��ֵ�Ǵ�0-4095 ��4096 ƽ��һ�����ص���24 */
    temp = 75;
    LCD_ShowString(15,temp,"X:",COLOR_WHITE,COLOR_BLUE,24,0);  // ��ʾ16�ַ���
    RockerProgressBar(50,temp,adcXValue);

    /* y�� */
    temp = temp + 32 + 5;
    LCD_ShowString(15,temp,"Y:",COLOR_WHITE,COLOR_BLUE,24,0);  // ��ʾ16�ַ���
    RockerProgressBar(50,temp,adcYValue);

    while(1) 
    {
        /* ҡ�� */
        adc_key_scan(); // ҡ��ɨ��
        RockerProgressBar(50,75,adcXValue);  // x��
        RockerProgressBar(50,75 + 16 + 16 + 5,adcYValue);  // y��

    }
}

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
#include "adc_bat.h"


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
//��ؽ�������ʾ
void RockerProgressBar(uint16_t startx,uint16_t starty,uint16_t value)
{
	  int temp;
		LCD_Fill(startx,starty + ((24 - 16)/2),startx + ((value-2500)/9),starty + ((24 - 16)/2) + 16,COLOR_GREEN);   // ?????????
		LCD_Fill(startx + ((value-2500)/9),starty + ((24 - 16)/2),220,starty + ((24 - 16)/2) + 16,COLOR_WHITE);   // ?????????
	  //�������
	  LCD_Fill(startx,starty + ((24 - 16)/2) + 16,220 + 10,starty + ((24 - 16)/2) + 16 + 16,COLOR_BLUE);   // ???
	  //��ʾ����
	  temp = startx + ((value-2500)/9);
	  if(temp > (220 - 30))
				temp = 220 - 30;
		LCD_ShowIntNum(temp,starty + ((24 - 16)/2) + 16,value,Num_count(value),COLOR_RED,COLOR_BLUE,16);    // ???x??? 
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
    uint8_t Times = 0;  // ����
    uint16_t temp;  // ��ʱ����
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);  // ���ȼ�����

    /* ���İ� */
    systick_config();          					// �δ�ʱ����ʼ��
    led_gpio_config();   								// led��ʼ��
    key_gpio_config(); 				 					// key��ʼ��
    usart_gpio_config(115200U);  				// ����0��ʼ��

    /* ��չ�� */
    bat_io_init();             //��ع����ʼ��
    adc_config();              // ҡ�˳�ʼ��   
    Lcd_Init();                // spi lcd��ʼ��


    /* SPI LCD */
    LCD_Fill(0,0,LCD_W,LCD_H,COLOR_BLUE); // ����ɫ ����
    LCD_ShowPicture((LCD_W-222) / 2,0,222,60,gImage_lCKFB);         // ��ʾͼƬ
    LCD_DrawRectangle(10,60 + 10,LCD_W -10,LCD_H -10,COLOR_BLACK);   // ����һ������
    LCD_ShowString(20,75,"BAT VCC: 2.4-4.2V",COLOR_WHITE,COLOR_BLUE,24,0); // ��ʾ16�ַ���
    
    while(1) 
    {
        temp = adc_bat_get_value();
        RockerProgressBar(20,95,temp);
        delay_1ms(100);
        
        if(++Times> 100) 
        {
            Times=0;
            BAT_EN_ONFF();//�ػ�
        }
    }
}

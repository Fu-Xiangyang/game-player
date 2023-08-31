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
    //adc_config();              // ҡ�˳�ʼ��   
    Lcd_Init();                // spi lcd��ʼ��
    dac_audio_init(22000);     // dac��Ƶ��ʼ��

    /* SPI LCD */
    LCD_Fill(0,0,LCD_W,LCD_H,COLOR_BLUE); // ����ɫ ����
    LCD_ShowPicture((LCD_W-222) / 2,0,222,60,gImage_lCKFB);         // ��ʾͼƬ
    LCD_DrawRectangle(10,60 + 10,LCD_W -10,LCD_H -10,COLOR_BLACK);   // ����һ������
     
    /**/
    temp = 75;
    LCD_ShowString(15,temp,"Press KEY-B Music Toggle!!",COLOR_WHITE,COLOR_BLUE,16,0);
    temp = temp + 20;
    LCD_ShowChinese(15,temp,"�������֣�",COLOR_WHITE,COLOR_BLUE,16,0);    // ��ʾ16�����ַ���
    Draw_Solid_Circle(15 + 100,temp + 8,8,COLOR_WHITE);  // ��ʾʵ��Բ		

    while(1) 
    {
        /* ��Ƶ */
        if(audio_status == 1)  // ��������
        {
            audio_status = 3;
            Draw_Solid_Circle(15 + 100,temp + 8,8,COLOR_GREEN);  // ��ʾʵ��Բ
            start_play_audio(audioSouce_Background,sizeof(audioSouce_Background),1); // �������� ���β���
        }else if(audio_status == 2) //״̬�ر� 
        {
            audio_status = 0;
          Draw_Solid_Circle(15 + 100,temp + 8,8,COLOR_WHITE);  // ��ʾʵ��Բ
            stop_play_audio();
        }
    }
}

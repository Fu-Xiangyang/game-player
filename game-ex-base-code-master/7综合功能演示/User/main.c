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
void RockerProgressBar1(uint16_t startx,uint16_t starty,uint16_t value)
{
    int temp;

    if(value >= 2500 && value <= 4200) //2.4V
    {
        LCD_Fill(startx,starty + ((24 - 16)/2),startx + ((value-2500)/10),starty + ((24 - 16)/2) + 16,COLOR_GREEN);   // ��������ɫ
        LCD_Fill(startx + ((value-2500)/10),starty + ((24 - 16)/2),220,starty + ((24 - 16)/2) + 16,COLOR_WHITE);   // ��������ɫ
        /* ���ԭ����ʾ��ֵ */
        LCD_Fill(startx,starty + ((24 - 16)/2) + 16,220 + 10,starty + ((24 - 16)/2) + 16 + 16,COLOR_BLUE);   // ��ɫ
        /* ��ʾ��ǰֵ */
        temp = startx + ((value-2500)/10);
        if(temp > (220 - 30)) temp = 220 - 30;
        LCD_ShowIntNum(temp,starty + ((24 - 16)/2) + 16,value,Num_count(value),COLOR_RED,COLOR_BLUE,16);    // ��ʾx��ֵ 
    }
    else if(value < 2500)
    {
        LCD_Fill(startx ,starty + ((24 - 16)/2),220,starty + ((24 - 16)/2) + 16,COLOR_WHITE);    // ��������ɫ
        LCD_ShowIntNum(startx,starty + ((24 - 16)/2) + 16,value,Num_count(value),COLOR_RED,COLOR_BLUE,16);    // ��ʾx��ֵ 
    }
    else //>4200
    {
        LCD_Fill(startx,starty + ((24 - 16)/2),startx + 220,starty + ((24 - 16)/2) + 16,COLOR_GREEN);  // ��������ɫ
        LCD_ShowIntNum(220-30,starty + ((24 - 16)/2) + 16,value,Num_count(value),COLOR_RED,COLOR_BLUE,16);    // ��ʾx��ֵ 
    }

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
    uint16_t Times = 0;  // ����
    uint8_t keyTimes = 0;// ��������
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
    pwm_config(200,1000);      // �����
    i2c_eeprom_init();         // eeprom��ʼ��
    dac_audio_init(22000);     // dac��Ƶ��ʼ��

    /* EEPROM */
    eeprom_buffer_read_timeout(&keyTimes,0x00,1);	  // �ȶ�����keyTimes
    keyTimes ++;
    eeprom_buffer_write_timeout(&keyTimes,0x00,1);	// ��д��keyTimes

    /* SPI LCD */
    LCD_Fill(0,0,LCD_W,LCD_H,COLOR_BLUE); // ����ɫ ����
    LCD_ShowPicture((LCD_W-222) / 2,0,222,60,gImage_lCKFB);         // ��ʾͼƬ
    LCD_DrawRectangle(10,60 + 10,LCD_W -10,LCD_H -10,COLOR_BLACK);   // ����һ������
    LCD_ShowChinese(15,60 + 10 + 5,"��ʼ��Ϸ",COLOR_WHITE,COLOR_BLUE,12,0);   // ��ʾ12�����ַ���
    LCD_ShowChinese(15,60 + 10 + 5 + 12,"��ʼ��Ϸ",COLOR_WHITE,COLOR_BLUE,16,0);    // ��ʾ16�����ַ���
    LCD_ShowChinese(15,60 + 10 + 5 + 12 + 16,"��ʼ��Ϸ",COLOR_WHITE,COLOR_BLUE,24,0);    // ��ʾ24�����ַ���
    LCD_ShowChinese(15,60 + 10 + 5 + 12 + 16 + 24 ,"��ʼ��Ϸ",COLOR_WHITE,COLOR_BLUE,32,0);   // ��ʾ32�����ַ���
    LCD_ShowString(15,60 + 10 + 5 + 12 + 16 + 24 + 32 ,"123abc",COLOR_WHITE,COLOR_BLUE,16,0);  // ��ʾ16�ַ���
    LCD_ShowString(15,60 + 10 + 5 + 12 + 16 + 24 + 32 +16,"123abc",COLOR_WHITE,COLOR_BLUE,24,0);  // ��ʾ24�ַ���
    LCD_ShowString(15,60 + 10 + 5 + 12 + 16 + 24 + 32 + 16 + 24,"123abc",COLOR_WHITE,COLOR_BLUE,32,0);  // ��ʾ32�ַ���
    LCD_ShowString(15,60 + 10 + 5 + 12 + 16 + 24 + 32 + 16 + 24 + 32,"EEPROM:",COLOR_RED,COLOR_BLUE,32,0);  // ��ʾ32�ַ���
    LCD_ShowIntNum(15 + 7*16,60 + 10 + 5 + 12 + 16 + 24 + 32 + 16 + 24 + 32,keyTimes,Num_count(keyTimes),COLOR_RED,COLOR_BLUE,32);     // ��ʾEEPROM��������� ��¼�����Ĵ���

    pwmMotorSetValue(1000); // �������  
    delay_1ms(500);  // �ӳ�3s
    pwmMotorSetValue(0); // ������� 
    LCD_Fill(15,75,LCD_W -10,LCD_H -10,COLOR_BLUE); // ��ɫ ����
     
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

    temp = temp + 32 + 5;
    LCD_ShowString(15,temp+4,"VCC:",COLOR_WHITE,COLOR_BLUE,16,0);   // ��ʾ16�ַ���
    RockerProgressBar1(50,temp,adc_bat_get_value());
    
    /* ����� */
    temp = temp + 32 + 20;
    LCD_ShowChinese(15,temp,"����",COLOR_WHITE,COLOR_BLUE,16,0);    // ��ʾ16�����ַ���
    /* �������֣�*/
    temp = temp + 16 + 20;
    LCD_ShowChinese(15,temp,"�������֣�",COLOR_WHITE,COLOR_BLUE,16,0);    // ��ʾ16�����ַ���
    Draw_Solid_Circle(15 + 100,temp + 8,8,COLOR_WHITE);  // ��ʾʵ��Բ		

    while(1) 
    {

        /* ҡ�� */
        adc_key_scan(); // ҡ��ɨ��
        RockerProgressBar(50,75,adcXValue);  // x��
        RockerProgressBar(50,75 + 16 + 16 + 5,adcYValue);  // y��
        RockerProgressBar1(50,149,adc_bat_get_value());//��ʾ��ѹ
        
        /* ����� �����ҡ�˿��ƵĻ� ����1:4*/
        if(pwm_motor_status)  // �����״̬���� 
        {
            Draw_Solid_Circle(15 + 100,temp - 16 - 20 + 8,8,COLOR_GREEN);  // ��ʾʵ��Բ
            pwmMotorSetValue(adcYValue / 4); // ���������
        }else  // �����״̬�ر� 
        {
            Draw_Solid_Circle(15 + 100,temp - 16 - 20 + 8,8,COLOR_WHITE);  // ��ʾʵ��Բ
            pwmMotorSetValue(0); // �ر������
        }

        /* ��Ƶ */
        if(audio_status == 1)  // ��������
        {
            audio_status = 3;
            Draw_Solid_Circle(15 + 100,temp + 8,8,COLOR_GREEN);  // ��ʾʵ��Բ
            start_play_audio(audioSouce_Background,sizeof(audioSouce_Background),1); // �������� ���β���
        }else if(audio_status == 2) // �����״̬�ر� 
        {
            audio_status = 0;
          Draw_Solid_Circle(15 + 100,temp + 8,8,COLOR_WHITE);  // ��ʾʵ��Բ
            stop_play_audio();
        }

        delay_1ms(20);
        if(++Times > 500) 
        {
            Times=0;
            BAT_EN_ONFF();//��عػ�
        }
    }
}

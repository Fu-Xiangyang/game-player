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
#include "usart0.h"
#include "led.h"
#include "key.h"
#include "beep.h"
#include "i2c.h"
#include "eeprom.h"
#include "spi.h"
#include "adc_key.h"
#include "taskTimer.h"
#include "adc_photosensitive.h"
#include "w25qxx.h"
#include "dac_audio.h"
#include "exmc_sdram.h"
#include "nes_main.h"
#include "malloc.h"
#ifdef _LCD_8080_
#include "lcd9841.h"
#include "TFTLCD_Init.h"
#else
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
#endif
#include "Dac.h"
#include "DacAudio.h"



//#define __GWDBG__
#ifdef __GWDBG__
#define GWDBG(format, ...) printf("GWDBG[%s,%d] " format "\r\n", __func__, __LINE__, ##__VA_ARGS__)
#else
#define GWDBG(format, ...)
#endif




/************************************************
�������� �� main
��    �� �� ������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� LC
*************************************************/
int main(void)
{
    ErrStatus init_state;

    
    /* ���İ� */
    systick_config();
    usart0_init();
    
    /* ��չ�� */
    adc_config();
    key_init();
    Dac_Init();
    DacAudio_Init();

    // DacAudio_Play(dacAudio_AliPay,sizeof(dacAudio_AliPay),9000);
    GWDBG("EXMC");
    /* config the EXMC access mode */
    init_state = exmc_synchronous_dynamic_ram_init(EXMC_SDRAM_DEVICE0);
    GWDBG("");
    if (ERROR == init_state)
    {
        GWDBG("\r\n\r\nSDRAM initialize fail!");
        while (1);
    }
    my_mem_init(SRAMIN);
    my_mem_init(SRAMEX);
    my_mem_init(SRAMCCM);
    delay_1ms(50);
    #ifdef  USE_HARDWARESPI
    spi_config();
    #endif
    LCD_Init();//LCD��ʼ��
    LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
    
    
    
    GWDBG("nes_load start\r\n");
    nes_load(NULL);//NES��Ϸѭ��
    GWDBG("nes_load end\r\n");
    
    while (1);
}




void HardFault_Handler(void)
{
	uint32_t i;
	uint8_t t = 0;
	uint32_t temp;
	temp = SCB->CFSR;			  // fault״̬�Ĵ���(@0XE000ED28)����:MMSR,BFSR,UFSR
	printf("CFSR:%8X\r\n", temp); //��ʾ����ֵ
	temp = SCB->HFSR;			  //Ӳ��fault״̬�Ĵ���
	printf("HFSR:%8X\r\n", temp); //��ʾ����ֵ
	temp = SCB->DFSR;			  //����fault״̬�Ĵ���
	printf("DFSR:%8X\r\n", temp); //��ʾ����ֵ
	temp = SCB->AFSR;			  //����fault״̬�Ĵ���
	printf("AFSR:%8X\r\n", temp); //��ʾ����ֵ

	/* if Hard Fault exception occurs, go to infinite loop */
	while (1)
	{
	}
}

#include "adc_bat.h"
#include "adc_key.h"
#include "stdio.h"

//#define DEBUG   // ���������ʹ��adc_key����Ĵ�ӡ��Ϣ

//��ѹ�ȼ� 01234
const uint16_t BATDATA[6] = {2600,3000,3400,3600,3800,4000};


/* ADC�ĸ��ӹ��� ��������ͨGPIO  */
void bat_io_init(void)
{
//		/* enable the clock */
    rcu_periph_clock_enable(ADC_BAT_RCU);
	
    /* configure GPIO port ���ӹ�����Ҫ����Ϊ GPIO_MODE_ANALOG */ 
    gpio_mode_set(ADC_BAT_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADC_BAT_PIN);
    
    
    rcu_periph_clock_enable(BAT_EN_RCU);
    gpio_bit_set(BAT_EN_PORT,BAT_EN_PIN);
    gpio_mode_set(BAT_EN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,BAT_EN_PIN);
    gpio_output_options_set(BAT_EN_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,BAT_EN_PIN);
    
}


/* ��ȡֵ����  Ĭ��3.3V��׼�ȶ�  */
uint16_t adc_bat_get_value(void)
{
    uint8_t i;
	uint16_t adcValue=0;
    
    for(i=0;i<16;i++)
    {
        adcValue += adc_channel_sample(ADC_CHANNEL_9);  // ����15ͨ�� 
    }
    adcValue = (adcValue>>4)*3300/2048;  //��ѹ����/2
    
    return adcValue;
    
}








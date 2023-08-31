#include "adc_photosensitive.h"
#include "adc_key.h"
#include "stdio.h"

#define DEBUG   // ���������ʹ��adc_key����Ĵ�ӡ��Ϣ


/* ���������ֵ  ADC12λ ���ֵΪ4095
*/

/* ADC�ĸ��ӹ��� ��������ͨGPIO  */
void adc_photosensitive_init(void)
{
		/* enable the clock */
    rcu_periph_clock_enable(ADC_PHOTOSENSITIVE_RCU);
	   /* enable ADC0 clock */
    rcu_periph_clock_enable(RCU_ADC0);
	  /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
	
    /* configure GPIO port ���ӹ�����Ҫ����Ϊ GPIO_MODE_ANALOG */ 
    gpio_mode_set(ADC_PHOTOSENSITIVE_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADC_PHOTOSENSITIVE_PIN);
}


/* ���������ȡֵ���� */
void adc_photosensitive_get_value(void)
{
	uint16_t 	adcValue;
	adcValue = adc_channel_sample(ADC_CHANNEL_15);  // ����15ͨ�� 
	 
#ifdef DEBUG
	printf("adc_photosensitive value is :%d \r\n",adcValue);
#endif
}
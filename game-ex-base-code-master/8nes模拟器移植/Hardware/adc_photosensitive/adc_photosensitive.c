#include "adc_photosensitive.h"
#include "adc_key.h"
#include "stdio.h"

#define DEBUG   // 打开这个将会使能adc_key下面的打印信息


/* 光敏电阻的值  ADC12位 最大值为4095
*/

/* ADC的附加功能 优先于普通GPIO  */
void adc_photosensitive_init(void)
{
		/* enable the clock */
    rcu_periph_clock_enable(ADC_PHOTOSENSITIVE_RCU);
	   /* enable ADC0 clock */
    rcu_periph_clock_enable(RCU_ADC0);
	  /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
	
    /* configure GPIO port 附加功能需要配置为 GPIO_MODE_ANALOG */ 
    gpio_mode_set(ADC_PHOTOSENSITIVE_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADC_PHOTOSENSITIVE_PIN);
}


/* 光敏电阻获取值函数 */
void adc_photosensitive_get_value(void)
{
	uint16_t 	adcValue;
	adcValue = adc_channel_sample(ADC_CHANNEL_15);  // 采样15通道 
	 
#ifdef DEBUG
	printf("adc_photosensitive value is :%d \r\n",adcValue);
#endif
}
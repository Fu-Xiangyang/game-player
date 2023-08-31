#include "adc_key.h"
#include "stdio.h"

#define DEBUG // ���������ʹ��adc_key����Ĵ�ӡ��Ϣ

/* ���򿪹ص�ֵ  ADC12λ ���ֵΪ4095
Ĭ��ֵ 																							 8
A:100R-10K  		10K*(100R+10K)*4095 = 4054.5   ʵ��  4028   �ɲ�����Χ  4000-4095
B:10K-10K     	10K*(10K+10K)*4095 = 2047.5					 2033								2000-2050
C:100K-10K     	10K*(100K+10K)*4095 = 372.3					 372								320-400
D:43K-10K       10K*(43K+10K)*4095 = 772.6					 770								730-820
center:560K-10K 10K*(560K+10K)*4095 = 71.8           80								  50-120
*/

/* ADC�ĸ��ӹ��� ��������ͨGPIO  */
static void adc_init(void)
{
    
    
    
    
    
//  /* enable the clock */
////  rcu_periph_clock_enable(ADC_RCU);
//  /* enable ADC0 clock */
//  rcu_periph_clock_enable(RCU_ADC0);
//  /* config ADC clock */
//  adc_clock_config(ADC_ADCCK_PCLK2_DIV8);

//  /* configure GPIO port ���ӹ�����Ҫ����Ϊ GPIO_MODE_ANALOG */
//  gpio_mode_set(ADC_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_PIN);
//    
    
    
		/* enable the clock */
    rcu_periph_clock_enable(ADCX_RCU);
    /* configure GPIO port ���ӹ�����Ҫ����Ϊ GPIO_MODE_ANALOG */ 
    gpio_mode_set(ADCX_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADCX_PIN);
		/* enable the clock */
    rcu_periph_clock_enable(ADCY_RCU);
    /* configure GPIO port ���ӹ�����Ҫ����Ϊ GPIO_MODE_ANALOG */ 
    gpio_mode_set(ADCY_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADCY_PIN);
    
    
    
}

void adc_config(void)
{
  
		/* enable ADC0 clock */
    rcu_periph_clock_enable(RCU_ADC0);
		/* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
		/* reset ADC */
		adc_deinit();
	  /* configure the ADC  mode */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT); // ����ADC�������ڶ���ģʽ
	
    /* ADC contineous function disable */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE); // �ر�����ģʽ
    /* ADC scan mode disable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE); // �ر�ɨ��ģʽ
	
		/* ADC data alignment config */
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT); // LSB���룬��λ����
	
    /* ADC channel length config */
    adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL,1U); //  ADC����ͨ�� ����Ϊ1

    /* enable ADC interface */
    adc_enable(ADC0);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);  // ADCУ׼
    /* wait for ADC stability */
    delay_1ms(10);
    
    adc_init();
}

/*!
    \brief      ADC channel sample  ADCͨ������
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint16_t adc_channel_sample(uint8_t channel)
{
  /* ADC regular channel config */
  adc_regular_channel_config(ADC0, 0U, channel, ADC_SAMPLETIME_15); // 15����������
  /* ADC software trigger enable */
  adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL); // ADC�������ʹ��

  /* wait the end of conversion flag */
  while (!adc_flag_get(ADC0, ADC_FLAG_EOC))
    ;
  /* clear the end of conversion flag */
  adc_flag_clear(ADC0, ADC_FLAG_EOC);
  /* return regular channel sample value */
  return (adc_regular_data_read(ADC0));
}

/* ���򰴼�ɨ�躯�� */
five_key_enum five_way_key_scan(void)
{
    uint16_t 	adcXValue,adcYValue;
    
	adcXValue = adc_channel_sample(ADC_CHANNEL_1);  // ����
	adcYValue = adc_channel_sample(ADC_CHANNEL_11);  // ����
    
  if (adcYValue >= 3000 )
  { // 
    return five_key_down;
  }
  if ( adcYValue <= 1000)
  { // 
    return five_key_up;
  }
  if (adcXValue >= 3000 )
  { // 
    return five_key_left;
  }
  if ( adcXValue <= 1000)
  { // 
    return five_key_right;
  }
  
    return five_key_null;
}
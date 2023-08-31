#include "adc_key.h"
#include "stdio.h"

//#define DEBUG   // ���������ʹ��adc_key����Ĵ�ӡ��Ϣ

uint16_t 	adcXValue,adcYValue;


/* ADC�ĸ��ӹ��� ��������ͨGPIO  */
static void adc_gpio_init(void)
{
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
    delay_1ms(1);
		
		/* adc ���ų�ʼ�� */
		adc_gpio_init();
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
    while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
    /* clear the end of conversion flag */
    adc_flag_clear(ADC0, ADC_FLAG_EOC);
    /* return regular channel sample value */
    return (adc_regular_data_read(ADC0));
}





/* ң��ɨ�躯�� */
void adc_key_scan(void)
{
	adcXValue = 4090-adc_channel_sample(ADC_CHANNEL_1);  // ����
	adcYValue = adc_channel_sample(ADC_CHANNEL_11);  // ����
#ifdef DEBUG
	printf("adcXValue is :%d ",adcXValue);
	printf("adcYValue is :%d \r\n",adcYValue);
#endif
	
//	/* �ж�����һ���������� */
//	if(adcValue >= 4000 && adcValue <= 4095){  // ��
//#ifdef DEBUG
//		printf("five_way_key up press\r\n");
//		// timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,125);  // 
//		dac_audio_set_value(2048);  // ����ֵΪ2048
//#endif
//	}else if(adcValue >= 2000 && adcValue <= 2050){  // ��
//#ifdef DEBUG
//		printf("five_way_key left press\r\n");
//		timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,249);  // 
//#endif
//	}else if(adcValue >= 320 && adcValue <= 400){  // ��
//#ifdef DEBUG
//		printf("five_way_key down press\r\n");
//		timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,375);  // 
//#endif
//	}else if(adcValue >= 730 && adcValue <= 820){  // 
//#ifdef DEBUG
//		printf("five_way_key right press\r\n");
//		timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,499);  // 
//#endif
//	}else if(adcValue >= 50 && adcValue <= 120){  // �м䰴��
//#ifdef DEBUG
//		printf("five_way_key middle press\r\n");
//#endif
//	}else{  // Ĭ�� û�в���
//		// printf("five_way_key no press\r\n");
//		timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,0);  // ֹͣ
//	}
}
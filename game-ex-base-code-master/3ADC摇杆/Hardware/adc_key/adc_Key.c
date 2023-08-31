#include "adc_key.h"
#include "stdio.h"

//#define DEBUG   // 打开这个将会使能adc_key下面的打印信息

uint16_t 	adcXValue,adcYValue;


/* ADC的附加功能 优先于普通GPIO  */
static void adc_gpio_init(void)
{
		/* enable the clock */
    rcu_periph_clock_enable(ADCX_RCU);
    /* configure GPIO port 附加功能需要配置为 GPIO_MODE_ANALOG */ 
    gpio_mode_set(ADCX_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,ADCX_PIN);
  
		/* enable the clock */
    rcu_periph_clock_enable(ADCY_RCU);
    /* configure GPIO port 附加功能需要配置为 GPIO_MODE_ANALOG */ 
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
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT); // 所有ADC都工作在独立模式
	
    /* ADC contineous function disable */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE); // 关闭连续模式
    /* ADC scan mode disable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE); // 关闭扫描模式
	
		/* ADC data alignment config */
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT); // LSB对齐，低位对齐
	
    /* ADC channel length config */
    adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL,1U); //  ADC规则通道 长度为1

    /* enable ADC interface */
    adc_enable(ADC0);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);  // ADC校准
    /* wait for ADC stability */
    delay_1ms(1);
		
		/* adc 引脚初始化 */
		adc_gpio_init();
}


/*!
    \brief      ADC channel sample  ADC通道采样
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint16_t adc_channel_sample(uint8_t channel)
{
    /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 0U, channel, ADC_SAMPLETIME_15); // 15个采样周期 
    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL); // ADC软件触发使能

    /* wait the end of conversion flag */
    while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
    /* clear the end of conversion flag */
    adc_flag_clear(ADC0, ADC_FLAG_EOC);
    /* return regular channel sample value */
    return (adc_regular_data_read(ADC0));
}





/* 遥感扫描函数 */
void adc_key_scan(void)
{
	adcXValue = 4090-adc_channel_sample(ADC_CHANNEL_1);  // 采样
	adcYValue = adc_channel_sample(ADC_CHANNEL_11);  // 采样
#ifdef DEBUG
	printf("adcXValue is :%d ",adcXValue);
	printf("adcYValue is :%d \r\n",adcYValue);
#endif
	
//	/* 判断是哪一个按键按下 */
//	if(adcValue >= 4000 && adcValue <= 4095){  // 上
//#ifdef DEBUG
//		printf("five_way_key up press\r\n");
//		// timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,125);  // 
//		dac_audio_set_value(2048);  // 设置值为2048
//#endif
//	}else if(adcValue >= 2000 && adcValue <= 2050){  // 左
//#ifdef DEBUG
//		printf("five_way_key left press\r\n");
//		timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,249);  // 
//#endif
//	}else if(adcValue >= 320 && adcValue <= 400){  // 下
//#ifdef DEBUG
//		printf("five_way_key down press\r\n");
//		timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,375);  // 
//#endif
//	}else if(adcValue >= 730 && adcValue <= 820){  // 
//#ifdef DEBUG
//		printf("five_way_key right press\r\n");
//		timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,499);  // 
//#endif
//	}else if(adcValue >= 50 && adcValue <= 120){  // 中间按下
//#ifdef DEBUG
//		printf("five_way_key middle press\r\n");
//#endif
//	}else{  // 默认 没有操作
//		// printf("five_way_key no press\r\n");
//		timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,0);  // 停止
//	}
}
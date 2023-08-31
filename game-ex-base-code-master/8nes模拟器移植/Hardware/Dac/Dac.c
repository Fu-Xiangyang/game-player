#include "Dac.h"


/* PA5  ADC1 */
#define DAC_AUDIO_RCU  		RCU_GPIOA
#define DAC_AUDIO_PORT  	GPIOA
#define DAC_AUDIO_PIN    	GPIO_PIN_5 

void Dac_Init()
{ /* enable the clock of peripherals */
    rcu_periph_clock_enable(DAC_AUDIO_RCU);
    rcu_periph_clock_enable(RCU_DAC);

    /* once enabled the DAC, the corresponding GPIO pin is connected to the DAC converter automatically */
    gpio_mode_set(DAC_AUDIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, DAC_AUDIO_PIN);

    dac_deinit();                                          // 复位DAC
                                                           /* configure the DAC1 */
                                                           // dac_trigger_source_config(DAC1, DAC_TRIGGER_T3_TRGO); // 使用定时器3作为触发源
    dac_trigger_source_config(DAC1, DAC_TRIGGER_SOFTWARE); // 使用定时器3作为触发源
    dac_trigger_enable(DAC1);

    dac_wave_mode_config(DAC1, DAC_WAVE_DISABLE); // 不使能波形
                                                  //	dac_wave_mode_config(DAC1, DAC_WAVE_MODE_LFSR); //
                                                  // dac_lfsr_noise_config(DAC1, DAC_LFSR_BITS11_0);
    dac_output_buffer_enable(DAC1);

    /* enable DAC1 and set data */
    dac_enable(DAC1); // 使能DAC

    dac_data_set(DAC1, DAC_ALIGN_8B_R, 0); // 8位数据右对齐
    dac_software_trigger_enable(DAC1);     // 软件触发使能 输出数据

    
}

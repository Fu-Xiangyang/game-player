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

    dac_deinit();                                          // ��λDAC
                                                           /* configure the DAC1 */
                                                           // dac_trigger_source_config(DAC1, DAC_TRIGGER_T3_TRGO); // ʹ�ö�ʱ��3��Ϊ����Դ
    dac_trigger_source_config(DAC1, DAC_TRIGGER_SOFTWARE); // ʹ�ö�ʱ��3��Ϊ����Դ
    dac_trigger_enable(DAC1);

    dac_wave_mode_config(DAC1, DAC_WAVE_DISABLE); // ��ʹ�ܲ���
                                                  //	dac_wave_mode_config(DAC1, DAC_WAVE_MODE_LFSR); //
                                                  // dac_lfsr_noise_config(DAC1, DAC_LFSR_BITS11_0);
    dac_output_buffer_enable(DAC1);

    /* enable DAC1 and set data */
    dac_enable(DAC1); // ʹ��DAC

    dac_data_set(DAC1, DAC_ALIGN_8B_R, 0); // 8λ�����Ҷ���
    dac_software_trigger_enable(DAC1);     // �������ʹ�� �������

    
}

#include "dac_audio.h"
#include "audio_source.h"
#include "string.h"

uint8_t audio_status = 0; // ����״̬

typedef struct 
{
  uint32_t audioIndex; // ��¼��ǰ��Ƶ����λ��
  const uint8_t *audioCurrentPlayP;    // ָ��ǰ���ŵ���Ƶ
	uint32_t audioLength; // ��¼��ǰ��Ƶ�ĳ���
	uint8_t audio_play_mode; // 0�ǲ���һ��  1��ѭ������
}audio;


audio currentAudio; // ��ǰ��Ƶ 

void dac_audio_gpio_init(void)
{
		/* enable the clock of peripherals */
    rcu_periph_clock_enable(DAC_AUDIO_RCU);
    rcu_periph_clock_enable(RCU_DAC);
	
	  /* once enabled the DAC, the corresponding GPIO pin is connected to the DAC converter automatically */
    gpio_mode_set(DAC_AUDIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, DAC_AUDIO_PIN);
	
		dac_deinit();  // ��λDAC
    /* configure the DAC1 */
	  dac_trigger_source_config(DAC1, DAC_TRIGGER_SOFTWARE); // ʹ�ö�ʱ��3��Ϊ����Դ
    dac_trigger_enable(DAC1);
	
    dac_wave_mode_config(DAC1, DAC_WAVE_DISABLE); // ��ʹ�ܲ���
    dac_output_buffer_enable(DAC1);

    /* enable DAC1 and set data */
    dac_enable(DAC1); // ʹ��DAC
	
    dac_data_set(DAC1, DAC_ALIGN_8B_R, 0);  // 8λ�����Ҷ��� 
		dac_software_trigger_enable(DAC1);  // �������ʹ�� �������
}

/* ��ʱ������ 
T=��TIM_Period+1��*��TIM_Prescaler+1��/TIMxCLK
samplingRate:������
*/
void Timer3_config(uint16_t samplingRate)
{
    timer_parameter_struct timer_initpara;

    nvic_irq_enable(TIMER3_IRQn, 0, 1); // �����ж����ȼ�
	
	  rcu_periph_clock_enable(RCU_TIMER3);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(TIMER3);  // ��ʱ����λ
	  /* TIMER2 configuration */
    timer_initpara.prescaler         = 20 - 1;  // Ԥ��Ƶ
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE; // ����ģʽ
    timer_initpara.counterdirection  = TIMER_COUNTER_UP; // ��������
    timer_initpara.period            = 200000000 / 20 / samplingRate -1; // ����
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1; // ʱ�ӷ�Ƶ
    timer_initpara.repetitioncounter = 0; // �ظ�������
    timer_init(TIMER3,&timer_initpara);
	
		timer_master_output_trigger_source_select(TIMER3,TIMER_TRI_OUT_SRC_UPDATE);
	  /* TIMER2 interrupt enable */
    timer_interrupt_enable(TIMER3,TIMER_INT_UP);  // �ж�ʹ��
		
	  timer_disable(TIMER3);
}

void dac_audio_init(uint16_t samplingRate)
{
		dac_audio_gpio_init(); 					// gpio��ʼ��
		Timer3_config(samplingRate); 	  // ������Ƶ������
}


void TIMER3_IRQHandler(void)
{
	if (timer_interrupt_flag_get(TIMER3, TIMER_INT_FLAG_UP))
	 { 
		 /* clear TIMER interrupt flag */
		 timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);

		 dac_data_set(DAC1, DAC_ALIGN_8B_R, currentAudio.audioCurrentPlayP[currentAudio.audioIndex]);  // 8λ�����Ҷ��� 
		 dac_software_trigger_enable(DAC1);  // �������ʹ�� �������
		 
		 currentAudio.audioIndex ++; // �ȴ���һ������
		 
		
		 if(currentAudio.audioLength == currentAudio.audioIndex){
			  /* �жϣ�����ģʽ */
			 if(currentAudio.audio_play_mode == 1)  // ѭ������
			 {
			    currentAudio.audioIndex = 0;            // ��ͷ��ʼ
			 }else   // ���β���
			 {		
				 /* ������� ֹͣ���� */
			   timer_disable(TIMER3);  // �رն�ʱ��
			   audio_status = 2; // ��ʾֹͣ
			 }

		 }	 
	 }
}

/* ��ʼ�������� */
void start_play_audio(const uint8_t *audio,uint32_t audio_length,uint8_t mode)
{
	currentAudio.audioIndex = 0;              // ��ͷ��ʼ
	currentAudio.audioLength = audio_length;  // ��ǰ��Ƶ�ĳ��� 
	currentAudio.audioCurrentPlayP = audio;   // ָ�򲥷ŵ���Ƶ 
	currentAudio.audio_play_mode = mode;     // ����ģʽ
	timer_enable(TIMER3);        // ������ʱ��
	
	// printf("%d %d  ",currentAudio.audioIndex,currentAudio.audioLength);
}

/* ֹͣ�������� */
void stop_play_audio(void)
{
	timer_disable(TIMER3);  // �رն�ʱ��
}
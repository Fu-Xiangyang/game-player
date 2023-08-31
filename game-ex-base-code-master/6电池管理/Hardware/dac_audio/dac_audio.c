#include "dac_audio.h"
#include "audio_source.h"
#include "string.h"

uint8_t audio_status = 0; // 音乐状态

typedef struct 
{
  uint32_t audioIndex; // 记录当前音频播放位置
  const uint8_t *audioCurrentPlayP;    // 指向当前播放的音频
	uint32_t audioLength; // 记录当前音频的长度
	uint8_t audio_play_mode; // 0是播放一次  1是循环播放
}audio;


audio currentAudio; // 当前音频 

void dac_audio_gpio_init(void)
{
		/* enable the clock of peripherals */
    rcu_periph_clock_enable(DAC_AUDIO_RCU);
    rcu_periph_clock_enable(RCU_DAC);
	
	  /* once enabled the DAC, the corresponding GPIO pin is connected to the DAC converter automatically */
    gpio_mode_set(DAC_AUDIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, DAC_AUDIO_PIN);
	
		dac_deinit();  // 复位DAC
    /* configure the DAC1 */
	  dac_trigger_source_config(DAC1, DAC_TRIGGER_SOFTWARE); // 使用定时器3作为触发源
    dac_trigger_enable(DAC1);
	
    dac_wave_mode_config(DAC1, DAC_WAVE_DISABLE); // 不使能波形
    dac_output_buffer_enable(DAC1);

    /* enable DAC1 and set data */
    dac_enable(DAC1); // 使能DAC
	
    dac_data_set(DAC1, DAC_ALIGN_8B_R, 0);  // 8位数据右对齐 
		dac_software_trigger_enable(DAC1);  // 软件触发使能 输出数据
}

/* 定时器配置 
T=（TIM_Period+1）*（TIM_Prescaler+1）/TIMxCLK
samplingRate:采样率
*/
void Timer3_config(uint16_t samplingRate)
{
    timer_parameter_struct timer_initpara;

    nvic_irq_enable(TIMER3_IRQn, 0, 1); // 设置中断优先级
	
	  rcu_periph_clock_enable(RCU_TIMER3);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(TIMER3);  // 定时器复位
	  /* TIMER2 configuration */
    timer_initpara.prescaler         = 20 - 1;  // 预分频
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE; // 对齐模式
    timer_initpara.counterdirection  = TIMER_COUNTER_UP; // 计数方向
    timer_initpara.period            = 200000000 / 20 / samplingRate -1; // 周期
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1; // 时钟分频
    timer_initpara.repetitioncounter = 0; // 重复计数器
    timer_init(TIMER3,&timer_initpara);
	
		timer_master_output_trigger_source_select(TIMER3,TIMER_TRI_OUT_SRC_UPDATE);
	  /* TIMER2 interrupt enable */
    timer_interrupt_enable(TIMER3,TIMER_INT_UP);  // 中断使能
		
	  timer_disable(TIMER3);
}

void dac_audio_init(uint16_t samplingRate)
{
		dac_audio_gpio_init(); 					// gpio初始化
		Timer3_config(samplingRate); 	  // 配置音频采样率
}


void TIMER3_IRQHandler(void)
{
	if (timer_interrupt_flag_get(TIMER3, TIMER_INT_FLAG_UP))
	 { 
		 /* clear TIMER interrupt flag */
		 timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);

		 dac_data_set(DAC1, DAC_ALIGN_8B_R, currentAudio.audioCurrentPlayP[currentAudio.audioIndex]);  // 8位数据右对齐 
		 dac_software_trigger_enable(DAC1);  // 软件触发使能 输出数据
		 
		 currentAudio.audioIndex ++; // 等待下一次数据
		 
		
		 if(currentAudio.audioLength == currentAudio.audioIndex){
			  /* 判断，播放模式 */
			 if(currentAudio.audio_play_mode == 1)  // 循环播放
			 {
			    currentAudio.audioIndex = 0;            // 从头开始
			 }else   // 单次播放
			 {		
				 /* 播放完毕 停止播放 */
			   timer_disable(TIMER3);  // 关闭定时器
			   audio_status = 2; // 显示停止
			 }

		 }	 
	 }
}

/* 开始播放音乐 */
void start_play_audio(const uint8_t *audio,uint32_t audio_length,uint8_t mode)
{
	currentAudio.audioIndex = 0;              // 从头开始
	currentAudio.audioLength = audio_length;  // 当前音频的长度 
	currentAudio.audioCurrentPlayP = audio;   // 指向播放的音频 
	currentAudio.audio_play_mode = mode;     // 播放模式
	timer_enable(TIMER3);        // 开启定时器
	
	// printf("%d %d  ",currentAudio.audioIndex,currentAudio.audioLength);
}

/* 停止播放音乐 */
void stop_play_audio(void)
{
	timer_disable(TIMER3);  // 关闭定时器
}
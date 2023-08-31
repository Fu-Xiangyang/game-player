#ifndef _DAC_AUDIO_H
#define _DAC_AUDIO_H

#include "gd32f4xx.h"
#include "systick.h"


/* PA5  ADC1 */
#define DAC_AUDIO_RCU  		RCU_GPIOA
#define DAC_AUDIO_PORT  	GPIOA
#define DAC_AUDIO_PIN    	GPIO_PIN_5 


extern uint8_t audio_status; // “Ù¿÷◊¥Ã¨

void dac_audio_init(uint16_t samplingRate);
void start_play_audio(const uint8_t *audio,uint32_t audio_length,uint8_t mode);
void stop_play_audio(void);


#endif

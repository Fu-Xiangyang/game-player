#ifndef _ADC_KEY_H
#define _ADC_KEY_H

#include "gd32f4xx.h"
#include "systick.h"


/* PC1 PA1  */
#define ADCX_RCU    RCU_GPIOC
#define ADCX_PORT   GPIOC
#define ADCX_PIN    GPIO_PIN_1

#define ADCY_RCU    RCU_GPIOA
#define ADCY_PORT   GPIOA
#define ADCY_PIN    GPIO_PIN_1

extern uint16_t 	adcXValue,adcYValue;

void adc_config(void);
uint16_t adc_channel_sample(uint8_t channel);
void adc_key_scan(void);


#endif
#ifndef _ADC_KEY_H
#define _ADC_KEY_H

#include "gd32f4xx.h"
#include "systick.h"


/* PC4   */
#define ADC_RCU  RCU_GPIOC
#define ADC_PORT  GPIOC
#define ADC_PIN    GPIO_PIN_4

#define ADCX_RCU    RCU_GPIOC
#define ADCX_PORT   GPIOC
#define ADCX_PIN    GPIO_PIN_1

#define ADCY_RCU    RCU_GPIOA
#define ADCY_PORT   GPIOA
#define ADCY_PIN    GPIO_PIN_1


typedef enum 
{
    five_key_null = 0,
    five_key_up , 
    five_key_down,   
    five_key_left,   
    five_key_right,  
    five_key_mid 
} five_key_enum;

void adc_config(void);
uint16_t adc_channel_sample(uint8_t channel);
five_key_enum five_way_key_scan(void);


#endif
#ifndef _ADC_BAT_H
#define _ADC_BAT_H

#include "gd32f4xx.h"
#include "systick.h"


/* PC5   */
#define ADC_BAT_RCU  RCU_GPIOB
#define ADC_BAT_PORT  GPIOB
#define ADC_BAT_PIN    GPIO_PIN_1


#define BAT_EN_RCU  RCU_GPIOB
#define BAT_EN_PORT  GPIOB
#define BAT_EN_PIN    GPIO_PIN_8

#define BAT_EN_ONFF()			gpio_bit_reset(BAT_EN_PORT,BAT_EN_PIN)



void bat_io_init(void);
uint16_t adc_bat_get_value(void);

#endif

#ifndef _ADC_PHOTOSENSITIVE_H
#define _ADC_PHOTOSENSITIVE_H

#include "gd32f4xx.h"
#include "systick.h"


/* PC5   */
#define ADC_PHOTOSENSITIVE_RCU  RCU_GPIOC
#define ADC_PHOTOSENSITIVE_PORT  GPIOC
#define ADC_PHOTOSENSITIVE_PIN    GPIO_PIN_5



void adc_photosensitive_init(void);
void adc_photosensitive_get_value(void);


#endif
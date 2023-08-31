#ifndef _KEY_H
#define _KEY_H

#include "gd32f4xx.h"
#include "systick.h"

#define KEYL_RCU  RCU_GPIOA
#define KEYL_PORT  GPIOA
#define KEYL_PIN    GPIO_PIN_0

#define KEYR_RCU  RCU_GPIOB
#define KEYR_PORT  GPIOB
#define KEYR_PIN    GPIO_PIN_2

#define KEYA_RCU  RCU_GPIOG
#define KEYA_PORT  GPIOG
#define KEYA_PIN    GPIO_PIN_9

#define KEYB_RCU  RCU_GPIOB
#define KEYB_PORT  GPIOB
#define KEYB_PIN    GPIO_PIN_15

typedef enum 
{
    one_key_null = 0,
    one_key_left,   
    one_key_right,  
    one_key_a,
    one_key_b
} one_key_enum;
void key_init(void);
void key_get_value(uint32_t port,uint32_t pin);
extern one_key_enum get_key_val();
#endif
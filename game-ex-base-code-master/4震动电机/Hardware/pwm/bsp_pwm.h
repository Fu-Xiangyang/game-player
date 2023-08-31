#ifndef _BSP_PWM_H
#define _BSP_PWM_H


#include "gd32f4xx.h"
#include "systick.h"

#define BSP_PWM_RCU     RCU_GPIOA               // PWM端口时钟
#define BSP_PWM_PORT    GPIOA 									// PWM端口
#define BSP_PWM_PIN     GPIO_PIN_2              // PWM引脚
#define BSP_PWM_AF      GPIO_AF_1								// PWM引脚复用


#define BSP_PWM_TIMER_RCU  				RCU_TIMER1		// 定时器时钟
#define BSP_PWM_TIMER      				TIMER1        // 定时器
#define BSP_PWM_CHANNEL  					TIMER_CH_2    // 定时器通道


extern uint8_t pwm_motor_status; // 震动马达的状态  0 关闭 1开始 

void pwm_config(uint16_t pre,uint16_t per);			// PWM配置 
void pwmMotorSetValue(uint16_t value);          // 设置PWM的值



#endif  /* BSP_PWM_H */

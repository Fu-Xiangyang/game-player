#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "gd32f4xx.h"
#include "systick.h"

#define BSP_KEYA_RCU   RCU_GPIOG      // 按键端口时钟
#define BSP_KEYA_PORT  GPIOG          // 按键端口
#define BSP_KEYA_PIN   GPIO_PIN_9     // 按键引脚

#define BSP_KEYA_EXTI_IRQN			 		EXTI5_9_IRQn          // 外部中断0
#define BSP_KEYA_EXTI_PORT_SOURCE 	EXTI_SOURCE_GPIOG   // 外部中断端口资源
#define BSP_KEYA_EXTI_PIN_SOURCE  	EXTI_SOURCE_PIN9    // 外部中断引脚资源
#define BSP_KEYA_EXTI_LINE 				  EXTI_9              // 外部中断
#define BSP_KEYA_EXTI_IRQHANDLER    EXTI5_9_IRQHandler    // 外部中断函数名


#define BSP_KEYB_RCU   RCU_GPIOB      // 按键端口时钟
#define BSP_KEYB_PORT  GPIOB          // 按键端口
#define BSP_KEYB_PIN   GPIO_PIN_15     // 按键引脚

#define BSP_KEYB_EXTI_IRQN			 		EXTI10_15_IRQn          // 外部中断0
#define BSP_KEYB_EXTI_PORT_SOURCE 	EXTI_SOURCE_GPIOB      // 外部中断端口资源
#define BSP_KEYB_EXTI_PIN_SOURCE  	EXTI_SOURCE_PIN15      // 外部中断引脚资源
#define BSP_KEYB_EXTI_LINE 				  EXTI_15                // 外部中断
#define BSP_KEYB_EXTI_IRQHANDLER    EXTI10_15_IRQHandler    // 外部中断函数名

void key_gpio_config(void); // key gpio引脚配置
void key_scan(void); // 按键扫描

#endif
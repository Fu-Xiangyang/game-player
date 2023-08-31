#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "gd32f4xx.h"
#include "systick.h"

#define BSP_KEYA_RCU   RCU_GPIOG      // �����˿�ʱ��
#define BSP_KEYA_PORT  GPIOG          // �����˿�
#define BSP_KEYA_PIN   GPIO_PIN_9     // ��������

#define BSP_KEYA_EXTI_IRQN			 		EXTI5_9_IRQn          // �ⲿ�ж�0
#define BSP_KEYA_EXTI_PORT_SOURCE 	EXTI_SOURCE_GPIOG   // �ⲿ�ж϶˿���Դ
#define BSP_KEYA_EXTI_PIN_SOURCE  	EXTI_SOURCE_PIN9    // �ⲿ�ж�������Դ
#define BSP_KEYA_EXTI_LINE 				  EXTI_9              // �ⲿ�ж�
#define BSP_KEYA_EXTI_IRQHANDLER    EXTI5_9_IRQHandler    // �ⲿ�жϺ�����


#define BSP_KEYB_RCU   RCU_GPIOB      // �����˿�ʱ��
#define BSP_KEYB_PORT  GPIOB          // �����˿�
#define BSP_KEYB_PIN   GPIO_PIN_15     // ��������

#define BSP_KEYB_EXTI_IRQN			 		EXTI10_15_IRQn          // �ⲿ�ж�0
#define BSP_KEYB_EXTI_PORT_SOURCE 	EXTI_SOURCE_GPIOB      // �ⲿ�ж϶˿���Դ
#define BSP_KEYB_EXTI_PIN_SOURCE  	EXTI_SOURCE_PIN15      // �ⲿ�ж�������Դ
#define BSP_KEYB_EXTI_LINE 				  EXTI_15                // �ⲿ�ж�
#define BSP_KEYB_EXTI_IRQHANDLER    EXTI10_15_IRQHandler    // �ⲿ�жϺ�����

void key_gpio_config(void); // key gpio��������
void key_scan(void); // ����ɨ��

#endif
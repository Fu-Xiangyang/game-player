/********************************************************************************
  * �� �� ��: bsp_key.c
  * �� �� ��: ����
  * �޸�����: LC
  * �޸�����: 2022��04��15��
  * ���ܽ���:          
  ******************************************************************************
  * ע������:
*********************************************************************************/
#include "bsp_key.h"
#include "sys.h"
#include "bsp_led.h"
#include "stdio.h"
#include "bsp_pwm.h"
#include "dac_audio.h"

void key_gpio_config(void)
{
	/* ����ʱ�� */
	rcu_periph_clock_enable(BSP_KEYA_RCU);
	rcu_periph_clock_enable(RCU_SYSCFG);    // ϵͳ����ʱ��
	
	/* ����Ϊ����ģʽ ����ģʽ */ 
	gpio_mode_set(BSP_KEYA_PORT,GPIO_MODE_INPUT,GPIO_PUPD_PULLDOWN,BSP_KEYA_PIN);  // ����Ĭ��״̬�ǵ͵�ƽ������Ϊ����
	
	/* ʹ��NVIC�ж� �жϷ���Ϊ2λ��ռ���ȼ���2λ�����ȼ� */
	nvic_irq_enable(BSP_KEYA_EXTI_IRQN,3U,3U);  // ��ռ���ȼ�3�������ȼ�3
	/* �����ж��ߵ�GPIO */
	syscfg_exti_line_config(BSP_KEYA_EXTI_PORT_SOURCE,BSP_KEYA_EXTI_PIN_SOURCE);
	/* ��ʼ���ж��� */
	exti_init(BSP_KEYA_EXTI_LINE,EXTI_INTERRUPT,EXTI_TRIG_BOTH);
	
	/* ʹ���ж� */
	exti_interrupt_enable(BSP_KEYA_EXTI_LINE);
	/* ����жϱ�־λ */
	exti_interrupt_flag_clear(BSP_KEYA_EXTI_LINE);
	
	
	/* ����ʱ�� */
	rcu_periph_clock_enable(BSP_KEYB_RCU);
	
	/* ����Ϊ����ģʽ ����ģʽ */ 
	gpio_mode_set(BSP_KEYB_PORT,GPIO_MODE_INPUT,GPIO_PUPD_PULLDOWN,BSP_KEYB_PIN);  // ����Ĭ��״̬�ǵ͵�ƽ������Ϊ����
	
	/* ʹ��NVIC�ж� �жϷ���Ϊ2λ��ռ���ȼ���2λ�����ȼ� */
	nvic_irq_enable(BSP_KEYB_EXTI_IRQN,3U,3U);  // ��ռ���ȼ�3�������ȼ�3
	/* �����ж��ߵ�GPIO */
	syscfg_exti_line_config(BSP_KEYB_EXTI_PORT_SOURCE,BSP_KEYB_EXTI_PIN_SOURCE);
	/* ��ʼ���ж��� */
	exti_init(BSP_KEYB_EXTI_LINE,EXTI_INTERRUPT,EXTI_TRIG_BOTH);
	
	/* ʹ���ж� */
	exti_interrupt_enable(BSP_KEYB_EXTI_LINE);
	/* ����жϱ�־λ */
	exti_interrupt_flag_clear(BSP_KEYB_EXTI_LINE);
}

/************************************************
�������� �� BSP_KEY_EXTI_IRQHandler
��    �� �� �жϴ�����
��    �� �� ��
�� �� ֵ �� ��
��    �� �� LCEDA
*************************************************/
void BSP_KEYA_EXTI_IRQHANDLER(void)
{
  if(exti_interrupt_flag_get(BSP_KEYA_EXTI_LINE) == SET)   // �жϱ�־λΪ1����������	
	{
		if(gpio_input_bit_get(BSP_KEYA_PORT,BSP_KEYA_PIN) == SET)  // ��������
		{
			 /* �������²����Ĺ��� */ 
			gpio_bit_toggle(PORT_LED2,PIN_LED2);                   // led��ƽ״̬��ת
			printf("keyA press!\r\n");                              // ���ڴ�ӡkey press!
			pwm_motor_status = !pwm_motor_status;                   // �����״̬����
		}else{                                                   // �����ͷ� 
			/* �����ɿ������Ĺ��� */ 
			printf("keyA release!\r\n");                            // ���ڴ�ӡkey release!
		}
	  exti_interrupt_flag_clear(BSP_KEYA_EXTI_LINE);            // ���жϱ�־λ
	}
}


/************************************************
�������� �� BSP_KEYB_EXTI_IRQHandler
��    �� �� �жϴ�����
��    �� �� ��
�� �� ֵ �� ��
��    �� �� LCEDA
*************************************************/
void BSP_KEYB_EXTI_IRQHANDLER(void)
{
  if(exti_interrupt_flag_get(BSP_KEYB_EXTI_LINE) == SET)   // �жϱ�־λΪ1����������	
	{
		if(gpio_input_bit_get(BSP_KEYB_PORT,BSP_KEYB_PIN) == SET)  // ��������
		{
			 /* �������²����Ĺ��� */ 
			gpio_bit_toggle(PORT_LED2,PIN_LED2);                   // led��ƽ״̬��ת
			printf("keyB press!\r\n");                              // ���ڴ�ӡkey press!
			if(audio_status == 0) // ������״̬ �ǿ�������
				audio_status = 1;
			else if(audio_status == 3)  // �������� �ر����� 
			{
				audio_status = 2;
			}
		}else{                                                   // �����ͷ� 
			/* �����ɿ������Ĺ��� */ 
			printf("keyB release!\r\n");                            // ���ڴ�ӡkey release!
		}
	  exti_interrupt_flag_clear(BSP_KEYB_EXTI_LINE);            // ���жϱ�־λ
	}
}

///************************************************
//�������� �� key_scan
//��    �� �� ����ɨ�躯��
//��    �� �� ��
//�� �� ֵ �� ��
//��    �� �� LC
//*************************************************/
//void key_scan(void)
//{
//	/* �ȶ�ȡ�������ŵĵ�ƽ ����ߵ�ƽ���������� */
//	if(gpio_input_bit_get(BSP_KEY_PORT,BSP_KEY_PIN) == SET)         // ��������
//	{
//		delay_1ms(20);  // �ӳ����� 
//		if(gpio_input_bit_get(BSP_KEY_PORT,BSP_KEY_PIN) == SET)       // �ٴμ�ⰴ���Ƿ��� 
//		{
//			/* ִ�ж�Ӧ�Ĺ��� */
//			gpio_bit_toggle(PORT_LED2,PIN_LED2);    // led��ƽ״̬��ת
//			printf("key press!\r\n");								// ���ڴ�ӡkey press!
//			while(gpio_input_bit_get(BSP_KEY_PORT,BSP_KEY_PIN) == SET);  // ��ⰴ���ɿ�
//			printf("key release!\r\n");             // ���ڴ�ӡkey release!
//		}
//	}
//}
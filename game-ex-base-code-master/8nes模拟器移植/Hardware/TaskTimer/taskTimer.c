#include "taskTimer.h"
#include "stdio.h"



#define TASK_NUM  5 // Ҫ����������
uint16_t DelayTable[TASK_NUM] = {0}; // 


/* ��ʱ������ 
T=��TIM_Period+1��*��TIM_Prescaler+1��/TIMxCLK
		TIM_Period * 20000 / 200MHZ  
*/
void taskTimer_config(uint32_t TIM_Period)
{
		uint8_t i;
    timer_parameter_struct timer_initpara;
	
		/* �ж����� */
	  nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3); // �������ȼ���
    nvic_irq_enable(TIMER2_IRQn, 0, 1); // �����ж����ȼ�

	  rcu_periph_clock_enable(RCU_TIMER2);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(TIMER2);  // ��ʱ����λ
	  /* TIMER2 configuration */
    timer_initpara.prescaler         = TIM_Period -1;  // Ԥ��Ƶ
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE; // ����ģʽ
    timer_initpara.counterdirection  = TIMER_COUNTER_UP; // ��������
    timer_initpara.period            = 20000 -1; // ����
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1; // ʱ�ӷ�Ƶ
    timer_initpara.repetitioncounter = 0; // �ظ�������
    timer_init(TIMER2,&timer_initpara);
	  /* TIMER2 interrupt enable */
    timer_interrupt_enable(TIMER2,TIMER_INT_UP);  // �ж�ʹ��
		
	  timer_enable(TIMER2);
		
		/* ���ʱ���� */
		for (i=0;i<TASK_NUM;i++)
				DelayTable[i] = 0;//����
}

/*!
    \brief      this function handles TIMER0 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER2_IRQHandler(void)
{
	uint8_t i;
    /* clear TIMER interrupt flag */
    timer_interrupt_flag_clear(TIMER2,TIMER_INT_FLAG_UP);
	
	//	printf("taskTimer handler!\n");
		for (i=0;i<TASK_NUM;i++)
			DelayTable[i]++; //������ÿ��Ԫ�ض���1
}

uint8_t  SysGetSignal_1ms(uint16_t nms,uint8_t index)
{
	if (index < TASK_NUM)
	{
		if (DelayTable[index] >= nms) //�������ڶ�ʱֵ��˵��ʱ�䵽
		{
			DelayTable[index] = 0;
			return 1;
		}
	}
	return 0;
}

//��λһ����ʱ�ź�
//index ��ʱ���������
void SysResetSignal(uint8_t index)
{
	if (index < TASK_NUM)
		DelayTable[index] = 0;
}
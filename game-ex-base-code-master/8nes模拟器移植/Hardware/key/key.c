#include "key.h"
#include "usart0.h"

//#define KEYL_RCU  RCU_GPIOA
//#define KEYL_PORT  GPIOA
//#define KEYL_PIN    GPIO_PIN_0

//#define KEYR_RCU  RCU_GPIOB
//#define KEYR_PORT  GPIOB
//#define KEYR_PIN    GPIO_PIN_2

//#define KEYA_RCU  RCU_GPIOG
//#define KEYA_PORT  GPIOG
//#define KEYA_PIN    GPIO_PIN_9

//#define KEYB_RCU  RCU_GPIOB
//#define KEYB_PORT  GPIOB
//#define KEYB_PIN    GPIO_PIN_15

void key_init(void)
{
	rcu_periph_clock_enable(KEYL_RCU);
	/* configure led GPIO port */
	gpio_mode_set(KEYL_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, KEYL_PIN);
	//gpio_output_options_set(KEYL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, KEYL_PIN);

	rcu_periph_clock_enable(KEYR_RCU);
	/* configure led GPIO port */
	gpio_mode_set(KEYR_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, KEYR_PIN);
	//gpio_output_options_set(KEYR_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, KEYR_PIN);


	/* enable the led clock */
	rcu_periph_clock_enable(KEYA_RCU);
	/* configure led GPIO port */
	gpio_mode_set(KEYA_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, KEYA_PIN);
	//gpio_output_options_set(KEYA_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, KEYA_PIN);

	/* enable the led clock */
	rcu_periph_clock_enable(KEYB_RCU);
	/* configure led GPIO port */
	gpio_mode_set(KEYB_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, KEYB_PIN);
	//gpio_output_options_set(KEYB_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, KEYB_PIN);
}

void key_get_value(uint32_t port, uint32_t pin)
{
	if (SET == gpio_input_bit_get(port, pin))
	{
		delay_1ms(10); // 延迟消抖
		if (SET == gpio_input_bit_get(port, pin))
		{
			/* 执行按键按下的动作 */
			printf("currect port is %d,pin is %d\n", port, pin);
			while (SET == gpio_input_bit_get(port, pin))
				; // 等待按键松开
		}
	};
}

one_key_enum get_key_val()
{
	if(	gpio_input_bit_get(KEYL_PORT, KEYL_PIN) == SET ||
		gpio_input_bit_get(KEYR_PORT, KEYR_PIN) == SET ||
		gpio_input_bit_get(KEYA_PORT, KEYA_PIN) == SET ||
		gpio_input_bit_get(KEYB_PORT, KEYB_PIN) == SET )
	{
		//delay_1ms(10); // 延迟消抖
		if (gpio_input_bit_get(KEYL_PORT, KEYL_PIN) == SET)
		{ // 上
			return one_key_left;
		}
		else if (gpio_input_bit_get(KEYR_PORT, KEYR_PIN) == SET)
		{ // 左
			return one_key_right;
		}
		else if (gpio_input_bit_get(KEYA_PORT, KEYA_PIN) == SET)
		{ // 下
			return one_key_a;
		}
		else if (gpio_input_bit_get(KEYB_PORT, KEYB_PIN) == SET)
		{ //
			return one_key_b;
		}
		else
		{ // 默认 没有操作
			return one_key_null;
		}
	}
	else
	{
		return one_key_null;
	}
}
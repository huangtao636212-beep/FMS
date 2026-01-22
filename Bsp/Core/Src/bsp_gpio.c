#include "bsp_gpio.h"
#include "stm32h7xx.h"

void bsp_clock_init(GPIO_TypeDef* port)
{

    if (port == GPIOA) LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
    else if (port == GPIOB) LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
    else if (port == GPIOC) LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
    else if (port == GPIOD) LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOD);
    else if (port == GPIOE) LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);
	else if (port == GPIOF) LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOF);
	else if (port == GPIOG) LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOG);
	else
	{
		/* unknown port: do nothing */
	}
}
void bsp_gpio_init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	bsp_clock_init(GPIOA);
	bsp_clock_init(GPIOD);
	bsp_clock_init(GPIOH);
	
	/**/
	LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
	GPIO_InitStruct.Pin = LED1_Pin|LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	/**/
	LL_GPIO_SetOutputPin(LED0_GPIO_Port, LED0_Pin);
	GPIO_InitStruct.Pin = LED0_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(LED0_GPIO_Port, &GPIO_InitStruct);
  /**/
	LL_GPIO_ResetOutputPin(U3_DIR_GPIO_Port, U3_DIR_Pin);
	GPIO_InitStruct.Pin = U3_DIR_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(U3_DIR_GPIO_Port, &GPIO_InitStruct);



	
}


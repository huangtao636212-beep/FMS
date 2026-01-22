#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_bus.h"
/*Pin to point*/

#define LED1_Pin LL_GPIO_PIN_9
#define LED1_GPIO_Port GPIOE
#define LED0_Pin LL_GPIO_PIN_11
#define LED0_GPIO_Port GPIOD
#define U3_DIR_Pin LL_GPIO_PIN_12
#define U3_DIR_GPIO_Port GPIOD

void bsp_clock_init(GPIO_TypeDef* port);

#ifdef __cplusplus
}
#endif

#endif

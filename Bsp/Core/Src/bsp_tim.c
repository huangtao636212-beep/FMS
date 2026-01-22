#include "bsp_tim.h"
#include "stm32h7xx_ll_tim.h"
#include "stm32h7xx_ll_bus.h"
#if REMOVE_LL
void bsp_tim5_init(uint16_t pre)
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);
	
	TIM_InitStruct.Prescaler=199;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 4294967295;
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	LL_TIM_Init(TIM5, &TIM_InitStruct);
	LL_TIM_DisableARRPreload(TIM5);
	LL_TIM_SetClockSource(TIM5, LL_TIM_CLOCKSOURCE_INTERNAL);
	LL_TIM_SetTriggerOutput(TIM5, LL_TIM_TRGO_RESET);
	LL_TIM_DisableMasterSlaveMode(TIM5);
}
#endif


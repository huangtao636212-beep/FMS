#ifndef SVC_TIM_H
#define SVC_TIM_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "stdint.h"
void svc_delay_us(uint32_t us);
uint32_t svc_time_now_ms(void);
#ifdef __cplusplus
}
#endif

#endif

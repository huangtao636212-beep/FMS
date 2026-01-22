#include "svc_tim.h"

#if REMOVE_LL
#include "bsp_tim.h"
#else
#include "tim.h"
#endif

typedef struct{
uint32_t deadline_us;
	uint8_t active;
}svc_tim_us_t;

uint32_t svc_tim_now_us(void)
{
	return (uint32_t)TIM5->CNT;  // 1us 分辨率
}

uint32_t svc_time_now_ms(void)
{
    // 注意：这是向下取整的 ms
    return svc_tim_now_us() / 1000u;
}

static inline void svc_tim_us_start(svc_tim_us_t* t, uint32_t delay_us)
{
    t->deadline_us = svc_tim_now_us() + delay_us;
    t->active = 1;
}

static inline uint8_t svc_tim_us_expired(svc_tim_us_t* t)
{
    if (!t->active) return 0;
    if ((int32_t)(svc_tim_now_us() - t->deadline_us) >= 0) {
        t->active = 0;
        return 1;
    }
    return 0;
}
void svc_delay_us(uint32_t us)
{
	svc_tim_us_t t;
    svc_tim_us_start(&t, us);                   
    while (!svc_tim_us_expired(&t)) {           
        __NOP();
    }
	
	
}

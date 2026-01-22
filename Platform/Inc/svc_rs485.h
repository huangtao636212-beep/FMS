#ifndef SVC_RS485_H
#define SVC_RS485_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "bsp_usart.h"
//#define DMA_RX_BUF_SIZE		1024
#define RING_BUF_SIZE			4096

typedef struct
{
	uint8_t  buf[RING_BUF_SIZE];
	volatile uint32_t rb_w;//写，单调递增
	volatile uint32_t rb_r;//读，单调递增
	volatile uint32_t rb_drop;//溢出丢包的数
}ringbuf_t;
extern ringbuf_t g_ring_buf;
extern uint8_t dma_rx_buf[DMA_RX_BUF_SIZE];
void usart_rx_dma_move_new_bytes_isr(void);
uint32_t uart_rx_read(uint8_t *out, uint32_t maxlen);
uint32_t uart_rx_available(void);
uint32_t uart_rx_drop_count(void);
void uart_rx_init(void);
#ifdef __cplusplus
}
#endif

#endif

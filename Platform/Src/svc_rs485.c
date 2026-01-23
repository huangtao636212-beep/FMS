#include "svc_rs485.h"

#include "stm32h7xx.h"      // 或 stm32h723xx.h
#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_ll_usart.h"
#include <string.h>

/*============= 软件环形缓冲（轻量、ISR安全）===========*/
ringbuf_t g_ring_buf;

/*读取当前数据使用情况*/
static uint32_t ring_buf_used(const ringbuf_t* rb)
{
	return (uint32_t)(rb->rb_w - rb->rb_r);
}

static uint32_t ring_buf_free(const ringbuf_t* rb)
{
	return (uint32_t)(RING_BUF_SIZE-ring_buf_used(rb));
}
/*外包函数，不适用mem函数*/
static inline void svc_memcpy_u8(uint8_t* dst,const uint8_t* src,uint32_t n)
{
	while(n--)
	{
		*dst++=*src++;
	}
}
/*包含丢包的写入环形*/
static void ring_buf_write_bytes(ringbuf_t* rb,const uint8_t* data,uint32_t len)
{
	if(len == 0)return ;
	uint32_t free = ring_buf_free(rb);
	if(len>free)
	{
		rb->rb_drop++;
		len=free;
		if(len==0)return;
	}
	uint32_t w = rb->rb_w;
	uint32_t off=w&(RING_BUF_SIZE-1);
	
	uint32_t first = RING_BUF_SIZE-off;
	if(first > len)
	{
		first=len;
	}
	svc_memcpy_u8(&rb->buf[off],data,first);
	if(len>first)
	{
		svc_memcpy_u8(&rb->buf[0],data+first,len-first);
	}
	__DMB();/* 确保数据先写入内存再更新指针 */
	
	rb->rb_w=w+len;
}
/* 读取：返回实际读取长度 */
static uint32_t ring_buf_read_bytes(ringbuf_t* rb,uint8_t* out,uint32_t maxlen)
{
	/*1）确定读取的范围大小*/
	uint32_t used=ring_buf_used(rb);
	if(used==0)return 0;
	if(maxlen > used) maxlen=used;
	
	/*2）确定读取的指针和下标*/
	uint32_t r=rb->rb_r;
	uint32_t off=r&(RING_BUF_SIZE-1);
	
	/*3）从头与否的两种方式将读取的值从环形中赋值给out*/
	uint32_t first=RING_BUF_SIZE-off;
	if(first>maxlen)
	{
		first=maxlen;
	}
	svc_memcpy_u8(out,&rb->buf[off],first);
	if(maxlen>first)
	{
		svc_memcpy_u8(out+first,&rb->buf[0],maxlen-first);
	}
	__DMB();
	rb->rb_r=r+maxlen;
	return maxlen;
}
/*========= DMA 循环接收 + 增量搬运 =======*/
//uint8_t dma_rx_buf[DMA_RX_BUF_SIZE];
static volatile uint16_t s_last_pos=0;/* 上次搬运到的位置 [0..DMA_RX_BUF_SIZE) */
/*当前dma写到哪了*/
static inline uint16_t dma_get_pos(void)
{
	uint32_t ndtr=LL_DMA_GetDataLength(DMA1,LL_DMA_STREAM_0);
	uint32_t pos=(uint16_t)(DMA_RX_BUF_SIZE-ndtr);
	if(pos>=DMA_RX_BUF_SIZE)pos=0;
	return pos;
}
void usart_rx_dma_move_new_bytes_isr(void)
{
	uint16_t pos=dma_get_pos();
	uint16_t last=s_last_pos;
	if(pos == last) return;
	if(pos > last)
	{
		uint16_t len=(uint16_t )(pos-last);
		ring_buf_write_bytes(&g_ring_buf,&dma_rx_buf[last],len);
		
	}
	else{
		//回卷
		uint16_t len_last=(uint16_t)(DMA_RX_BUF_SIZE-last);
		ring_buf_write_bytes(&g_ring_buf,&dma_rx_buf[last],len_last);
		
		if(pos>0)
		{
			ring_buf_write_bytes(&g_ring_buf,&dma_rx_buf[0],pos);
		}
	}
	s_last_pos=pos;
}
/* 任务/主循环读取接口 */
uint32_t uart_rx_read(uint8_t *out, uint32_t maxlen)
{
	
    return ring_buf_read_bytes(&g_ring_buf, out, maxlen);
}

/* 当前可读字节数 */
uint32_t uart_rx_available(void)
{
    return ring_buf_used(&g_ring_buf);
}

/* 溢出丢包计数（建议接故障日志） */
uint32_t uart_rx_drop_count(void)
{
    return g_ring_buf.rb_drop;
}

	
//void USART3_IRQHandler(void)
//{
//    if (LL_USART_IsActiveFlag_IDLE(USART3)) {
//        /* 先搬运：把尾巴数据及时取走 */
//        usart_rx_dma_move_new_bytes_isr();

//        /* 清 IDLE（H7 用 ICR 清；读 RDR 是个保险动作，避免边界条件） */
//        LL_USART_ClearFlag_IDLE(USART3);
//        (void)LL_USART_ReceiveData8(USART3);
//    }
//}
//void DMA1_Stream0_IRQHandler(void)
//{
//    if (LL_DMA_IsActiveFlag_HT0(DMA1)) {
//        LL_DMA_ClearFlag_HT0(DMA1);
//        usart_rx_dma_move_new_bytes_isr();
//    }

//    if (LL_DMA_IsActiveFlag_TC0(DMA1)) {
//        LL_DMA_ClearFlag_TC0(DMA1);
//        usart_rx_dma_move_new_bytes_isr();
//    }

//    if (LL_DMA_IsActiveFlag_TE0(DMA1)) {
//        LL_DMA_ClearFlag_TE0(DMA1);
//        /* 工业建议：记录错误、必要时重启 DMA（这里先不展开） */
//    }
//}

void uart_rx_init(void)
{
	
    memset(&g_ring_buf, 0, sizeof(g_ring_buf));
    s_last_pos = 0;
}

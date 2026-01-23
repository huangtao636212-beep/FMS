#include "bsp_usart.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_usart.h"
#include "stm32h7xx_ll_bus.h"
#include "stm32h7xx_ll_dma.h"

uint8_t dma_rx_buf[DMA_RX_BUF_SIZE];
//#if REMOVE_LL
void bsp_usart_init(void)
{	
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	LL_USART_InitTypeDef USART_InitStruct = {0};
	
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOD);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	
	/* 2. 配置GPIO引脚 */
    // 2.1 配置USART TX/RX引脚 (PD8, PD9)
	GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
	LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	// 2.2 配置RS485方向控制引脚DE (例如 PD12)
    GPIO_InitStruct.Pin        = LL_GPIO_PIN_12;
    GPIO_InitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull       = LL_GPIO_PULL_DOWN; /* 默认拉低更安全：上电默认 RX */
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
 /* 3. 配置USART时钟源 */
    LL_RCC_SetUSARTClockSource(LL_RCC_USART234578_CLKSOURCE_PCLK1);
	/* 4. 配置DMA接收 */
    // 4.1 复位并暂停DMA流（安全关键操作）
    LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);
    while(LL_DMA_IsEnabledStream(DMA1, LL_DMA_STREAM_0)); // 等待确认禁用
    LL_DMA_DeInit(DMA1, LL_DMA_STREAM_0);
	
	// 4.2 配置DMA参数
	LL_DMA_SetPeriphRequest(DMA1, LL_DMA_STREAM_0, LL_DMAMUX1_REQ_USART3_RX);
	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_0, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_0, LL_DMA_PRIORITY_MEDIUM);
	LL_DMA_SetMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MODE_CIRCULAR);
	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_PERIPH_NOINCREMENT);
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MEMORY_INCREMENT);
	
	// 4.3 【关键】设置DMA地址和长度
	LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_0, LL_DMA_PDATAALIGN_BYTE);
	LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_0, LL_DMA_MDATAALIGN_BYTE);
	LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_0);
  
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_0, (uint32_t)&USART3->RDR);
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_0, (uint32_t)dma_rx_buf);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, DMA_RX_BUF_SIZE);
  
	SCB_CleanInvalidateDCache_by_Addr((uint32_t*)&dma_rx_buf, sizeof(dma_rx_buf));
	 // 4.4 配置DMA中断（半传输、全传输、错误）
    LL_DMA_EnableIT_HT(DMA1, LL_DMA_STREAM_0);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_0);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_0);
	
	NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
	
	USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
	USART_InitStruct.BaudRate = 115200;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	
	LL_USART_Init(USART3, &USART_InitStruct);
	
	LL_USART_DisableFIFO(USART3); // 禁用FIFO以获得更确定的行为
    LL_USART_ConfigAsyncMode(USART3);
	
	NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
    // DMA中断：抢占优先级6，子优先级0（略低于USART）
    NVIC_SetPriority(DMA1_Stream0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 6, 0));
	
	LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_12);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
	
	LL_USART_EnableDMAReq_RX(USART3);
    LL_USART_EnableIT_IDLE(USART3);
	
	LL_USART_Enable(USART3);
	while(!LL_USART_IsActiveFlag_TEACK(USART3) || 
		!LL_USART_IsActiveFlag_REACK(USART3));
	
	NVIC_EnableIRQ(USART3_IRQn);
    NVIC_EnableIRQ(DMA1_Stream0_IRQn);
}
//#endif

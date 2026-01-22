/*
  ******************************************************************************
  * @file    stl_util.h
  * @author  MCD Application Team
  * @brief   Header file of STL Utility.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STL_UTIL_H
#define STL_UTIL_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  STL_UTIL_CHECKSUM_COMP_OK = 0x0051445dU, /* non-trivial pattern */
  STL_UTIL_CHECKSUM_COMP_KO = 0x00482d95U /* non-trivial pattern */
} STL_UTIL_ChecksumComp_t;

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void STL_UTIL_CRC_Init(void);
void STL_UTIL_CRC_DeInit(void);
void STL_UTIL_CRC_Reset(void);
uint32_t STL_UTIL_CRC_Calculate(const uint32_t *pBuffer, uint32_t BufferLength);

STL_UTIL_ChecksumComp_t STL_UTIL_ChecksumCompare(void *pData, uint32_t Lenght, uint32_t Checksum);
uint32_t STL_UTIL_ChecksumUpdate(void *pData, uint32_t Lenght);

void STL_UTIL_Disable_IT(void);
void STL_UTIL_Enable_IT(void);

void STL_UTIL_Disable_FPU_IXC_IT(void);
void STL_UTIL_Restore_FPU_IXC_IT(void);


#endif /* STL_UTIL_H */

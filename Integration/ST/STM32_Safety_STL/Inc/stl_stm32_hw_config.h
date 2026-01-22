/*
  ******************************************************************************
  * @file    stl_stm32_hw_config.h
  * @author  MCD Application Team
  * @brief   Header file of STM32 HW configuration
  *          In order to be CMSIS agnostic, HW resources used by STL are re-defined here
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STL_STM32_HW_CONFIG_H
#define STL_STM32_HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  volatile uint32_t DR;          /*!< CRC Data register,                           Address offset: 0x00 */
  volatile uint32_t IDR;         /*!< CRC Independent data register,               Address offset: 0x04 */
  volatile uint32_t CR;          /*!< CRC Control register,                        Address offset: 0x08 */
  uint32_t          RESERVED2;   /*!< Reserved,                                                    0x0C */
  volatile uint32_t INIT;        /*!< Initial CRC value register,                  Address offset: 0x10 */
  volatile uint32_t POL;         /*!< CRC polynomial register,                     Address offset: 0x14 */
} STL_Wrapper_CRC_TypeDef;

/* Exported constants --------------------------------------------------------*/
#define STL_WRP_PERIPH_BASE                        (0x40000000UL)
#if defined (STM32H723xx) || defined (STM32H725xx) || defined (STM32H730xx) || defined (STM32H730xxQ) || defined (STM32H733xx) || \
    defined (STM32H735xx) || defined (STM32H742xx) || defined (STM32H743xx) || defined (STM32H753xx) || defined (STM32H750xx)
#define STL_WRP_AHB1PERIPH_BASE                    (STL_WRP_PERIPH_BASE + 0x18020000UL)
#define STL_WRP_CRC_BASE                           (STL_WRP_AHB1PERIPH_BASE + 0x00004C00UL)
#define STL_WRP_RCC_BASE                           (STL_WRP_AHB1PERIPH_BASE + 0x00004400UL)
#elif defined (STM32H7A3xx) || defined (STM32H7A3xxQ) || defined (STM32H7B0xx) || defined (STM32H7B0xxQ) || \
      defined (STM32H7B3xx) || defined (STM32H7B3xxQ)
#define STL_WRP_AHB1PERIPH_BASE                    (STL_WRP_PERIPH_BASE + 0x00020000UL)
#define STL_WRP_AHB4PERIPH_BASE                    (STL_WRP_PERIPH_BASE + 0x18020000UL)
#define STL_WRP_CRC_BASE                           (STL_WRP_AHB1PERIPH_BASE + 0x00003000UL)
#define STL_WRP_RCC_BASE                           (STL_WRP_AHB4PERIPH_BASE + 0x00004400UL)
#else
#error "Unknown device"
#endif

#define STL_WRP_CRC                                ((STL_Wrapper_CRC_TypeDef *) STL_WRP_CRC_BASE)

#define STL_WRP_CRC_IDR_DEFAULT_VALUE              0x00000000UL
#define STL_WRP_CRC_CR_OUTPUT_DATA_REVERSE_NONE    0x00000000UL
#define STL_WRP_CRC_CR_OUTPUT_DATA_REVERSE         0x00000080UL
#define STL_WRP_CRC_CR_INPUT_DATA_REVERSE_NONE     0x00000000UL
#define STL_WRP_CRC_CR_INPUT_DATA_REVERSE_WORD     0x00000060UL
#define STL_WRP_CRC_CR_INPUT_DATA_REVERSE_BYTE     0x00000020UL
#define STL_WRP_CRC_CR_POLYLENGTH_32B              0x00000000UL
#define STL_WRP_CRC_CR_DR_RESET                    0x00000001UL
#define STL_WRP_CRC_INIT_DEFAULT_VALUE             0xFFFFFFFFUL
#define STL_WRP_CRC_POL_DEFAULT_CRC32_POLY         0x04C11DB7UL

#if defined (STM32H723xx) || defined (STM32H725xx) || defined (STM32H730xx) || defined (STM32H730xxQ) || defined (STM32H733xx) || \
    defined (STM32H735xx) || defined (STM32H742xx) || defined (STM32H743xx) || defined (STM32H753xx) || defined (STM32H750xx)
#define STL_WRP_RCC_AHB1ENR_OFFSET                 (0xE0UL)
#define STL_WRP_RCC_AHB1ENR_CRCEN_Pos              (19UL)
#define STL_WRP_RCC_AHB1ENR_CRCEN_Msk              (0x1UL << STL_WRP_RCC_AHB1ENR_CRCEN_Pos)
#define STL_WRP_RCC_AHB1ENR_CRCEN                  STL_WRP_RCC_AHB1ENR_CRCEN_Msk
#define STL_WRP_RCC_AHBxENR_OFFSET                 STL_WRP_RCC_AHB1ENR_OFFSET
#define STL_WRP_RCC_AHBxENR_CRCEN                  STL_WRP_RCC_AHB1ENR_CRCEN
#elif defined (STM32H7A3xx) || defined (STM32H7A3xxQ) || defined (STM32H7B0xx) || defined (STM32H7B0xxQ) || \
      defined (STM32H7B3xx) || defined (STM32H7B3xxQ)
#define STL_WRP_RCC_AHB1ENR_OFFSET                 (0xD8UL)
#define STL_WRP_RCC_AHB1ENR_CRCEN_Pos              (9UL)
#define STL_WRP_RCC_AHB1ENR_CRCEN_Msk              (0x1UL << STL_WRP_RCC_AHB1ENR_CRCEN_Pos)
#define STL_WRP_RCC_AHB1ENR_CRCEN                  STL_WRP_RCC_AHB1ENR_CRCEN_Msk
#define STL_WRP_RCC_AHBxENR_OFFSET                 STL_WRP_RCC_AHB1ENR_OFFSET
#define STL_WRP_RCC_AHBxENR_CRCEN                  STL_WRP_RCC_AHB1ENR_CRCEN
#else
#error "Unknown device"
#endif

/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* STL_STM32_HW_CONFIG_H */


/*
  ******************************************************************************
  * @file    stl_user_api.h
  * @author  MCD Application Team
  * @brief   Header file of STL User API.
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
#ifndef STL_USER_API_H
#define STL_USER_API_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported constants --------------------------------------------------------*/
#define STL_OK_DEF              0x00006679U /* non-trivial & enumerated pattern */
#define STL_KO_DEF              0x0000aab5U /* non-trivial & enumerated pattern */
#define STL_PASSED_DEF          0x00001fe1U /* non-trivial & enumerated pattern */
#define STL_PARTIAL_PASSED_DEF  0x00014b55U /* non-trivial & enumerated pattern */
#define STL_FAILED_DEF          0x0000d32dU /* non-trivial & enumerated pattern */
#define STL_NOT_TESTED_DEF      0x0001ffffU /* non-trivial & enumerated pattern */
#define STL_ERROR_DEF           0x0001a976U /* non-trivial & enumerated pattern */
#define STL_TEST_ENABLE_DEF     0x0003d7a2U /* non-trivial & enumerated pattern */
#define STL_TEST_DISABLE_DEF    0x0006d5f9U /* non-trivial & enumerated pattern */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  STL_OK = STL_OK_DEF, /* Scheduler function successfully executed */
  STL_KO = STL_KO_DEF  /* Scheduler function unsuccessfully executed (defensive programming error,
                          checksum error). In this case the STL_TmStatus_t values are not relevant */
} STL_Status_t;        /* Type for the status return value of the STL function execution */

typedef enum
{
  STL_PASSED = STL_PASSED_DEF,                 /* Test passed. For Flash/RAM, test is passed and end
                                                  of configuration is also reached */
  STL_PARTIAL_PASSED = STL_PARTIAL_PASSED_DEF, /* Used only for RAM and Flash testing. Test passed, but
                                                  end of Flash/RAM configuration not yet reached */
  STL_FAILED = STL_FAILED_DEF,                 /* Hardware error detection by Test Module */
  STL_NOT_TESTED = STL_NOT_TESTED_DEF,         /* Initial value after a SW init, SW config, SW reset,
                                                  SW de-init or value when Test Module not executed */
  STL_ERROR = STL_ERROR_DEF                    /* Test Module unsuccessfully executed (defensive
                                                  programing check failed) */
} STL_TmStatus_t;                              /* Type for the result of a Test Module */

typedef enum
{
  STL_TEST_ENABLE = STL_TEST_ENABLE_DEF,       /* Used to add a Test Module in the list to run */
  STL_TEST_DISABLE = STL_TEST_DISABLE_DEF      /* Used to remove a Test Module from the list to run*/
} STL_TmEnable_t;                              /* Type used/checked only for Multiple testing, to add/remove
                                                  a test module in/from the test list */

typedef enum
{
  STL_CPU_TM1_IDX = 0U,          /* CPU ARM Core Test Module 1 index */
  STL_CPU_TM1L_IDX,              /* CPU ARM Core Test Module 1L index */
  STL_CPU_TM2_IDX,               /* CPU ARM Core Test Module 2 index */
  STL_CPU_TM3_IDX,               /* CPU ARM Core Test Module 3 index */
  STL_CPU_TM4_IDX,               /* CPU ARM Core Test Module 4 index */
  STL_CPU_TM5_IDX,               /* CPU ARM Core Test Module 5 index */
  STL_CPU_TM6_IDX,               /* CPU ARM Core Test Module 6 index */
  STL_CPU_TM7_IDX,               /* CPU ARM Core Test Module 7 index */
  STL_CPU_TM7L_IDX,              /* CPU ARM Core Test Module 7L index */
  STL_CPU_TM8_IDX,               /* CPU ARM Core Test Module 8 index */
  STL_CPU_TM9_IDX,               /* CPU ARM Core Test Module 9 index */
  STL_CPU_TM10_IDX,               /* CPU ARM Core Test Module 10 index */
  STL_CPU_TM11_IDX,               /* CPU ARM Core Test Module 11 (Single Precision) index */
  STL_CPU_TM11_DP_IDX,               /* CPU ARM Core Test Module 11 Double Precision index */
  STL_CPU_TM12_IDX,               /* CPU ARM Core Test Module 12 index */
  STL_CPU_TM_MAX                 /* Number of CPU ARM Core Test Modules */
} STL_CpuTmxIndex_t;             /* Type for index of CPU ARM Core Test Modules */

typedef struct STL_UserTmConfig_struct       /* Structure tag that define a User Test */
{
  uint32_t TmId;                             /* ID of one User Test */
  STL_TmStatus_t (*pFunc)(void *FuncParam);  /* Pointer to user function to be run by the STL scheduler */
  void *pFuncParam;                          /* Pointer to user function parameters */
  struct STL_UserTmConfig_struct *pNext;     /* Pointer to next User tests configuration – to be set to NULL
                                                for the last user function */
} STL_UserTmConfig_t;                        /* Type used to define User Tests test list – only useful
                                                during multiple testing – if not used, to be set to NULL */

typedef struct STL_UserTmStatus_struct       /* Structure tag that define a User Test test status */
{
  uint32_t TmId;                             /* ID of one User Test */
  STL_TmStatus_t TmStatus;                   /* Test status of the User Test corresponding to this ID */
  struct STL_UserTmStatus_struct *pNext;     /* Pointer to next User Test test status definition */
} STL_UserTmStatus_t;                        /* Type used to define User Tests status */

typedef struct
{
  STL_TmStatus_t aCpuTmStatus[STL_CPU_TM_MAX]; /* Array for CPU ARM Core Test Module test status */
  STL_TmStatus_t FlashTmStatus;                /* Flash Test Module status */
  STL_TmStatus_t RamTmStatus;                  /* RAM Test Module status */
  STL_UserTmStatus_t *pUserTmStatus;           /* Pointer to User Tests status */
} STL_TmListStatus_t;                          /* Type used for Test Modules status */

typedef struct
{
  STL_TmEnable_t aCpuTmEnable[STL_CPU_TM_MAX]; /* Array for CPU ARM Core Test Module test list */
  STL_TmEnable_t FlashTmEnable;                /* Flash Test Module enablement information */
  STL_TmEnable_t RamTmEnable;                  /* RAM Test Module enablement information */
} STL_TmListEnable_t;                          /* Type used to define STL internal tests list */

typedef struct STL_MemSubset_struct
{
  uint32_t StartAddr;                          /* Start address of Flash or RAM  memory subset */
  uint32_t EndAddr;                            /* End address of Flash or RAM memory subset */
  struct STL_MemSubset_struct *pNext;          /* Pointer to the next Flash or RAM memory subset -
                                                  to be set to NULL for the last subset  */
} STL_MemSubset_t;                             /* Type used to define Flash or RAM subsets to test */

typedef struct
{
  STL_MemSubset_t *pSubset;                    /* Pointer to the Flash or RAM subsets to test */
  uint32_t NumSectionsAtomic;                  /* Number of Flash or RAM sections to be tested during
                                                  an atomic test */
} STL_MemConfig_t;                             /* Type used to fully define Flash or RAM test configuration */

typedef struct
{
  STL_TmStatus_t aCpuTmStatus[STL_CPU_TM_MAX]; /* Array of forced status value for CPU Test Modules */
  STL_TmStatus_t FlashTmStatus;                /* Forced status value for Flash Test Module */
  STL_TmStatus_t RamTmStatus;                  /* Forced status value for RAM Test Module */
} STL_ArtifFailingConfig_t;                    /* Type used to force Test Modules status to a specific value
                                                  for each STL Test Module */

/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* User API functions prototypes */
/* All */
STL_Status_t STL_SCH_Init(void);                                  /* To initalise the scheduler */

/* CPU */
STL_Status_t STL_SCH_RunCpuTM1(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 1 - only for single test */

STL_Status_t STL_SCH_RunCpuTM1L(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 1L - only for single test */

STL_Status_t STL_SCH_RunCpuTM2(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 2 - only for single test */

STL_Status_t STL_SCH_RunCpuTM3(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 3 - only for single test */

STL_Status_t STL_SCH_RunCpuTM4(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 4 - only for single test */

STL_Status_t STL_SCH_RunCpuTM5(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 5 - only for single test */

STL_Status_t STL_SCH_RunCpuTM6(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 6 - only for single test */

STL_Status_t STL_SCH_RunCpuTM7(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 7 - only for single test */

STL_Status_t STL_SCH_RunCpuTM7L(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 7L - only for single test */

STL_Status_t STL_SCH_RunCpuTM8(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 8 - only for single test */

STL_Status_t STL_SCH_RunCpuTM9(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 9 - only for single test */

STL_Status_t STL_SCH_RunCpuTM10(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 10 - only for single test */

STL_Status_t STL_SCH_RunCpuTM11(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 11 (Single Precision) - only for single test */

STL_Status_t STL_SCH_RunCpuTM11Dp(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 11 Double Precision - only for single test */

STL_Status_t STL_SCH_RunCpuTM12(STL_TmStatus_t *pSingleTmStatus);  /* To run CPU Test Module 12 - only for single test */

/* Flash */
STL_Status_t STL_SCH_InitFlash(STL_TmStatus_t *pSingleTmStatus);  /* To initialise Flash test - only for single test */

STL_Status_t STL_SCH_ConfigureFlash(STL_TmStatus_t *pSingleTmStatus, STL_MemConfig_t *pFlashConfig);
/* To configure Flash subsets - only for single test */

STL_Status_t STL_SCH_RunFlashTM(STL_TmStatus_t *pSingleTmStatus); /* To run Flash test - only for single test */

STL_Status_t STL_SCH_ResetFlash(STL_TmStatus_t *pSingleTmStatus); /* To reset Flash test - for single or multiple test*/

STL_Status_t STL_SCH_DeInitFlash(STL_TmStatus_t *pSingleTmStatus);/* To de-initialise Flash test - only for single test */

/* Ram */
STL_Status_t STL_SCH_InitRam(STL_TmStatus_t *pSingleTmStatus);    /* To initialise Ram test - only for single test */

STL_Status_t STL_SCH_ConfigureRam(STL_TmStatus_t *pSingleTmStatus, STL_MemConfig_t *pRamConfig);
/* To configure Ram subsets - only for single test */

STL_Status_t STL_SCH_RunRamTM(STL_TmStatus_t *pSingleTmStatus);   /* To run Ram test - only for single test */

STL_Status_t STL_SCH_ResetRam(STL_TmStatus_t *pSingleTmStatus);   /* To reset Ram test - for single or multiple test */

STL_Status_t STL_SCH_DeInitRam(STL_TmStatus_t *pSingleTmStatus);  /* To de-initialise Ram test - only for single test */

/* Multiple test (CPU, RAM, Flash, User Test) */
STL_Status_t STL_SCH_InitAllTM(STL_TmListStatus_t *pTmListStatus, STL_TmListEnable_t *pTmListEnable);
/* To initialise all tests - only for multiple test */

STL_Status_t STL_SCH_ConfigureAllTM(STL_TmListStatus_t *pTmListStatus, STL_MemConfig_t *pFlashConfig, STL_MemConfig_t *pRamConfig, const STL_UserTmConfig_t *pUserTmConfig);
/* To configure all tests - only for multiple test */

STL_Status_t STL_SCH_RunAllTM(STL_TmListStatus_t *pTmListStatus); /* To run all tests - only for multiple test */

/* artificial failing */
STL_Status_t STL_SCH_StartArtifFailing(const STL_ArtifFailingConfig_t *pArtifFailingConfig);
/* To set artificial failing configuration and start
   artificial failing feature - for single and multiple tests */
STL_Status_t STL_SCH_StopArtifFailing(void);
/* To stop artificial failing */

#endif /* STL_USER_API_H */

/**
  ******************************************************************************
  * @file    fsr.h
  * @brief   This file contains all the register definitions and function 
  *          prototypes for the fsr.c file.
  * @author  doublehan07
  * @version V1.0
  * @date    2024-12-14
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FSR_H
#define __FSR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported constants --------------------------------------------------------*/
extern __IO uint16_t _u16ADC_Value[50];
extern __IO uint32_t reference_vdd;

/* Exported functions --------------------------------------------------------*/
uint32_t dbh_FSR_GetADCValue(void);

#ifdef __cplusplus
}
#endif

#endif /* __FSR_H */
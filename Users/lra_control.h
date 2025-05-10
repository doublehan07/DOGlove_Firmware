/**
  ******************************************************************************
  * @file    lra_control.h
  * @brief   This file contains all the function prototypes for the lra_control.c
  * @author  doublehan07
  * @version V1.0
  * @date    2024-08-15
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LRA_CONTROL_H
#define __LRA_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported functions ------------------------------------------------------- */
void dbh_LRA_Control_Init(void);

uint8_t dbh_GetChannel(void);
uint8_t dbh_GetWaveNum(uint8_t channel);
uint16_t dbh_GetDuration(uint8_t channel);
void dbh_ResetCounter(uint8_t channel);
void dbh_DecCounter(void);
uint16_t dbh_GetCounter(uint8_t channel);

void dbh_IncTimestampInMS(void);
uint16_t dbh_GetTimestamp(void);

#ifdef __cplusplus
}
#endif

#endif /* __LRA_CONTROL_H */

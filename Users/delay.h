/**
  ******************************************************************************
  * @file    delay.h
  * @brief   This file contains all the function prototypes for the delay.c file.
  * @author  doublehan07
  * @version V1.0
  * @date    2023-04-29
  ******************************************************************************
  */
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __sDELAY_H
#define __sDELAY_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void dbh_DecTick(void);
void dbh_DelayMS(uint32_t delay_time_ms);

#endif /* __sDELAY_H */

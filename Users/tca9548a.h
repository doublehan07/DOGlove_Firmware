/**
  ******************************************************************************
  * @file    tca9548a.h
  * @brief   This file contains all the register definitions and function 
  *          prototypes for the tca9548a.c file.
  * @author  doublehan07
  * @version V1.0
  * @date    2024-09-30
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/    
#ifndef __TCA9548A_H
#define __TCA9548A_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported macro ------------------------------------------------------------*/
#define TCA9548A_SLAVE_ADDRESS (0x70<<1) // TCA9548A I2C address

/* Exported functions prototypes ---------------------------------------------*/
uint8_t dbh_TCA9548A_Init(void);
void dbh_TCA9548A_SelectChannel(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif /* __TCA9548A_H */
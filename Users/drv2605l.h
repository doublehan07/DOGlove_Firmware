/**
  ******************************************************************************
  * @file    drv2605l.h
  * @brief   This file contains all the register definitions and function 
  *          prototypes for the drv2605l.c file.
  * @author  doublehan07
  * @version V1.0
  * @date    2024-08-01
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV2605L_H
#define __DRV2605L_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported macro ------------------------------------------------------------*/
#define DRV2605L_REG_STATUS                     0x00
#define DRV2605L_REG_MODE                       0x01
#define DRV2605L_REG_RTP_INPUT                  0x02
//Library selection register
#define DRV2605L_REG_LIB_SEL                    0x03
#define DRV2605L_REG_WAV_SEQ_1                  0x04
#define DRV2605L_REG_WAV_SEQ_2                  0x05
#define DRV2605L_REG_WAV_SEQ_3                  0x06
#define DRV2605L_REG_WAV_SEQ_4                  0x07
#define DRV2605L_REG_WAV_SEQ_5                  0x08
#define DRV2605L_REG_WAV_SEQ_6                  0x09
#define DRV2605L_REG_WAV_SEQ_7                  0x0A
#define DRV2605L_REG_WAV_SEQ_8                  0x0B
#define DRV2605L_REG_GO                         0x0C
//Time offset register
#define DRV2605L_REG_TOFFSET_OVERDRIVE          0x0D        
#define DRV2605L_REG_TOFFSET_SUSTAIN_P          0x0E
#define DRV2605L_REG_TOFFSET_SUSTAIN_N          0x0F
#define DRV2605L_REG_TOFFSET_BRAKE              0x10
//Audio to Vibe control register
#define DRV2605L_REG_ATH_CTRL                   0x11
#define DRV2605L_REG_ATH_MIN_INPUT              0x12
#define DRV2605L_REG_ATH_MAX_INPUT              0x13
#define DRV2605L_REG_ATH_MIN_DRIVE              0x14
#define DRV2605L_REG_ATH_MAX_DRIVE              0x15
//Voltage register
#define DRV2605L_REG_RATED_VOLTAGE              0x16
#define DRV2605L_REG_OD_CLAMP_VOLTAGE           0x17
//Auto calibration register
#define DRV2605L_REG_A_CAL_COMP                 0x18
#define DRV2605L_REG_A_CAL_BEMF                 0x19
//Control register
#define DRV2605L_REG_FEEDBACK_CTRL              0x1A
#define DRV2605L_REG_CTRL_1                     0x1B  
#define DRV2605L_REG_CTRL_2                     0x1C
#define DRV2605L_REG_CTRL_3                     0x1D
#define DRV2605L_REG_CTRL_4                     0x1E
#define DRV2605L_REG_CTRL_5                     0x1F
//LRA control register
#define DRV2605L_REG_LRA_OPEN_LOOP_PERIOD       0x20
#define DRV2605L_REG_VBAT_VOLTAGE               0x21
#define DRV2605L_REG_LRA_RESONANCE_PERIOD       0x22

//DRV2605L I2C Slave Address
#define DRV2605L_SLAVE_ADDRESS                  (0x5A<<1) //The LSB is the R/W bit

typedef struct
{
  __IO uint8_t DEVICE_ID;     /*!< Specifies the device ID, BIT[7:5] */
  __IO uint8_t DIAG_RESULT;   /*!< Specifies the diagnostic result, BIT[3] */
  __IO uint8_t OVER_TEMP;     /*!< Specifies the over temperature status, BIT[1] */
  __IO uint8_t OC_DETECT;     /*!< Specifies the over current detection status, BIT[0] */
} DRV2605L_STATUS_TypeDef;


/* Exported functions ------------------------------------------------------- */
void dbh_DRV2605L_Init(void);
void dbh_DRV2605L_PlayWaveform(uint8_t num);
void dbh_DRV2605L_StopWaveform(void);
void DRV2605L_GetStatus(void);
uint8_t DRV2605L_GetDIAG(void);

#ifdef __cplusplus
}
#endif

#endif /* __DRV2605L_H */

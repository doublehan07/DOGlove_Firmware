/**
  ******************************************************************************
  * @file    ads1256.h
  * @brief   This file contains all the register definitions and function 
  *          prototypes for the ads1256.c file.
  * @author  doublehan07
  * @version V1.0
  * @date    2024-07-28
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADS1256_H
#define __ADS1256_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported macro ------------------------------------------------------------*/
// ADS1256 register map 
#define ADS1256_REG_STATUS        0x00   
#define ADS1256_REG_MUX           0x01   
#define ADS1256_REG_ADCON         0x02   
#define ADS1256_REG_DRATE         0x03   
#define ADS1256_REG_IO            0x04   
#define ADS1256_REG_OFC0          0x05   
#define ADS1256_REG_OFC1          0x06   
#define ADS1256_REG_OFC2          0x07   
#define ADS1256_REG_FSC0          0x08   
#define ADS1256_REG_FSC1          0x09   
#define ADS1256_REG_FSC2          0x0A 

// ADS1256 command definitions
#define ADS1256_CMD_WAKEUP        0x00 
#define ADS1256_CMD_RDATA         0x01 
#define ADS1256_CMD_RDATAC        0x03 
#define ADS1256_CMD_SDATAC        0x0F 
#define ADS1256_CMD_RREG          0x10 
#define ADS1256_CMD_WREG          0x50 
#define ADS1256_CMD_SELFCAL       0xF0 
#define ADS1256_CMD_SELFOCAL      0xF1 
#define ADS1256_CMD_SELFGCAL      0xF2 
#define ADS1256_CMD_SYSOCAL       0xF3 
#define ADS1256_CMD_SYSGCAL       0xF4 
#define ADS1256_CMD_SYNC          0xFC 
#define ADS1256_CMD_STANDBY       0xFD 
#define ADS1256_CMD_RESET         0xFE
// #define ADS1256_CMD_WAKEUP        0xFF
 
// ADS1256 input multiplexer control register
// ADS1256_MUX = ADS1256_MUXP | ADS1256_MUXN
// Positive input channel select codes 
#define ADS1256_MUXP_AIN0         0x00 // AIN0 = 0x0000 0000 (default)
#define ADS1256_MUXP_AIN1         0x10 // AIN1 = 0x0001 0000
#define ADS1256_MUXP_AIN2         0x20 // AIN2 = 0x0010 0000
#define ADS1256_MUXP_AIN3         0x30 // AIN3 = 0x0011 0000
#define ADS1256_MUXP_AIN4         0x40 // AIN4 = 0x0100 0000
#define ADS1256_MUXP_AIN5         0x50 // AIN5 = 0x0101 0000
#define ADS1256_MUXP_AIN6         0x60 // AIN6 = 0x0110 0000
#define ADS1256_MUXP_AIN7         0x70 // AIN7 = 0x0111 0000
#define ADS1256_MUXP_AINCOM       0x80 // AINCOM = 0x1000 0000
// Negative input channel select codes
#define ADS1256_MUXN_AIN0         0x00 // AIN0 = 0x0000 0000
#define ADS1256_MUXN_AIN1         0x01 // AIN1 = 0x0000 0001 (default)
#define ADS1256_MUXN_AIN2         0x02 // AIN2 = 0x0000 0010
#define ADS1256_MUXN_AIN3         0x03 // AIN3 = 0x0000 0011
#define ADS1256_MUXN_AIN4         0x04 // AIN4 = 0x0000 0100
#define ADS1256_MUXN_AIN5         0x05 // AIN5 = 0x0000 0101
#define ADS1256_MUXN_AIN6         0x06 // AIN6 = 0x0000 0110
#define ADS1256_MUXN_AIN7         0x07 // AIN7 = 0x0000 0111
#define ADS1256_MUXN_AINCOM       0x08 // AINCOM = 0x0000 1000

// ADS1256 A/D control register
// Programmable gain amplifier setting codes
#define ADS1256_GAIN_1            0x00 // 1x gain = 0x0000 0000 (default)
#define ADS1256_GAIN_2            0x01 // 2x gain = 0x0000 0001
#define ADS1256_GAIN_4            0x02 // 4x gain = 0x0000 0010
#define ADS1256_GAIN_8            0x03 // 8x gain = 0x0000 0011
#define ADS1256_GAIN_16           0x04 // 16x gain = 0x0000 0100
#define ADS1256_GAIN_32           0x05 // 32x gain = 0x0000 0101
#define ADS1256_GAIN_64           0x06 // 64x gain = 0x0000 0110
// #define ADS1256_GAIN_64           0x07 // 64x gain = 0x0000 0111

// ADS1256 A/D data rate register
// data rate setting codes
#define ADS1256_DRATE_30000SPS    0xF0 // 30,000SPS = 0x1111 0000 (default)
#define ADS1256_DRATE_15000SPS    0xE0 // 15,000SPS = 0x1110 0000
#define ADS1256_DRATE_7500SPS     0xD0 // 7,500SPS = 0x1101 0000
#define ADS1256_DRATE_3750SPS     0xC0 // 3,750SPS = 0x1100 0000
#define ADS1256_DRATE_2000SPS     0xB0 // 2,000SPS = 0x1011 0000
#define ADS1256_DRATE_1000SPS     0xA1 // 1,000SPS = 0x1010 0001
#define ADS1256_DRATE_500SPS      0x92 // 500SPS = 0x1001 0010
#define ADS1256_DRATE_100SPS      0x82 // 100SPS = 0x1000 0010
#define ADS1256_DRATE_60SPS       0x72 // 60SPS = 0x0111 0010
#define ADS1256_DRATE_50SPS       0x63 // 50SPS = 0x0110 0011
#define ADS1256_DRATE_30SPS       0x53 // 30SPS = 0x0101 0011
#define ADS1256_DRATE_25SPS       0x43 // 25SPS = 0x0100 0011
#define ADS1256_DRATE_15SPS       0x33 // 15SPS = 0x0011 0011
#define ADS1256_DRATE_10SPS       0x23 // 10SPS = 0x0010 0011
#define ADS1256_DRATE_5SPS        0x13 // 5SPS = 0x0001 0011
#define ADS1256_DRATE_2_5SPS      0x03 // 2.5SPS = 0x0000 0011

/* Exported functions ------------------------------------------------------- */
void dbh_ADS1256_Init(uint8_t device);
void dbh_ADS1256_SelectChannel(uint8_t channel, uint8_t device);
int32_t dbh_ADS1256_ReadData(uint8_t device);

#ifdef __cplusplus
}
#endif

#endif /* __ADS1256_H */

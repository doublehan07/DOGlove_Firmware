/**
  ******************************************************************************
  * @file    tc9548a.c
  * @brief   This file contains the functions to interface with the TCA9548A I2C multiplexer
  * @author  doublehan07
  * @version V1.0
  * @date    2024-09-30
  ******************************************************************************
  */

#include "tca9548a.h"
#include "i2c.h"

/**
  * @brief  Initialize the TCA9548A I2C multiplexer
  * @retval 1 if successful, 0 if failed
  */
uint8_t dbh_TCA9548A_Init(void)
{
    uint8_t tx_data = 0xA4; // Set the control register to 0xA4, this is a randomly chosen known value
    uint8_t rx_data = 0;

    HAL_I2C_Master_Transmit(&hi2c1, TCA9548A_SLAVE_ADDRESS, &tx_data, 1, 1000); // Write the data to the control register
    HAL_I2C_Master_Receive(&hi2c1, TCA9548A_SLAVE_ADDRESS, &rx_data, 1, 1000); // Read the data from the control register
    tx_data = 0x00; // Set the control register to 0x00 to disable all channels
    HAL_I2C_Master_Transmit(&hi2c1, TCA9548A_SLAVE_ADDRESS, &tx_data, 1, 1000); // Write the data to the control register

    if (rx_data == 0xA4)
    {
        return 1; // Success
    }
    else
    {
        return 0; // Failure
    }
}

/**
  * @brief  Select the channel on the TCA9548A I2C multiplexer
  * @param  channel: the channel to select, 0-7
  * @retval None
  */
void dbh_TCA9548A_SelectChannel(uint8_t channel)
{
    uint8_t tx_data = 0;

    if (channel < 8)
    {
        tx_data = 1 << channel; // Set the bit corresponding to the channel to 1
    }
    else
    {
        tx_data = 0; // Set all bits to 0, disable all channels
    }

    HAL_I2C_Master_Transmit(&hi2c1, TCA9548A_SLAVE_ADDRESS, &tx_data, 1, 1000); // Write the data to the control register
}

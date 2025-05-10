/**
  ******************************************************************************
  * @file    ads1256.c
  * @brief   This file contains the functions to interface with the ADS1256 A/D converter
  * @author  doublehan07
  * @version V1.0
  * @date    2024-07-28
  ******************************************************************************
  */

#include "ads1256.h"
#include "spi.h"

// Read the DRDY pin for the specified device, 0 for device 1, 1 for device 2, 2 for device 3
#define ADS1256_DRDY(x)  ((x) == 2 ? HAL_GPIO_ReadPin(ADS1256_DRDY_3_GPIO_Port, ADS1256_DRDY_3_Pin) : \
                         ((x) == 1 ? HAL_GPIO_ReadPin(ADS1256_DRDY_2_GPIO_Port, ADS1256_DRDY_2_Pin) : \
                                     HAL_GPIO_ReadPin(ADS1256_DRDY_1_GPIO_Port, ADS1256_DRDY_1_Pin)))

// Set the CS pin low to select the device, 0 for device 1, 1 for device 2, 2 for device 3
#define CS_LOW(x)        ((x) == 2 ? HAL_GPIO_WritePin(SPI1_CS3_GPIO_Port, SPI1_CS3_Pin, GPIO_PIN_RESET) : \
                         ((x) == 1 ? HAL_GPIO_WritePin(SPI1_CS2_GPIO_Port, SPI1_CS2_Pin, GPIO_PIN_RESET) : \
                                     HAL_GPIO_WritePin(SPI1_CS1_GPIO_Port, SPI1_CS1_Pin, GPIO_PIN_RESET)))

// Set the CS pin high to release the device, 0 for device 1, 1 for device 2, 2 for device 3
#define CS_HIGH(x)       ((x) == 2 ? HAL_GPIO_WritePin(SPI1_CS3_GPIO_Port, SPI1_CS3_Pin, GPIO_PIN_SET) : \
                         ((x) == 1 ? HAL_GPIO_WritePin(SPI1_CS2_GPIO_Port, SPI1_CS2_Pin, GPIO_PIN_SET) : \
                                     HAL_GPIO_WritePin(SPI1_CS1_GPIO_Port, SPI1_CS1_Pin, GPIO_PIN_SET)))

__IO HAL_StatusTypeDef status;

/**
  * @brief  Use WREG command to write to a single register on the ADS1256
  * @param  reg: the register address to write to
  * @param  data: the 1-byte data to write to the register
  * @retval None
  */
void ADS1256_WREG(uint8_t reg, uint8_t data, uint8_t device)
{
    uint8_t commands[3] = {0};

    //Build the command to write to the specified register
    commands[0] = ADS1256_CMD_WREG | (reg & 0x0F); // Send the write register command (0b 0101 rrrr where rrrr is the register address)
    commands[1] = 0x00; // Send the number of registers to write minus one (0x00 for one register)
    commands[2] = data; // Send the data to write to the register

    while (ADS1256_DRDY(device) == GPIO_PIN_SET); // Wait for DRDY to go low to indicate the device is ready
    CS_LOW(device); // Select the current device
    HAL_SPI_Transmit(&hspi1, commands, 3, 1000); // Send the write register command
    CS_HIGH(device); // Release the current device
}

/**
  * @brief  Use SELFCAL command to do a self-calibration on the ADS1256
  * @retval None
  */
void ADS1256_SelfCal(uint8_t device)
{
    uint8_t command = ADS1256_CMD_SELFCAL;

    while (ADS1256_DRDY(device) == GPIO_PIN_SET); // Wait for DRDY to go low to indicate the device is ready
    CS_LOW(device); // Select the current device
    HAL_SPI_Transmit(&hspi1, &command, 1, 1000); // Send the self-calibration command
    while (ADS1256_DRDY(device) == GPIO_PIN_SET); // Wait for DRDY to go low to indicate the calibration is complete
    CS_HIGH(device); // Release the current device
}

/**
  * @brief  Initialize the ADS1256
  * @retval None
  */
void dbh_ADS1256_Init(uint8_t device)
{    
    // Set the low 4 bits of the status register to 0x06 (0b 0000 0110)
    // Bit 3:   0 - Most significant bit first
    // Bit 2:   1 - Auto-calibration enabled
    // Bit 1:   1 - Analog input buffer enabled
    // Bit 0:   0 - !DRDY (Read only, don't care)
    ADS1256_WREG(ADS1256_REG_STATUS, 0x06, device);    

    // Set the A/D control register to 0x20 (0b 0010 0000)
    // Bit 7:   0 - Reserved, always 0 (Read only)
    // Bit 6-5: 00 - Clock out OFF
    // Bit 4-3: 00 - Sensor detect OFF
    // Bit 2-0: 000 - Programmable gain amplifier setting = 1
    ADS1256_WREG(ADS1256_REG_ADCON, 0x00 | ADS1256_GAIN_1, device);

    // Set the A/D data rate register to 30,000SPS
    ADS1256_WREG(ADS1256_REG_DRATE, ADS1256_DRATE_30000SPS, device);

    // Perform a self-calibration
    ADS1256_SelfCal(device);

    // Set the input multiplexer register to AIN0 and AINCOM
    dbh_ADS1256_SelectChannel(0, device);
}

/**
  * @brief  Select the specified channel on the ADS1256
  * @param  channel: the channel to select (0-7)
  * @retval None
  */
void dbh_ADS1256_SelectChannel(uint8_t channel, uint8_t device)
{
    uint8_t commands[2] = {0};

    // Build the command to select the specified channel
    commands[0] = ADS1256_CMD_SYNC; // Send the SYNC command
    commands[1] = ADS1256_CMD_WAKEUP; // Send the WAKEUP command

    if (channel < 8)
    {
        while (ADS1256_DRDY(device) == GPIO_PIN_SET); // Wait for DRDY to go low to indicate the device is ready

        // Set the input multiplexer register to the specified channel
        switch (channel)
        {
            case 0:
                ADS1256_WREG(ADS1256_REG_MUX, ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM, device);
                break;
            case 1:
                ADS1256_WREG(ADS1256_REG_MUX, ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM, device);
                break;
            case 2:
                ADS1256_WREG(ADS1256_REG_MUX, ADS1256_MUXP_AIN2 | ADS1256_MUXN_AINCOM, device);
                break;
            case 3:
                ADS1256_WREG(ADS1256_REG_MUX, ADS1256_MUXP_AIN3 | ADS1256_MUXN_AINCOM, device);
                break;
            case 4:
                ADS1256_WREG(ADS1256_REG_MUX, ADS1256_MUXP_AIN4 | ADS1256_MUXN_AINCOM, device);
                break;
            case 5:
                ADS1256_WREG(ADS1256_REG_MUX, ADS1256_MUXP_AIN5 | ADS1256_MUXN_AINCOM, device);
                break;
            case 6:
                ADS1256_WREG(ADS1256_REG_MUX, ADS1256_MUXP_AIN6 | ADS1256_MUXN_AINCOM, device);
                break;
            case 7:
                ADS1256_WREG(ADS1256_REG_MUX, ADS1256_MUXP_AIN7 | ADS1256_MUXN_AINCOM, device);
                break;
        }

        // Send the SYNC and WAKEUP command to synchronize the A/D conversion
        CS_LOW(device); // Select the current device
        HAL_SPI_Transmit(&hspi1, commands, 1, 1000); // Send the SYNC command
        // The duration of the SYNC command and the WAKEUP command is at least 24 * tCLKIN, that is, 24 * 1 / 7.68MHz = 3.125us
        HAL_SPI_Transmit(&hspi1, commands+1, 1, 1000); // Send the WAKEUP command
        CS_HIGH(device); // Release the current device
    }
}

/**
  * @brief  Read the conversion data from the ADS1256
  * @retval the 24-bit conversion data
  */
int32_t dbh_ADS1256_ReadData(uint8_t device)
{
    int32_t result = 0;
    uint32_t data = 0;
    uint8_t rx_data[3] = {0};
    uint8_t command = ADS1256_CMD_RDATA;

    while (ADS1256_DRDY(device) == GPIO_PIN_SET); // Wait for DRDY to go low to indicate the device is ready
    CS_LOW(device); // Select the current device
    status = HAL_SPI_Transmit(&hspi1, &command, 1, 1000); // Send the RDATA command to read the conversion
    status = HAL_SPI_Receive(&hspi1, rx_data, 3, 1000); // Read the conversion data
    CS_HIGH(device); // Release the current device

    // Combine the 3 bytes of conversion data into a single 24-bit value
    data = (rx_data[0] << 16) | (rx_data[1] << 8) | rx_data[2];

    if (data & 0x800000) // If the most significant bit is set, the value is negative
    {
        // Do two's complement to get the negative value
        data = ~data + 1;
        data &= 0xFFFFFF; // Mask off the upper 8 bits
        result = -data;
    }
    else // The value is positive
    {
        result = data;
    }

    return result;
}

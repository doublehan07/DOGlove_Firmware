/**
  ******************************************************************************
  * @file    drv2605l.c
  * @brief   This file contains the functions to interface with the DRV2605L haptic driver
  * @author  doublehan07
  * @version V1.0
  * @date    2024-08-01
  ******************************************************************************
  */

#include "drv2605l.h"
#include "i2c.h"
#include "delay.h"

__IO HAL_StatusTypeDef i2c_status;
__IO DRV2605L_STATUS_TypeDef reg_status;

/**
  * @brief  Write data to a register of the DRV2605L
  * @param  reg: The register address to write to
  * @param  data: The data to write to the register
  * @retval None
  */
void DRV2605L_WriteReg(uint8_t reg, uint8_t data)
{
    uint8_t tx_data[2] = {reg, data};

    i2c_status = HAL_I2C_Master_Transmit(&hi2c1, DRV2605L_SLAVE_ADDRESS, tx_data, 2, 1000); // Write the data to the register
}

/**
  * @brief  Read data from a register of the DRV2605L
  * @param  reg: The register address to read from
  * @param  data: Pointer to the variable to store the read data
  * @retval None
  */
void DRV2605L_ReadReg(uint8_t reg, uint8_t *data)
{
    uint8_t command = reg;

    i2c_status = HAL_I2C_Master_Transmit(&hi2c1, DRV2605L_SLAVE_ADDRESS, &command, 1, 1000); // Send the command to read the register
    i2c_status = HAL_I2C_Master_Receive(&hi2c1, DRV2605L_SLAVE_ADDRESS, data, 1, 1000); // Read the register
}

/**
  * @brief  Get the status of the DRV2605L
  * @retval None
  *
  * This function reads the status register of the DRV2605L and updates the reg_status structure.
  */
void DRV2605L_GetStatus(void)
{
    uint8_t rx_data = 0;

    DRV2605L_ReadReg(DRV2605L_REG_STATUS, &rx_data); // Read the status register

    if (i2c_status == HAL_OK)
    {
        reg_status.DEVICE_ID = (rx_data>>5) & 0x07; // Bits [7:5] is DEVICE_ID, should be 0x07 for DRV2605L
        reg_status.DIAG_RESULT = (rx_data>>3) & 0x01; // Bit [3] is DIAG_RESULT
        reg_status.OVER_TEMP = (rx_data>>1) & 0x01; // Bit [1] is OVER_TEMP
        reg_status.OC_DETECT = rx_data & 0x01; // Bit [0] is OC_DETECT
    }
    else
    {
        reg_status.DEVICE_ID = 0;
        reg_status.DIAG_RESULT = 1;
        reg_status.OVER_TEMP = 1;
        reg_status.OC_DETECT = 1;
    }
}

/**
  * @brief  Get the diagnostic result of the DRV2605L
  * @retval The diagnostic result
  */
uint8_t DRV2605L_GetDIAG(void)
{
    return reg_status.DIAG_RESULT;
}

/**
  * @brief  Initialize the DRV2605L
  * @retval None
  *
  * This function initializes the DRV2605L by configuring its registers and starting the auto-calibration process.
  * It sets the feedback control register, control registers, and mode register according to the LRA specifications.
  * It also reads the auto-calibration compensation result, back-EMF result, and back-EMF gain.
  */
void dbh_DRV2605L_Init(void)
{
    uint8_t A_CAL_COMP = 0;
    uint8_t A_CAL_BEMF = 0;
    uint8_t BEMF_GAIN = 0;

    DRV2605L_GetStatus(); // Get the status of the DRV2605L
    if (reg_status.DEVICE_ID == 0x07) // If the device ID is correct, start the initialization
    {
        // Select the Immersion LRA library 0x0000 0110
        // Bit 7-5: 000 - Reserved
        // Bit 4: 0 - HI_Z (high-impedance state) mode is disabled
        // Bit 3: 0 - Reserved
        // Bit 2-0: 110 - LRA Library (6) is selected
        DRV2605L_WriteReg(DRV2605L_REG_LIB_SEL, 0x06);

        // The Vrms of the LRA is 1.2V, so set the rated voltage to 47 (Page 24, Equation 5 in the DRV2605L datasheet where t_sample_time = 300us)
        DRV2605L_WriteReg(DRV2605L_REG_RATED_VOLTAGE, 0x2F);

        // The Vp of the LRA is 1.7V, so set the overdrive clamp voltage to 89 (Page 24, Equation 7 in the DRV2605L datasheet)
        DRV2605L_WriteReg(DRV2605L_REG_OD_CLAMP_VOLTAGE, 0x59);

        // Set the feedback control register to 0x1011 0110
        // Bit 7: 1 - LRA Mode is enabled
        // Bit 6-4: 011 - FB_BRAKE_FACTOR is 4x (default)
        // Bit 3-2: 01 - LOOP_GAIN is Medium (default)
        // Bit 1-0: 10 - BEMF_GAIN is 15x (default)
        DRV2605L_WriteReg(DRV2605L_REG_FEEDBACK_CTRL, 0xB6);

        // Set the control 1 register to 0x1001 0011
        // Bit 7: 1 - STARTUP_BOOST is enabled (default)
        // Bit 6: 0 - Reserved
        // Bit 5: 0 - AC_COUPLE is disabled (default)
        // Bit 4-0: 10011 - Set the DRIVE_TIME to 19 (0x13)
        DRV2605L_WriteReg(DRV2605L_REG_CTRL_1, 0x93);
        // DRV2605L_WriteReg(DRV2605L_REG_CTRL_2, 0xF5);
        // DRV2605L_WriteReg(DRV2605L_REG_CTRL_3, 0x80);
        // DRV2605L_WriteReg(DRV2605L_REG_CTRL_4, 0x20);

        // Set the mode register to 0x0000 0111
        // Bit 7: 0 - Device does not reset
        // Bit 6: 0 - Device is not in standby mode
        // Bit 5-3: 000 - Reserved
        // Bit 2-0: 111 - Device is in Auto-Calibration mode. After Auto-Calibration, the bits will be set to 0x000 (Internal trigger)
        DRV2605L_WriteReg(DRV2605L_REG_MODE, 0x07);

        // Set the GO register to 0x01 to start the Auto-Calibration
        DRV2605L_WriteReg(DRV2605L_REG_GO, 0x01);

        dbh_DelayMS(1000); // Wait for 1 second for the Auto-Calibration to complete

        // Read the auto-calibration compensation result
        DRV2605L_ReadReg(DRV2605L_REG_A_CAL_COMP, &A_CAL_COMP); // Read the A_CAL_COMP register

        // Read the auto-calibration Back-EMF result
        DRV2605L_ReadReg(DRV2605L_REG_A_CAL_BEMF, &A_CAL_BEMF); // Read the A_CAL_BEMF register

        // Read the auto-calibration Back-EMF gain
        DRV2605L_ReadReg(DRV2605L_REG_FEEDBACK_CTRL, &BEMF_GAIN); // Read the FEEDBACK_CTRL register
        BEMF_GAIN = BEMF_GAIN & 0x03; // Bits [1:0] is BEMF_GAIN. For LRA Mode, 0x00 is 3.75x, 0x01 is 7.5x, 0x02 is 15x, 0x03 is 22.5x
    }
}

/**
  * @brief  Play a waveform on the DRV2605L
  * @param  num: The waveform number to play (1-123)
  * @retval None
  *
  * This function plays a waveform on the DRV2605L by writing the waveform number to the waveform sequence register.
  * It also sets the mode register to exit standby mode and starts the waveform sequence.
  */
void dbh_DRV2605L_PlayWaveform(uint8_t num)
{
    uint8_t waveform = num;

    waveform = waveform > 123 ? 123 : waveform; // Limit the waveform to 123
    waveform = waveform < 1 ? 1 : waveform; // Limit the waveform to 1

    DRV2605L_WriteReg(DRV2605L_REG_MODE, 0x00); // Set the mode register to 0x00 to exit the standby mode
    
    DRV2605L_WriteReg(DRV2605L_REG_WAV_SEQ_1, waveform); // Set the waveform sequence to the selected waveform

    DRV2605L_WriteReg(DRV2605L_REG_GO, 0x01); // Start the waveform sequence
}

/**
  * @brief  Stop the waveform sequence on the DRV2605L
  * @retval None
  *
  * This function stops the waveform sequence on the DRV2605L by setting the mode register to standby mode.
  */
void dbh_DRV2605L_StopWaveform(void)
{
    uint8_t mode = 0;

    DRV2605L_ReadReg(DRV2605L_REG_MODE, &mode); // Read the mode register

    if (!(mode & 0x40)) // If the device is not in standby mode
    {
        // Set the standby mode
        // Bit 7: DEV_RESET
        // Bit 6: 1 - Device is in standby mode
        // Bit 5-3: Reserved
        // Bit 2-0: Mode
        DRV2605L_WriteReg(DRV2605L_REG_MODE, 0x40 | mode); // Set the mode register to standby mode
    }
}

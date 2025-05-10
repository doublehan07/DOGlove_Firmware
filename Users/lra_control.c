/**
  ******************************************************************************
  * @file    lra_control.c
  * @brief   This file contains the functions to control the LRA
  * @author  doublehan07
  * @version V1.0
  * @date    2024-08-15
  ******************************************************************************
  */

#include "lra_control.h"
#include "usart.h"

__IO uint8_t current_channel = 0;
__IO uint8_t wave_num[8] = {0};
__IO uint16_t duration[8] = {0};
__IO uint16_t lra_counter[8] = {0};
__IO uint16_t current_timestamp = 0;
uint8_t rx_data[10] = {0};

/**
  * @brief  Initialize the LRA control
  * @retval None
  *
  * This function initializes the LRA control by setting up the UART to receive data.
  */
void dbh_LRA_Control_Init(void)
{
    HAL_UARTEx_ReceiveToIdle_IT(&huart1, rx_data, 10);
}

/**
  * @brief  UART receive event callback
  * @param  huart: UART handle
  * @param  Size: Size of the received data
  * @retval None
  *
  * This function is called when the UART receives data. It processes the received data and updates the LRA control parameters.
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    uint8_t checksum = 0;
    uint8_t i = 0;

    if (huart->Instance == USART1)
    {
        // Process the received data
        // Header (0x55) | Header (0xAA) | Channel(0-7) X | Waveform Number X | Duration_H X | Dutation_L X | CRC 

        // Check the header
        if (rx_data[0] != 0x55 || rx_data[1] != 0xAA)
        {
            //The header is not correct, discard the data
        }
        else // The header is correct
        {
            // Check the CRC
            for (i = 2; i < 6; i++)
            {
                checksum += rx_data[i];
            }

            if (checksum != rx_data[6])
            {
                //The CRC is not correct, discard the data
            }
            else // The CRC is correct
            {
                current_channel = rx_data[2];
                if (current_channel < 5) // The channel is valid
                {
                    wave_num[current_channel] = rx_data[3];
                    duration[current_channel] = (rx_data[4] << 8) | rx_data[5];
                    lra_counter[current_channel] = 0;
                }
            }
        }

        //Reset the uart buffer
        HAL_UARTEx_ReceiveToIdle_IT(&huart1, rx_data, 10);
        if (huart1.RxXferCount < huart1.RxXferSize) // If the buffer is not empty, discard the data
        {
            huart1.RxXferCount = huart1.RxXferSize;
            huart1.pRxBuffPtr = rx_data;
        }
    }
}

/**
  * @brief  Get the current channel
  * @retval The current channel
  *
  * This function returns the current channel number.
  */
uint8_t dbh_GetChannel(void)
{
    return current_channel;
}

/**
  * @brief  Get the waveform number for the specified channel
  * @param  channel: The channel number (0-7)
  * @retval The waveform number for the specified channel
  *
  * This function returns the waveform number for the specified channel.
  */
uint8_t dbh_GetWaveNum(uint8_t channel)
{
    return wave_num[channel];
}

/**
  * @brief  Get the duration for the specified channel
  * @param  channel: The channel number (0-7)
  * @retval The duration for the specified channel
  *
  * This function returns the duration for the specified channel.
  */
uint16_t dbh_GetDuration(uint8_t channel)
{
    return duration[channel];
}

/**
  * @brief  Reset the counter for the specified channel
  * @param  channel: The channel number (0-7)
  * @retval None
  *
  * This function resets the counter for the specified channel to the duration value.
  */
void dbh_ResetCounter(uint8_t channel)
{
    lra_counter[channel] = duration[channel];
}

/**
  * @brief  Decrement the counters for all channels
  * @retval None
  *
  * This function decrements the counters for all channels. If the counter reaches zero, it stops the LRA.
  */
void dbh_DecCounter(void)
{
    uint8_t i = 0;

    for (i = 0; i < 8; i++)
    {
        if (lra_counter[i] > 0)
        {
            lra_counter[i]--;
        }
    }
}

/**
  * @brief  Get the counter for the specified channel
  * @param  channel: The channel number (0-7)
  * @retval The counter for the specified channel
  *
  * This function returns the counter for the specified channel.
  */
uint16_t dbh_GetCounter(uint8_t channel)
{
    return lra_counter[channel];
}

/**
  * @brief  Increment the timestamp
  * @retval None
  *
  * This function increments the timestamp. If it exceeds 65535, it wraps around to 0.
  */
void dbh_IncTimestampInMS(void)
{
    current_timestamp++;
    if (current_timestamp > 65535)
    {
        current_timestamp = 0;
    }
}

/**
  * @brief  Get the current timestamp
  * @retval The current timestamp
  *
  * This function returns the current timestamp.
  */
uint16_t dbh_GetTimestamp(void)
{
    return current_timestamp;
}

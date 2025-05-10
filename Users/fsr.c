/**
  ******************************************************************************
  * @file    fsr.c
  * @brief   This file contains the functions to get the ADC value of the reference voltage.
  * @author  doublehan07
  * @version V1.0
  * @date    2024-12-14
  ******************************************************************************
  */

 #include "fsr.h"

 #define VREFINT_CAL_ADDR ((uint16_t*)(0x1FFFF7BAUL)) // VREFINT_CAL_ADDR is the address of the word holding the factory calibration value of the Vrefint.

__IO uint16_t _u16ADC_Value[50];
__IO uint32_t reference_vdd = 0;
__IO uint16_t vrefint = 0;

/**
  * @brief  Get the ADC value of the reference voltage
  * @retval The reference voltage in microvolts
  *
  * This function calculates the reference voltage by averaging the ADC values.
  * It uses the factory calibration value of the Vrefint to calculate the current Vdda.
  */
uint32_t dbh_FSR_GetADCValue(void)
{
	uint32_t temp1 = 0;
	uint8_t i = 0;

	uint32_t vrefint_Cal = (*VREFINT_CAL_ADDR); //Should be 1525
	double current_vdda = 0.0;

	for(i = 0; i < 50; )
	{
		temp1 += _u16ADC_Value[i++];
	}
	vrefint = temp1 / 50;

	if (vrefint > 100) // To avoid division by zero
	{
		current_vdda = 3.3 * vrefint_Cal / vrefint;
		reference_vdd = (uint32_t)(current_vdda * 1000000);
	}

	return reference_vdd;
}

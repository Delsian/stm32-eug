/*
 * hal.c
 *
 *  Created on: 3 квіт. 2017 р.
 *      Author: ekrashtan
 */

#include "hal.h"
#include "lin.h"

extern void Error_Handler(void);
UART_HandleTypeDef huart;
TIM_HandleTypeDef htim;

#ifdef USE_ADC
/* ADC1 init function */
void MX_ADC1_Init(void)
{
	ADC_HandleTypeDef hadc1;
	ADC_ChannelConfTypeDef sConfig;

	/**Common config
	*/
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}

	/**Configure Regular Channel
	*/
	sConfig.Channel = ADC_CHANNEL_6;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

}
#endif // USE_ADC

#ifdef USE_I2C
/* I2C1 init function */
void MX_I2C1_Init(void)
{
	I2C_HandleTypeDef hi2c1;
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}

}
#endif // USE_I2C

#ifdef USE_PWM
/* TIM3 PWM init function */
void MX_TIM3_Init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;

	htim.Instance = TIM3;
	htim.Init.Prescaler = 0;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = 0;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if (HAL_TIM_PWM_Init(&htim) != HAL_OK)
	{
		Error_Handler();
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_TIM_MspPostInit(&htim);

}
#endif // USE_PWM

#ifdef USE_ENCODER
/* TIM3 encoder init function */
void MX_TIM3_Init(void)
{
	TIM_HandleTypeDef htim;
	TIM_Encoder_InitTypeDef sConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htim.Instance = TIM3;
	htim.Init.Prescaler = 0;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = 16;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
	sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC1Filter = 5;
	sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC2Filter = 5;
	if (HAL_TIM_Encoder_Init(&htim, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_TIM_Encoder_Start(&htim, TIM_CHANNEL_ALL);
}
#endif // USE_ENCODER

/* USART2 init function */
void MX_USART2_UART_Init(void)
{

	huart.Instance = USART2;
	huart.Init.BaudRate = LIN_BAUDRATE;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.Mode = UART_MODE_TX_RX;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_LIN_Init(&huart, UART_LINBREAKDETECTLENGTH_11B) != HAL_OK)
	{
		Error_Handler();
	}

}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, OUT1_Pin|OUT2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, S1_Pin|S2_Pin|S3_Pin|S4_Pin
                          |TP2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : OUT1_Pin OUT2_Pin */
  GPIO_InitStruct.Pin = OUT1_Pin|OUT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : LIN_WAKE_Pin LIN_SLP_Pin */
  GPIO_InitStruct.Pin = LIN_WAKE_Pin|LIN_SLP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOA,LIN_SLP_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA,LIN_WAKE_Pin,GPIO_PIN_SET);

  /*Configure GPIO pins : S1_Pin S2_Pin S3_Pin S4_Pin
                           TP2_Pin */
  GPIO_InitStruct.Pin = S1_Pin|S2_Pin|S3_Pin|S4_Pin
                          |TP2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure peripheral I/O remapping */
  __HAL_AFIO_REMAP_PD01_ENABLE();

}



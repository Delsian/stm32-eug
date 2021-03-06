/*
 * hal.h
 *
 *  Created on: 3 ���. 2017 �.
 *      Author: ekrashtan
 */

#ifndef HAL_H_
#define HAL_H_

#include "stm32f1xx_hal.h"

void MX_GPIO_Init(void);
void MX_ADC1_Init(void);
void MX_TIM3_Init(void);
void MX_USART2_UART_Init(void);
void MX_I2C1_Init(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);



#endif /* HAL_H_ */

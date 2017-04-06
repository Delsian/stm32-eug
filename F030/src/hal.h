/*
 * hal.h
 *
 *  Created on: 3 квіт. 2017 р.
 *      Author: ekrashtan
 */

#ifndef HAL_H_
#define HAL_H_

#include "stm32f0xx_hal.h"

void MX_GPIO_Init(void);
void MX_TIM_Init(void);
void MX_UART_Init(void);
void MX_I2C1_Init(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void SystemClock_Config(void);


#endif /* HAL_H_ */

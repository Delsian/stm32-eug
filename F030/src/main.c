/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hal.h"
#include "board.h"

void delay_ms(uint32_t ticks);

#define STATE_BUTTON 1
#define STATE_ENC1 2
#define LOOP_COUNT 0x8

int main(void)
{
	uint16_t counter = 0;
	uint8_t brightness = 5;

	uint8_t old_state = 0;

    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_ADC_Init();

  while (1)
  {
	  counter++;
	  if (counter& LOOP_COUNT) {
		  HAL_GPIO_WritePin(GPIOA, LED_B, GPIO_PIN_SET);
	  }
	  if ((counter&(LOOP_COUNT-1)) > brightness) {
		  HAL_GPIO_WritePin(GPIOA, LED_B, GPIO_PIN_RESET);
	  }

	  if(HAL_GPIO_ReadPin(GPIOA, ENCB)==0 && !(old_state&STATE_BUTTON)) {
		  // Button press
		  old_state |= STATE_BUTTON;
		  brightness++;
	  }
	  if(HAL_GPIO_ReadPin(GPIOA, ENCB)) {
		  // Release
		  old_state &= ~STATE_BUTTON;
	  }

	  // Encoder
	  if(HAL_GPIO_ReadPin(GPIOA, ENC1)) {
		  old_state &= ~STATE_ENC1;
	  } else {
		  if(!(old_state&STATE_ENC1)) {
			  old_state |= STATE_ENC1;
			  if (HAL_GPIO_ReadPin(GPIOB, ENC2)) {
				  brightness++;
				  if (brightness>LOOP_COUNT) brightness=LOOP_COUNT;
			  } else {
				  if (brightness)
					  brightness--;
			  }
		  }
	  }

  }

}




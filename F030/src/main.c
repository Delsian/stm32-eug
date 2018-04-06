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
#define STATE_LIGHT_ON 4
#define STATE_CHARGED 8
#define STATE_LOW_BATT 0x10
#define STATE_ADC_COMPL 0x20
#define STATE_ADC_RUN 0x40


#define LOOP_COUNT 0x10 // !!! pow2
#define MAX_BRIGHTNESS (LOOP_COUNT-5)

#define ADC_LO_VAL 1700
#define ADC_HI_VAL 2350

extern ADC_HandleTypeDef hadc;
static uint8_t old_state;

void ADC1_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&hadc);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	uint32_t data = HAL_ADC_GetValue(hadc);
	old_state &= ~(STATE_LOW_BATT|STATE_CHARGED);
	if (data< ADC_LO_VAL) {
		old_state |= STATE_LOW_BATT;
	} else if (data> ADC_HI_VAL) {
		old_state |= STATE_CHARGED;
	}
	old_state |= STATE_ADC_COMPL;
}

int main(void)
{
	uint16_t counter = 0;
	uint8_t brightness = MAX_BRIGHTNESS/2;

    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_ADC_Init();
    HAL_ADC_Start_IT(&hadc);

  while (1)
  {
	  counter++;
	  // === Indication
	  if (old_state&STATE_LIGHT_ON) {
		  // Big lamp
		  if ((counter&(LOOP_COUNT-1))==0 && (old_state&STATE_ADC_COMPL)) {
			  HAL_GPIO_WritePin(GPIOA, LAMP, GPIO_PIN_SET);
		  }
		  if ((counter&(LOOP_COUNT-1)) >= brightness) {
			  HAL_GPIO_WritePin(GPIOA, LAMP, GPIO_PIN_RESET);
		  }
	  }
	  if (!(old_state&STATE_LIGHT_ON) || (old_state&STATE_LOW_BATT)) {
		  // button blink
		  if ((counter&0x3FFF)==0) {
			  if (old_state&STATE_LOW_BATT) {
				  HAL_GPIO_TogglePin(GPIOA, LED_R);
				  HAL_GPIO_WritePin(GPIOA, LED_G|LED_B, GPIO_PIN_SET);
			  } else if (old_state&STATE_CHARGED) {
				  HAL_GPIO_TogglePin(GPIOA, LED_G);
				  HAL_GPIO_WritePin(GPIOA, LED_R|LED_B, GPIO_PIN_SET);
			  } else {
				  HAL_GPIO_TogglePin(GPIOA, LED_B);
				  HAL_GPIO_WritePin(GPIOA, LED_G|LED_R, GPIO_PIN_SET);
			  }
		  }
	  }

	  // === Handling button
	  if(HAL_GPIO_ReadPin(GPIOA, ENCB)==0 && !(old_state&STATE_BUTTON)) {
		  // Button press
		  old_state |= STATE_BUTTON;
		  old_state ^= STATE_LIGHT_ON;
		  HAL_GPIO_WritePin(GPIOA, LED_G|LED_B|LED_R, GPIO_PIN_SET);
	  }
	  if(HAL_GPIO_ReadPin(GPIOA, ENCB)) {
		  // Release
		  old_state &= ~STATE_BUTTON;
	  }

	  // === Encoder
	  if(HAL_GPIO_ReadPin(GPIOA, ENC1)) {
		  old_state &= ~STATE_ENC1;
	  } else {
		  if(!(old_state&STATE_ENC1)) {
			  old_state |= STATE_ENC1;
			  if (HAL_GPIO_ReadPin(GPIOB, ENC2)) {
				  brightness++;
				  if (brightness>MAX_BRIGHTNESS) brightness=MAX_BRIGHTNESS;
			  } else {
				  if (brightness)
					  brightness--;
			  }
		  }
	  }

	  // === Analog
	  if ((counter&0x1FFF)==0x1000) {
		  HAL_GPIO_WritePin(GPIOA, LAMP, GPIO_PIN_RESET); // Switch off until measurement
		  old_state &= ~STATE_ADC_COMPL;
		  HAL_ADC_Start_IT(&hadc);
//		  if(old_state&STATE_ADC_COMPL) {
//			  if((old_state&STATE_LOW_BATT) && brightness>MAX_BRIGHTNESS/2) {
//				  brightness=MAX_BRIGHTNESS/2;
//			  }
//			  old_state &= ~STATE_ADC_COMPL;
//			  HAL_ADC_Start_IT(&hadc);
//		  }
	  }
  }

}




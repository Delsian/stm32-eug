/*
 * lin.c
 *
 *  Created on: 3 квіт. 2017 р.
 *      Author: ekrashtan
 */

#include "hal.h"
#include "lin.h"

extern UART_HandleTypeDef huart;

/* Create the Lin ID parity */
#define BIT(data,shift) ((addr&(1<<shift))>>shift)
static uint8_t LinAddrParity(uint8_t addr)
{
  uint8_t p0 = BIT(addr,0) ^ BIT(addr,1) ^ BIT(addr,2) ^ BIT(addr,4);
  uint8_t p1 = ~(BIT(addr,1) ^ BIT(addr,3) ^ BIT(addr,4) ^ BIT(addr,5));
  return (p0 | (p1<<1))<<6;
}

/* Lin defines its checksum as an inverted 8 bit sum with carry */
static uint8_t LinDataChecksum(const uint8_t* message, char nBytes, uint16_t sum)
{
    while (nBytes-- > 0) sum += *(message++);
    // Add the carry
    while(sum>>8)  // In case adding the carry causes another carry
      sum = (sum&255)+(sum>>8);
    return (~sum);
}

t_lin_error LinSendResp(tLinFrame* Frame)
{
	while (HAL_UART_GetState(&huart) != HAL_UART_STATE_READY) {}

	t_lin_error err = NO_ERROR;

	HAL_UART_Transmit(&huart, Frame->message, 2, HAL_MAX_DELAY);

	return err;
}

typedef enum {
	START,
	WAIT_FOR_55,
	ADDR,
	DATA1,
	DATA2,
	MSG_CRC,
	MSG_OK
} tRxState;

t_lin_error LinGetFrame(tLinFrame* Frame)
{
	tRxState state = START;
	t_lin_error err = NO_ERROR;
	uint8_t rxbyte;
	while(state != MSG_OK) {
		HAL_StatusTypeDef status = HAL_UART_Receive(&huart,&rxbyte,1,HAL_MAX_DELAY);
		if (status == HAL_OK) {
			switch (state) {
			case START:
				if (rxbyte == 0 && (huart.Instance->ISR && USART_ISR_FE_Msk))
					state = WAIT_FOR_55;
				break;
			case WAIT_FOR_55:
				if (rxbyte == 0x55)
					state = ADDR;
				break;
			case ADDR:
				Frame->addr = rxbyte & 0x3F;
				if ((LinAddrParity(Frame->addr)&0xC0) == (rxbyte&0xC0))
				{
					// Chk addr
					state = DATA1;
				}
				else
				{
					state = START;
				}
				break;
			case DATA1:
				Frame->message[0] = rxbyte;
				state = DATA2;
				break;
			case DATA2:
				Frame->message[1] = rxbyte;
				state = MSG_CRC;
				break;
			case MSG_CRC:
				state = MSG_OK;
				break;
			default:
				break;
			}
		}
	}
	return err;
}

/*
 * lin.c
 *
 *  Created on: 3 квіт. 2017 р.
 *      Author: ekrashtan
 */

#include "hal.h"
#include "lin.h"

extern UART_HandleTypeDef huart;

void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart);
}

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

t_lin_error LinSendFrame(tLinFrame* Frame, uint8_t proto)
{
	static uint8_t tx_buf[12];
	while (HAL_UART_GetState(&huart) != HAL_UART_STATE_READY) {}

	t_lin_error err = NO_ERROR;
	HAL_LIN_SendBreak(&huart);
	tx_buf[0] = 0x55; // Sync byte
	tx_buf[1] = (Frame->addr & 0x3f) | LinAddrParity(Frame->addr);
	memcpy(&(tx_buf[2]),Frame->message, Frame->msgLen);
	tx_buf[Frame->msgLen+2] = LinDataChecksum(Frame->message, Frame->msgLen,(proto==1) ? 0:tx_buf[1]);
	HAL_UART_Transmit_IT(&huart, tx_buf, Frame->msgLen+3);

	return err;
}


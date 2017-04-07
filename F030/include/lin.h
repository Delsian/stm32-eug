/*
 * lin.h
 *
 *  Created on: 3 квіт. 2017 р.
 *      Author: ekrashtan
 */

#ifndef LIN_H_
#define LIN_H_

#include <stdint.h>

#define LIN_BAUDRATE 19200

typedef enum {
	NO_ERROR,
	BIT_ERROR,
	ID_PARITY_ERROR,
	CHECKSUM_ERROR,
	NO_ID_MATCH,
	TIMEOUT_ERROR,
	DATA_RECEIVED,
	DATA_REQUEST,
	WAKE_UP,
	UART_ERROR,
	SYNCH_BREAK_ERROR
} t_lin_error;

typedef enum {
	ID_DATAREQUEST, ID_DATASENT
} t_message_direction;

typedef struct{
	uint8_t id;
	t_message_direction dir;
	uint8_t length;
} t_id_list;

typedef struct
{
	uint8_t identifier;
	uint8_t length;
} t_header;

typedef struct{
	t_lin_error error_code;
	uint8_t data_byte;
} t_one_databyte_output;

typedef struct
{
	uint8_t data[8];
	uint8_t checksum;
} t_response;

typedef struct
{
	uint8_t addr;
	union {
		uint8_t message[8];
		uint32_t msg32[2];
	};
	uint8_t msgLen;
} tLinFrame;

//=======
void LinInit(void);
void LinSendWakeUpSignal(void);
typedef void (*LinFrameCB)(tLinFrame* Frame);

// Master
t_lin_error LinSendFrame(tLinFrame* Frame, uint8_t proto);
// Slave
t_lin_error LinGetFrame(tLinFrame* Frame);


#endif /* LIN_H_ */

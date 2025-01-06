/**********************************
 *  uart.h
 *
 *  Created on: Aug 2, 2024
 * 
 *  Author: Muhannad Abdallah
 **********************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/* Enumeration to specify the number of data bits in the UART frame */
typedef enum {
    FIVE_BIT_MODE = 0,  /* 5 data bits */
    SIX_BIT_MODE = 1,   /* 6 data bits */
    SEVEN_BIT_MODE = 2, /* 7 data bits */
    EIGHT_BIT_MODE = 3, /* 8 data bits */
    NINE_BIT_MODE = 7,  /* 9 data bits */
} UART_BitData;

/* Enumeration to specify the type of parity used in the UART frame */
typedef enum {
    EVEN_PARITY = 2, /* Even parity */
    ODD_PARITY = 3,  /* Odd parity */
} UART_Parity;

/* Enumeration to specify the number of stop bits in the UART frame */
typedef enum {
    ONE_STOP_BIT,  /* 1 stop bit */
    TWO_STOP_BIT,  /* 2 stop bits */
} UART_StopBit;

/* Enumeration to specify the baud rate for UART communication */
typedef enum {
    RATE_ONE = 2400,   /* 2400 bps */
    RATE_TWO = 4800,   /* 4800 bps */
    RATE_THREE = 9600, /* 9600 bps */
    RATE_FOUR = 14400, /* 14400 bps */
} UART_BaudRate;

/* Structure to configure UART settings */
typedef struct {
    UART_BitData bit_data;   /* Data bits configuration */
    UART_Parity parity;      /* Parity configuration */
    UART_StopBit stop_bit;   /* Stop bits configuration */
    UART_BaudRate baud_rate; /* Baud rate configuration */
} UART_ConfigType;

/* 
 * Description:
 * Initializes the UART device with the specified settings.
 * This includes setting up the frame format, enabling the UART,
 * and configuring the baud rate.
 * 
 * Parameters:
 * - Config_Ptr: Pointer to the UART configuration structure.
 */
void UART_init(const UART_ConfigType* Config_Ptr);

/* 
 * Description:
 * Sends a byte of data through the UART.
 * 
 * Parameters:
 * - data: The byte of data to be sent.
 */
void UART_sendByte(const uint8 data);

/* 
 * Description:
 * Receives a byte of data through the UART.
 * 
 * Returns:
 * - The received byte of data.
 */
uint8 UART_receiveByte(void);

/* 
 * Description:
 * Sends a string of data through the UART.
 * 
 * Parameters:
 * - Str: Pointer to the string to be sent.
 */
void UART_sendString(const uint8 *Str);

/* 
 * Description:
 * Receives a string of data through the UART until the '#' character is encountered.
 * 
 * Parameters:
 * - Str: Pointer to the buffer where the received string will be stored.
 */
void UART_receiveString(uint8 *Str);

#endif /* UART_H_ */

/*******************************
 *  uart.h
 *
 *  Created on: Aug 2, 2024
 * 
 *  Author: Muhannad Abdallah
 ******************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/* 
 * Enum: UART_BitData
 * Description: Specifies the number of data bits in the UART frame.
 */
typedef enum {
    FIVE_BIT_MODE = 0,   /* 5 data bits */
    SIX_BIT_MODE = 1,    /* 6 data bits */
    SEVEN_BIT_MODE = 2,  /* 7 data bits */
    EIGHT_BIT_MODE = 3,  /* 8 data bits */
    NINE_BIT_MODE = 7    /* 9 data bits */
} UART_BitData;

/* 
 * Enum: UART_Parity
 * Description: Specifies the parity mode in the UART frame.
 */
typedef enum {
    EVEN_PARITY = 2,  /* Even parity */
    ODD_PARITY = 3    /* Odd parity */
} UART_Parity;

/* 
 * Enum: UART_StopBit
 * Description: Specifies the number of stop bits in the UART frame.
 */
typedef enum {
    ONE_STOP_BIT,  /* 1 stop bit */
    TWO_STOP_BIT   /* 2 stop bits */
} UART_StopBit;

/* 
 * Enum: UART_BaudRate
 * Description: Specifies the baud rate for UART communication.
 */
typedef enum {
    RATE_ONE = 2400,    /* 2400 baud rate */
    RATE_TWO = 4800,    /* 4800 baud rate */
    RATE_THREE = 9600,  /* 9600 baud rate */
    RATE_FOUR = 14400   /* 14400 baud rate */
} UART_BaudRate;

/* 
 * Struct: UART_ConfigType
 * Description: Configuration structure for UART settings.
 */
typedef struct {
    UART_BitData bit_data;     /* Data bit mode (e.g., 8-bit mode) */
    UART_Parity parity;        /* Parity mode (e.g., even parity) */
    UART_StopBit stop_bit;     /* Number of stop bits (e.g., 1 stop bit) */
    UART_BaudRate baud_rate;   /* Baud rate for UART communication */
} UART_ConfigType;

/* 
 * Function: UART_init
 * Description: Initializes the UART device with the specified settings.
 * Parameters: 
 *   - Config_Ptr: Pointer to the UART_ConfigType structure containing the desired UART settings.
 * Responsibilities:
 *   - Sets up the frame format (data bits, parity, stop bits).
 *   - Enables the UART.
 *   - Configures the baud rate.
 */
void UART_init(const UART_ConfigType* Config_Ptr);

/* 
 * Function: UART_sendByte
 * Description: Sends a single byte of data to another UART device.
 * Parameters: 
 *   - data: The byte to be sent.
 */
void UART_sendByte(const uint8 data);

/* 
 * Function: UART_receiveByte
 * Description: Receives a single byte of data from another UART device.
 * Returns:
 *   - The received byte of data.
 */
uint8 UART_receiveByte(void);

/* 
 * Function: UART_sendString
 * Description: Sends a string of data through UART to another UART device.
 * Parameters:
 *   - Str: Pointer to the string to be sent.
 */
void UART_sendString(const uint8 *Str);

/* 
 * Function: UART_receiveString
 * Description: Receives a string of data through UART from another UART device.
 * Parameters:
 *   - Str: Pointer to a buffer where the received string will be stored.
 * Note:
 *   - The function will receive characters until the '#' symbol is encountered.
 */
void UART_receiveString(uint8 *Str);

#endif /* UART_H_ */

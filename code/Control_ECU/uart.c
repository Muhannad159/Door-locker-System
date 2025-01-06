/*******************************
 *  uart.c
 *
 *  Created on: Aug 2, 2024
 * 
 *  Author: Muhannad Abdallah
 ******************************/

#include "uart.h"
#include <avr/io.h>
#include "common_macros.h"

/*
 * Description:
 * Initializes the UART module based on the configuration structure provided.
 * This includes setting up the frame format (data bits, parity, stop bits),
 * enabling the transmitter and receiver, and setting the baud rate.
 *
 * Parameters:
 *  - Config_Ptr: Pointer to a structure containing the desired UART configuration.
 */
void UART_init(const UART_ConfigType* Config_Ptr) {
    uint16 ubrr_value = 0;

    /* U2X = 1 for double transmission speed */
    UCSRA = (1 << U2X);

    /***************************** UCSRB Description **********************
     * RXCIE = 0 Disable USART RX Complete Interrupt Enable
     * TXCIE = 0 Disable USART TX Complete Interrupt Enable
     * UDRIE = 0 Disable UART Data Register Empty Interrupt Enable
     * RXEN  = 1 Receiver Enable
     * TXEN  = 1 Transmitter Enable
     * UCSZ2 = 0 For 8-bit data mode
     *********************************************************************/
    UCSRB = (1 << RXEN) | (1 << TXEN);

    /* Adjusting UCSZ2 bit while preserving the others */
    UCSRB = (UCSRB & 0xFB) | (Config_Ptr->bit_data & 0x04);

    /****************************** UCSRC Description **********************
     * URSEL  = 1 The URSEL must be one when writing the UCSRC
     * UMSEL  = 0 Asynchronous operation
     * UPM1:0 = 00 Disable parity bit
     * USBS   = 0 One stop bit
     * UCSZ1:0 = 11 For 8-bit data mode
     * UCPOL  = 0 Used with synchronous operation only
     **********************************************************************/
    /* URSEL bit must be one when writing the register */
    UCSRC = (1 << URSEL);

    /* Adjusting UCSZ1 & UCSZ0 bits while preserving the others */
    UCSRC = (UCSRC & 0x79) | (Config_Ptr->bit_data & 0x03);

    /* Adjusting USBS bit while preserving the others */
    UCSRC = (UCSRC & 0xF7) | ((Config_Ptr->stop_bit << 3) & 0x08);

    /* Adjusting UPM1:0 while preserving the others */
    UCSRC = (UCSRC & 0xCF) | ((Config_Ptr->parity << 4) & 0x30);

    /* Calculate the UBRR register value */
    ubrr_value = (uint16)(((F_CPU / (Config_Ptr->baud_rate * 8UL))) - 1);

    /* Set the baud rate */
    UBRRH = (uint8)(ubrr_value >> 8);
    UBRRL = (uint8)ubrr_value;
}

/*
 * Description:
 * Sends a single byte of data through UART.
 *
 * Parameters:
 *  - data: The byte of data to send.
 */
void UART_sendByte(const uint8 data) {
    /* Wait for the transmit buffer to be empty (UDRE flag set) */
    while (BIT_IS_CLEAR(UCSRA, UDRE));

    /* Put the data into the buffer, which sends it */
    UDR = data;
}

/*
 * Description:
 * Receives a single byte of data from UART.
 *
 * Returns:
 *  - The received byte of data.
 */
uint8 UART_receiveByte(void) {
    /* Wait for the data to be received (RXC flag set) */
    while (BIT_IS_CLEAR(UCSRA, RXC));
    
    /* Get and return the received data from the buffer */
    return UDR;
}

/*
 * Description:
 * Sends a string of data through UART.
 *
 * Parameters:
 *  - Str: Pointer to the string of data to send.
 */
void UART_sendString(const uint8 *Str) {
    uint8 i = 0;
    
    /* Send each character in the string until the null terminator */
    while (Str[i] != '\0') {
        UART_sendByte(Str[i]);
        i++;
    }
}

/*
 * Description:
 * Receives a string of data through UART until the '#' symbol is encountered.
 *
 * Parameters:
 *  - Str: Pointer to the buffer where the received string will be stored.
 */
void UART_receiveString(uint8 *Str) {
    uint8 i = 0;

    /* Receive characters until the '#' symbol */
    Str[i] = UART_receiveByte();
    while (Str[i] != '#') {
        i++;
        Str[i] = UART_receiveByte();
    }

    /* Replace the '#' symbol with a null terminator */
    Str[i] = '\0';
}

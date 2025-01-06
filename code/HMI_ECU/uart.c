/*********************************
 *  uart.c
 *
 *  Created on: Aug 2, 2024
 * 
 *  Author: Muhannad Abdallah
 *********************************/

#include "uart.h"
#include <avr/io.h>
#include "common_macros.h"

/* 
 * Description:
 * Initializes the UART device based on the configuration provided in Config_Ptr.
 * Configures the frame format (data bits, parity, stop bits), enables the UART,
 * and sets the baud rate.
 * 
 * Parameters:
 * - Config_Ptr: Pointer to the UART configuration structure.
 */
void UART_init(const UART_ConfigType* Config_Ptr) {
    uint16 ubrr_value = 0;
    
    /* U2X = 1 for double transmission speed */
    UCSRA = (1 << U2X);

    /* 
     * UCSRB: Control and status register B
     * - RXEN = 1: Enable receiver
     * - TXEN = 1: Enable transmitter
     * - UCSZ2 = 0: Configure for 8-bit data mode (UCSZ2 is bit 2 of UCSZ[2:0])
     */
    UCSRB = (1 << RXEN) | (1 << TXEN);
    
    /* Adjust UCSZ2 based on the bit data mode */
    UCSRB = (UCSRB & 0xFB) | (Config_Ptr->bit_data & 0x04);

    /* 
     * UCSRC: Control and status register C
     * - URSEL = 1: Write to UCSRC (instead of UBRRH)
     * - UMSEL = 0: Asynchronous operation
     * - U

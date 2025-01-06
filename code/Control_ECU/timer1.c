/*******************************
 *  timer1.c
 *
 *  Created on: Aug 2, 2024
 * 
 *  Author: Muhannad Abdallah
 ******************************/
#include "timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile void(*callback_ptr)(void) = ((void*)0);  // Pointer to callback function

/*
 * ISR for Timer1 Compare Match A
 */
ISR(TIMER1_COMPA_vect) {
    if (callback_ptr != ((void*)0)) {
        (*callback_ptr)();  // Call the callback function if it's set
    }
}

/*
 * ISR for Timer1 Overflow
 */
ISR(TIMER1_OVF_vect) {
    if (callback_ptr != ((void*)0)) {
        (*callback_ptr)();  // Call the callback function if it's set
    }
}

/*
 * Initializes Timer1 based on the provided configuration.
 * 
 * Parameters:
 *  - Config_Ptr: Pointer to Timer1_ConfigType structure containing configuration settings.
 */
void Timer1_init(const Timer1_ConfigType* Config_Ptr) {
    // Set the initial value of the timer counter
    TCNT1 = Config_Ptr->initial_value;

    // Set the Timer/Counter Control Register A (TCCR1A) with Force Output Compare (FOC1A) bit
    TCCR1A = (1 << FOC1A);

    // Adjust the prescaler settings while preserving other bits
    TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler & 0x07);

    // Set the mode (normal or compare) in the Timer/Counter Control Register B (TCCR1B)
    TCCR1B = (TCCR1B & 0xF7) | (Config_Ptr->mode & 0x08);

    // Configure the Output Compare Register A (OCR1A) and Timer Interrupt Mask Register (TIMSK)
    if (Config_Ptr->mode == COMPARE_MODE) {
        OCR1A = Config_Ptr->compare_value;
        TIMSK |= (1 << OCIE1A);  // Enable Timer1 Compare Match A interrupt
    } else if (Config_Ptr->mode == NORMAL_MODE) {
        TIMSK |= (1 << TOIE1);   // Enable Timer1 Overflow interrupt
    }
}

/*
 * Deinitializes Timer1 by resetting its registers and disabling interrupts.
 */
void Timer1_deInit(void) {
    TCNT1 = 0;       // Clear Timer Counter
    TCCR1A = 0;      // Clear Timer/Counter Control Register A
    TCCR1B = 0;      // Clear Timer/Counter Control Register B
    OCR1A = 0;       // Clear Output Compare Register A
    TIMSK = 0;       // Disable Timer Interrupts
}

/*
 * Sets the callback function to be called when Timer1 generates an interrupt.
 * 
 * Parameters:
 *  - a_ptr: Pointer to the callback function.
 */
void Timer1_setCallBack(void(*a_ptr)(void)) {
    callback_ptr = a_ptr;  // Set the callback function pointer
}

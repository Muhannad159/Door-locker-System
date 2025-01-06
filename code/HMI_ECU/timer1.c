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

/* Global pointer to function used for callback implementation */
volatile void (*callback_ptr)(void) = ((void*)0);

/* 
 * ISR for Timer1 Compare Match A interrupt.
 * This ISR is executed when the Timer1 compare value matches the timer count.
 */
ISR(TIMER1_COMPA_vect) {
    if (callback_ptr != ((void*)0)) {
        (*callback_ptr)();  /* Execute the callback function if it is set */
    }
}

/* 
 * ISR for Timer1 Overflow interrupt.
 * This ISR is executed when Timer1 overflows, meaning it reaches its maximum count value.
 */
ISR(TIMER1_OVF_vect) {
    if (callback_ptr != ((void*)0)) {
        (*callback_ptr)();  /* Execute the callback function if it is set */
    }
}

/* 
 * Description:
 * Initializes Timer1 based on the provided configuration structure.
 * 
 * Parameters:
 * - Config_Ptr: Pointer to the Timer1 configuration structure containing 
 *   the initial value, compare value (if applicable), prescaler, and mode of operation.
 */
void Timer1_init(const Timer1_ConfigType* Config_Ptr) {
    /* Set the initial value of the timer */
    TCNT1 = Config_Ptr->initial_value;

    /* Set the Force Output Compare bit (FOC0) for non-PWM mode */
    TCCR1A = (1 << FOC0);

    /* Set the prescaler for Timer1 while preserving other bits */
    TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->prescaler & 0x07);

    /* Set the mode of operation for Timer1 */
    TCCR1B = (TCCR1B & 0xF7) | (Config_Ptr->mode & 0x08);

    /* Configure Timer1 based on the mode */
    if (Config_Ptr->mode == COMPARE_MODE) {
        /* Set the compare value if using compare mode */
        OCR1A = Config_Ptr->compare_value;
        /* Enable Timer1 Compare Match A interrupt */
        TIMSK |= (1 << OCIE1A);
    } else if (Config_Ptr->mode == NORMAL_MODE) {
        /* Enable Timer1 Overflow interrupt if using normal mode */
        TIMSK |= (1 << TOIE1);
    }
}

/* 
 * Description:
 * Deinitializes Timer1 by resetting all associated registers to their default values.
 */
void Timer1_deInit(void) {
    TCNT1 = 0;       /* Reset Timer1 counter */
    TCCR1A = 0;      /* Reset Timer1 Control Register A */
    TCCR1B = 0;      /* Reset Timer1 Control Register B */
    OCR1A = 0;       /* Reset Output Compare Register A */
    TIMSK = 0;       /* Disable all Timer1 interrupts */
}

/* 
 * Description:
 * Sets the callback function to be called when Timer1 interrupt occurs.
 * 
 * Parameters:
 * - a_ptr: Pointer to the callback function to be executed when the interrupt is triggered.
 */
void Timer1_setCallBack(void(*a_ptr)(void)) {
    callback_ptr = a_ptr;  /* Set the global callback pointer to the provided function */
}

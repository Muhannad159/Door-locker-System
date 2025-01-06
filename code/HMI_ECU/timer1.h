/**********************************
 *  timer1.h
 *
 *  Created on: Aug 2, 2024
 * 
 *  Author: Muhannad Abdallah
 **********************************/

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"

/* Enumeration for Timer1 Prescaler values */
typedef enum {
    F_CPU_NOCLOCK = 0,  /* No clock source (Timer/Counter stopped) */
    F_CPU_CLOCK,        /* No prescaling */
    F_CPU_8,            /* Clock/8 */
    F_CPU_64,           /* Clock/64 */
    F_CPU_256,          /* Clock/256 */
    F_CPU_1024,         /* Clock/1024 */
} Timer1_Prescaler;

/* Enumeration for Timer1 Operating Modes */
typedef enum {
    NORMAL_MODE = 0,    /* Normal mode */
    COMPARE_MODE = 4,   /* Compare mode */
} Timer1_Mode;

/* Configuration structure for Timer1 settings */
typedef struct {
    uint16 initial_value;         /* Initial value for the timer */
    uint16 compare_value;         /* Compare value (used in Compare mode only) */
    Timer1_Prescaler prescaler;   /* Prescaler value */
    Timer1_Mode mode;             /* Timer mode (Normal or Compare) */
} Timer1_ConfigType;

/* 
 * Description:
 * Initializes Timer1 with the specified settings.
 * The configuration includes setting the initial value, compare value (if applicable),
 * prescaler, and mode of operation.
 * 
 * Parameters:
 * - Config_Ptr: Pointer to the Timer1 configuration structure.
 */
void Timer1_init(const Timer1_ConfigType* Config_Ptr);

/* 
 * Description:
 * Deinitializes Timer1, stopping the timer and resetting any settings.
 */
void Timer1_deInit(void);

/* 
 * Description:
 * Sets the callback function to be called when the Timer1 interrupt occurs.
 * 
 * Parameters:
 * - a_ptr: Pointer to the callback function.
 */
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */

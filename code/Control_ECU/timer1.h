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

/* Enum to specify the timer prescaler options */
typedef enum {
    F_CPU_NOCLOCK = 0,   /* No clock source for the timer */
    F_CPU_CLOCK,         /* Timer clock = F_CPU */
    F_CPU_8,             /* Timer clock = F_CPU / 8 */
    F_CPU_64,            /* Timer clock = F_CPU / 64 */
    F_CPU_256,           /* Timer clock = F_CPU / 256 */
    F_CPU_1024           /* Timer clock = F_CPU / 1024 */
} Timer1_Prescaler;

/* Enum to specify the timer mode */
typedef enum {
    NORMAL_MODE = 0,     /* Normal mode of operation */
    COMPARE_MODE = 4     /* Compare mode of operation */
} Timer1_Mode;

/* Struct to hold configuration parameters for Timer1 */
typedef struct {
    uint16 initial_value;   /* Initial value of the timer counter */
    uint16 compare_value;   /* Value to compare with (used in compare mode only) */
    Timer1_Prescaler prescaler; /* Prescaler setting for timer clock */
    Timer1_Mode mode;       /* Mode of operation (normal or compare) */
} Timer1_ConfigType;

/*
 * Description:
 * Initializes Timer1 with the specified configuration settings.
 * 
 * Parameters:
 *  - Config_Ptr: Pointer to a Timer1_ConfigType structure that holds the 
 *                configuration settings for Timer1.
 */
void Timer1_init(const Timer1_ConfigType* Config_Ptr);

/*
 * Description:
 * Deinitializes Timer1 by resetting its registers and disabling it.
 */
void Timer1_deInit(void);

/*
 * Description:
 * Sets the callback function to be called when the timer interrupt occurs.
 * 
 * Parameters:
 *  - a_ptr: Pointer to a function with no arguments and no return value, 
 *            which will be called when the timer interrupt occurs.
 */
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */

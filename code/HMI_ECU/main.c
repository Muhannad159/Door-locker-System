/**********************************
 * 	main.c
 *
 *  Created on: Aug 2, 2024
 * 
 *  Author: Muhannad Abdallah
 **********************************/

#include "uart.h"
#include "keypad.h"
#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include "Timer1.h"

/* Define constants for password length, special keys, and communication signals */
#define PASSWORD_LENGTH 5
#define ENTER_BUTTON 13
#define CONTROL_ECU_READY 0x10
#define HMI_ECU_READY 0x11
#define OPEN_GATE 0x15

/* Global variables for tracking system steps and timing */
uint8 step = 1;
uint8 tick1 = 0;
uint8 tick = 0;

/* 
 * Description:
 * Function to create and confirm the system password.
 * The user enters the password twice, and it is sent to the CONTROL_ECU for verification.
 */
void create_system_password() {
    UART_sendByte(HMI_ECU_READY); /* Signal CONTROL_ECU to start transmission */

    LCD_clearScreen();
    uint8 i = 0;
    LCD_displayString("Plz enter pass:");
    LCD_moveCursor(1, 0);

    /* User enters the password */
    while (i < PASSWORD_LENGTH) {
        UART_sendByte(KEYPAD_getPressedKey()); /* Send each key to CONTROL_ECU */
        _delay_ms(250); /* Debounce delay */
        LCD_displayString("*");
        i++;
    }
    /* Wait for the user to press the enter button */
    while (KEYPAD_getPressedKey() != ENTER_BUTTON);
    _delay_ms(150); /* Debounce delay */

    /* Prompt user to re-enter the password for confirmation */
    LCD_clearScreen();
    i = 0;
    LCD_displayString("Plz re-enter the");
    LCD_moveCursor(1, 0);
    LCD_displayString("same pass:");

    UART_sendByte(HMI_ECU_READY); /* Signal CONTROL_ECU to start transmission again */

    /* User re-enters the password */
    while (i < PASSWORD_LENGTH) {
        UART_sendByte(KEYPAD_getPressedKey()); /* Send each key to CONTROL_ECU */
        _delay_ms(250); /* Debounce delay */
        LCD_displayString("*");
        i++;
    }
    while (KEYPAD_getPressedKey() != ENTER_BUTTON);
    _delay_ms(250); /* Debounce delay */
}

/* 
 * Description:
 * Function to read the password from the user when trying to open the door or change passwords.
 */
void read_password() {
    char read_password[PASSWORD_LENGTH]; /* Array to store the entered password */
    LCD_clearScreen();
    uint8 i = 0;
    LCD_displayString("Plz enter pass:");
    LCD_moveCursor(1, 0);

    /* User enters the password */
    while (i < PASSWORD_LENGTH) {
        read_password[i] = KEYPAD_getPressedKey();
        _delay_ms(250); /* Debounce delay */
        LCD_displayString("*");
        i++;
    }
    while (KEYPAD_getPressedKey() != ENTER_BUTTON);
    _delay_ms(250); /* Debounce delay */

    /* Send the entered password to CONTROL_ECU */
    for (i = 0; i < PASSWORD_LENGTH; i++) {
        UART_sendByte(read_password[i]);
    }
}

/* 
 * Description:
 * Function to rotate the motor and open the door when the password is correct.
 * This function is executed by the Timer1 callback.
 */
void rotate_motor_open_door() {
    tick++;
    if (tick == 15) {
        TCNT1 = 0;
        LCD_clearScreen();
        LCD_displayString("Locking in 3 sec");
    } else if (tick == 18) {
        TCNT1 = 0;
        LCD_clearScreen();
        LCD_displayString("Door is Locking");
    } else if (tick == 33) {
        Timer1_deInit();
    }
}

/* 
 * Description:
 * Function to lock the system when the entered password is incorrect more than 3 times.
 * This function is executed by the Timer1 callback.
 */
void system_locked(void) {
    tick1++;
    if (tick1 == 30) {
        Timer1_deInit();
    }
}

int main(void) {
    /* Initialize UART with the desired settings */
    UART_ConfigType uart = {EIGHT_BIT_MODE, EVEN_PARITY, ONE_STOP_BIT, RATE_THREE};

    /* Initialize Timer1 with the desired settings */
    Timer1_ConfigType timer1 = {
        0,          /* Initial value */
        31250,      /* Compare value */
        F_CPU_256,  /* Prescaler */
        COMPARE_MODE
    };

    SREG |= (1 << 7); /* Enable global interrupts */
    LCD_init();
    UART_init(&uart);

    uint8 choice, step = 1;

    while (1) {
        if (step == 1) {
            create_system_password();

            /* Wait for CONTROL_ECU to be ready */
            while (UART_receiveByte() != CONTROL_ECU_READY);

            /* Get the next step from CONTROL_ECU */
            step = UART_receiveByte();
        } else if (step == 2) {
            UART_sendByte(HMI_ECU_READY); /* Signal CONTROL_ECU to start communication */

            LCD_clearScreen();
            LCD_displayString("+ : Open Door");
            LCD_moveCursor(1, 0);
            LCD_displayString("- : Change Pass");
            choice = KEYPAD_getPressedKey();
            _delay_ms(250); /* Debounce delay */

            if (choice == '+') {
                UART_sendByte(choice); /* Send choice to CONTROL_ECU */
                while (UART_receiveByte() != CONTROL_ECU_READY);
                step = UART_receiveByte(); /* Get the next step from CONTROL_ECU */
            } else if (choice == '-') {
                UART_sendByte(choice); /* Send choice to CONTROL_ECU */
                while (UART_receiveByte() != CONTROL_ECU_READY);
                step = UART_receiveByte(); /* Get the next step from CONTROL_ECU */
            }
        } else if (step == 3) {
            UART_sendByte(HMI_ECU_READY); /* Signal CONTROL_ECU to start communication */
            read_password(); /* Read password from user */

            while (UART_receiveByte() != CONTROL_ECU_READY);
            uint8 receiveByte = UART_receiveByte();

            if (receiveByte == OPEN_GATE) {
                LCD_clearScreen();
                Timer1_init(&timer1); /* Initialize Timer1 */
                LCD_displayString("Door is Unlocking");
                Timer1_setCallBack(&rotate_motor_open_door);
                step = 6;
            } else if (receiveByte == 5) {
                step = 5;
            }
        } else if (step == 4) {
            UART_sendByte(HMI_ECU_READY); /* Signal CONTROL_ECU to start communication */
            read_password(); /* Read password from user */

            while (UART_receiveByte() != CONTROL_ECU_READY);
            step = UART_receiveByte(); /* Get the next step from CONTROL_ECU */
        } else if (step == 5) {
            LCD_clearScreen();
            LCD_displayString("ERROR");
            Timer1_init(&timer1); /* Initialize Timer1 */
            Timer1_setCallBack(&system_locked);
            step = 6;
        } else if (step == 6) {
            /* Return to step 2 if the system was locked */
            if (tick1 == 30) {
                step = 2;
                tick1 = 0;
            }
            /* Return to step 2 after door operation */
            if (tick == 33) {
                step = 2;
                tick = 0;
            }
        }
    }
}

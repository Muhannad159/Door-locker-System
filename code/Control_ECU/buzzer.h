/*******************************
 *  buzzer.h
 *
 *  Created on: Aug 2, 2024
 * 
 *  Author: Muhannad Abdallah
 ******************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include"std_types.h"

#define BUZZER_PORT PORTA_ID
#define BUZZER_PIN PIN0_ID


void Buzzer_init();

void Buzzer_on(void);

void Buzzer_off(void);



#endif /* BUZZER_H_ */

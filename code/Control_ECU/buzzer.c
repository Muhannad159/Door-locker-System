/*******************************
 *  buzzer.c
 *
 *  Created on: Aug 2, 2024
 * 
 *  Author: Muhannad Abdallah
 ******************************/

#include"buzzer.h"
#include"gpio.h"


void Buzzer_init(){
	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN,LOGIC_LOW);
}

void Buzzer_on(void){
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN,LOGIC_HIGH);
}

void Buzzer_off(void){
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN,LOGIC_LOW);
}

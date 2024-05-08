/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 PWM2.c
 Autor: Ian Anleu Rivera
 Proyecto: Proyecto 2
 Hardware: ATMEGA328P
 Creado: 06/07/2024
 Ultima modificacion: 08/07/2024
    -----------------------------------------------
*/

#include "PWM2.h"

uint16_t contador_pwm = 0;
uint16_t limite_contador1 = 0;
uint16_t limite_contador2 = 0;

void setupTimer2(void){
	//Interrupciones
	TIMSK2 = 0;
	TIMSK2 = (1<<TOIE2); //Overflow
	
	//Forma, modo, prescaler
	TCCR2A = 0;
	TCCR2B = 0;
	TCCR2B |= (1<<CS21); //Operación temporizador | Prescaler 8
	
	//Inicial en el valor límite
	TCNT2 = TIMERTOP;
}

void actualizar_servomanual(uint8_t adchleido, uint8_t servo_a_modificar){
	if (servo_a_modificar == 1){
		limite_contador1 = adchleido; //Servo 1
	}else{
		limite_contador2 = adchleido; //Servo 4
	}
}

ISR(TIMER2_OVF_vect){
	//Cuando llega al tope pasan .007ms entonces se actualiza la señal en PB3 o PB0
	//dependiendo de la comparación de contadores
	
	//Servo 1
	if (contador_pwm >= limite_contador1){
		PORTB &= 0xF7;
	}
	else{
		PORTB |= 0x08;
	}
	
	//Servo 4
	if (contador_pwm >= limite_contador2){
		PORTB &= 0xFE;
	}
	else{
		PORTB |= 0x01;
	}
	
	contador_pwm += 1;
	TCNT2 = TIMERTOP;
	
	//Limite
	if (contador_pwm > MANUALTOP){
		contador_pwm = 0;
	}
}
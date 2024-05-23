/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 PWM2.c
 Autor: Ian Anleu Rivera
 Proyecto: Proyecto 2
 Hardware: ATMEGA328P
 Creado: 06/07/2024
 Ultima modificacion: 22/05/2024
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
	switch (servo_a_modificar){
		case 1:
			if(adchleido<240){ 
				if(adchleido>39){
					limite_contador1 = adchleido; //Servo 1
				}//LIMITE INF. SERVO 1
				else{
					limite_contador1 = 39;
				}
			}//LIMITE SUP. SERVO 1
			else{
				limite_contador1 = 240;
			}
			break;
		case 4:
			if(adchleido<235){
				if(adchleido>178){
					limite_contador2 = adchleido; //Servo 4
				}//LIMITE INF. SERVO 4
				else{
					limite_contador2 = 178;
				}
			}//LIMITE SUP. SERVO 4
			else{
				limite_contador2 = 235;
			}
			break;
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
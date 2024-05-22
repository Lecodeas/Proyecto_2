/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 PWM1.c
 Autor: Ian Anleu Rivera
 Proyecto: Proyecto 2
 Hardware: ATMEGA328P
 Creado: 14/04/2024
 Ultima modificacion: 29/04/2024
 -----------------------------------------------
 */

#include "PWM1.h"

uint8_t listo_actualizar_pwm = 0;

ISR(TIMER1_OVF_vect){
	//Regresa a los 20ms
	listo_actualizar_pwm = 0;
}


void setupPWM1(void){
	//Interrupciones
	TIMSK1 = 0;
	TIMSK1 = (1<<TOIE1); //Overflow
	
	//Top en ICR1
	ICR1H = (PWMTOP & 0xFF00) >> 8;
	ICR1L = (PWMTOP & 0x00FF);
	
	//Inicializar servo al minimo
	OCR1AH = (SERVOMIN & 0xFF00) >> 8;
	OCR1AL = (SERVOMIN & 0x00FF);
	
	OCR1BH = (SERVOMIN & 0xFF00) >> 8;
	OCR1BL = (SERVOMIN & 0x00FF);
	
	//Forma, modo, prescaler
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11); //A1 No inverted | B1 No inverted | Fast con ICR1 Top
	TCCR1B |= (1<<WGM12) | (1<<WGM13) | (1<<CS11); //Fast con ICR1 Top | Prescaler 8 
}

static void actualizar_pwm(uint16_t i, uint8_t NumServo){
	listo_actualizar_pwm = 1;
	while (listo_actualizar_pwm != 0){
		//El valor del pulso depende del leído en ADCH
		//El OCR1 modificado depende de NumServo
		if (NumServo==3){
			OCR1AH = (i & 0xFF00) >> 8;
			OCR1AL = (i & 0x00FF);
		}
		if(NumServo==2){
			OCR1BH = (i & 0xFF00) >> 8;
			OCR1BL = (i & 0x00FF);
		}
	}
}

void actualizar_servo(uint8_t adchleido, uint8_t servo_a_modificar){
	//Conversión de lectura de ADC a pulso en PWM
	uint16_t valorservo = 0;
	switch(servo_a_modificar){
		case 2:
			if (adchleido<227){
				if (adchleido>71){
					float razon = (float)adchleido/(float)255;
					razon = (((float)SERVOMAX - (float)SERVOMIN)*razon) + (float)SERVOMIN;
					valorservo = (uint16_t)razon;
				}//LIMITE INF. SERVO 2
				else{
					float razon = (float)71/(float)255;
					razon = (((float)SERVOMAX - (float)SERVOMIN)*razon) + (float)SERVOMIN;
					valorservo = (uint16_t)razon;
				}
			}// LIMITE SUP. SERVO 2
			else{
				float razon = (float)227/(float)255;
				razon = (((float)SERVOMAX - (float)SERVOMIN)*razon) + (float)SERVOMIN;
				valorservo = (uint16_t)razon;
			}
			break;
		case 3:
			if (adchleido<229){
				if (adchleido>156){
					float razon = (float)adchleido/(float)255;
					razon = (((float)SERVOMAX - (float)SERVOMIN)*razon) + (float)SERVOMIN;
					valorservo = (uint16_t)razon;
				}//LIMITE INF. SERVO 2
				else{
					float razon = (float)156/(float)255;
					razon = (((float)SERVOMAX - (float)SERVOMIN)*razon) + (float)SERVOMIN;
					valorservo = (uint16_t)razon;
				}
			}// LIMITE SUP. SERVO 2
			else{
				float razon = (float)229/(float)255;
				razon = (((float)SERVOMAX - (float)SERVOMIN)*razon) + (float)SERVOMIN;
				valorservo = (uint16_t)razon;
			}
			break;
	}//SWITCH SERVOS
	actualizar_pwm(valorservo, servo_a_modificar);
}
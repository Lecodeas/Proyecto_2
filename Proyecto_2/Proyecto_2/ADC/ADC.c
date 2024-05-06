/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 ADC.c
 Autor: Ian Anleu Rivera
 Proyecto: Proyecto 2
 Hardware: ATMEGA328P
 Creado: 24/04/2024
 Ultima modificacion: 29/04/2024
 -----------------------------------------------
 */

#include "ADC.h"

uint8_t valoradc;

void setupADC(void){
	ADMUX = 0;
	ADMUX |= (1<<REFS0)|(1<<ADLAR)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0); //Vcc ref | Just. Izq. | Mux ADC7
	ADCSRA = 0;
	ADCSRA |= (1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Int. En | Presc. 128 (125kHz)
	ADCSRA |= (1<<ADEN); // Enable
	ADCSRA |= (1<<ADSC); //Primer Inicio
}

uint8_t fetchADCH(void){
	return (valoradc);
}

ISR(ADC_vect){
	valoradc = ADCH; // Almacenar valor
}
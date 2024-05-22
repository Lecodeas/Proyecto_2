/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 UART.c
 Autor: Ian Anleu Rivera
 Proyecto: Proyecto 2
 Hardware: ATMEGA328P
 Creado: 08/05/2024
 Ultima modificacion: 08/05/2024
 -----------------------------------------------
 */

#include "UART.h"

void setupUART(void){
	//No necesito nada de este registro
	UCSR0A = 0;
	
	//Rx int. | Tx En. | Rx en.
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	
	//Async | No Pariety | 1 stop | 8 data
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	
	//UDR0 9600
	UBRR0 = 103;
	
}

void enviarChar(char Caracter){
	while (!(UCSR0A & (1<<UDRE0))); //Hasta que esté vacío puedo enviar el dato
	UDR0 = Caracter;
}

void enviarNum(uint8_t adchleido){
	//Envío de caracter por caracter a UART
	uint8_t c = 0; //Centenas
	uint8_t d = 0; //Decenas
	uint8_t u = 0; //Unidades
	while(adchleido > 99){
		c++;
		adchleido -= 100;
	}
	enviarChar((char) c+48);
	while(adchleido > 9){
		d++;
		adchleido -= 10;
	}
	enviarChar((char) d+48);
	while(adchleido != 0){
		u++;
		adchleido -= 1;
	}
	enviarChar((char) u+48);
	enviarChar('\n');
}

uint8_t StrToNum(char* str){
	//Cadena de 4 caracteres
	uint8_t i = 0;
	uint8_t retVal = 0;
	uint8_t u = 0;
	uint8_t d = 0;
	uint8_t c = 0;
	for(i = 0; str[i] != '\0'; i++){
		//Mientras el caracter no sea nulo, se procesa
		switch(i){
			case 1: //Unidad
				u = ((uint8_t) str[i]) - 48;
				break;
			case 2: //Unidad
				d = 10*(((uint8_t) str[i]) - 48);
				break;
			case 3: //Unidad
				c = 100*(((uint8_t) str[i]) - 48);
				break;
		}
	}
	retVal = u+d+c;
	return retVal;
}
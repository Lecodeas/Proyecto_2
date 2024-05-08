/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 UART.h
 Autor: Ian Anleu Rivera
 Proyecto: Proyecto 2
 Hardware: ATMEGA328P
 Creado: 08/05/2024
 Ultima modificacion: 08/05/2024
 -----------------------------------------------
 */


#ifndef UART_H_
#define UART_H_

#include <avr/io.h> //IO registros
#include <stdint.h> //Lib para enteros
#include <avr/interrupt.h> //Lib para interrupciones

void setupUART(void);
void enviarChar(char Caracter);
void enviarNum(uint8_t adchleido);


#endif /* UART_H_ */
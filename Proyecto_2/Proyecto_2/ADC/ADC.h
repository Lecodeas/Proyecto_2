/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 ADCH.h
 Autor: Ian Anleu Rivera
 Proyecto: Proyecto 2
 Hardware: ATMEGA328P
 Creado: 24/04/2024
 Ultima modificacion: 29/04/2024
    -----------------------------------------------
*/	


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>//IO registros
#include <stdint.h> //Lib para enteros
#include <avr/interrupt.h> //Lib para interrupciones

void setupADC(void);
uint8_t fetchADCH(void);

#endif /* ADC_H_ */
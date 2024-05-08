/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 PWM2.h
 Autor: Ian Anleu Rivera
 Proyecto: Proyecto 2
 Hardware: ATMEGA328P
 Creado: 06/07/2024
 Ultima modificacion: 06/07/2024
    -----------------------------------------------
*/



#ifndef PWM2_H_
#define PWM2_H_

#include <avr/io.h> //IO registros
#include <stdint.h> //Lib para enteros
#include <avr/interrupt.h> //Lib para interrupciones

//Constantes
#define TIMERTOP 242
#define MANUALTOP 2858

void setupTimer2(void);
void actualizar_servomanual(uint8_t adchleido, uint8_t servo_a_modificar);



#endif /* PWM2_H_ */
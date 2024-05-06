/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 PWM1.h
 Autor: Ian Anleu Rivera
 Proyecto: Proyecto 2
 Hardware: ATMEGA328P
 Creado: 14/04/2024
 Ultima modificacion: 29/04/2024
    -----------------------------------------------
*/


#ifndef PWM1_H_
#define PWM1_H_

#include <avr/io.h> //IO registros
#include <stdint.h> //Lib para enteros
#include <avr/interrupt.h> //Lib para interrupciones

//Constantes
#define PWMTOP 39999
#define SERVOMAX 4797
#define SERVOMIN 1271

void setupPWM1(void);
void actualizar_servo(uint8_t adchleido, uint8_t servo_a_modificar);

#endif /* PWM1_H_ */
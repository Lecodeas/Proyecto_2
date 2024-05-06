/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 Proyecto_2.c
 Autor: Ian Anleu Rivera
 Proyecto: Proyecto 2
 Hardware: ATMEGA328P
 Creado: 29/04/2024
 Ultima modificacion: 29/04/2024
    -----------------------------------------------
*/

//HEADER FILES
#define F_CPU 160000000 //F cpu en 16 Mhz
#include <avr/io.h> // IO regs
#include <util/delay.h> //Delays
#include <stdint.h> //Lib para enteros
#include <avr/interrupt.h> //Lib para interrupciones
#include "PWM1/PWM1.h" //PWM1 Hecha
#include "ADC/ADC.h" //PWM1 Hecha

//VARIABLES GLOBALES
volatile uint8_t adchleido = 0; //Donde almacenar el adch

//PROTOTIPOS DE FUNCIÓN
void setupGeneral(void);

//PRINCIPAL
int main(void)
{
	cli(); 
    setupADC();
	setupPWM1();
	setupGeneral();
	sei();
	
    while (1) 
    {
		//Servo 1
		adchleido = fetchADCH(); //Recibo el adch actual
		actualizar_servo(adchleido, 1); //Lo envía al servo 1
		ADMUX &= 0xF0;
		ADMUX |= (1<<REFS0)|(1<<ADLAR)|(1<<MUX2)|(1<<MUX1); //Remultiplexado ADC6
		ADCSRA |= (1<<ADSC); //Volver a iniciar ADC
		_delay_ms(1);
		
		//Servo 2
		adchleido = fetchADCH(); //Recibo el adch actual
		actualizar_servo(adchleido, 2); //Lo envía al servo 1
		ADMUX &= 0xF0;
		ADMUX |= (1<<REFS0)|(1<<ADLAR)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0); //Remultiplexado ADC7
		ADCSRA |= (1<<ADSC); //Volver a iniciar ADC
		_delay_ms(1);
    }
}

//INTERRUPCIONES


//OTRAS FUNCIONES

void setupGeneral(void){
	//Entradas y Salidas
	//ENTRADAS
	DDRD = 0; //Solo PD1 será salida
	
	//SALIDAS
	DDRC = 0; //Todas salidas (Digital disabled para PC4 y PC5)
	DDRB = 0xFF; //Todas salidas
	DDRD |= (1<<DDD1);		
}
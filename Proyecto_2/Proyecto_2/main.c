/* -----------------------------------------------
 Universidad del Valle de Guatemala
 IE2023: Programacion de Microcontroladores
 Proyecto_2.c
 Autor: Ian Anleu Rivera
 Proyecto: Proyecto 2
 Hardware: ATMEGA328P
 Creado: 29/04/2024
 Ultima modificacion: 08/05/2024
    -----------------------------------------------
*/

//HEADER FILES
#define F_CPU 160000000 //F cpu en 16 Mhz
#include <avr/io.h> // IO regs
#include <util/delay.h> //Delays
#include <stdint.h> //Lib para enteros
#include <avr/interrupt.h> //Lib para interrupciones
#include "PWM1/PWM1.h" //PWM1 Hecha
#include "PWM2/PWM2.h" //PWM2 Hecha
#include "ADC/ADC.h" //ADC Hecha
#include "UART/UART.h" //UART Hecha

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
	setupTimer2();
	setupUART();
	setupGeneral();
	sei();
	
    while (1) 
    {
		//Servo 1 PB3
		adchleido = fetchADCH(); //Recibo el adch actual
		actualizar_servomanual(adchleido, 1); //Lo envía al servo 1
		ADMUX &= 0xF0;
		ADMUX |= (1<<MUX2) | (1<<MUX1); //Remultiplexado ADC6
		ADCSRA |= (1<<ADSC); //Volver a iniciar ADC
		_delay_us(500);	
	
		//Servo 2 OC1B
		adchleido = fetchADCH(); //Recibo el adch actual
		actualizar_servo(adchleido, 2); //Lo envía al servo 2
		ADMUX &= 0xF0;
		ADMUX |= (1<<MUX2) | (1<<MUX0); //Remultiplexado ADC5
		ADCSRA |= (1<<ADSC); //Volver a iniciar ADC
		_delay_us(500);
		
		//Servo 3 OC1A
		adchleido = fetchADCH(); //Recibo el adch actual
		actualizar_servo(adchleido, 3); //Lo envía al servo 3
		ADMUX &= 0xF0;
		ADMUX |=  (1<<MUX2); //Remultiplexado ADC4
		ADCSRA |= (1<<ADSC); //Volver a iniciar ADC
		_delay_us(500);
		
		//Servo 4 PB0
		adchleido = fetchADCH(); //Recibo el adch actual
		actualizar_servomanual(adchleido, 4); //Lo envía al servo 4
		ADMUX &= 0xF0;
		ADMUX |= (1<<MUX2)|(1<<MUX1)|(1<<MUX0); //Remultiplexado ADC7
		ADCSRA |= (1<<ADSC); //Volver a iniciar ADC
		_delay_us(500);
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
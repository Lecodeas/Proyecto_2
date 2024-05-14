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
#include <avr/eeprom.h> //Lib para EEPROM
#include "PWM1/PWM1.h" //PWM1 Hecha
#include "PWM2/PWM2.h" //PWM2 Hecha
#include "ADC/ADC.h" //ADC Hecha
#include "UART/UART.h" //UART Hecha

//CONSTANTES
#define MANUAL 1
#define MEMORIA 2
#define NUBE 3
#define MEMORIALECTURA 4

//VARIABLES GLOBALES
volatile uint8_t adchleido = 0; //  Donde almacenar el adch
volatile uint8_t servo1 = 0; // Valor de servo 1
volatile uint8_t servo2 = 0; // Valor de servo 2
volatile uint8_t servo3 = 0; // Valor de servo 3
volatile uint8_t servo4 = 0; // Valor de servo 4
uint8_t Estado = MANUAL; //Variable de estados
uint8_t Direccion = 0; // Dirección de los 4 estados de Servo

//PROTOTIPOS DE FUNCIÓN
void setupGeneral(void);

//PRINCIPAL
int main(void)
{
	cli(); 
	setupGeneral();
    setupADC();
	setupPWM1();
	setupTimer2();
	setupUART();
	sei();
	
    while (1) 
    {
		switch(Estado){
			
			case MANUAL: //MODO MANUAL
				//Servo 1 PB3
				adchleido = fetchADCH(); //Recibo el adch actual
				servo1 = adchleido;
				actualizar_servomanual(adchleido, 1); //Lo envía al servo 1
				ADMUX &= 0xF0;
				ADMUX |= (1<<MUX2) | (1<<MUX1); //Remultiplexado ADC6
				ADCSRA |= (1<<ADSC); //Volver a iniciar ADC
				_delay_us(250);
				
				//Servo 2 OC1B
				adchleido = fetchADCH(); //Recibo el adch actual
				servo2 = adchleido;
				actualizar_servo(adchleido, 2); //Lo envía al servo 2
				ADMUX &= 0xF0;
				ADMUX |= (1<<MUX2) | (1<<MUX0); //Remultiplexado ADC5
				ADCSRA |= (1<<ADSC); //Volver a iniciar ADC
				_delay_us(250);
				
				//Servo 3 OC1A
				adchleido = fetchADCH(); //Recibo el adch actual
				servo3 = adchleido;
				actualizar_servo(adchleido, 3); //Lo envía al servo 3
				ADMUX &= 0xF0;
				ADMUX |=  (1<<MUX2); //Remultiplexado ADC4
				ADCSRA |= (1<<ADSC); //Volver a iniciar ADC
				_delay_us(250);
				
				//Servo 4 PB0
				adchleido = fetchADCH(); //Recibo el adch actual
				servo4 = adchleido;
				actualizar_servomanual(adchleido, 4); //Lo envía al servo 4
				ADMUX &= 0xF0;
				ADMUX |= (1<<MUX2)|(1<<MUX1)|(1<<MUX0); //Remultiplexado ADC7
				ADCSRA |= (1<<ADSC); //Volver a iniciar ADC
				_delay_us(250);
				
				break;	
				
				
			case MEMORIA: //MODO EEPROM
				//Servo 1 PB3
				actualizar_servomanual(servo1, 1); //Lo envía al servo 1
				_delay_us(250);
			
				//Servo 2 OC1B
				actualizar_servo(servo2, 2); //Lo envía al servo 2
				_delay_us(250);
			
				//Servo 3 OC1A
				actualizar_servo(servo3, 3); //Lo envía al servo 3
				_delay_us(250);
			
				//Servo 4 PB0
				actualizar_servomanual(servo4, 4); //Lo envía al servo 4
				_delay_us(250);
			
				break;
				
				
			case NUBE:
				// Operaciones de nube
				
				break;
		} // SWITCH ESTADOS
    } // WHILE LOOP
} // MAIN 

//OTRAS FUNCIONES
void setupGeneral(void){
	//Entradas y Salidas
	//ENTRADAS
	DDRD = 0; //Solo PD1 será salida
	
	//SALIDAS
	DDRC = 0x0F; //Todas salidas (Digital disabled para PC4 y PC5)
	DDRB = 0xFF; //Todas salidas
	DDRD |= (1<<DDD1);		
	
	//LED de estado inicial
	PORTC &= 0xF0; //Limpiar nibble de PC0, 1 y 2
	PORTC |= (1<<DDC0); //Led de Estado 1
	
	//Interrupciones de Pines
	PCICR = 0;
	PCICR |= (1<<PCIE2); //Interrupciones de PIND
	PCMSK2 = 0;
	PCMSK2 |= (1<<PCINT21) | (1<<PCINT20); //PD4, PD5
} //Setup General

// INTERRUPCIONES
ISR(PCINT2_vect){
	uint8_t EstadoPinD = PIND; //Almacenar 
	switch(Estado){	
		// MODIFICACIÓN MANUAL ----------------
		case MANUAL: // PD4, Cambiar estado | PD5, Escribir a memoria 
			if (EstadoPinD & (1<<DDD4)){ //Si PD4 está en 0, cambia de estado
				Estado = MEMORIA;
				//LED de estado 1
				PORTC &= 0xF0; //Limpiar nibble de PC0, 1 y 2
				PORTC |= (1<<DDC1); //Led de Estado 2
				Direccion = 0;
			}
			if (EstadoPinD & (1<<DDD5)){ //Si PD5 está en 0, escribe la posición a la EEPROM
				enviarChar('W');
				enviarChar('\n');
				// Operaciones de Eeprom
				eeprom_write_byte((uint8_t*) (4*Direccion), servo1);
				eeprom_write_byte((uint8_t*) ((4*Direccion)+1), servo2);
				eeprom_write_byte((uint8_t*) ((4*Direccion)+2), servo3);
				eeprom_write_byte((uint8_t*) ((4*Direccion)+3), servo4);
				Direccion+=1;
				if (Direccion == 4){
					Direccion = 0;
				}
			}
			break;
			
		// LEER DESDE MEMORIA ----------------
		case MEMORIA: // PD4, Cambiar estado | PD5, Leer desde memoria 
			if (EstadoPinD & (1<<DDD4)){ //Si PD4 está en 0, cambia de estado
				Estado = NUBE;
				//LED de estado 1
				PORTC &= 0xF0; //Limpiar nibble de PC0, 1 y 2
				PORTC |= (1<<DDC2); //Led de Estado 3
				Direccion = 0;
			}
			if (EstadoPinD & (1<<DDD5)){ //Si PD5 está en 0, cambia la posición de lectura desde la EEPROM
				// Operaciones de Eeprom
				servo1 = eeprom_read_byte((uint8_t*)(4*Direccion));
				servo2 = eeprom_read_byte((uint8_t*)((4*Direccion)+1));
				servo3 = eeprom_read_byte((uint8_t*)((4*Direccion)+2));
				servo4 = eeprom_read_byte((uint8_t*)((4*Direccion)+3));
				Direccion+=1;
				if (Direccion == 4){
					Direccion = 0;
				}
			}
			break;
		
		// OPERACIONES DE NUBE ----------------
		case NUBE: // PD4, Cambiar estado | PD5, Nada 
			if (EstadoPinD & (1<<DDD4)){ //Si PD4 está en 0, cambia de estado
				Estado = MANUAL;
				//LED de estado 1
				PORTC &= 0xF0; //Limpiar nibble de PC0, 1 y 2
				PORTC |= (1<<DDC0); //Led de Estado 1
			}
			break;
	} // SWITCH ESTADOS
} // ISR Pines
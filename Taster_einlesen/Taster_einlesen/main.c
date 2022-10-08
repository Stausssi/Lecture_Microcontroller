/*
 * Taster_einlesen.c
 *
 * Created: 30.03.2020 09:43:49
 * Author : Simon
 */ 

#include <avr/io.h>
#include <avr/delay.h>

#include "defines.h"

int main(void) {
	// Taster als Eingaenge
	DDRB &= ~((1 << T1) | (1 << T2) | (1 << T3) | (1 << T4));
	// 0b 0011 1100
	// ~  1100 0011
	
	// LEDS als Ausgaenge
	DDRC |= ((1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4));
	// 0b 0001 1110
	
	int isOn = 1;
	
    while (1) {
		// Taster T2.1 gedrueckt
		if (PINB & (1 << T1))	{
			if(isOn == 0) {
				PORTC &= ~(1<< LED1); // LED aus
				isOn = 1;
			} else {
				PORTC |= (1 << LED1); // LED ein
				isOn = 0;
			}
			
			_delay_ms(100);
		} 
		
		// Taster T2.2 gedrueckt
		if (PINB & (1 << T2))	PORTC |= (1 << LED2);
		else					PORTC &= ~(1 << LED2);
		
		// Taster T2.3 gedrueckt
		if (PINB & (1 << T3))	PORTC |= (1 << LED3); 
		else					PORTC &= ~(1 << LED3);
		
		// Taster T2.4 gedrueckt, Betrieb invers
		if (PINB & (1 << T4))	PORTC &= ~(1 << LED4);
		else					PORTC |= (1 << LED4);
    }
}


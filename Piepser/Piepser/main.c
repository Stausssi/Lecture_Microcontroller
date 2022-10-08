/*
 * Piepser.c
 *
 * Created: 01.04.2020 09:24:55
 * Author : Simon
 */ 

#include "header.h"

int main(void)
{	
	// Piepser als Ausgang
	DDRB |= 1 << PIEZO_B;
	
	// Timer1 intialisieren
	TCCR1A |= (1 << WGM10) | (1 << WGM11) | (1 << COM1A1);
	TCCR1B |= 8;
	OCR1A = 0xFF;
	
    while (1) 
    {	
		// Prescale veraendern
		for(TCCR1B = 8; TCCR1B < 15; TCCR1B++) _delay_ms(500);
    }
}


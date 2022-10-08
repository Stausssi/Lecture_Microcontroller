/*
 * Blink_Timer.c
 *
 * Created: 01.04.2020 10:53:58
 * Author : Simon
 */ 

#include "header.h"

ISR(TIMER1_COMPA_vect) {
	// LED1.0 togglen
	PORTD ^= 1 << LED10_D;
}

int main(void)
{
	// IRQs erlauben
	SREG |= 1 << IBIT;
	
	// LED als Ausgang
	DDRD |= 1 << LED10_D;
	
	// Timer1 konfigurieren
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	
	// Rechenweg:
	// CPU Frequenz: 12MHz (83ns) -> Zähler auf 6 024 069 nach 500ms
	// Output Compare Register A (OCR0A) besteht aus 8 Bit -> 255
	// Also: Prescaler 256 und 16 Bit Zähler benutzen. 
	// Durch Prescaler ist die Zahl nun 23 532, Hexadezimal: 0x5BEC
	// Somit ergeben sich:
	OCR1AH = 0x5B;
	OCR1AL = 0xEC;
	
	// Timer1 interrupts aktivieren
	// Wird ausgeloest wenn counter = 0x5BEC
	TIMSK1 |= 1 << OCIE1A;
	
    while (1);
}


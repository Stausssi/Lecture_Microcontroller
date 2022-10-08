/*
 * RGB_LED_Dimmen.c
 *
 * Created: 01.04.2020 08:06:30
 * Author : Simon
 */ 

#include "header.h"

/* Aufgabe
 * 
 * Regenbogen: Alle Farben der RGB darstellen und dimmen mit Tastern
 * 
 */

// Gruen togglen
ISR(TIMER2_OVF_vect)
{
	PORTD ^= (1 << RGB_G_D);
}

ISR(TIMER2_COMPA_vect)
{
	PORTD ^= (1 << RGB_G_D);
}

int main(void)
{
	// IRQs erlauben
	SREG |= 1<<IBIT;
	
    // RGB-LED ueber PWM dimmen / Farbwechsel
	DDRD |= (1 << RGB_B_D) | (1 << RGB_R_D) | (1 << RGB_G_D);

	// Taster als Eingaenge
	DDRB &= ~((1 << T21_B) | (1 << T22_B) | (1 << T23_B) | (1 << T24_B) | (1 << T20_B));
	DDRD &= ~(1 << T25_D);
	
	// Timer0 als PWM ca. 5kHz mit 50%TV
	TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);	// Fast-PWM mit TOP=0xFF und COM als non-inverting-Mode
	TCCR0B |= 1 << CS01;													// Prescaler=8 -> 5,8kHz
	OCR0A = 0x7F;															// TV=0x7F / 0xFF = 0,5 -> 50%
	OCR0B = 0x7F;															// TV=0x7F / 0xFF = 0,5 -> 50%
	
	// Timer2 fuer Gruen
	TIMSK2 |= 1 << TOIE2 | 1 << OCIE2A;
	TCCR2B |= 1 << WGM20 | 1 << WGM21;
	TCCR2B &= ~(1 << COM2B1 | 1 << COM2B1);
	TCCR2B |= 1 << CS21;
	OCR2A = 130;
	
    while (1) {
		// T2.1 -> Blau heller
		if(PINB & (1 << T21_B) && OCR0A < 0xFF) OCR0A++;	
		
		// T2.2 -> Blau dunkler
		if(PINB & (1 << T22_B) && OCR0A > 0) OCR0A--;
		
		// T2.3 -> Rot heller
		if(PINB & (1 << T23_B) && OCR0B < 0xFF) OCR0B++;
		
		// T2.4 -> Rot dunkler
		if(PINB & (1 << T24_B) && OCR0B > 0) OCR0B--;
		
		// T2.5 -> Gruen heller (an)
		if(PIND & (1 << T25_D) && OCR2A < 0xFF) OCR2A++;
		
		// T2.0 -> Gruen dunkler (aus)
		if(PINB & (1 << T20_B) && OCR2A > 0) OCR2A--;
		
		_delay_ms(20);
    }
}


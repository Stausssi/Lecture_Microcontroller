/*
 * ADC_Dimmen.c
 *
 * Created: 02.04.2020 09:05:48
 * Author : Simon
 */ 

// DHBW-Header einbinden
#include "header.h"

// IRQ nach Beendigung des ADC
ISR(ADC_vect) {
	// Setze Helligkeit der RGB auf Wert der ADC
	OCR0A = ADCH;
}

int main(void)
{
	// IRQs aktivieren
	SREG |= 1 << IBIT;
	
    // RGB als Ausgang
	DDRD |= 1 << RGB_B_D;
	
	// POTI als Eingang
	DDRC &= ~(1 << POTI31_C);
	
	// ADC initialisieren
	ADMUX |= (1 << REFS0);									// Referenz=VCC
	ADMUX |= (1 << ADLAR);									// Linksbuendig, -> Vernachlaessigung von den zwei LSB
	
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);	// Prescaler 128
	ADCSRA |= (1 << ADEN);									// ADC aktivieren
	ADCSRA |= (1 << ADIF) | (1 << ADIE);					// Interrupts aktivieren und Flag setzen
	
	// Timer0 fuer blaues Licht
	TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);	// Fast-PWM mit TOP=0xFF und COM als non-inverting-Mode
	TCCR0B |= 1 << CS01;									// Prescaler=8 -> 5,8kHz
	
    while (1) 
    {
		// ADC starten
		ADCSRA |= 1 << ADSC;
    }
}


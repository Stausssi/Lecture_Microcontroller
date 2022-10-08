/*
 * ADC_Prozent.c
 *
 * Created: 02.04.2020 12:28:15
 * Author : Simon
 */ 

// Header Vorlage einbinden
#include "header.h"

// IRQ nach Beendigung des ADC
ISR(ADC_vect) {
	// Prozent berechnen
	int percentage = (float) ADCH / 255 * 99;

	// LEDs ausschalten
	PORTC &= ~((1 << LED11_C) | (1 << LED12_C) | (1 << LED13_C)  | (1 << LED14_C));
	PORTD &= ~(1 << LED10_D);
	
	// 10er anzeigen
	PORTC |= (percentage / 10) << 1;
	PORTD |= 1 << LED10_D;
	
	_delay_ms(1000);
	
	// LEDs ausschalten
	PORTC &= ~((1 << LED11_C) | (1 << LED12_C) | (1 << LED13_C)  | (1 << LED14_C));
	PORTD &= ~(1 << LED10_D);
	
	// Einer anzeigen
	PORTC |= (percentage % 10) << 1;
	
	_delay_ms(1000);
}

int main(void)
{
    // IRQs aktivieren
    SREG |= 1 << IBIT;
    
    // LEDs als Ausgang
    DDRC |= (1 << LED11_C) | (1 << LED12_C) | (1 << LED13_C) | (1 << LED14_C);
	DDRD |= 1 << LED10_D;
    
    // POTI als Eingang
    DDRC &= ~(1 << POTI31_C);
    
    // ADC initialisieren
    ADMUX |= (1 << REFS0);									// Referenz=VCC
    ADMUX |= (1 << ADLAR);									// Linksbuendig, -> Vernachlaessigung von den zwei LSB
    
    ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);	// Prescaler 128
    ADCSRA |= (1 << ADEN);									// ADC aktivieren
    ADCSRA |= (1 << ADIF) | (1 << ADIE);					// Interrupts aktivieren und Flag setzen
	
	// ADC starten
    while (1) { ADCSRA |= 1 << ADSC; }
}


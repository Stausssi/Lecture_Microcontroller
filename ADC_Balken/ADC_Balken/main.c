/*
 * ADC_Balken.c
 *
 * Created: 02.04.2020 08:14:25
 * Author : Simon
 */ 

#include "header.h"


int main(void)
{
    // LEDs als Ausgaenge
	DDRC |= (1 << LED11_C) | (1 << LED12_C) | (1 << LED13_C) | (1 << LED14_C) | (1 << LED15_C);
	
	// POTI als Eingang
	DDRC &= ~(1 << POTI31_C);
	
	// ADC initialisieren
	ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3));	// Multiplexer deaktivieren
	ADMUX |= (1 << REFS0);												// Referenz=VCC
	ADMUX |= (1 << ADLAR);												// Linksbuendig, -> Vernachlaessigung von den zwei LSB
	
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);				// Prescaler 128
	ADCSRA |= (1 << ADEN);												// ADC aktivieren
	
	ADCSRB &= ~((1 << ADTS0) | (1 << ADTS1) | (1 << ADPS2));			// Free running mode
	
    while (1) {
		// ADC Starten
		ADCSRA |= 1 << ADSC;
		while (ADCSRA & (1 << ADSC));	// Warte auf Ende der Umwandlung
		
		// LEDs auschalten
		PORTC &= ~((1 << LED11_C) | (1 << LED12_C) | (1 << LED13_C) | (1 << LED14_C) | (1 << LED15_C));
		
		// Ergebnis (ADCH) auswerten -> 8 Bit -> 255 -> 5V -> 1V = 51 -> 0,5V -> 26
		if (ADCH > 26) PORTC |= 1 << LED11_C;	// LED1.1 an
		
		if (ADCH > 77) PORTC |= 1 << LED12_C;	// LED1.2 an
		
		if (ADCH > 128) PORTC |= 1 << LED13_C;	// LED1.3 an
		
		if (ADCH > 179) PORTC |= 1 << LED14_C;	// LED1.4 an
		
		if (ADCH > 230) PORTC |= 1 << LED15_C;	// LED1.5 an
    }
}


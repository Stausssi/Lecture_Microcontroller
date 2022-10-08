/*
 * ADC_Farbwechsel.c
 *
 * Created: 02.04.2020 09:29:55
 * Author : Simon
 */ 

// Header Vorlage einbinden
#include "header.h"

// IRQ nach Beendigung des ADC
ISR(ADC_vect) {
	// Maximalwert des ADC: 255
	// Jede RGB hat einen Bereich von 255/3 = 85

	// Alle Farben ausschalten
	OCR0A = OCR0B = OCR2A = 0;
	PORTD &= ~((1 << RGB_R_D) | (1 << RGB_G_D) | (1 << RGB_B_D));
	
	// 0-84 fuer Rot
	if(ADCH < 85) OCR0B = ADCH * 3; // Volle Helligkeit bei 85, keine Helligkeit bei 0
	
	// 85-169 fuer Gruen
	else if(ADCH < 170) OCR2A = (ADCH - 85) * 3; // Volle Helligkeit bei 170, Keine Helligkeit bei 85
	
	// 170-255 fuer Blau
	else if (ADCH < 256) OCR0A = (ADCH - 170) * 3; // Volle Helligkeit bei 255, Keine Helligkeit bei 170
}

// Interrupts fuer togglen der RGB gruen
ISR(TIMER2_OVF_vect) { PORTD ^= (1 << RGB_G_D); }

ISR(TIMER2_COMPA_vect) { PORTD ^= (1 << RGB_G_D); }

int main(void)
{
    // IRQs aktivieren
    SREG |= 1 << IBIT;
    
    // RGB als Ausgang
    DDRD |= (1 << RGB_B_D) | (1 << RGB_R_D) | (1 << RGB_G_D);
    
    // POTI als Eingang
    DDRC &= ~(1 << POTI31_C);
    
    // ADC initialisieren
    ADMUX |= (1 << REFS0);									// Referenz=VCC
    ADMUX |= (1 << ADLAR);									// Linksbuendig, -> Vernachlaessigung von den zwei LSB
    
    ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);	// Prescaler 128
    ADCSRA |= (1 << ADEN);									// ADC aktivieren
    ADCSRA |= (1 << ADIF) | (1 << ADIE);					// Interrupts aktivieren und Flag setzen
    
    // Timer0 fuer blau und rotes Licht
    TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);	// Fast-PWM mit TOP=0xFF und COM als non-inverting-Mode
    TCCR0B |= 1 << CS01;													// Prescaler=8 -> 5,8kHz
	
	// Timer2 fuer Gruene PWM simulierung
	TIMSK2 |= 1 << TOIE2 | 1 << OCIE2A;
	TCCR2B |= 1 << WGM20 | 1 << WGM21;
	TCCR2B &= ~(1 << COM2B1 | 1 << COM2B1);
	TCCR2B |= 1 << CS21;
	
    while (1) {
		// ADC starten
		ADCSRA |= 1 << ADSC;
    }
}


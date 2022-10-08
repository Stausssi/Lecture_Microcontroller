/*
 * LED_Blink_IRQ.c
 *
 * Created: 31.03.2020 08:28:36
 * Author : Simon
 */ 

#include "header.h"

// LED1-5 nacheinander aufleuchten (Abstand 1sek)
// Nach 5 Richtungswechsel. Rückwärts anmachen
// Interrupts über T0 (PCINT0) -> Stehen bleiben, RGB Grün, 1sek
// Interrupts über T5 (PCINT17) -> RGB Blau, Zurücksetzten auf LED1, 1sek


int led = 1, direction = 1;

ISR(PCINT0_vect) {
	PORTD |= (1 << RGB3);
	
	_delay_ms(1000);

	PORTD &= ~(1 << RGB3);
}

ISR(PCINT2_vect) {
	PORTD |= (1 << RGB2);
	
	_delay_ms(1000);

	PORTD &= ~(1 << RGB2);
		
	led = 1;
	direction = 1;
}

int main(void)
{
	// IRQs konfigurieren
	SREG |= (1 << IBIT);

    // T2.0 als PCINT2 (PB2) -> Gruppe0
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT0);
	
	// T2.5 als PCINT17 -> Gruppe2
	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT17);
	
	// Taster als Eingaenge
	DDRB &= ~(1 << T0);
	DDRD &= ~(1 << T5);
	
	// LEDs als Ausgaenge
	DDRC |= (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4) | (1 << LED5);
	DDRD |= (1 << RGB1) | (1 << RGB2) | (1 << RGB3) | (1 << LED0);

    while (1) 
    {
		PORTC &= ~((1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4) | (1 << LED5));
		PORTC |= led << 1;
		
		if(direction == 1) led*= 2;
		else led /= 2;
		
		if(led == 16 || led == 1) direction *= -1; 
		
		_delay_ms(500);
    }
} 


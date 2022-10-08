/*
 * Blink_LED.c
 *
 * Created: 30.03.2020 08:17:58
 * Author : Simon
 */ 

#define F_CPU 12000000UL

#include <avr/io.h>
#include <avr/delay.h>

int main(void) {
	// Port C1 als Ausgang konfigurieren
	// DDRC |= 0x02;
	// Bit  7 6 5 4 3 2 1 0
	// DDRC x x x x x x 0 x
	//      0 0 0 0 0 0 1 0
	//      ===============
	// OR   x x x x x x 1 x
	// ->   0b0000 0010 
	// ->   0x02
	// 1 << 1
	// 0b 0000 0001 << 1
	// -> 0000 0010
	// -> 0x02
	// 1 >> DDC1
	
	DDRC |= ( 1 << DDC1 );
	
    while (1) {
		// LED1.1 an Port C1 einschalten
		PORTC |= ( 1 << PC1 );
		
		_delay_ms(500);
		
		// LED1.1 an Port C1 ausschalten
		PORTC &= ~( 1 << PC1 );
		
		_delay_ms(500);
    }
}
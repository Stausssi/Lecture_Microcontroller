/*
 * Zaehlen.c
 *
 * Created: 30.03.2020 10:52:10
 * Author : Simon
 */ 

// 2.1 Inkrement / Dekrement
// 2.2 Richtung ändern
// 2.3 Reset
// 2.4 Startwert 21

#include "defines.h"

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB &= ~((1 << T1) | (1 << T2) | (1 << T3) | (1 << T4));
	
	DDRC |= (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4) | (1 << LED5);
	
	int count = 0, direction = 1;
	
    while (1) 
    {
		if(PINB & (1 << T1)) {
			if(direction == 1)	count++;
			else				count--;
		}
		
		else if(PINB & (1 << T2)) direction *= -1;
		
		else if(PINB & (1 << T3)) count = 0;
		
		else if(PINB & (1 << T4)) count = 21;
		
		PORTC &= ~(0x1F << 1);
		PORTC |= count << 1;
		
		_delay_ms(150);
    }
}


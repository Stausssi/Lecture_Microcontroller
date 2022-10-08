/* *******************************************
// Header-Datei für die Bosch Übungsplatine
//
*/

// Taster definieren
#define T20_B PB0
#define T21_B PB2
#define T22_B PB3
#define T23_B PB4
#define T24_B PB5
#define T25_D PD1

// LEDs defnieren
#define LED10_D PD4
#define LED11_C PC1
#define LED12_C PC2
#define LED13_C PC3
#define LED14_C PC4
#define LED15_C PC5
#define RGB_R_D PD5
#define RGB_G_D PD7
#define RGB_B_D PD6

#define PIEZO_B PB1
#define POTI31_C PC0

// CPU Takt vorgeben
#define F_CPU 12000000UL

// Include Dateien einbinden
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define IBIT 7

/*
4-Bit Zahl als Binärwert auf LED1.1...1.4 ausgeben
*/
void bin_zahl_ausgeben (uint8_t zahl, uint8_t hbit)
{
	// PortC1..4 als Ausgang für LED1.1...1.5 und alle ausschalten
	DDRC |= (( 1 << LED11_C ) | ( 1 << LED12_C ) | ( 1 << LED13_C ) | ( 1 << LED14_C ) | ( 1 << LED15_C ) );
	PORTC &= ~(( 1 << LED11_C ) | ( 1 << LED12_C ) | ( 1 << LED13_C ) | ( 1 << LED14_C ) | ( 1 << LED15_C ) );
	
	// Wert ausgeben
	PORTC |= ( zahl << 1 );
	
	// Kennung für High-Bits ausgeben
	if (hbit)
	PORTC |= ( 1 << LED15_C );
	else
	PORTC &= ~( 1 << LED15_C );
}

/*
Ulis Debugger
*/

void ulis_debugger (uint8_t wert)
{
	uint8_t bit30, bit74;
	uint8_t bup_TCNT0, bup_TCNT1L, bup_TCNT1H, bup_TCNT2, bup_DDRC, bup_PORTC;
	
	// IRQs deaktivieren
	SREG &= ~(1 << IBIT );
	
	// Bisherige Registerinhalte sichern
	bup_TCNT0 = TCNT0;
	bup_TCNT1L = TCNT1L;
	bup_TCNT1H = TCNT1H;
	bup_TCNT2 = TCNT2;
	
	// bisherigen PORTC sichern
	bup_DDRC = DDRC;
	bup_PORTC = PORTC;
	
	// übergebenen Wert in 2 4-Bit Zahlen zerlegen
	bit30 = wert & 0x0F;
	bit74 = (( wert >> 4 ) & 0x0F );
	
	// Wert Bit[3:0] ausgeben auf LED1.1 ...1.4
	bin_zahl_ausgeben (bit30, 0);
	
	// 1s warten
	_delay_ms(1000);
	
	// Wert Bit[7:4] ausgeben auf LED1.1 ...1.4 und LED1.5 als Bit[8:4] Kennung
	bin_zahl_ausgeben (bit74, 1);
	
	// 1s warten
	_delay_ms(1000);
	
	// Register wieder herstellen
	TCNT0 = bup_TCNT0;
	TCNT1L = bup_TCNT1L;
	TCNT1H = bup_TCNT1H;
	TCNT2 = bup_TCNT2;
	
	// PortC wieder herstellen
	DDRC = bup_DDRC;
	PORTC = bup_PORTC;
	
	// IRQs aktivieren
	SREG |= (1 << IBIT );
	
}
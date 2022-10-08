/*
* PingPong.c
*
* Created: 02.04.2020 12:14:28
* Author : Simon
*/

// Header-Vorlage einbinden
#include "header.h"

// stdlib Library fuer random
#include <stdlib.h>
#include <time.h>

// bool Library
#include <stdbool.h>

// Globale Variablen initialisieren
volatile int led = 4, direction = 1, p1Mistakes = 0, p2Mistakes = 0, delayDuration = 100;

volatile bool p1Pressed = false, p2Pressed = false;


/* -----[Hilfsmethoden]----- */
// Hilfsmethode fuer Delay
void delay(int duration) {
	for(int i = 0; i < duration; i++) _delay_ms(1);
}

// Hilfsmethode für RGB
void rgb(int player) {
	OCR0B = player * 50;
	
	OCR2A = 125 - OCR0B / 2;
}

// Hilfsmethode für Piepser
void soundFeedback(bool correct, int duration) {
	// Maximale Lautstaerke
	OCR1A = 0xFF;
	
	// Hoher Ton -> Richtig
	// Tiefer Ton -> Falsch
	if (correct) TCCR1B = 10;
	else TCCR1B = 13;
	
	delay(duration);
	
	// Piepser stummschalten
	OCR1A = 0x3FF;
}

// Hilfsmethode fuer Anzeige
void setLEDOn(int leds) {
	// Alle LEDs ausschalten
	PORTC &= ~((1 << LED11_C) | (1 << LED12_C) | (1 << LED13_C) | (1 << LED14_C) | (1 << LED15_C));
	
	// Benoetigte LEDs einschalten
	PORTC |= leds << 1;
}

// Hilfsmethode fuer fehler
void addMistake(volatile int* player) {
	(*player)++;
	soundFeedback(false, 100);
}

/* ----[Interrupt Events]---- */
// Interrupt fuer Taster T2.0 -> Spieler 1
ISR(PCINT0_vect) {
	// Nur auf steigende Flanken reagieren
	if(PINB & (1 << T20_B)) {
		// Zu Spieler hin
		if(direction == -1) {
			if(!p1Pressed) {
				p1Pressed = true;
				// If the first LED is on
				if(PINC & (1 << LED11_C)) {
					// Change Direction and give Sound-Feedback
					direction *= -1;
					soundFeedback(true, 100);
					p1Pressed = false;
				}
				// Spieler hat zu frueh gedrueckt
				else addMistake(&p1Mistakes);
			}
			// Spieler hat bereits gedrueckt
			else addMistake(&p1Mistakes);
		}
		// Von Spieler weg -> Ignorieren
		else p1Pressed = false;
	}
}

// Interrupt fuer Taster T2.5 -> Spieler 2
ISR(PCINT2_vect) {
	// Nur auf steigende Flanken reagieren
	if(PIND & (1 << T25_D)) {
		// Zu Spieler hin
		if(direction == 1) {
			if(!p2Pressed) {
				p2Pressed = true;
				// If the last LED is on
				if(PINC & (1 << LED15_C)) {
					// Change direction and give Sound-Feedback
					direction *= -1;
					soundFeedback(true, 100);
					p2Pressed = false;
				}
				// Spieler hat zu frueh gedrueckt
				else addMistake(&p2Mistakes);
			}
			// Spieler hat bereits gedrueckt
			else addMistake(&p2Mistakes);
		}
		// Von Spieler weg -> Ignorieren
		else p2Pressed = false;
	}
}

// IRQ nach Beendigung des ADC -> Programmstart
ISR(ADC_vect) {
	// 255 -> 5000ms delay -> * 17,65
	// 0 -> 500ms delay -> + 500
	delayDuration = ADCH * 17.65 + 500;
}

// Interrupts fuer togglen der RGB gruen
ISR(TIMER2_OVF_vect) { PORTD ^= (1 << RGB_G_D); }

ISR(TIMER2_COMPA_vect) { PORTD ^= (1 << RGB_G_D); }

int main(void)
{
	/* -----[Interrupts Initialisieren]----- */
	// IRQs konfigurieren
	SREG |= (1 << IBIT);

	// T2.0 als PCINT2 (PB2) -> Gruppe
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT0);
	
	// T2.5 als PCINT17 -> Gruppe
	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT17);
	
	
	/* -----[Ein-/Ausgangskonfiguration]----- */
	// Taster als Eingaenge
	DDRB &= ~(1 << T20_B);
	DDRD &= ~(1 << T25_D);
	
	// LEDs als Ausgaenge
	DDRC |= (1 << LED11_C) | (1 << LED12_C) | (1 << LED13_C) | (1 << LED14_C) | (1 << LED15_C);
	DDRD |= (1 << RGB_B_D) | (1 << RGB_R_D) | (1 << RGB_G_D) | (1 << LED10_D);
	
	// POTI als Eingang
	DDRC &= ~(1 << POTI31_C);
	
	// Piepser als Ausgang
	DDRB |= 1 << PIEZO_B;
	
	
	/* -----[ADC Initialisieren]----- */
	ADMUX |= (1 << REFS0);									// Referenz=VCC
	ADMUX |= (1 << ADLAR);									// Linksbuendig, -> Vernachlaessigung von den zwei LSB
	
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);	// Prescaler 128
	ADCSRA |= (1 << ADEN);									// ADC aktivieren
	ADCSRA |= (1 << ADIF) | (1 << ADIE);					// Interrupts aktivieren und Flag setzen
	
	// ADC Starten
	ADCSRA |= 1 << ADSC;
	
	
	/* -----[RGB Initialisieren]----- */
	// Timer0 fuer rotes Licht
	TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0B1);	// Fast-PWM mit TOP=0xFF und COM als non-inverting-Mode
	TCCR0B |= 1 << CS01;
	
	// Timer2 fuer Gruene PWM simulierung
	TIMSK2 |= (1 << TOIE2) | (1 << OCIE2A);
	TCCR2B |= (1 << WGM13) | (1 << WGM12) | (1 << CS12);
	
	
	/* -----[Piesper Initialisieren]----- */
	// Timer1 intialisieren fuer Piepser
	TCCR1A |= (1 << WGM10) | (1 << WGM11) | (1 << COM1A1);
	
	
	/* -----[Richtung Zufall]----- */
	srand(time(NULL));					// Zufaelligen Seed initialisieren
	int randInt = rand() % 2 + 1;		// Zahl zwischen 1 und 2 erzeugen
	if(randInt == 1) direction = 1;		// Abhaengig von der Zahl Richtung festlegen
	else direction = -1;
	
	while (1) {
		// Interrupts vorruebergehend deaktivieren
		SREG &= ~(1 << IBIT);
		
		// Ueberpruefen ob ein Spieler verloren hat
		if(p1Mistakes >= 5 || p2Mistakes >= 5) {
			// LEDs aus
			setLEDOn(0);
			
			// RGB auf Gruen
			OCR0B = 0;
			OCR2A = 120;
			
			// Spieler1 hat verloren
			if(p1Mistakes >= 5 && p2Mistakes < 5) led = 16;

			// Spieler2 hat verloren
			else if (p2Mistakes >= 5 && p1Mistakes < 5) led = 1;
			
			// Unentschieden
			else led = 17;
			
			soundFeedback(true, 500);
			setLEDOn(led);
			
			// Neues Spiel durch betaetigen des "Reset" Knopfes
			while(1);
		}
		
		else {
			// Interrupts aktivieren
			SREG |= (1 << IBIT);
			
			// Leds anschalten
			setLEDOn(led);
			
			// Delay pro Runde weniger
			// 5 LEDs -> Pro LED 5ms weniger -> Pro Runde 25ms weniger
			delayDuration -= 5;
			delay(delayDuration);

			// Je nach Richtung in- / dekrementieren und ueber RGB Fehler anzeigen
			if(direction == 1) {
				led *= 2;
				rgb(p2Mistakes);
			}
			else {
				led /= 2;
				rgb(p1Mistakes);
			}
			
			// Spieler1 hat zu spaet (oder garnicht) gedrueckt
			if (led == 0) {
				// Fehler nur hinzufuegen wenn der Spieler davor nicht gedrueckt hat
				if(!p1Pressed) addMistake(&p1Mistakes);
				
				direction *= -1;
				led = 1;
				
				p1Pressed = false;
			}
			
			// Spieler 2 hat zu spaet (oder garnicht) gedrueckt
			else if (led == 32) {
				// Fehler nur hinzufuegen wenn der Spieler davor nicht gedrueckt hat
				if(!p2Pressed) addMistake(&p2Mistakes);
				
				direction *= -1;
				led = 16;
				
				p2Pressed = false;
			}
		}
	}
}


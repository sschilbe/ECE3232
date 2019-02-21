/**
*
* @file
* gpio_control.h
*
* @brief
* Interface for controlling GPIO module
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 19/02/2019
* Last Modified: 19/02/2019
*/

/*------------------------------------------------------------
INCLUDES
------------------------------------------------------------*/
#include "gpio_control.h"
#include "fsl_device_registers.h"

/*------------------------------------------------------------
MACROS
------------------------------------------------------------*/

/*------------------------------------------------------------
TYPES
------------------------------------------------------------*/

/*------------------------------------------------------------
VARIABLES
------------------------------------------------------------*/

/*------------------------------------------------------------
PROCEDURES
------------------------------------------------------------*/
void gpio_init() {
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTE_MASK;	// enables port B, C, and E

	PORTB_PCR21 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; // drive strength (current) to high
	PORTB_PCR22 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; // drive strength (current) to high
	PORTE_PCR26 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; // drive strength (current) to high

	// setting up GPIO module
	GPIOB_PDDR |= 0x01 << 21; //output enable pin 21 on port B (default value is 0 -> Blue LED turns ON)
	GPIOB_PDDR |= 0x01 << 22; //output enable pin 22 on port B (default value is 0 -> Red LED turns ON)
	GPIOE_PDDR |= 0x01 << 26; //output enable pin 26 on port B (default value is 0 -> Green LED turns ON)

	set_leds();

	PORTC_PCR6 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; // drive strength (current) to high
	GPIOC_PDDR |= 0x00 << 6; //input enable pin 6 on port C (this line doesn't have any effect, default is 0/input)
}

boolean read_switch( int switch_num ) {
	if( switch_num == 2 ) {
		return ( GPIOC_PDIR & ( 0x01 << 6 ) ) >> 6;
	} else if( switch_num == 3 ) {
		return ( GPIOA_PDIR & ( 0x01 << 4 ) ) >> 4;
	}

	return false;
}

void set_leds() {
	for( int i = 0; i < NUMBER_OF_LEDS; i++ ) {
		if( leds[i].state ) {
			if( i == GREEN ) {
				GPIOE_PCOR = 0x01 << leds[i].mask; //set output high (LED off)
			} else {
				GPIOB_PCOR = 0x01 << leds[i].mask; //set output low (LED on)
			}
		} else {
			if( i == GREEN ) {
				GPIOE_PSOR = 0x01 << leds[i].mask; //set output high (LED off)
			} else {
				GPIOB_PSOR = 0x01 << leds[i].mask; //set output high (LED off)
			}
		}
	}
}

void set_led_on( int led ) {
	for( int i = 0; i < NUMBER_OF_LEDS; i++ ) {
		leds[i].state = false; // Turn off all lights
	}

	if( led < NUMBER_OF_LEDS && led >= 0 ) {
		leds[led].state = true;
	}

	set_leds();
}





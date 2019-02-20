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

#ifndef SOURCES_GPIO_CONTROL_H_
#define SOURCES_GPIO_CONTROL_H_

/*------------------------------------------------------------
INCLUDES
------------------------------------------------------------*/
#include "bool.h"

/*------------------------------------------------------------
MACROS
------------------------------------------------------------*/
#define RED 			0
#define GREEN 			1
#define BLUE 			2

#define NUMBER_OF_LEDS 	3

/*------------------------------------------------------------
TYPES
------------------------------------------------------------*/
typedef struct led_type {
	boolean state;
	int 	mask;
}	Led;
/*------------------------------------------------------------
VARIABLES
------------------------------------------------------------*/
// Initially all of the LEDs are set to the off position
// Tracking the current state of the LED allows us to toggle them on and off easier
Led leds[] = {	{false, 0x22},
				{false, 0x21},
				{false, 0x26}
			 }; // Red, Green, Blue

/*------------------------------------------------------------
PROTOTYPES
------------------------------------------------------------*/
/*
 * @brief
 * Initialize the GPIO module for input and output operations
 * The LED is configured as an output and SW2 is configured as input
 * Ports: B, C, E
 * Pins: B21, B22, C6, E26
 */
void gpio_init();

boolean read_switch( int switch_num );

void set_leds();

void set_led_on( int led );

#endif /* SOURCES_GPIO_CONTROL_H_ */

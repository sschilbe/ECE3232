/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *
 * @file
 * main.c
 *
 * @brief
 * Central controlling logic for FRDM-K64F Dodge em' controller
 *
 * Author: Meneley, Julia | Schilbe, Seth
 * Data: 19/02/2019
 */

/*------------------------------------------------------------
 INCLUDES
 ------------------------------------------------------------*/
#include "adc_control.h"
#include "bool.h"
#include "gpio_control.h"
#include <stdlib.h>
#include "uart_control.h"

/*------------------------------------------------------------
 MACROS
 ------------------------------------------------------------*/
#define POWER_SWITCH 2

/*------------------------------------------------------------
 TYPES
 ------------------------------------------------------------*/
typedef struct player_struct {
	int id;
	int lives;
	int starting_lives;
	boolean hit;
} Player;

typedef struct message_struct {
	int msg_id;
	int player_id;
} Message;

/*------------------------------------------------------------
 VARIABLES
 ------------------------------------------------------------*/
boolean power_on = false;
boolean connected = false;
Player player;

/*------------------------------------------------------------
 PROTOTYPES
 ------------------------------------------------------------*/
boolean check_power_switch();

void process_game_message(char * input_message);

/*------------------------------------------------------------
 PROCEDURES
 ------------------------------------------------------------*/

int main(void) {
	// Initialize all modules that are used by the device
	gpio_init();
	uart_init();
	adc_init();
	// To-Do: Init DAC module

	put_string("Test");
	char in[40];
	boolean temp_power;
	for (;;) {
		// If the switch has been pressed toggle the power state
		temp_power = power_on;
		if( ( power_on ^= check_power_switch() ) && !temp_power ) {
			while( get_line( in, '\n' ) ); // Clear any messages that might be in the uart buffer when the device turns on
		}

		if (power_on) {
			if( get_line( in, '\n' ) ) {
				connected = ( in[0] == '0' ) ? false : true;
			}
			if (!connected) {
				set_led_on( RED);

			} else {
				set_led_on( GREEN);

				// Get the current state of the game from the serial port

			}
		} else {
			set_led_on( INVALID_LED); // Turn all LEDS off

			// Let the game know the device has been turned off and disconnect
			if (connected) {
				connected = false;

			}
		}
	}

	return 0;
}

boolean check_power_switch() {
	if (!read_switch( POWER_SWITCH)) {
		//Switch has been pressed, wait until not
		while (!read_switch( POWER_SWITCH));
		return true;
	}

	return false;
}

void process_game_message(char * message) {
	// To-Do: Read a message from the game

	if (player.hit) {
		// The player was hit, we need to let the user know through
		// the speaker on the controller
		// To-Do: Play sound through speaker based on number of lives player has remaining

		player.lives--;
	}

	if (player.lives == 0) {
		// To-Do: Some kind of game over message played through the speaker/LEDS
	}
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

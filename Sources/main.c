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
 * Date Created: 19/02/2019
 * Last Modified: 03/03/2019
 */

/*------------------------------------------------------------
 INCLUDES
 ------------------------------------------------------------*/
#include "adc_control.h"
#include "bool.h"
#include "communication_controller.h"
#include "ftm_control.h"
#include "gpio_control.h"
#include <stdio.h>
#include <string.h>
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

/*------------------------------------------------------------
 VARIABLES
 ------------------------------------------------------------*/
boolean power_on = false;
boolean connected = false;
boolean game_started = false;
Player  player;

/*------------------------------------------------------------
 PROTOTYPES
 ------------------------------------------------------------*/
boolean check_power_switch();

void fill_player_message( esp_msg * message, Player player, int msg_id );

void process_game_message( esp_msg * message );

/*------------------------------------------------------------
 PROCEDURES
 ------------------------------------------------------------*/

int main(void) {
	// Initialize all modules that are used by the device
	gpio_init();
	uart_init();
	adc_init();
	ftm_init();

	boolean temp_power;
	esp_msg message;
	memset( &message, 0, sizeof( message ) );

	for (;;) {
		// If the switch has been pressed toggle the power state
		temp_power = power_on;
		if( ( power_on ^= check_power_switch() ) && !temp_power ) {
			set_led_on( RED );
			while( get_line( NULL, '\n' ) ); // Clear any messages that might be in the uart buffer when the device turns on
		}

		if (power_on) {
			message = get_game_message();
			process_game_message( &message );

			if( game_started ) {
				// Send the accelerometer data to the game
				message.x = -1.0;
				message.y = 0;
				message.z = 0;
				fill_player_message( &message, player, PLAYER_DATA_EVENT );
				send_player_message( &message );
			}
		} else {
			set_led_on( INVALID_LED ); // Turn all LEDS off

			// Let the game know the device has been turned off and disconnect
			if ( connected ) {
				connected = false;
				fill_player_message( &message, player, DISCONNECT_EVENT );
				send_player_message( &message );
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


void process_game_message( esp_msg * message ) {
	esp_msg response;
	memset( &response, 0, sizeof( response ) );

	switch( message->msg_id ) {
		case INVALID_EVENT:
			// We either didn't get an event or the controller received an invalid event
			break;
		case CONNECT_EVENT:
			connected = true;
			player.id = message->player_id;
			player.lives = player.starting_lives = message->lives;

			set_led_on( GREEN );

			fill_player_message( &response, player, CONNECT_EVENT );
			send_player_message( &response );
			break;
		case DISCONNECT_EVENT:
			set_led_on( RED );
			connected = false;
			break;
		case GAME_START_EVENT:
			game_started = true;
			break;
		case PLAYER_HIT_EVENT:
			// The game was reset and we didn't get a message, reset the player accordingly
			if( message->lives >= player.lives ) {
				player.lives = message->lives;
				player.starting_lives = message->lives+1;
			} else {
				player.lives--;
			}

			// Play a tone on the speaker as a ratio of the current lives compared with the starting lives
			buzz( 3000 + (1000 * ( player.starting_lives - 1 - player.lives ) ), 2.0 );
			break;
	}
}

void fill_player_message( esp_msg * message, Player player, int msg_id ) {
	message->msg_id = msg_id;
	message->player_id = player.id;
	message->connected = connected;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

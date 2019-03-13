/**
*
* @file
* communication_controller.c
*
* @brief
* Implementation for controlling communication between controller and game
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 23/02/2019
* Last Modified: 03/03/2019
*/

/*------------------------------------------------------------
INCLUDES
------------------------------------------------------------*/
#include "communication_controller.h"
#include "uart_control.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------
MACROS
------------------------------------------------------------*/

/*------------------------------------------------------------
TYPES
------------------------------------------------------------*/

/*------------------------------------------------------------
VARIABLES
------------------------------------------------------------*/
char in[MESSAGE_BUFFER_SIZE] = {0};
char out[MESSAGE_BUFFER_SIZE] = {0};

/*------------------------------------------------------------
PROCEDURES
------------------------------------------------------------*/
esp_msg get_game_message() {
	esp_msg message;
	message.msg_id = INVALID_EVENT;

	if( get_line( in, '\n' ) ) {
		message.msg_id = 	atoi( &in[0] );
		message.player_id = atoi( &in[2] );
		message.connected = atoi( &in[4] );
		message.hit = 		atoi( &in[6] );
		message.lives = 	atoi( &in[8] );
	}

	return message;
}

void send_player_message( esp_msg * message ) {
	format_player_message( message );
	put_string( out );
}

void format_player_message( esp_msg * message ) {
	snprintf( out, MESSAGE_BUFFER_SIZE, "%d|%d|%d|%d|%d\n", message->msg_id, message->player_id, message->x, message->y, message->z );
}

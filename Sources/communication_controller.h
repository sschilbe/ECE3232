/**
*
* @file
* communication_control.h
*
* @brief
* Interface for controlling communication between controller and game
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 23/02/2019
* Last Modified: 23/02/2019
*/

#ifndef SOURCES_COMMUNICATION_CONTROLLER_H_
#define SOURCES_COMMUNICATION_CONTROLLER_H_

/*------------------------------------------------------------
INCLUDES
------------------------------------------------------------*/
#include "bool.h"
#include <stdint.h>

/*------------------------------------------------------------
MACROS
------------------------------------------------------------*/
#define INVALID_PLAYER_ID 0xFF

/*------------------------------------------------------------
TYPES
------------------------------------------------------------*/
typedef enum {
	CONNECT_EVENT = 0x00, 	// Device has been connected to the game
	DISCONNECT_EVENT,		// Device has been disconnected from the game
	PLAYER_HIT_EVENT,		// Player has been hit in the game
	INVALID_EVENT			// Invalid event
} msg_id;

typedef struct message_type {
	uint16_t	msg_id;
	uint8_t		player_id;
	boolean		connected;
	float		x;
	float 		y;
	float 		z;
} esp_msg; // Embedded Serial Protocol

/*------------------------------------------------------------
VARIABLES
------------------------------------------------------------*/

/*------------------------------------------------------------
PROTOTYPES
------------------------------------------------------------*/
void send_player_message( char * message );

#endif /* SOURCES_COMMUNICATION_CONTROLLER_H_ */

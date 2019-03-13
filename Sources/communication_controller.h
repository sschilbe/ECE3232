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
#define MESSAGE_BUFFER_SIZE 50

/*------------------------------------------------------------
TYPES
------------------------------------------------------------*/
typedef enum {
	CONNECT_EVENT = 0x00, 	// Device has been connected to the game
	DISCONNECT_EVENT,		// Device has been disconnected from the game
	GAME_START_EVENT,		// The game has started
	PLAYER_HIT_EVENT,		// Player has been hit in the game
	PLAYER_DATA_EVENT,		// Player data including acceleration
	INVALID_EVENT			// Invalid event
} msg_id;

typedef struct message_type {
	uint16_t	msg_id;
	uint8_t		player_id;
	boolean		connected;
	boolean 	hit;
	int			lives;
	int			x;
	int 		y;
	int 		z;
} esp_msg; // Embedded Serial Protocol

/*------------------------------------------------------------
VARIABLES
------------------------------------------------------------*/

/*------------------------------------------------------------
PROTOTYPES
------------------------------------------------------------*/
esp_msg get_game_message();

void format_player_message( esp_msg * message );

void send_player_message( esp_msg * message );

#endif /* SOURCES_COMMUNICATION_CONTROLLER_H_ */

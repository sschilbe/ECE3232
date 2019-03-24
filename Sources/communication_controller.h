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
* Last Modified: 24/03/2019
*/

#ifndef SOURCES_COMMUNICATION_CONTROLLER_H_
#define SOURCES_COMMUNICATION_CONTROLLER_H_

/*------------------------------------------------------------
INCLUDES
------------------------------------------------------------*/
#include "bool.h"

/*------------------------------------------------------------
MACROS
------------------------------------------------------------*/
#define INVALID_PLAYER_ID 	( 0xFF )
#define MESSAGE_BUFFER_SIZE ( 50 )

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
	int			msg_id;
	int			player_id;
	boolean		connected;
	boolean 	hit;
	int			lives;
	int			x;
	int			y;
	int	 		z;
} esp_msg; // Embedded Serial Protocol

/*------------------------------------------------------------
VARIABLES
------------------------------------------------------------*/

/*------------------------------------------------------------
PROTOTYPES
------------------------------------------------------------*/
/*
 * @brief
 * Use the uart module to read in a game message from the serial protocol
 *
 * Modules: uart_controller
 * Pins: None
 *
 * Return
 *  -> esp_msg - message to the controller from the UNITY game
 */
esp_msg get_game_message();

/*
 * @brief
 * Use the uart module to write a game message to the serial protocol
 *
 * Modules: uart_controller
 * Pins: None
 *
 * Input
 *  -> esp_msg - message to the UNITY game from the controller
 */
void send_player_message( esp_msg * message );

#endif /* SOURCES_COMMUNICATION_CONTROLLER_H_ */

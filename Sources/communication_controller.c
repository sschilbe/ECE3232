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
* Last Modified: 23/02/2019
*/

/*------------------------------------------------------------
INCLUDES
------------------------------------------------------------*/
#include "communication_controller.h"
#include "uart_control.h"

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
void send_player_message( char * message ) {
	put_string( message );
}




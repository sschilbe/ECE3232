/**
*
* @file
* uart_control.h
*
* @brief
* Interface for controlling UART module for serial interface
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 19/02/2019
* Last Modified: 20/02/2019
*/

#ifndef SOURCES_UART_CONTROL_H_
#define SOURCES_UART_CONTROL_H_

/*------------------------------------------------------------
INCLUDES
------------------------------------------------------------*/
#include "bool.h"

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
PROTOTYPES
------------------------------------------------------------*/
/*
 * @brief
 * Read a single character from the serial communication
 * Modules: UART0
 */
boolean get_char( char * character );

/*
 * @brief
 * Read a line from the serial communication
 * Modules: UART0
 */
boolean get_line( char * string, char final_char );

/*
 * @brief
 * Initialize the UART module for serial communication
 * with baud rate of 9600
 * Ports: B
 * Pins: B16, B17
 */
void uart_init();

/*
 * @brief
 * Write a single character to the serial communication
 * Modules: UART0
 */
void out_char( char * out );

/*
 * @brief
 * Write a string to the serial communication
 * Modules: UART0
 */
void put_string( char * string );

#endif /* SOURCES_UART_CONTROL_H_ */

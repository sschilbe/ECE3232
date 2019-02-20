/**
*
* @file
* uart_control.c
*
* @brief
* Implementation for controlling UART module for serial interface
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 19/02/2019
* Last Modified: 20/02/2019
*/

/*------------------------------------------------------------
INCLUDES
------------------------------------------------------------*/
#include "fsl_device_registers.h"
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
PROTOTYPES
------------------------------------------------------------*/
char get_char() {
	// Poll RDRF until there is a character to be received
	while( !( UART0_S1 & 0x20 ) ) {}

	return (char)UART0_D;
}

void get_line( char * string ) {
	//To-Do: Implement function to get line of arbitrary length
}

void uart_init() {
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; // Enable PORT B clock
	SIM_SCGC4 |= ( 0x01 << 10 ); //Enable UART0 clock

	// Mux the pins
	PORTB_PCR16 |= PORT_PCR_MUX(3);
	PORTB_PCR17 |= PORT_PCR_MUX(3);

	// Baud rate 9600
	UART0_BDH = 0x00;
	UART0_BDL = 0x88;

	// Enable input and output
	UART0_C2 |= 0x0C;
}

void out_char( char * out ) {
	while( !( UART0_S1 & 0x80 ) ); // Wait for buffer

	UART0_D = *out;

	while( !( UART0_S1 & 0x40 ) ); // Wait for transmit to finish
}

void put_string( char * string ) {
	while( *string++ ) {
		out_char( string );
	}
}

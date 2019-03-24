/**
*
* @file
* dac_control.h
*
* @brief
* Interface for controlling digital to analog conversion
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 21/03/2019
* Last Modified: 24/03/2019
*/

#ifndef SOURCES_DAC_CONTROL_H_
#define SOURCES_DAC_CONTROL_H_

/*------------------------------------------------------------
INCLUDES
------------------------------------------------------------*/

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
 * Initialize the DAC module for outputting analog data
 * Must be called before any other functions in this module are called
 *
 * Modules: DAC0
 * Pins: By default the DAC goes to the header pin
 */
void dac_init( void );

/*
 * @brief
 * Read the value of all configured acceleration pins one at a time
 * Modules: DAC0
 * Pins: None
 *
 * Input
 *  -> V_out - output voltage to be sent out of the dac
 */
void set_DAC( float V_out );

#endif /* SOURCES_DAC_CONTROL_H_ */

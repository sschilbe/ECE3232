/**
*
* @file
* acceleration_control.h
*
* @brief
* Interface for controlling acceleration data
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 15/03/2019
* Last Modified: 24/03/2019
*/

#ifndef SOURCES_ACCELERATION_CONTROL_H_
#define SOURCES_ACCELERATION_CONTROL_H_

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
 * Initialize the necessary modules for reading in acceleration data
 * Must be called before any other functions in this module are called
 *
 * Modules: ADC0, ADC1, PORTB
 * Pins: B3, B10, B11 -> Set as adc inputs
 */
void acceleration_init();

/*
 * @brief
 * Set a baseline for the acceleration data, this assumes that the
 * device is currently laying at rest while this function is running
 *
 * Modules: None
 * Pins: None
 */
void configure_acceleration();

/*
 * @brief
 * Read the value of all configured acceleration pins one at a time
 * Modules: ADC0, ADC1
 * Pins: None
 */
void read_acceleration_data();

/*
 * @brief
 * Returns the average acceleration values for all configured axis
 * Modules: None
 * Pins: None
 *
 * Return
 * 	-> average_acceleration - Array with float values for average acceleration
 */
float * get_acceleration_data();

/*
 * @brief
 * Returns the average acceleration values for all configured axis in a format
 * that is acceptable to unity
 * Modules: None
 * Pins: None
 *
 * Return
 * 	-> average_acceleration - Array with float values for unity acceleration
 */
float * get_unity_acceleration();

#endif /* SOURCES_ACCELERATION_CONTROL_H_ */

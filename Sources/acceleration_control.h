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
* Last Modified: 15/03/2019
*/

#ifndef SOURCES_ACCELERATION_CONTROL_H_
#define SOURCES_ACCELERATION_CONTROL_H_

/*------------------------------------------------------------
INCLUDES
------------------------------------------------------------*/

/*------------------------------------------------------------
MACROS
------------------------------------------------------------*/
#define NUMBER_OF_AXIS				3
#define MAX_ACCELERATION_READINGS 	100

/*------------------------------------------------------------
TYPES
------------------------------------------------------------*/

/*------------------------------------------------------------
VARIABLES
------------------------------------------------------------*/

/*------------------------------------------------------------
PROTOTYPES
------------------------------------------------------------*/
// To-Do: Document
void acceleration_init();

// To-Do: Document
void configure_acceleration();

// To-Do: Document
void read_acceleration_data();

// To-Do: Document
float * get_acceleration_data();

// To-Do: Document
float * get_unity_acceleration();

#endif /* SOURCES_ACCELERATION_CONTROL_H_ */

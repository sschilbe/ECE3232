/**
*
* @file
* ftmcontrol.h
*
* @brief
* Interface for controlling FTM module
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 03/03/2019
* Last Modified: 03/03/2019
*/

#ifndef SOURCES_FTM_CONTROL_H_
#define SOURCES_FTM_CONTROL_H_

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
void FTM2_IRQHandler( void );

/*
 * @brief
 * Initialize the FTM module, no clock is selected so no wave
 * will be output until the buzz function is called
 * Ports: B, FTM2
 * Pins: B19
 */
void ftm_init(void);


void myDelay( float time );

/*
 * @brief
 * This function will accept a voltage level (V) frequency (Hz) and an amount of time (sec) for this frequency to play.
 * 	Inputs:
 * 		V_out -> The maximum voltage that should be outputted on the sine wave
 * 		freq -> The frequency of wave to be sent to the buzzer. Must be within 3000 and 5000.
 * 		time -> The time that this frequency should be played for. Must be within 0 and 10 sec.
 *
 * Ports: B, FTM2
 * Pins: B19
 */
void buzz( float V_out, float freq, float time );

#endif /* SOURCES_FTM_CONTROL_H_ */


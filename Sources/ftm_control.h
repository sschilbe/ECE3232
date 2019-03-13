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
/*
 * @brief
 * Initialize the FTM module, no clock is selected so no wave
 * will be output until the buzz function is called
 * Ports: B, FTM2
 * Pins: B19
 */
void ftm_init(void);

/*
 * @brief
 * This function will accept a frequency (Hz) and an amount of time (sec) for this frequency to play.
 * 	Inputs:
 * 		f_set -> The frequency of square wave to be sent to the buzzer. Must be within 3000 and 5000.
 * 		t_set -> The time that this frequency should be played for. Must be within 0 and 10 sec.
 *
 * Ports: B, FTM2
 * Pins: B19
 */
void buzz(int f_set, float t_set);

#endif /* SOURCES_FTM_CONTROL_H_ */


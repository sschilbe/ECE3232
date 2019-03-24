/**
*
* @file
* ftm_control.h
*
* @brief
* Interface for controlling FTM module
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 03/03/2019
* Last Modified: 24/03/2019
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
/* Interrupt routine for FTM1 */
void FTM1_IRQHandler( void );

/* Interrupt routine for FTM2 */
void FTM2_IRQHandler( void );

/*
 * @brief
 * Initialize the FTM1 module to track time
 * Ports: FTM1
 */
void FTM1_init( void );

/*
 * @brief
 * Initialize the FTM2 module, no clock is selected so no wave
 * will be output until the buzz function is called
 *
 * Inputs:
 * 		mod -> the value to be placed into the MOD register for the timer
 *
 * Ports: B, FTM2
 * Pins: B19
 */
void FTM2_init( int mod );

/*
 * @brief
 * Delay function that blocks the execution of any other functions
 *
 * Modules: FTM1
 * Pins: None
 */
void my_blocking_delay( float time );

/*
 * @brief
 * Setup for sending multiple tones over the DAC
 * Modules: None
 * Pins: None
 */
void play_buzzer( float *v, float *f, float *t, int N );

/*
 * @brief
 * Reset the lost life count to the maximum value
 * Modules: None
 * Pins: None
 */
void reset_lost_life_count();

/*
 * @brief
 * Get the current value of the lost life count
 * Modules: None
 * Pins: None
 */
int get_lost_life_count();

#endif /* SOURCES_FTM_CONTROL_H_ */


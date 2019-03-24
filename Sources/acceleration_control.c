/**
*
* @file
* acceleration_control.c
*
* @brief
* Implementation for controlling acceleration data
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 15/03/2019
* Last Modified: 24/03/2019
*/


/*------------------------------------------------------------
INCLUDES
------------------------------------------------------------*/
#include "acceleration_control.h"
#include "fsl_device_registers.h"

/*------------------------------------------------------------
MACROS
------------------------------------------------------------*/
#define X_AXIS 						( 0 )
#define Y_AXIS 						( 1 )
#define Z_AXIS 						( 2 )

#define ORIGINAL_MIN 				( 0 )
#define ORIGINAL_MAX 				( 0xFFFF )
#define SCALED_MIN 					( 0.0 )
#define SCALED_MAX					( 3.3 )
#define UNITY_MIN 					( -1.0 )
#define UNITY_MAX					( 1.0 )

#define NUMBER_OF_AXIS				( 3 )
#define MAX_ACCELERATION_READINGS 	( 5 )

/*------------------------------------------------------------
TYPES
------------------------------------------------------------*/

/*------------------------------------------------------------
VARIABLES
------------------------------------------------------------*/
int acceleration_readings = 0;
float acceleration_data[MAX_ACCELERATION_READINGS][NUMBER_OF_AXIS];
float average_acceleration[NUMBER_OF_AXIS];
float zero_acceleration[NUMBER_OF_AXIS];

/*------------------------------------------------------------
PROCEDURES
------------------------------------------------------------*/
/* HELPER */
/*
 * @brief
 * Linearly scales an original int to a float value inbetween a scale range
 * Modules: None
 * Pins: None
 */
float int_acceleration_scale( int x, int original_min, int original_max, float scaled_min, float scaled_max );

/*
 * @brief
 * Linearly scales an original float to a float value inbetween a scale range
 * Modules: None
 * Pins: None
 */
float float_acceleration_scale( float x, float original_min, float original_max, float scaled_min, float scaled_max );

/*
 * @brief
 * acceleration_init(), see acceleration_control.h for more info
 */
void acceleration_init() {
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK ; /* Enable the ADC0 Clock */
	SIM_SCGC3 |= SIM_SCGC3_ADC1_MASK; /* Enable the ADC1 Clock */
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; /* Enable the PORTB Clock */

	ADC0_CFG1 |= ADC_CFG1_MODE(3); /* 16 bit ADC */
	ADC1_CFG1 |= ADC_CFG1_MODE(3); /* 16 bit ADC */

	ADC0_CFG1 |= ADC_CFG1_ADIV(3); /* Clock divide 8 */
	ADC1_CFG1 |= ADC_CFG1_ADIV(3); /* Clock divide 8 */

	// Mux the ADC to the pins on PORT B
	PORTB_PCR3 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; // drive strength (current) to high
	PORTB_PCR10 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; // drive strength (current) to high
	PORTB_PCR11 |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; // drive strength (current) to high

	ADC0_SC3 |= 0x01 << 7; // Start calibration
	while ( ADC0_SC3 & (0x01 << 7) ){};

	// Make sure calibration was successful
	if ( ADC0_SC3 & (0x01 << 6) ) {
		// To-Do: Throw an error?
		acceleration_readings++;
	}

	ADC1_SC3 |= 0x01 << 7; // Start calibration
	while ( ADC1_SC3 & (0x01 << 7) ){};

	// Make sure calibration was successful
	if ( ADC1_SC3 & (0x01 << 6) ) {
		// To-Do: Throw an error?
		acceleration_readings++;
	}

	ADC0_SC3 |= 0x111; // Turn on hardware averaging with 32 samples
	ADC1_SC3 |= 0x111; // Turn on hardware averaging with 32 samples
}

/*
 * @brief
 * configure_acceleration(), see acceleration_control.h for more info
 */
void configure_acceleration() {
	float * config_acceleration;
	for( int i = 0; i < MAX_ACCELERATION_READINGS - 1; i++ ) {
		read_acceleration_data();
	}

	config_acceleration = get_acceleration_data();
	zero_acceleration[X_AXIS] = config_acceleration[X_AXIS]; // X-axis zero value
	zero_acceleration[Y_AXIS] = config_acceleration[Y_AXIS]; // Y-axis zero value
	zero_acceleration[Z_AXIS] = config_acceleration[Z_AXIS] - 0.3; // Z-axis zero value (have to take off offset because z = 1 at rest )
}

/*
 * @brief
 * read_acceleration_data(), see acceleration_control.h for more info
 */
void read_acceleration_data() {
	int x_accel, y_accel, z_accel;

	ADC0_SC1A = 0b01101 & ADC_SC1_ADCH_MASK; 		// Write to SC1A to start conversion of SE13 on PTB3
	while( ADC0_SC2 & ADC_SC2_ADACT_MASK ); 		// Conversion in progress
	while( !( ADC0_SC1A & ADC_SC1_COCO_MASK ) ); 	// Wait until conversion complete
	z_accel = ADC0_RA;

	ADC1_SC1A = 0b01110 & ADC_SC1_ADCH_MASK; 		// Write to SC1A to start conversion of SE14 on PTB10
	while( ADC1_SC2 & ADC_SC2_ADACT_MASK ); 		// Conversion in progress
	while( !( ADC1_SC1A & ADC_SC1_COCO_MASK ) ); 	// Wait until conversion complete
	y_accel = ADC1_RA;

	ADC1_SC1A = 0b01111 & ADC_SC1_ADCH_MASK; 		// Write to SC1A to start conversion of SE15 on PTB11
	while( ADC1_SC2 & ADC_SC2_ADACT_MASK ); 		// Conversion in progress
	while( !( ADC1_SC1A & ADC_SC1_COCO_MASK ) ); 	// Wait until conversion complete
	x_accel = ADC1_RA;

	/*
	 * After some filtering we know that the minimum value will be 0 and the max value will be 0xFFFF
	 * The final acceleration data should be in a range of -1 to 1, this is easiest because this is
	 * the format that UNITY expects movement values to be in. Therefore we must scale the acceleration
	 * values:
	 * 0 - 0xFFFF -> -1 - 1
	 */
	acceleration_data[acceleration_readings][X_AXIS] = int_acceleration_scale( x_accel, ORIGINAL_MIN, ORIGINAL_MAX, SCALED_MIN, SCALED_MAX ) - zero_acceleration[X_AXIS];
	acceleration_data[acceleration_readings][Y_AXIS] = int_acceleration_scale( y_accel, ORIGINAL_MIN, ORIGINAL_MAX, SCALED_MIN, SCALED_MAX ) - zero_acceleration[Y_AXIS];
	acceleration_data[acceleration_readings][Z_AXIS] = int_acceleration_scale( z_accel, ORIGINAL_MIN, ORIGINAL_MAX, SCALED_MIN, SCALED_MAX ) - zero_acceleration[Z_AXIS];
	acceleration_readings++;

	if( acceleration_readings == MAX_ACCELERATION_READINGS ) {
		acceleration_readings = 0;
	}
}

/*
 * @brief
 * get_acceleration_data(), see acceleration_control.h for more info
 */
float * get_acceleration_data() {
	// Add up all the acceleration values
	for( int j = 0; j < NUMBER_OF_AXIS; j++ ) {
		average_acceleration[j] = 0;
	}

	for( int i = 0; i < acceleration_readings; i++ ) {
		for( int j = 0; j < NUMBER_OF_AXIS; j++ ) {
			average_acceleration[j] += acceleration_data[i][j];
		}
	}

	// Average the accelerations
	for( int i = 0; i < NUMBER_OF_AXIS; i++ ) {
		average_acceleration[i] /= acceleration_readings;
	}

	acceleration_readings = 0;

	return average_acceleration;
}

/*
 * @brief
 * get_unity_acceleration(), see acceleration_control.h for more info
 */
float * get_unity_acceleration() {
	float * voltage_levels = get_acceleration_data();
	voltage_levels[X_AXIS] = float_acceleration_scale( voltage_levels[X_AXIS], -0.3, 0.3, UNITY_MIN, UNITY_MAX );
	voltage_levels[Y_AXIS] = float_acceleration_scale( voltage_levels[Y_AXIS], -0.3, 0.3, UNITY_MIN, UNITY_MAX );
	voltage_levels[Z_AXIS] = float_acceleration_scale( voltage_levels[Z_AXIS], -0.3, 0.3, UNITY_MIN, UNITY_MAX );

	return voltage_levels;
}

float int_acceleration_scale( int x, int original_min, int original_max, float scaled_min, float scaled_max ) {
	/*
	 * This function must perform the following conversion:
	 *         (SCALED_MAX - SCALED_MIN)(x - ORIGINAL_MIN)
	 *  f(x) = -------------------------------------------  + SCALED_MIN
     *            	   ORIGINAL_MAX - ORIGINAL_MIN
	 *
	 * This formula essentially scales an original distribution of values from
	 * ORIGINAL_MIN - ORIGINAL_MAX -> SCALED_MIN - SCALED_MAX in a linear fashion
	 * while preserving distribution
	 */
	return ( (scaled_max - scaled_min) * ( x - original_min ) ) / ( original_max - original_min ) + scaled_min;
}

float float_acceleration_scale( float x, float original_min, float original_max, float scaled_min, float scaled_max ) {
	/*
	 * This function must perform the following conversion:
	 *         (SCALED_MAX - SCALED_MIN)(x - ORIGINAL_MIN)
	 *  f(x) = -------------------------------------------  + SCALED_MIN
     *            	   ORIGINAL_MAX - ORIGINAL_MIN
	 *
	 * This formula essentially scales an original distribution of values from
	 * ORIGINAL_MIN - ORIGINAL_MAX -> SCALED_MIN - SCALED_MAX in a linear fashion
	 * while preserving distribution
	 */
	return ( (scaled_max - scaled_min) * ( x - original_min ) ) / ( original_max - original_min ) + scaled_min;
}



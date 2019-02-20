/**
*
* @file
* adc_control.c
*
* @brief
* Implementation for controlling ADC module
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 19/02/2019
* Last Modified: 20/02/2019
*/


/*------------------------------------------------------------
INCLUDES
------------------------------------------------------------*/
#include "adc_control.h"
#include "fsl_device_registers.h"

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

void adc_init() {
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; 	// Enable the ADC0 Clock
	ADC0_CFG1 |= ADC_CFG1_MODE(3); 		// 16 bit ADC
}

unsigned short adc_read() {
	ADC0_SC1A = 0x1A & ADC_SC1_ADCH_MASK; 		// Write to SC1A to start conversion of internal temperature sensor

	while( ADC0_SC2 & ADC_SC2_ADACT_MASK ); 		// Conversion in progress

	while( !( ADC0_SC1A & ADC_SC1_COCO_MASK ) ); 	// Wait until conversion complete
	return ADC0_RA;
}

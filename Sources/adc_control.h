/**
*
* @file
* adc_control.h
*
* @brief
* Interface for controlling ADC module
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 19/02/2019
* Last Modified: 20/02/2019
*/

#ifndef SOURCES_ADC_CONTROL_H_
#define SOURCES_ADC_CONTROL_H_

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
 * Initialize the ADC module for analog to digital conversion
 * Modules: ADC0
 */
void adc_init();

/*
 * @brief
 * Initialize the ADC module for analog to digital conversion
 * Modules: ADC0
 */
unsigned short adc_read();

#endif /* SOURCES_ADC_CONTROL_H_ */

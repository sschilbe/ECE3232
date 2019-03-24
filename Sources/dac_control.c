/**
*
* @file
* dac_control.c
*
* @brief
* Implementation for controlling DAC module
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 21/03/2019
* Last Modified: 24/03/2019
*/

/*------------------------------------------------------------
INCLUDES
------------------------------------------------------------*/
#include "dac_control.h"
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
PROCEDURES
------------------------------------------------------------*/
/*
 * @brief
 * dac_init(), see dac_control.h for more info
 */
void dac_init() {
	// enable DAC0 with system clock
	SIM_SCGC2 |= SIM_SCGC2_DAC0_MASK;

	// by default DAC0_OUT goes to the header --> no MUXing required
	// not using the buffer
	// use the default reference voltage (assumed 3.3V)

	// enable the DAC system
	DAC0_C0 |= DAC_C0_DACEN_MASK;
	// select the reference voltage
	DAC0_C0 |= (0x01 << 6);

	DAC0_C1 |= DAC_C1_DACBFEN_MASK;
	DAC0_C1 |= DAC_C1_DACBFMD(0);
}

/*
 * @brief
 * my_blocking_delay(), see dac_control.h for more info
 */
void my_blocking_delay( void ) {
	// enable DAC0 with system clock
	SIM_SCGC2 |= SIM_SCGC2_DAC0_MASK;

	// by default DAC0_OUT goes to the header --> no MUXing required
	// not using the buffer
	// use the default reference voltage (assumed 3.3V)

	// enable the DAC system
	DAC0_C0 |= DAC_C0_DACEN_MASK;
	// select the reference voltage
	DAC0_C0 |= (0x01 << 6);

	DAC0_C1 |= DAC_C1_DACBFEN_MASK;
	DAC0_C1 |= DAC_C1_DACBFMD(0);
}

/*
 * @brief
 * set_dac(), see dac_control.h for more info
 */
void set_DAC( float V_out ) {
	float V_ref = 3.3;

	// prevents the 'val' from being negative
	if( V_out < ( V_ref * ( 1.0 / 4096.0 ) ) ) {
		V_out = ( V_ref * ( 1.0 / 4096.0 ) );
	}
	// prevents val from being greater than 0xFFF
	else if( V_out > V_ref ){
		V_out = V_ref;
	}

	/*
	 * Find out the value to set into the DACx_DAT0[11:0] to produce a high voltage at the specified value.
	 * From page 911 of sub-family reference manual:
	 *
	 * 		V_out = V_ref * (1 + DACx_DATA0[11:0])/4096  --> DACx_DATA0[11:0] = V_out*V_ref*4096 - 1
	 *
	 * The minimum value that can be produced is 0.806 mV, the maximum is 3.3 V and the precision is 0.806 mV.
	 * This is good enough for the functions it will be used to perform.
	 */

	int val = ( V_out / V_ref ) * ( 4096 - 1 );


	// I'm not sure if this will work...
	DAC0_DAT0L = val & ( 0x0FF );
	DAC0_DAT0H = ( val & 0xF00 ) >> 8;

	// Apparently it does... :)

	// enable the DAC system
	DAC0_C0 |= DAC_C0_DACEN_MASK;
}

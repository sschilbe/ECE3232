/**
*
* @file
* dac_control.c
*
* @brief
* Implementation for controlling DAC module
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 03/21/2019
* Last Modified: 03/21/2019
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
void dac_init( void ) {
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


void setDAC( float V_out ) {

	/*
	 * Inputs:
	 * 		V_out - the voltage of the 'high' part of the square wave. This should also be the peak to peak value.
	 */
	float V_ref = 3.3;

	// prevents the 'val' from being negative
	if(V_out < (V_ref * (1.0/4096.0))){
		V_out = (V_ref * (1.0/4096.0));
	}
	// prevents val from being greater than 0xFFF
	else if	(V_out > V_ref){
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

	int val = (V_out / V_ref) * 4096 - 1;


	// I'm not sure if this will work...
	DAC0_DAT0L = val & (0x0FF);
	DAC0_DAT0H = (val & 0xF00)>>8;
	/*
	DAC0_DAT1L = val & (0x0FF);
	DAC0_DAT1H = val & ((0xF00)>>8);
	DAC0_DAT2L = val & (0x0FF);
	DAC0_DAT2H = val & ((0xF00)>>8);
	DAC0_DAT3L = val & (0x0FF);
	DAC0_DAT3H = val & ((0xF00)>>8);
	DAC0_DAT4L = val & (0x0FF);
	DAC0_DAT4H = val & ((0xF00)>>8);
	DAC0_DAT5L = val & (0x0FF);
	DAC0_DAT5H = val & ((0xF00)>>8);
	DAC0_DAT6L = val & (0x0FF);
	DAC0_DAT6H = val & ((0xF00)>>8);
	DAC0_DAT7L = val & (0x0FF);
	DAC0_DAT7H = val & ((0xF00)>>8);
	DAC0_DAT8L = val & (0x0FF);
	DAC0_DAT8H = val & ((0xF00)>>8);
	DAC0_DAT9L = val & (0x0FF);
	DAC0_DAT9H = val & ((0xF00)>>8);
	DAC0_DAT10L = val & (0x0FF);
	DAC0_DAT10H = val & ((0xF00)>>8);
	DAC0_DAT11L = val & (0x0FF);
	DAC0_DAT11H = val & ((0xF00)>>8);
	DAC0_DAT12L = val & (0x0FF);
	DAC0_DAT12H = val & ((0xF00)>>8);
	DAC0_DAT13L = val & (0x0FF);
	DAC0_DAT13H = val & ((0xF00)>>8);
	DAC0_DAT14L = val & (0x0FF);
	DAC0_DAT14H = val & ((0xF00)>>8);
	DAC0_DAT15L = val & (0x0FF);
	DAC0_DAT15H = val & ((0xF00)>>8);
	 */

	// enable the DAC system
	DAC0_C0 |= DAC_C0_DACEN_MASK;
}

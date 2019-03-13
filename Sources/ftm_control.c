/**
*
* @file
* ftm_control.c
*
* @brief
* Implementation for controlling FTM module
*
* Author: Meneley, Julia | Schilbe, Seth
* Created on: 03/03/2019
* Last Modified: 03/03/2019
*/

/*------------------------------------------------------------
INCLUDES
------------------------------------------------------------*/
#include "ftm_control.h"
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
void ftm_init( void ) {
	// NO CLOCK IS SELECTED FOR FTM2_SC_CLKS SO THE SQUARE WAVE WILL NOT START UNTIL THE buzz() FUNCTION IS CALLED UPON

	/*
	 * We want to produce a 50% duty cycle square wave with an amplitude which will be determined by the ADC somehow (cross that bridge when you get to it). Use either the
	 * prescale factor or the modulo value to determine the frequency. It will probably be easier to use the modulo value. For 50% duty cycle, we can just toggle every time
	 * the counter gets to the final value. This means the time required to get to the final value should be 1/2 of the period.
	 */
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;	// enables port B clock
	SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK; 	// Enable the FTM2 Clock
	PORTB_PCR18 |= PORT_PCR_MUX( 3 ); // MUX port B19 to FTM2_CH0

	/*
	 * status and control reg 1 (default -> all zero)
	 * bit7: (TOF) timer overflow flag (0 -> FTM counter has not overflowed, 1 -> FTM counter has overflowed)
	 * bit6: (TOIE) timer overflow interrupt enabled (0 -> disable, default)
	 * bit4-3: (CLKS) clock source selection (00 -> no clock, 01 -> system clock, 10 -> fixed frequency clock, 11 -> external cock)
	 * bit2-0: prescale factor (000 -> divide by 1) write protected therefore can only overwrite when MODE[WPDIS]=1
	 */

	FTM2_SC |= ( 0b000 << 0 ); // selects prescale factor of 1

	FTM2_MODE |= 0x01; // enable all registers in the FTM2 module with no restrcitions

	FTM2_MOD = 0xA41; // should give a 4 kHz square wave (based off of the system clock being 21 MHz)
	FTM2_C0V = 0x0; // this is the value that the count value will be compared to

	/*
	 * Use table 40-67 to determine the values in the FTMx_CnSC register to get 'Toggle Output on Match'
	 */
	FTM2_C0SC = ( 0b0101 << 2 ); // MSnB,MSnA,ELSnB,ELSnA

	// Initial value of output channel
	FTM2_OUTINIT |= 0x00; // default is all zeros, this doesn't actually change anything
	FTM2_CNTIN = 0x00;

	// FTMx_OUTMASK -> default is all unmasked, leave it this way

	// FTMx_CONF -> configure the number of times the FTM counter should overflow before TOF flag is set. default is
	// 0 which will set the flag each time the counter overflows. When there is a match on channel 5, reset the counter of channel 5 to zero
}

void buzz( int f_set, float t_set ) {
	float f_clk, T_set, time_to_overflow;
	int counts_in_period, num_of_overflows;

	// Handle the Inputs
	if( f_set < 3000 ) {
		f_set = 3000;
	} else if( f_set > 5000 ) {
		f_set = 5000;
	}
	if( t_set < 0 ) {
		t_set = 0;
	} else if( t_set > 10 ){
		t_set = 10;
	}

	// Frequency Calculations
	f_clk = 21000000; // module clock is 21 MHz
	T_set = 1.0 / f_set; // period of desired square wave (sec)
	counts_in_period = ( T_set * f_clk ) / 2;

	FTM2_MOD |= counts_in_period;

	// Time Calculations
	// How long will it take for the counter to reach its final value? This should be half of the intended period of the square wave.
	time_to_overflow = 0.5 * ( 1.0 / f_set );
	// how many overflows should occur before this amount of time has passed?
	num_of_overflows = t_set / time_to_overflow;
	// make this number even so it will start with the output being low, and end that way too
	if( !( num_of_overflows % 2 == 0 ) ){
		num_of_overflows++;
	}

	FTM2_SC |= (0b1000); // selects the system clock

	while( num_of_overflows ) {
		if( FTM2_SC & ( 0b1 << 7 ) ) {
			num_of_overflows--; // decrement the number of overflows
			FTM2_SC &= !( 0b1 << 7 ); // reset the flag back to 0
		}
		FTM2_SC |= (0b1000); // selects the system clock
	}

	FTM2_SC &= !( 0b11 << 3 ); // selects no clock therefore disables channel

	//FTM2_OUTINIT |= 0x00; // default is all zeros, this doesn't actually change anything
}

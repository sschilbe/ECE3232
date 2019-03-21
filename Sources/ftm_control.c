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
#include "dac_control.h"
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
static int mod = 2100 / 4; // mod to allow for a shorter delay

/*------------------------------------------------------------
PROCEDURES
------------------------------------------------------------*/
void FTM2_IRQHandler( void ) {
	mod++; // Dummy line so that a breakpoint can be set here
}

void ftm_init( void ) {
	dac_init();

	// NO CLOCK IS SELECTED FOR FTM2_SC_CLKS SO THE SQUARE WAVE WILL NOT START UNTIL THE buzz() FUNCTION IS CALLED UPON

	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;	// enables port B clock
	SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK; 	// Enable the FTM2 Clock
	PORTB_PCR18 |= PORT_PCR_MUX(3); // MUX port B19 to FTM2_CH0

	//Turn Write Protection Off
	FTM2_MODE |= FTM_MODE_WPDIS_MASK;

	FTM2_SC |= (0b000 << 0); // selects pre-scale factor of 1 (default)
	FTM2_SC |= (0b1 << 6 ); // Enable timer overflow interrupt

	FTM2_CNTIN = 0x00; // Initial value of counter = 0
	FTM2_MOD = mod;

	//Set MSB=1 and ELSnB=1
	// This is output compare and will toggle the channel value each time the counter reaches the value specified in FTM0_C0V
	// (doesn't really matter in this case sense we just want to know when there is a overflow)
	FTM2_C0SC |= 0x28;

	NVIC_EnableIRQ( FTM2_IRQn );
}

void myDelay( float time ) {

	/*
	 * This is an altered version of the code submitted for ECE3232 assignment 3. Enter a time of 0.000125 to result in a squarewave of
	 * frequency 4 kHz.
	 *
	 */

	int count = 0;

	float f_clk, t_over;
	int num_over;

	//f_clk = 21000000; // module clock is 21 MHz
	//t_over = 0xFFFF / f_clk; // time to reach one overflow (0.003121 sec)
	//num_over = time / t_over ; // how many overflows need to be reached?

	num_over = (time *21000000)/mod ; // how many overflows need to be reached?

	FTM2_SC |= (0b1000); // selects the system clock

	while(num_over){
		while( !(FTM2_SC & (0b1 << 7)) ){}// wait until the TOF (timer overflow) flag is set to 1
		num_over--; // decrement the number of overflows
		FTM2_SC &= !(0b1 << 7); // reset the flag back to 0
		FTM2_SC |= (0b1000); // selects the system clock
	}

	FTM2_SC &= !(0b11 << 3); // selects no clock therefore disables channel

}

void buzz( float V_out, float freq, float time ) {
	/*
	 * Inputs:
	 * 		V_out - the voltage of the 'high' part of the square wave. This should also be the peak to peak value.
	 * 		freq - the frequency of the square wave to be sent limited to 1.5 kHz to 4.5 kHz
	 * 		time - the amount of time (seconds) the buzzer will play for limited to 0 to 10 seconds
	 */

	// deal with frequency inputs (V_out will be handles in setDAC)
	if(freq > 4500){
		freq = 4500;
	}
	else if(freq < 1500){
		freq = 1500;
	}

	// deal with frequency inputs (V_out will be handles in setDAC)
	if(time > 10){
		time = 10;
	}
	else if(time < 0){
		time = 0;
	}

	// find the time required to delay for the given frequency
	float t_delay = 1/(2*freq);

	// find out how many periods should be completed
	int count = time * freq;


	while(count){
	// This loop will take approximately 1/freq seconds to complete

		// set the value that the DAC will output to the 'high' value
		setDAC(V_out);

		// delay the program
		myDelay(t_delay);

		// set the value that the DAC will output to the 'low' value
		setDAC(0); // won't actually be zero (0.806 mV), but hopefully close enough

		// delay the program
		myDelay(t_delay);

		count--;
	}
}

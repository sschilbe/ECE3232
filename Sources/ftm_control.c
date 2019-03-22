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
#define BLOCKING_MODULO	( 2100 / 4 )  // Small enough mod to give 0.026ms transparency
#define SYSTEM_FREQ		( 20960000 )  // Frequency of the system clock 20.96 MHz
#define MAX_VALUE		( 0xFFFF )

/*------------------------------------------------------------
TYPES
------------------------------------------------------------*/

/*------------------------------------------------------------
VARIABLES
------------------------------------------------------------*/
int delay_count = 0;
int buzzer_overflows = 0;
float high_voltage = 0;

/*------------------------------------------------------------
PROCEDURES
------------------------------------------------------------*/
/* Interrupt callback for handling DAC output */
void FTM1_IRQHandler( void ) {
	delay_count--;
	if(!delay_count) {
		// no more overflows needed --> turn off overflows then turn off FTM 16-bit counter
		NVIC_DisableIRQ( FTM1_IRQn );
		FTM1_SC &= ~(0b1 << 6 ); // Disable timer overflow interrupt
		FTM1_SC &= ~(0b1000); // de-selects the system clock from FTM counter
	}

	// set the timer overflow flag to 0 to clear interrupt.
	FTM1_SC &= ~FTM_SC_TOF_MASK;
}

/* Interrupt callback for handling DAC output */
void FTM2_IRQHandler( void ) {
	// Check the current value of the DAC and invert (low->high) or (high->low)
	( (DAC0_DAT0L & 0x0FF) | (DAC0_DAT0H & 0x0F)<<8 ) ? setDAC(0) : setDAC(high_voltage);
	buzzer_overflows--;

	if (!buzzer_overflows){
		// no more overflows needed --> turn off overflows then turn off FTM 16-bit counter
		NVIC_DisableIRQ( FTM2_IRQn );
		FTM2_SC &= ~(0b1 << 6 ); // Disable timer overflow interrupt
		FTM2_SC &= ~(0b1000); // de-selects the system clock from FTM counter

		setDAC(0); // make sure that the output is low

		DAC0_C0 &= ~DAC_C0_DACEN_MASK; // disable the DAC system
	}

	// set the timer overflow flag to 0 to clear interrupt.
	FTM2_SC &= ~FTM_SC_TOF_MASK;
}

void FTM1_init( void ) {
	SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK; 	// Enable the FTM1 Clock

	//Turn Write Protection Off
	FTM1_MODE |= FTM_MODE_WPDIS_MASK;

	FTM1_SC |= (0b000 << 0); // selects pre-scale factor of 1 (default)
	FTM1_SC |= (0b1 << 6 ); // Enable timer overflow interrupt

	FTM1_CNTIN = 0x00; // Initial value of counter = 0
	FTM1_MOD = BLOCKING_MODULO;

	//Set MSB=1 and ELSnB=1
	// This is output compare and will toggle the channel value each time the counter reaches the value specified in FTM0_C0V
	// (doesn't really matter in this case sense we just want to know when there is a overflow)
	FTM1_C0SC |= 0x28;

	NVIC_EnableIRQ( FTM1_IRQn );
}

void FTM2_init( int mod ) {
	NVIC_DisableIRQ( FTM2_IRQn ); // Disable interrupts if they are currently enabled

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

void myBlockingDelay( float time ) {

	/*
	 * Blocking function that delays all execution for the amount of time given as an input
	 */
	delay_count = (time * SYSTEM_FREQ) / BLOCKING_MODULO; // how many overflows need to be reached?
	FTM1_init();

	FTM1_SC |= (0b1000); // selects the system clock

	// Decremented by the interrupt, will exit after the correct amount of time
	while( delay_count ) {}
}

void buzz( float V_out, float freq, float time ) {
	/*
	 * Inputs:
	 * 		V_out - the voltage of the 'high' part of the square wave. This should also be the peak to peak value.
	 * 		freq - the frequency of the square wave to be sent limited to 1.5 kHz to 4.5 kHz
	 * 		time - the amount of time (seconds) the buzzer will play for limited to 0 to 10 seconds
	 */

	// deal with frequency inputs
	if(freq > 4500){
		freq = 4500;
	}
	else if(freq < 1500){
		freq = 1500;
	}

	// deal with frequency input
	if(time > 10){
		time = 10;
	}
	else if(time < 0){
		time = 0;
	}

	// deal with voltage inputs - this is set into the global variable so the ISR will have access to it
	if(V_out > 3.3){
		high_voltage = 3.3;
	}
	else if(V_out < 0){
		high_voltage = 0;
	} else{
		high_voltage = V_out;
	}

	// find out the correct mod value for the desired frequency
	int mod = (((1/(2*freq))*SYSTEM_FREQ) > MAX_VALUE) ? MAX_VALUE : ((1/(2*freq))*SYSTEM_FREQ);

	 // find out how many overflows are required for the desired time duration
	buzzer_overflows = 2 * time * freq;

	FTM2_init( mod );

	FTM2_SC |= (0b1000); // selects the system clock (should start the buzzer)
}

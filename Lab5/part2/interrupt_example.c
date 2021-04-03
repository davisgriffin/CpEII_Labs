#include "address_map_arm.h"

void disable_A9_interrupts (void);
void set_A9_IRQ_stack (void);
void config_GIC (void);
void config_KEYs (void);
void config_HPS_timer (void);
void enable_A9_interrupts (void);

/* ********************************************************************************
 * This program demonstrates use of interrupts with C code.  The program responds 
 * to interrupts from the pushbutton KEY port in the FPGA.
 *
 * The interrupt service routine for the KEYs indicates which KEY has been pressed
 * on the display HEX0.
 ********************************************************************************/

double rate = 0.25;
int counter = 0;
int reset = 0;
int blink = 1;	// allows LEDs to stay unlit after KEY3 is pressed

int main(void)
{
	disable_A9_interrupts ();	// disable interrupts in the A9 processor
	set_A9_IRQ_stack ();			// initialize the stack pointer for IRQ mode
	config_GIC ();					// configure the general interrupt controller
	config_KEYs ();				// configure pushbutton KEYs to generate interrupts
	config_HPS_timer (rate);
	enable_A9_interrupts ();	// enable interrupts in the A9 processor

	volatile int * LEDR_ptr = (int *) LEDR_BASE;
	volatile int delay_count;

	while (1) {
		if(blink)
			*(LEDR_ptr) = counter;
		else
			*(LEDR_ptr) = 0x00000000;
	}	
}

/* setup the KEY interrupts in the FPGA */
void config_KEYs()
{
	volatile int * KEY_ptr = (int *) KEY_BASE;	// pushbutton KEY base address

	*(KEY_ptr + 2) = 0xF; 	// enable interrupts for all four KEYs
}

void config_HPS_timer (double T) {
	volatile int * interval_timer_ptr = (int*) TIMER_BASE;

	int period = T / (1/100000000);
	*(interval_timer_ptr + 0x2) = (period & 0xFFFF);
	*(interval_timer_ptr + 0x3) = (period >> 16) & 0xFFFF;

	*(interval_timer_ptr + 0x1) = 0x7;
}

void stop_HPS_timer () {
	volatile int * interval_timer_ptr = (int*) TIMER_BASE;
	*(interval_timer_ptr + 0x1) = 0xB;
}

/****************************************************************************************
 * HPS Timer - Interrupt Service Routine                                
 *                                                                          
 * HPS Timer used for counting
 ***************************************************************************************/
void HPS_timer_ISR(void) {
	if(counter == 0x000003FF)
		counter = 0x00000000;
	else
		counter++;
	if(reset) {
		counter = 0;
		reset = !reset;
	}
}

/****************************************************************************************
 * Pushbutton - Interrupt Service Routine                                
 *                                                                          
 * This routine checks for button presses and updates HEX0
 * and affects alters the HPS Timer
 ***************************************************************************************/
void pushbutton_ISR( void )
{
	volatile int * KEY_ptr = (int *) KEY_BASE;
	volatile int * HEX3_HEX0_ptr = (int *) HEX3_HEX0_BASE;
	int press, HEX_bits;

	press = *(KEY_ptr + 3);					// read the pushbutton interrupt register
	*(KEY_ptr + 3) = press;					// Clear the interrupt

	if ( press & 0x1 ) {
		reset = !reset;
		HEX_bits = 0b00111111;
	}
	else if ( press & 0x2 ) {
		stop_HPS_timer();
		config_HPS_timer((rate /= 2));
		HEX_bits = 0b00000110;
	}
	else if ( press & 0x4 ) {
		stop_HPS_timer();
		config_HPS_timer((rate *= 2));
		HEX_bits = 0b01011011;
	}
	else if ( press & 0x8 ) {
		blink = !blink;
		HEX_bits = 0b01001111;
	}

	*HEX3_HEX0_ptr = HEX_bits;
	return;
}

#include "address_map_arm.h"

void disable_A9_interrupts (void);
void set_A9_IRQ_stack (void);
void config_GIC (void);
void config_KEYs (void);
void enable_A9_interrupts (void);

/* ********************************************************************************
 * This program demonstrates use of interrupts with C code.  The program responds 
 * to interrupts from the pushbutton KEY port in the FPGA.
 *
 * The interrupt service routine for the KEYs indicates which KEY has been pressed
 * on the display HEX0.
 ********************************************************************************/
int main(void)
{
	disable_A9_interrupts ();	// disable interrupts in the A9 processor
	set_A9_IRQ_stack ();			// initialize the stack pointer for IRQ mode
	config_GIC ();					// configure the general interrupt controller
	config_KEYs ();				// configure pushbutton KEYs to generate interrupts

	enable_A9_interrupts ();	// enable interrupts in the A9 processor

	while (1) 						// wait for an interrupt
		;
}

/* setup the KEY interrupts in the FPGA */
void config_KEYs()
{
	volatile int * KEY_ptr = (int *) KEY_BASE;	// pushbutton KEY base address

	*(KEY_ptr + 2) = 0xF; 	// enable interrupts for all four KEYs
}


/****************************************************************************************
 * Pushbutton - Interrupt Service Routine                                
 *                                                                          
 * This routine checks which KEY has been pressed. It writes to HEX0
 ***************************************************************************************/
void pushbutton_ISR( void )
{
	volatile int * KEY_ptr = (int *) KEY_BASE;
	volatile int * HEX3_HEX0_ptr = (int *) HEX3_HEX0_BASE;
	int press, HEX_bits;

	press = *(KEY_ptr + 3);					// read the pushbutton interrupt register
	*(KEY_ptr + 3) = press;					// Clear the interrupt

	if (press & 0x1)							// KEY0
		HEX_bits = 0b00111111;
	else if (press & 0x2)					// KEY1
		HEX_bits = 0b00000110;
	else if (press & 0x4)					// KEY2
		HEX_bits = 0b01011011;
	else 											// press & 0x8, which is KEY3
		HEX_bits = 0b01001111;

	*HEX3_HEX0_ptr = HEX_bits;
	return;
}
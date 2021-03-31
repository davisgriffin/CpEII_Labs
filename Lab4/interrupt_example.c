#include "address_map_arm.h"
#include "defines.h"

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

int KEY_num = 0;
int run = 0;
int direction = 0;
int counter = 0;
int light = 1;	// allows LEDs to stay unlit after KEY3 is pressed

int main(void)
{
	disable_A9_interrupts ();	// disable interrupts in the A9 processor
	set_A9_IRQ_stack ();			// initialize the stack pointer for IRQ mode
	config_GIC ();					// configure the general interrupt controller
	config_KEYs ();				// configure pushbutton KEYs to generate interrupts

	enable_A9_interrupts ();	// enable interrupts in the A9 processor

	volatile int * LEDR_ptr = (int *) LEDR_BASE;
	volatile int delay_count;

	while (1) {
		switch ( KEY_num ) {
			case 1:
				if(!direction)
					counter = 0;
				else
					counter = 0x3FF;
				break;
			case 4:
				*(LEDR_ptr) &= 0;
				light = 0;
				break;
			default:
				if(run) {
					if(direction) {
						if(counter == 0)
							counter = 0x3FF;
						else
							counter--;
					}
					else {
						if(counter == 0x3FF)
							counter = 0;
						else
							counter++;
					}
				}
				for (delay_count = 500000; delay_count != 0; --delay_count)
            		; // delay loop
				if(light)
					*(LEDR_ptr) = counter;
		}
	}
		
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

	switch( press ) {
		case KEY0:
			KEY_num = 1;
			HEX_bits = 0b00111111;
			break;
		case KEY1:
			KEY_num = 2;
			run = !run;
			HEX_bits = 0b00000110;
			break;
		case KEY2:
			KEY_num = 3;
			direction = !direction;
			HEX_bits = 0b01011011;
			break;
		case KEY3:
			KEY_num = 4;
			HEX_bits = 0b01001111;
			break;
		default:
			KEY_num = 0;
	}

	*HEX3_HEX0_ptr = HEX_bits;
	return;
}
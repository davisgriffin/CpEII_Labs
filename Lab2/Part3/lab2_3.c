/** Lab 2 Part 3 - Bouncing Light
 * @author Grifin Davis
 * LEDR array on DE10-Standard board bounces
 * light from right-to-left, and then
 * left-to-right.
 */

#include "address_map_arm.h"
#include <stdio.h>

int main(void) {
    /* Declare volatile pointers to I/O registers (volatile means that IO load
     * and store instructions will be used to access these pointer locations,
     * instead of regular memory loads and stores)
    */
    volatile int * LED_ptr = (int *)LED_BASE; // LED address

    int LED_bits = 0x00000001; // pattern for LED lights (single LED)
    int flag_dir = 0; // Flag determinines LED direction (left or right)
    volatile int
        delay_count; // volatile so the C compiler doesn't remove the loop

    while (1) {
		if( !flag_dir )
        	LED_bits = LED_bits << 1; // move LED left
        else
        	LED_bits = LED_bits >> 1; // move LED right
            
        *(LED_ptr) = LED_bits; // light up the LEDs

		// if the leftmost or rightmost LED is lit, reverse direction
        if ((LED_bits & 0x00000200) || (LED_bits & 0x00000001))
            flag_dir = !flag_dir;

        for (delay_count = 350000; delay_count != 0; --delay_count)
            ; // delay loop
    }
}

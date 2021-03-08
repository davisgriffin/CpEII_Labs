/** Lab 2 Part 2 - 7-segment LEDs
 * @author Griffin Davis
 * This lab alters getting_started.c
 * by only changing the LED Pattern when
 * KEY0 is pressed, and also lights 
 * HEX0 based on the binary value given
 * by SW3-0
 */

#include "address_map_arm.h"
#include <stdio.h>

int main(void) {
    /* Declare volatile pointers to I/O registers (volatile means that IO load
     * and store instructions will be used to access these pointer locations,
     * instead of regular memory loads and stores)
    */
    volatile int * LED_ptr       = (int *)LED_BASE; // LED address
    volatile int * SW_switch_ptr = (int *)SW_BASE;  // SW slider switch address
    volatile int * KEY_ptr       = (int *)KEY_BASE; // pushbutton KEY address
    volatile int * HEX_ptr 		 = (int *)HEX3_HEX0_BASE; // hex address

    int LED_bits = 0x0F0F0F0F; // pattern for LED lights
    int SW_value, KEY_value;
    
    // Also include hex and decimal values for easier grading
    char seg_bit [16] = {0b00111111, 0b00000110,0b01011011,0b01001111,0b01100110,0b01101101,0b01111101,0b00000111,0b01111111,0b01100111,0b01110111,0b01111100,0b00111001,0b01011110,0b01111001,0b01110001};
    //char seg_bit [16] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
    //char seg_bit[16] = {63,6,91,79,102,109,125,7,127,103,119,124,57,94,121,113};
    
    volatile int
        delay_count; // volatile so the C compiler doesn't remove the loop

    while (1) {
        SW_value = *(SW_switch_ptr); // read the SW slider (DIP) switch values

        KEY_value = *(KEY_ptr); // read the pushbutton KEY values
        if (KEY_value == 1)     // check if KEY0 is pressed
        {
            /* set pattern using SW values */
            LED_bits = SW_value | (SW_value << 8) | (SW_value << 16) |
                       (SW_value << 24);
            while (*KEY_ptr)
                ; // wait for pushbutton KEY release
        }
        else if (KEY_value == 2) { // check if KEY1 is pressed
        	// read binary value as int, set HEX0 to corresponding index in seg_bit
        	*(HEX_ptr) = 0xFF & seg_bit[SW_value & 0xF];
        }
        *(LED_ptr) = LED_bits; // light up the LEDs

        /* rotate the pattern shown on the LEDs */
        if (LED_bits & 0x80000000)
            LED_bits = (LED_bits << 1) | 1;
        else
            LED_bits = LED_bits << 1;

        for (delay_count = 350000; delay_count != 0; --delay_count)
            ; // delay loop
    }
}

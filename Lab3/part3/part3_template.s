/* ********************************************************************************
 * ECE 2045 Lab 3 Part3 Template 
 * This program displays a decimal digit on the 7-segment display HEX0. 
 * Pressing a KEY has the following result:
 * 	KEY 0. sets the display to 0
 * 	KEY 1. increments the displayed number
 * 	KEY 2. decrements the displayed number
 * 	KEY 3. sets the display to be "blank"
 ********************************************************************************/
 				.equ		KEY0, 0b0001
 				.equ		KEY1, 0b0010
 				.equ		KEY2, 0b0100
 				.equ		KEY3, 0b1000
				.text									// executable code follows
				.global	_start
_start:		
				MOV		R4, #0				// counter value to be displayed on HEX0

				LDR		R5, =0xFF200050	// base address of KEY pushbuttons
				LDR		R6, =0xFF200020	// base address of HEX3_HEX0 7-segs
				STR		R4, [R6]				// clear the display


/* Continue your code from here;
 * Call the subroutine SEG7_CODE to show a decimal digit on HEX0
 * It helps to develop a flow chart first before developing your code 
 */
 
			
HNDL_BTN:		LDR 	R2, [R5]		// contents of KEY pushbuttons
				CMP		R2, #0
				BEQ 	HNDL_BTN
				CMP		R2, #KEY0		// check KEY, alter counter value
				MOVEQ	R4, #0
				CMP		R2, #KEY1
				ADDEQ 	R4, R4, #1
				CMP		R2, #KEY2
				SUBEQ 	R4, R4, #1
				CMP		R2, #KEY3
				MOVEQ 	R4, #0xF

WAIT:			LDR 	R2, [R5]
				CMP 	R2, #0
				BNE 	WAIT

				MOV 	R0, R4			// r0 = counter value for SEG7_CODE
				BL 		SEG7_CODE

UPDT_HEX:		STR		R0, [R6]
				B		HNDL_BTN		// return to btn check


/* End your code here */
			
/* Subroutine to convert the digits from 0 to 9, or blank, to be shown on a HEX display.
 * Parameters: R0 = the decimal value of the digit to be displayed
 * Returns: R0 = bit patterm to be written to the HEX display
 */
SEG7_CODE:	
				MOV		R1, #0				// display a blank
				CMP		R0, #0
				MOVEQ		R1, #0b00111111	// display a 0
				CMP		R0, #1
				MOVEQ		R1, #0b00000110	// display a 1
				CMP		R0, #2
				MOVEQ		R1, #0b01011011	// display a 2
				CMP		R0, #3
				MOVEQ		R1, #0b01001111	// display a 3
				CMP		R0, #4
				MOVEQ		R1, #0b01100110	// display a 4
				CMP		R0, #5
				MOVEQ		R1, #0b01101101	// display a 5
				CMP		R0, #6
				MOVEQ		R1, #0b01111101	// display a 6
				CMP		R0, #7
				MOVEQ		R1, #0b00000111	// display a 7
				CMP		R0, #8
				MOVEQ		R1, #0b01111111	// display a 8
				CMP		R0, #9
				MOVEQ		R1, #0b01100111	// display a 9

				MOV		R0, R1				// return bit pattern

				BX 		LR

				.end    

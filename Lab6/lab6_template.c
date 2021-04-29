/********************************************************************************************
*  Lab 6 Template 
*  Add you name here
*  This template provides only a basic program structure for your reference;
*  Fill in your code throughout the entire program in appropriate places 
*  to finish the Lab 6 requirements.
*********************************************************************************************/

#include "address_map_arm.h"

/*******************************************************************************
 * This program demonstrates use of the JTAG UART port
 *
 * It performs the following:
 *	1. Gets a text string from the JTAG UART
 * 	2. Extracts the digits in the string and saves them to an array
 * 	3. Echos the digit characters back to the JTAG UART
 ******************************************************************************/
/* function prototypes */


// State machine type; You may add/remove states as needed
typedef enum {
	IDLE,		
    START,		
    ENTER,
    PRINT
} jtag_states;
 
 
int main(void) {
    
/* Declare variables and volatile pointers to I/O registers */
    jtag_states state = IDLE; 		//defines a state variable and assigns the first state
    char c;							//character read from JTAG
  

    while (1) {
        c = get_jtag(JTAG_UART_ptr);
            
//The commnets are suggestions only; It is up to you how to use these states below

        switch(state){		//in each state, use a switch (c) to take care of differnet cases. 
            case IDLE:			
                //Tell the user to press ESC to start type in a string
                //other work you may want it to do here
                //move on to the next state
                break;

            case START:			
               //Tell the user to type in a string, followed by Enter:
               //Wait for ESC; enter next state when received;
                //other work you may want it to do here;
                //move on to the next state;
               
                break;
            case ENTER:
     			// Processing the string as user type in
                 //Check for Enter; (end of string)
				//other work you may want it to do here
                //move on to the next state
                break;
            case PRINT: 
              //further processing string
              //send out processed string to the JTAG
              
                break;
      
        }
    }
}


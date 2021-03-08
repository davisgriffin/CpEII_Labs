/* Program for Part 1
Griffin Davis
 */

            .text
            .global start

start:
            LDR     R4,=RESULT   // r4 points to result location
            LDR     R2, [R4, #4] // r2 holds number of elements in the list
            ADD     R3, R4, #8   // r3 holds the address of the first number
            LDR     R0, [R3]     // r0 holds the first number

LOOP:       SUBS    R2, R2, #1   // decrement loop counter
            BEQ     DONE         // if Z==1, aka N=0 branch to label DONE
            ADD     R3, R3, #4   // r3 now holds the next number address
            LDR     R1, [R3]     // load the new number into r1
            CMP     R0, R1       // compare r0 and r1 (previous and new numbers)
            BGE     LOOP         // if r0 >= r1, branch to label LOOP
            MOV     R0, R1       // set the value of r0 to the value of r1
                                 // if r0 < r1
            B       LOOP         // branch to the label LOOP

DONE:       STR     R0, [R4]     // store value in r0 (largest number) to r4 address
END:        B       END
RESULT:     .word   0            
N:          .word   7            // number of elements in the list
NUMBERS:    .word   4, 5, 3, 6, 1, 8, 2 // numbers in the list
            .end
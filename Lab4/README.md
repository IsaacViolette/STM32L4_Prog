//Isaac Violette
//ECE 271 - Lab 4
//2/27/22

Something Cool:
When right joystick gets pressed, both green and red LEDs blink until the left joystick is pressed. Then it goes back into it's normal mode.

Post Lab: 

You can use the BIC instruction to bitwise-clear out bits. Is this instruction necessary? How could you
do the same thing if BIC did not exist?

ANSWER: I believe that's exactly what I was doing in this lab. I never used the BIC instruction, I instead would set a register, and/or bitwise and compare, then set the final register. 
It definitely would've been easier if I had used the BIC instruction as it would've been less instructions but it's not 
neccesary and I wasn't worried about capacity. Also, the prelab showed how to do it, so I followed that way.

How does the CMP compare instruction differ from the SUB subtract instruction?
The CMP instruction subtracts the second operand from the first operand and sets the status flag while the SUB instruction does the same thing but instaed of setting a status flag,
it stores the value into its destination.


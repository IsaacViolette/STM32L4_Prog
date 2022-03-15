Isaac Violette

ECE 271: Lab 6

3/15/22

Something Cool:

When you press the right part of the joystick, the motor turns clockwise only when holding. When you press the left
part of the joystick, the motor moves counterclockwise when holding.

Post Lab Questions:

Why is an ABI useful when writing code? Why not just pick any register we want for passing
parameters?

ANSWER:

ABI is important because all the registers should be untouched when they are being implemented. It allows for code
to be shared easily as the regsiters don't get changed when used throughout the code. We don't just pick any registers 
when passing parameters because then if the code would be shared, it may not match the standard someone else is using
so haveing the same standard for all assembly keeps people on the same page.


Why does C store local variables on the stack, rather than forcing you to just use all global
variables?

ANSWER:
I believe that global variables take much more space then local variables, and if you are using a local variable
it should be created and destroyed in the same space. Global variables would be used throughout the entire program,
so saving local variables on the stack is for a short amount of time and are put on and taken off in a short amount if time.
The biggest takeaway is that putting it on the stack takes up much less space. 

Find the binary image that is being uploaded to your board. How big is this file (written in
assembly) compared to the C version from Lab#5?

ANSWER:
8 kb Assembly file compared to a 21 kb C file. Now I did split my assembly file into two different files because I was having 
issues with it all being in one but having it all in one file made it about 8-10 kb. It's clearly takes a lot less space then 
the C file.


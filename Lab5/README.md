Isaac Violette
ECE 271 Lab 3
2/17/22

Something cool: When # symbol is pressed, the previous input is displayed at the end of the display

Post Lab: 

1. Can your code correctly handle if multiple keys are pressed at once? Why or why not?

My code cannot correctly handle when multiple keys are pressed at the same time. When I tested this out
whatever key that was let go of last was displayed on the screen. The reason behind this is because when two
buttons are pressed, it can create a short. Also, within my code, when I am using the keyoad scan function, 
I am checking rows and columns to figure out which button is being pressed. Since there are multiple, nothing happens 
and my button debouncer is waiting for whatever button is released last. Whatever button is released last, moves
through my debouncer and gets displayed while nothing happens to the other button that is pressed.  



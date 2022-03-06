Isaac Violette

ECE 271 Lab 5

3/2/22

Something cool: When Right joystick is pressed, the motor moves clockwise, and when the joysick is pressed left, the motor moves counterclockwise. When the joystick is pressed up, the motor moves clockwise 
infinitely, and when the joystick is pressed down the joystick moves counterclockwise infintely. In both of these cases, if the center button is pressed then the motors stop turning. When any button is pressed
on the joystick, the LCD displays what button is being pressed.

Post Lab: 
1. The Darlington array can only provide 500mA of current. If you needed a larger current, what
could you use instead of the Darlington array?

ANSWER:
If you need high current output, you can put the output pins in parallel which will allow for a current above 500mA.

2. Is it possible to rotate the motor less than 1/2 step? (Hint, see Chapter 16.6 of the textbook)

ANSWER:
Yes, it is possible to rotate the motor less than a half step This is called micro stepping, but this uses PWM to rotate the fractions of a full step. 

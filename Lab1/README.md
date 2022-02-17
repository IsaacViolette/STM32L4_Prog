NAME: Isaac Violette
DATE: Week of 24 January
LAB: LAB 1 -> Joystick Button Input and LED Output
Class: ECE 271

**README FILE**

Post Lab Questions:

Question 1:
The joystick buttons on the STM32L4 board have hardware debouncing. An example of this can be
seen in Figure 5. Explain briefly how this works.

Answer:
When two pieces of metal create contact when pressed together, they rebound a few times before staying together.
This bouncing creates multiple signals which makes the processor think the button has been pressed multiple times.
The way to fix this is with hardware debouncing which uses a siimple RC Circuit. When the button isn't pushed the capacitor
is fully charged, and there is no current on the resistors, and the voltage on the processor pin is 0. When the button is pushed,
the capacitor discharges quickly. When the button rebounds there isn't enough voltage left in the capacitor to pull the processor pin down so it
doesn't register as a press and only accepts one press. 

Qustion 2:
Debouncing can also be done in software. Explain how this could be done in software.

Answer:
There are two different methods for debouncing, there is the wait-and-see method and the counter debouncer.
With the wait-and-see, the program detects the button is pressed and re-examines the signal after a tiny delay. If the 
signal still shows the button is pressed, then the prgram reports a pressed button. This is a very slow way of checking the 
button so it can't be used in critical systems. The counter debouncer method is much faster, it checks the input 
at many intervals and if they are all positive readings then the button is pressed and it reports that to the program. 
This is a much faster way of debouncing with software.

Question 3:
Each GPIO in has for programmable output speeds. Low, medium, fast, and high. The slew rate can
be up to 80MHz. Why is the “low” speed recommended for controlling LEDs? (Hint: energy and
electromagnetic interference)

Answer:
LED's are a light emitting source so having a noisy signal is not wanted since it needs constant energy. When you have
a high slew rate it causes high electromagnetic interferance to nearby electronics which can cause malfunctions to 
nearby circuits. But it also causes high-frequency harmonics as well which can also create malfunctions. With a slower slew rate,
the signal for the LED's won't be as noisy and also won't be noticed by the user since it's still faster than our brains
can process.

Something Cool Explanation:

Press the joystick up, Red light turns on. Press the Joystick down, green light turns on. 
Press the center of the joystick, red and green lights blink simutaneusly. Press joystick right to stop the blinking.
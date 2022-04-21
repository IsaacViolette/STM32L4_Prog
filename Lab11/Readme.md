Isaac Violette

4/21/20

ECE271 - Lab 11

Post Lab:

Q1 - 

360/(44321/293) = 2.37 degrees

Q2 - 

Using the include math.h, if we tried using the sin function it would be extremely slow. Since it would be doing a 
bunch of calculations for each frequency. Every time the interupt handler is called, the angle would need to be calculated
which isn't reliable. Instead the lookup table has all the values and doesn't need to calculate anything.

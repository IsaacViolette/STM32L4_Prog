Isaac Violette
Lab 2 Postlab
2/6/22

Something Cool:
Display long strings by typing out each letter one by one, clearing once the diplay is full
and then displaying the rest of the string until the string is finsihed. This is all in
a continuous loop.

Post Lab Answers:

1) Duty Cycle is 1/4 so 4 common terminals, 120/4 = 30 + 4 (common terminal pins) = 34 pins

2) The LCD Driver that drives the display is on the Cortex M-4 CPU.

3) From page 766 from the manuel, the STM32L4 processor LCD driver can drive 176 (44x4) or 320 (40x8) LCD picture elements (pixels). 
The LCD_RAM size in terms of bits is 512 bits since it is 16 x 32-bit registers.

4) From page 433 of the textbook, there are 96 pixels on the STMl4 board. The bits that aren't hooked up with the LCD_RAM are reserved based
based from the textbook. In lab, Pascal mentioned that you shouldn't set those bits since they might be used in different processes. We need to be careful when 
setting those bits.

5) If I don't check the string length in my Print_array() for loop and I print an array of characters that is 3 characters, 
I get a 0 printed out in the 5th LCD segment while the first three segments are printed with the original string. 

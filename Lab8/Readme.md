Isaac Violette
Lab 8 Postlab
4/4/22

Question 1:

PWM Period = 1 second
Clock = 8MHz

For PSC
8 MHz / 800 = 10kHz

For ARR
1 = (ARR + 1) / 10 kHz --> ARR = 9999

For CCR1
(ARR + 1) / 2 = 5000

Counting Mode = Up-counting
ARR = 9999
CCR1 = 5000
PSC = 799

Question 2:

Largest PSC ---> 2^16-1 = 65535   --->  8 MHz/65535 = 122 Hz

Slowest ARR ---> PWM Period = (ARR+1)/122 ---> 0.016 Hz
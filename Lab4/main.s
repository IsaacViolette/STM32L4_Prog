;******************** (C) Yifeng ZHU *******************************************
; @file    main.s
; @author  Yifeng Zhu
; @date    May-17-2015
; @note
;           This code is for the book "Embedded Systems with ARM Cortex-M 
;           Microcontrollers in Assembly Language and C, Yifeng Zhu, 
;           ISBN-13: 978-0982692639, ISBN-10: 0982692633
; @attension
;           This code is provided for education purpose. The author shall not be 
;           held liable for any direct, indirect or consequential damages, for any 
;           reason whatever. More information can be found from book website: 
;           http:;www.eece.maine.edu/~zhu/book
;*******************************************************************************
;Isaac Violette
;ECE 271 - Lab 4: GPIOs in Assembly Language
;2/23/22

	INCLUDE core_cm4_constants.s		; Load Constant Definitions
	INCLUDE stm32l476xx_constants.s

	AREA    main, CODE, READONLY
	EXPORT	__main				; make __main visible to linker
	ENTRY

__main	PROC


	;=======================
	; Your code goes here!
	;=======================
	
	;turn GPIOA Clock on
	ldr		r1,=RCC_BASE 			;point R1 to the base register
	ldr		r3,[r1,#RCC_AHB2ENR]	;load 32-bit value at base+offset
	orr		r3,#0x13					;bitwise OR with the value for Clock A
	str		r3,[r1,#RCC_AHB2ENR]	;Store value back to base+offset
	
	;Initialize the LED GPIOs to be Outputs
	;PB-2
	;MASK
	ldr		r1,=GPIOB_BASE 	;point R1 to the base register
	ldr		r3,[r1,#GPIO_MODER]	;load 32-bit value at base+offset
	and		r3, #0xFFFFFFCF	;bitwise AND with the value for PB2
	str		r3,[r1,#GPIO_MODER]	;Store value back to base+offset
	;VALUE
	ldr		r1,=GPIOB_BASE 	;point R1 to the base register
	ldr		r3,[r1,#GPIO_MODER]	;load 32-bit value at base+offset
	orr		r3, #0x10		;bitwise OR with the value for PB2
	str		r3,[r1,#GPIO_MODER]	;Store value back to base+offset
	
	;PE8
	;MASK
	ldr		r1,=GPIOE_BASE 		;point R1 to the base register
	ldr		r3,[r1,#GPIO_MODER]	;load 32-bit value at base+offset
	and		r3, #0xFFFCFFFF		;bitwise AND with the value for PE8
	str		r3,[r1,#GPIO_MODER]	;Store value back to base+offset
	;VALUE
	ldr		r1,=GPIOE_BASE 		;point R1 to the base register
	ldr		r3,[r1,#GPIO_MODER]	;load 32-bit value at base+offset
	orr		r3, #0x10000		;bitwise OR with the value for PE8
	str		r3,[r1,#GPIO_MODER]	;Store value back to base+offset
	
	;Initialize LED GPIOS to be Push-Pull
	;Set PB2 (OT2) to zero
	ldr		r1,=GPIOB_BASE 			;point R1 to the base register
	ldr		r3,[r1,#GPIO_OTYPER]	;load 32-bit value at base+offset
	and		r3, #0xFFFFFFFB			;bitwise AND with the value for PB2
	str		r3,[r1,#GPIO_OTYPER]	;Store value back to base+offset
	
	;Set PE8 Offset
	ldr		r1,=GPIOE_BASE 			;point R1 to the base register
	ldr		r3,[r1,#GPIO_OTYPER]	;load 32-bit value at base+offset
	and		r3, #0xFFFFFEFF			;bitwise AND with the value for PE8
	str		r3,[r1,#GPIO_OTYPER]	;Store value back to base+offset
	
	;Initialize the LED GPIOs for No PullUp/PullDown
	;Set PB2 to 00
	ldr		r1,=GPIOB_BASE 			;point R1 to the base register
	ldr		r3,[r1,#GPIO_PUPDR]		;load 32-bit value at base+offset
	and		r3, #0xFFFFFFCF			;bitwise AND with the value for PB2
	str		r3,[r1,#GPIO_PUPDR]		;Store value back to base+offset
	
	;Set PE8 to 00
	ldr		r1,=GPIOE_BASE 			;point R1 to the base register
	ldr		r3,[r1,#GPIO_PUPDR]		;load 32-bit value at base+offset
	and		r3, #0xFFFCFFFF			;bitwise AND with the value for PE8
	str		r3,[r1,#GPIO_PUPDR]		;Store value back to base+offset
	
	;Initialize Joystick to be input
	ldr		r1,=GPIOA_BASE 			;point R1 to the base register
	ldr		r3,[r1,#GPIO_MODER]		;load 32-bit value at base+offset
	and		r3, #~GPIO_MODER_MODER0 ;bitwise AND with the value for GPIO Joystick
	and		r3, #~GPIO_MODER_MODER1
	and		r3, #~GPIO_MODER_MODER2
	and		r3, #~GPIO_MODER_MODER3
	and		r3, #~GPIO_MODER_MODER5
	str		r3,[r1,#GPIO_MODER]		;Store value back to base+offset
	
	;Initialize the Joystick GPIOs to be PullDown
	;mask
	ldr		r1,=GPIOA_BASE 			;point R1 to the base register
	ldr		r3,[r1,#GPIO_PUPDR]		;load 32-bit value at base+offset
	and		r3, #~GPIO_PUPDR_PUPDR0 ;bitwise AND with the value for GPIO Joystick
	and		r3, #~GPIO_PUPDR_PUPDR1
	and		r3, #~GPIO_PUPDR_PUPDR2
	and		r3, #~GPIO_PUPDR_PUPDR3
	and		r3, #~GPIO_PUPDR_PUPDR5
	str		r3,[r1,#GPIO_PUPDR]		;Store value back to base+offset
	
	;set
	ldr		r1,=GPIOA_BASE 				;point R1 to the base register
	ldr		r3,[r1,#GPIO_PUPDR]			;load 32-bit value at base+offset
	orr		r3, #GPIO_PUPDR_PUPDR0_1 	;bitwise OR with the value for GPIO Joystick
	orr		r3, #GPIO_PUPDR_PUPDR1_1
	orr		r3, #GPIO_PUPDR_PUPDR2_1
	orr		r3, #GPIO_PUPDR_PUPDR3_1
	orr		r3, #GPIO_PUPDR_PUPDR5_1
	str		r3,[r1,#GPIO_PUPDR]			;Store value back to base+offset
	
	
;set registers to only do the
	ldr		r1,=GPIOA_BASE
	ldr		r2,=GPIOB_BASE
	ldr 	r4,=GPIOE_BASE

;inifinte loop
LOOPFOREVER
	ldr		r3,[r1, #GPIO_IDR]
	and		r3, #GPIO_IDR_IDR_3
	cmp		r3,#GPIO_IDR_IDR_3 	;comparing if IDR is value three for Joystick push up
	bne 	PUSHEDDOWN 			;not equal go to next argument
;move to turn LED red if equal
TURNRED
	ldr		r3,[r2,#GPIO_ODR]	
	orr		r3, #0x4			
	str		r3,[r2,#GPIO_ODR]
	b LOOPFOREVER
;turn on red LED
PUSHEDDOWN
	ldr		r3,[r1, #GPIO_IDR]
	and		r3, #GPIO_IDR_IDR_5
	cmp		r3,#GPIO_IDR_IDR_5 	;comparing if IDR is value three for Joystick push up
	bne 	PUSHEDCENTER 		;not equal go to next argument
	;move to turn LED green
TURNGREEN
	ldr		r3,[r4,#GPIO_ODR]	
	orr		r3, #0x100			
	str		r3,[r4,#GPIO_ODR]
	b		LOOPFOREVER	
;turn on green LED
PUSHEDCENTER
	ldr		r3,[r1, #GPIO_IDR]
	and		r3, #GPIO_IDR_IDR_0
	cmp		r3,#GPIO_IDR_IDR_0 	
	bne 	PUSHRIGHT 			;if center isn't pressed move to pushright
	;move to turn on both LEDS
BOTHLEDS
	ldr		r3,[r4,#GPIO_ODR]	
	orr		r3, #0x100			
	str		r3,[r4,#GPIO_ODR]	

	ldr		r3,[r2,#GPIO_ODR]	
	orr		r3, #0x4			
	str		r3,[r2,#GPIO_ODR]	
	b 		LOOPFOREVER
	;turn on both LEDS
PUSHRIGHT
	ldr		r3,[r1, #GPIO_IDR]
	and		r3, #GPIO_IDR_IDR_2
	cmp		r3,#GPIO_IDR_IDR_2 	;comparing if IDR is value three for Joystick push right
	bne 	NOTHING ;otherwise loop through
	;when pushed right go into something cool
SOMETHINGCOOL
	;turn on Red LED
	ldr		r3,[r2,#GPIO_ODR]	
	orr		r3, #0x4			
	str		r3,[r2,#GPIO_ODR]
	
	;turn on Green LED
	ldr		r3,[r4,#GPIO_ODR]	
	orr		r3, #0x100			
	str		r3,[r4,#GPIO_ODR]
	
	mov r5,#0 ;delay to keep LED on
DELAY
	cmp r5,#0x11000
	bge DELAYDONE
	add r5,r5,#1
	b CHECKONE
CHECKONE ;while delaying check to see if left joystick is pressed to break out
	ldr		r3,[r1, #GPIO_IDR]
	and		r3, #GPIO_IDR_IDR_1
	cmp		r3,#GPIO_IDR_IDR_1 	;comparing if IDR is value three for Joystick push left
	beq 	NOTHING
	b 		DELAY
	
DELAYDONE ;when out of delay turn off LEDs
	ldr		r3,[r2,#GPIO_ODR]	
	and		r3, #~0x4			;turn off Red LED
	str		r3,[r2,#GPIO_ODR]
	
	ldr		r3,[r4,#GPIO_ODR]	
	and		r3, #~0x100			;Turn green LED off		
	str		r3,[r4,#GPIO_ODR]
	
	mov r5,#0
FINALDELAY ;delay to keep the LEDS off
	cmp r5,#0x11000
	bge FINISH
	add r5,r5,#1
	b CHECKTWO
	
CHECKTWO ;Check again while delaying to break out of loop if left joystick is pressed
	ldr		r3,[r1, #GPIO_IDR]
	and		r3, #GPIO_IDR_IDR_1
	cmp		r3,#GPIO_IDR_IDR_1 	;comparing if IDR is value three for Joystick push left
	beq 	NOTHING
	b		FINALDELAY

FINISH ;break out of something cool
	
	b SOMETHINGCOOL

NOTHING
	ldr		r3,[r4,#GPIO_ODR]	
	and		r3, #~0x100			;turn off Green LED
	str		r3,[r4,#GPIO_ODR]	

	ldr		r3,[r2,#GPIO_ODR]	
	and		r3, #~0x4			;turn off Red LED
	str		r3,[r2,#GPIO_ODR]	


	b LOOPFOREVER ;go back to the beginning of the loop




BRANCHFOREVER		b BRANCHFOREVER


	;====================================
	; No need to edit anything below here
	;====================================

	ENDP

	ALIGN

	AREA    myData, DATA, READWRITE
	ALIGN
array	DCD   1, 2, 3, 4
	END

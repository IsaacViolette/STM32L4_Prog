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
;ECE 271 - Lab 6: Stepper Motor in Assembly Language
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
	bl 		Stepper_Pin_Init ;Move to initialize GPIOB pin and clock
	bl		Joystick_Init
	
	bl		SOMETHING_COOL
	
	b		STOP

Stepper_Pin_Init

	;initialize GPIOB clock
	ldr		r1,=RCC_BASE
	ldr		r3,[r1,#RCC_AHB2ENR]	
	orr		r3,#RCC_AHB2ENR_GPIOBEN
	str		r3,[r1,#RCC_AHB2ENR]
	
	;Set GPIOB pin 2,3,6,7 as digital outputs
	ldr		r1,=GPIOB_BASE
	ldr		r3,[r1,#GPIO_MODER]
	and		r3,#~GPIO_MODER_MODER2
	and		r3,#~GPIO_MODER_MODER3
	and		r3,#~GPIO_MODER_MODER6
	and		r3,#~GPIO_MODER_MODER7
	orr		r3,#GPIO_MODER_MODER2_0
	orr		r3,#GPIO_MODER_MODER3_0
	orr		r3,#GPIO_MODER_MODER6_0
	orr		r3,#GPIO_MODER_MODER7_0
	str		r3,[r1,#GPIO_MODER]
	
	bx		lr
	
Joystick_Init
	;initialize GPIOA Clock
	ldr		r1,=RCC_BASE 			;point R1 to the base register
	ldr		r3,[r1,#RCC_AHB2ENR]	;load 32-bit value at base+offset
	orr		r3,#RCC_AHB2ENR_GPIOAEN				;bitwise OR with the value for Clock A
	str		r3,[r1,#RCC_AHB2ENR]	;Store value back to base+offset
	
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

	bx		lr
	
;Delay Function
DELAY
	push	{r5}	;create a new value for r5
	
	mov		r5,#0	;start r5 at 0
DELAY_LOOP
	add		r5,r5,#1;add one to the delay every loop
	cmp		r5,r0;if r5 is equal to r0, pop, otherwise keep looping
	bne		DELAY_LOOP;
	pop		{r5}
	bx		lr		;Delay finish, move back to where Delay was called


SOMETHING_COOL
	ldr		r1,=GPIOA_BASE	
	ldr		r3,[r1,#GPIO_IDR]
	and		r3, #GPIO_IDR_IDR_2
	cmp		r3, #GPIO_IDR_IDR_2 ;check if right joystick has been pressed
	beq		PUSH_RIGHT			;move to Right Joystick Code
	b		NEXT				;Move to see if left joystick is pressed
PUSH_RIGHT	;moves clockwise
	
	ldr		r1,=GPIOB_BASE
	ldr		r4,[r1,#GPIO_BSRR]	;setting up GPIOB BSRR
	
	;step 0
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_6
	orr		r4,#GPIO_BSRR_BR_3
	orr		r4,#GPIO_BSRR_BS_7
	orr		r4,#GPIO_BSRR_BS_2
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1800		;delay length for full stepper
	bl		DELAY
	pop		{r0}
	
	;step 1
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_7
	orr		r4,#GPIO_BSRR_BR_3
	orr		r4,#GPIO_BSRR_BS_6
	orr		r4,#GPIO_BSRR_BS_2
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1800		;delay length for full stepper
	bl		DELAY
	pop		{r0}
	
	;step 2
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_7
	orr		r4,#GPIO_BSRR_BR_2
	orr		r4,#GPIO_BSRR_BS_6
	orr		r4,#GPIO_BSRR_BS_3
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1800		;delay length for full stepper
	bl		DELAY
	pop		{r0}
	
	;step 3
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_6
	orr		r4,#GPIO_BSRR_BR_2
	orr		r4,#GPIO_BSRR_BS_7
	orr		r4,#GPIO_BSRR_BS_3
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1800		;delay length for full stepper
	bl		DELAY
	pop		{r0}

NEXT
	ldr		r3,[r1,#GPIO_IDR]
	and		r3, #GPIO_IDR_IDR_1
	cmp		r3, #GPIO_IDR_IDR_1
	beq		PUSH_LEFT			;check to see if the left joystick is pressed
	b		SOMETHING_COOL		;if not move loop to the top to check the options again
PUSH_LEFT ;moves counterclockwise

	ldr		r1,=GPIOB_BASE
	ldr		r4,[r1,#GPIO_BSRR]	;setting up GPIOB BSRR
	
	;step 3
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_6
	orr		r4,#GPIO_BSRR_BR_2
	orr		r4,#GPIO_BSRR_BS_7
	orr		r4,#GPIO_BSRR_BS_3
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1800		;delay length for full stepper
	bl		DELAY
	pop		{r0}
	
	;step 2
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_7
	orr		r4,#GPIO_BSRR_BR_2
	orr		r4,#GPIO_BSRR_BS_6
	orr		r4,#GPIO_BSRR_BS_3
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1800		;delay length for full stepper
	bl		DELAY
	pop		{r0}
	
	;step 1
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_7
	orr		r4,#GPIO_BSRR_BR_3
	orr		r4,#GPIO_BSRR_BS_6
	orr		r4,#GPIO_BSRR_BS_2
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1800		;delay length for full stepper
	bl		DELAY
	pop		{r0}
	
	;step 0
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_6
	orr		r4,#GPIO_BSRR_BR_3
	orr		r4,#GPIO_BSRR_BS_7
	orr		r4,#GPIO_BSRR_BS_2
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1800		;delay length for full stepper
	bl		DELAY
	pop		{r0}
	
	b		NEXT		;check to see if the left joystick is still being pressed
	
BRANCHFOREVER		b BRANCHFOREVER

STOP

	;====================================
	; No need to edit anything below here
	;====================================

	ENDP

	ALIGN

	AREA    myData, DATA, READWRITE
	ALIGN
array	DCD   1, 2, 3, 4
	END

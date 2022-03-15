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
	
	;Test Full and Half Step
	;mov 	r0,#360
	;bl		Stepper_Full_Step
	mov		r0,#360
	bl		Stepper_Half_Step
	
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

;start of full stepper
Stepper_Full_Step
	mov		r2,#512	;
	mul 	r0,r0,r2
	mov		r2,#360
	sdiv	r0,r0,r2 ;calculates each degree for rotation

	mov		r3,#0
FULL_STEP_ROTATION
	cmp		r3,r0	;if the motor has rotated the correct amount
	bge		EXIT	;move to the exit code
	add 	r3,r3,#1;add one to the register every time the motor moves 1 degree
	
	ldr		r1,=GPIOB_BASE
	ldr		r4,[r1,#GPIO_BSRR]	;getting ready for BSRR bits for GPIOB
	
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

	b		FULL_STEP_ROTATION ;loop to the top

EXIT
	pop		{lr}
	bx		lr		;branch back to main code


;start of half stepper
Stepper_Half_Step
	mov		r2,#512
	mul 	r0,r0,r2
	mov		r2,#360
	sdiv	r0,r0,r2 ;calculate how much the motor needs to turn

	mov		r3,#0
HALF_STEP_ROTATION
	cmp		r3,r0	;check to see if the loop is done
	bge		EXIT	;loop if so
	add 	r3,r3,#1;add one to counter
	
	ldr		r1,=GPIOB_BASE
	ldr		r4,[r1,#GPIO_BSRR]	;setting up GPIOB BSRR
	
	;step 7
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_6
	orr		r4,#GPIO_BSRR_BR_3
	orr		r4,#GPIO_BSRR_BS_2
	orr		r4,#GPIO_BSRR_BS_7
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1500		;delay length for half step
	bl		DELAY
	pop		{r0}
	
	;step 6
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_6
	orr		r4,#GPIO_BSRR_BR_2
	orr		r4,#GPIO_BSRR_BS_7
	orr		r4,#GPIO_BSRR_BS_3
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1500		;delay length for half step
	bl		DELAY
	pop		{r0}
	
	;step 5
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_7
	orr		r4,#GPIO_BSRR_BR_6
	orr		r4,#GPIO_BSRR_BS_2
	orr		r4,#GPIO_BSRR_BS_3
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1500		;delay length for half step
	bl		DELAY
	pop		{r0}
	
	;step 4
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_7
	orr		r4,#GPIO_BSRR_BR_2
	orr		r4,#GPIO_BSRR_BS_6
	orr		r4,#GPIO_BSRR_BS_3
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1500		;delay length for half step
	bl		DELAY
	pop		{r0}
	
	;step 3
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_7
	orr		r4,#GPIO_BSRR_BR_3
	orr		r4,#GPIO_BSRR_BS_2
	orr		r4,#GPIO_BSRR_BS_6
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1500		;delay length for half step
	bl		DELAY
	pop		{r0}
	
	;step 2
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_7
	orr		r4,#GPIO_BSRR_BR_2
	orr		r4,#GPIO_BSRR_BS_6
	orr		r4,#GPIO_BSRR_BS_2
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1500		;delay length for half step
	bl		DELAY
	pop		{r0}
	
	;step 1
	mov		r4,#0
	orr		r4,#GPIO_BSRR_BR_7
	orr		r4,#GPIO_BSRR_BR_6
	orr		r4,#GPIO_BSRR_BS_3
	orr		r4,#GPIO_BSRR_BS_2
	str		r4,[r1,#GPIO_BSRR]
	push	{r0}
	mov		r0,#1500		;delay length for half step
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
	mov		r0,#1500		;delay length for half step
	bl		DELAY
	pop		{r0}

	b		HALF_STEP_ROTATION


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

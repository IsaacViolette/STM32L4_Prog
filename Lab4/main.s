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
	
	;ODR to turn on Red and Green LED
	;Green LED
	ldr		r1,=GPIOE_BASE 		;point R1 to the base register
	ldr		r3,[r1,#GPIO_ODR]	;load 32-bit value at base+offset
	orr		r3, #0x100			;bitwise OR with the value for Green LED
	str		r3,[r1,#GPIO_ODR]	;Store value back to base+offset
	
	;Red LED
	ldr		r1,=GPIOB_BASE 		;point R1 to the base register
	ldr		r3,[r1,#GPIO_ODR]	;load 32-bit value at base+offset
	orr		r3, #0x4			;bitwise OR with the value for Red LED
	str		r3,[r1,#GPIO_ODR]	;Store value back to base+offset
	

BRANCHFOREVER		b BRANCHFOREVER ;Infinite loop


	;====================================
	; No need to edit anything below here
	;====================================

	ENDP

	ALIGN

	AREA    myData, DATA, READWRITE
	ALIGN
array	DCD   1, 2, 3, 4
	END

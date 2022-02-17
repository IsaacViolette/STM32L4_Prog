/* ECE271 Spring 2022 Lab1 Code template */
//Isaac Violette
//ECE 271
//Lab 1 - Joystick Button Input and LED Output
//Week of 24 January 2022

#include <stdint.h>

#include "stm32l476xx.h"

void System_Clock_Init(void) {

	/* Enable the high-speed system clock */
	/* You probably shouldn't touch this */
	RCC->CR |= ((uint32_t)RCC_CR_HSION);

	while((RCC->CR & (uint32_t) RCC_CR_HSIRDY)==0) ;

	RCC->CFGR&=(uint32_t)((uint32_t)~(RCC_CFGR_SW));
	RCC->CFGR|=(uint32_t)RCC_CFGR_SW_HSI;

	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS)==0) ;
}

int main(void){

	long int i;
	/* Initialize the system clock */
	System_Clock_Init();

	/* Your code goes here */
	
	//Turning the clock on to be enabled for pins
	RCC->AHB2ENR |= 0x13;
	
	//Initialize the LED GPIOs to be Outputs
	//PB-2
	GPIOB->MODER &= 0xFFFFFFCF; //mask
	GPIOB->MODER |= 0x10; //value
	
	//PE8
	GPIOE->MODER &= 0xFFFCFFFF; //mask
	GPIOE->MODER |= 0x10000; //value
	
	//Initialize LED GPIOS to be Push-Pull
	//Set PB2 (OT2) to zero
	GPIOB->OTYPER &= 0xFFFFFFFB; //Set
	
	//Set PE8 Offset
	GPIOE->OTYPER &= 0xFFFFFEFF; //Set
	
	//Initialize the LED GPIOs for No PullUp/PullDown
	//Set PB2 to 00
	GPIOB->PUPDR &= 0xFFFFFFCF; //Set
	
	//Set PE8 to 00
	GPIOE->PUPDR &= 0xFFFCFFFF; //Set
	
	
	//Part 2
	//Initializing the Joystick GPIOs to be Inputs
	GPIOA->MODER &= 0xFFFCF300; //Mask
	
	//Initialize the Joystick GPIOs to be PullDown
	GPIOA->PUPDR &= 0xFFFFF300; //Mask
	GPIOA->PUPDR |= 0x8AA; //Set
	
	//run the program endlessly
	while(1)
	{
		//The Joystick is pressed up
		if((GPIOA->IDR >> 3) & 0x1)
			GPIOB->ODR |= 1<<2; //Red LED shows
		
		//The Joystick is press down
		if((GPIOA->IDR >> 5) & 0x1)
			GPIOE->ODR |= 1<<8; //Green LED shows
		
		//Center of Joystick is pressed
		if((GPIOA->IDR >> 0) & 0x1)
		{
			//Something Cool Part (Lights Blink Continuously until Right is pressed on Joystick)
			while(1)
			{
				//Delay for a certain amount of time
				for(i = 0; i < 400000; i++)
				{
					if((GPIOA->IDR >> 2) & 0x1) //While delaying check to see if the Right Joystick is pressed
					{
						GPIOE->ODR &= 0x0; //Turn off Green LED
						GPIOB->ODR &= 0x0; //Turn off Red LED
						break; //break out of for loop
					}
				}
				
				GPIOE->ODR |= 1<<8; //turns on green LED
				GPIOB->ODR |= 1<<2; //turns on red LED
				
				//If the right button is pressed on Joystick
				if((GPIOA->IDR >> 2) & 0x1)
				{
					GPIOE->ODR &= 0x0; //Turn off Green LED
					GPIOB->ODR &= 0x0; //Turn off Red LED
					break; //Break out of While loop (stops blinking)
				}
				
				for(i = 0; i < 400000; i++) //While delaying check to see if the Right Joystick is pressed
				{
					if((GPIOA->IDR >> 2) & 0x1)
					{
						GPIOE->ODR &= 0x0; //Turn off Green LED
						GPIOB->ODR &= 0x0; //Turn off Red LED
						break; //break out of for loop
					}
				}
				
				GPIOE->ODR &= 0x0; //Turn off Green LED
				GPIOB->ODR &= 0x0; //Turn off Red LED
				
			}
		}
		
		else
		{
			GPIOE->ODR &= 0x0; //Turn off Green LED
			GPIOB->ODR &= 0x0; //Turn off Red LED
		}
		//For part 2 joystick center button
		/*
		if((GPIOA->IDR >> 0) & 0x1)
		{
			GPIOE->ODR |= 1<<8;
			GPIOB->ODR |= 1<<2;
		}
		*/
		
	}
	
	return 0;
}

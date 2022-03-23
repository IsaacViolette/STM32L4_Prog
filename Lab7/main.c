//Isaac Violette
//ECE 271
//Lab 7: System Timer and Interrupts
//3/23/22

#include <stdint.h>
#include "stm32l476xx.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>

void System_Clock_Init(void);
void SysTick_Initialize(int ticks);
void SysTick_Handler(void);
void Delay(uint32_t nTime);
void letterS(void);
void letterO(void);

/*Global Variable*/
volatile int TimeDelay;

int main(void){
	
	System_Clock_Init();
	
	SysTick_Initialize(8000);
	__ASM("CPSIE i");
	
	//Turning the clock on to be enabled for pins
	RCC->AHB2ENR |= 0x13;
	
	//Initialize the LED GPIOs to be Outputs
	//PB-2
	GPIOB->MODER &= 0xFFFFFFCF; //mask
	GPIOB->MODER |= 0x10; //value
	
	//Initialize LED GPIOS to be Push-Pull
	//Set PB2 (OT2) to zero
	GPIOB->OTYPER &= 0xFFFFFFFB; //Set
	
	//Initialize the LED GPIOs for No PullUp/PullDown
	//Set PB2 to 00
	GPIOB->PUPDR &= 0xFFFFFFCF; //Set
	
	while(1)
	{
		GPIOB->ODR |= 1<<2; //Turn on Red LED
		Delay(1000);
		GPIOB->ODR &= 0x0; //Turn off Red LED
		Delay(1000);
		
		//Something Cool
		//letterS();
		//letterO();
		//letterS();
		
		//Delay(1500);
	}

}


void letterS(void)
{
		GPIOB->ODR |= 1<<2; //Turn on Red LED
		Delay(300);
		GPIOB->ODR &= 0x0; //Turn off Red LED
		Delay(300);
		GPIOB->ODR |= 1<<2; //Turn on Red LED
		Delay(300);
		GPIOB->ODR &= 0x0; //Turn off Red LED
		Delay(300);
		GPIOB->ODR |= 1<<2; //Turn on Red LED
		Delay(300);
		GPIOB->ODR &= 0x0; //Turn off Red LED
		Delay(1500);
}

void letterO(void)
{
		GPIOB->ODR |= 1<<2; //Turn on Red LED
		Delay(1000);
		GPIOB->ODR &= 0x0; //Turn off Red LED
		Delay(300);
		GPIOB->ODR |= 1<<2; //Turn on Red LED
		Delay(1000);
		GPIOB->ODR &= 0x0; //Turn off Red LED
		Delay(300);
		GPIOB->ODR |= 1<<2; //Turn on Red LED
		Delay(1000);
		GPIOB->ODR &= 0x0; //Turn off Red LED
		Delay(1500);
}


void System_Clock_Init(void){
	
	/*Disable MSI Clock*/
	RCC->CR &= ~(RCC_CR_MSION);
	
	/*Set MSIRANGE for 8 MHz Clcok*/
	RCC->CR |= RCC_CR_MSIRANGE_7;
	
	/*Set MSIRGSEL bit to 1 for MSI clock to use new value*/
	RCC->CR |= RCC_CR_MSIRGSEL;
	
	/*Re-enable the MSI clock by setting the MSION bit back to 1*/
	RCC->CR |= RCC_CR_MSION;
	
	/*Wait for MSI Clock to be Ready*/
	while((RCC->CR & RCC_CR_MSIRDY ) == 0);
}

/*SysTick_Initialize Function came from from Dr. Yifeng Zhu*/
void SysTick_Initialize(int ticks){
	
	/*Disable SysTick Counter*/
	SysTick->CTRL = 0;
	
	/*Set the Value in the Reload Register*/
	SysTick->LOAD = ticks - 1;
	
	/*Set Interrupt priority of SysTick*/
	NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	
	/*Set SysTick value to zero*/
	SysTick->VAL = 0;
	
	/*Use the internal processor clock rather than the External Clock*/
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	
	/*Enable Interupts*/
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	/*Enable SysTick Timer*/
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void){
	/*Counts down the Delay*/
	if(TimeDelay > 0)
	{
		TimeDelay--;
	}
}

void Delay(uint32_t nTime){

	TimeDelay = nTime;
	
	/*Runs the Delay, keeps going until the TimeDelay is zero or less*/
	while(TimeDelay > 0);
}




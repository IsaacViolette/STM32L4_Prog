//Isaac Violette
//ECE 271
//Lab 8: Pulse Width Modulation and Servo Control
//3/30/22

#include <stdint.h>
#include "stm32l476xx.h"
#include <stdio.h>
#include <stdlib.h>

void System_Clock_Init(void);
void SysTick_Initialize(int ticks);
void SysTick_Handler(void);
void Delay(uint32_t nTime);
void TIM1_Init_LED(void);
void Pulse_LED(void);
void TIM1_Init_Servo(void);

/*Global Variable*/
volatile int TimeDelay;

int main(void){
	int i;
	int degree = 153;
	
	/* Comment out call to the timer function since board auto runs on 4MHz */	
	//System_Clock_Init();
	
	/* Call TIM1 Initialization */
	//TIM1_Init_LED();
	
	/* Set TM1 initlialization for Servo */
	TIM1_Init_Servo();
	
	/* Enable the Clock for LED Pins */
	RCC->AHB2ENR |= 0x13;
	
	/* Set the GPIO MODER register to be alternate function for the Green LED */
	GPIOE->MODER &= ~(GPIO_MODER_MODER8); //Mask
	GPIOE->MODER |= GPIO_MODER_MODER8_1; //Value

	/* Set GPIOE AFR for alternate function to be TIM1_CH1N */
	
	GPIOE->AFR[1] &= 0xFFFFFFF0; //MASK
	GPIOE->AFR[1] |= 0x00000001; //VALUE
	
	/* Set GPIOE OTYPER pin 8 to be Push/Pull */
	GPIOE->OTYPER = ~(GPIO_OTYPER_OT_8);
	
	/* Set GPIOE PUPDR for Pin 8 to be no pull-up/pull-down */
	GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPDR8);
	
	
	
	//Inintialize clock for GPIOA (Joystick)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
		
	//Initialize the Joystick GPIOs to be Inputs
	GPIOA->MODER &= 0xFFFFF300;
	
	//Initialize the Joystick GPIOs to be PullDown
	GPIOA->PUPDR &= 0xFFFFF300; //Mask
	GPIOA->PUPDR |= 0x8AA; //Set

	
	/* Test LED Duty Cycle */
	//GPIOE->ODR |= 1<<8;
	
	/* Test LED PWM Pule */
	//Pulse_LED();
	
	/* Rotate Servo -90, 0, 90 degrees */
	/*
	while(1)
	{
		TIM1->CCR1 = 65;								//90
		for(i = 0; i < 1000000; i++);
		TIM1->CCR1 = 153;								//0
		for(i = 0; i < 1000000; i++);
		TIM1->CCR1 = 263; 							//-90
		for(i = 0; i < 1000000; i++);
	}
	*/
	
	/*Something cool */
	TIM1->CCR1 = degree;
	while(1)
	{
		//right joystick, servo moves right
		while((GPIOA->IDR >> 2) & 0x1)
		{
			if(degree != 65)
			{
				TIM1->CCR1 = degree--;
			}
			for(i = 0; i < 3500; i++);
		}
		//left joystick, servo moves left
		while((GPIOA->IDR >> 1) & 0x1)
		{
			if(degree != 263)
			{
				TIM1->CCR1 = degree++;
			}
			for(i = 0; i < 3500; i++);

		}
	}
	
}

/* Code Used from pg 394 of Textbook */
void TIM1_Init_LED(void){
	/* Enable TIM1 clock */
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	/* Make counting direction up-counting */
	TIM1->CR1 &= ~TIM_CR1_DIR;
	
	/* Slow down the input clock by dividing the 4MHz clock down to 100kHz */
	TIM1->PSC = 39; // 4MHz / (1+39) = 100 KHz
	
	/*  Auto-reload every 1000 counts or 10ms */
	TIM1->ARR = 999; // PWM Period = (999 + 1) * 1/100kHz = 0.01s
	
	/* Clear output bits for channel 1 */
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;
	
	/* Set PWM Mode 1 output on channel 1 so OC1M = 110 */
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	
	/* Output Preload enable */
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE;
	
	/* Clear CC1NP field to select output polarity */
	TIM1->CCER &= ~TIM_CCER_CC1NP;
	
	/* Enable complementary output of Channel 1 by setting CC1NE bit */
	TIM1->CCER |= TIM_CCER_CC1NE;
	
	/* Set main output enable */
	TIM1->BDTR |= TIM_BDTR_MOE;
	
	/* Set output compare register for channel 1 to have a duty cycle of x% */
	TIM1->CCR1 = 0;
	
	/* Enable Counter */
	TIM1->CR1 |= TIM_CR1_CEN;
}

/* TIM1 Servo Inititilaization */
void TIM1_Init_Servo()
{
	/* Enable TIM1 clock */
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	/* Make counting direction up-counting */
	TIM1->CR1 &= ~TIM_CR1_DIR;
	
	/* Slow down the input clock by dividing the 4MHz clock down to 100kHz */
	TIM1->PSC = 39; // 4MHz / (1+39) = 100 KHz
	
	/*  Auto-reload every 2000 counts or 20ms */
	TIM1->ARR = 1999; // PWM Period = (1999 + 1) * 1/100kHz = 0.02s
	
	/* Clear output bits for channel 1 */
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;
	
	/* Set PWM Mode 1 output on channel 1 so OC1M = 110 */
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	
	/* Output Preload enable */
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE;
	
	/* Clear CC1NP field to select output polarity */
	TIM1->CCER &= ~TIM_CCER_CC1NP;
	
	/* Enable complementary output of Channel 1 by setting CC1NE bit */
	TIM1->CCER |= TIM_CCER_CC1NE;
	
	/* Set main output enable */
	TIM1->BDTR |= TIM_BDTR_MOE;
	
	/* Set output compare register for channel 1 to have a duty cycle of x% */
	TIM1->CCR1 = 0;
	
	/* Enable Counter */
	TIM1->CR1 |= TIM_CR1_CEN;
}

void Pulse_LED(void)
{
	int i;
	int brightness = 1;
	int stepSize = 1;
	
	/* PWM on LED */
	/* I tried making my own but never worked, This is from the book and it seems
		 to be the most effective way of it working  */
	while(1)
	{
		if((brightness >= 999) || (brightness <= 0))
			stepSize = -stepSize; //change direction
		
		brightness += stepSize; //step size increment
		
		TIM1->CCR1 = brightness; //set brightness (PWM)
		
		for(i = 0; i < 1000; i++); //short delay
	}
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
	if(TimeDelay > 0)
	{
		TimeDelay--;
	}
}

void Delay(uint32_t nTime){

	TimeDelay = nTime;
	
	while(TimeDelay > 0);
}




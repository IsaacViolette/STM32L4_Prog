//Isaac Violette
//ECE 271
//Lab 11: Digital to Analog Converter (DAC)
//4/20/22

#include <stdint.h>
#include <math.h>
#include "stm32l476xx.h"
#include "LCD.h"
#include <stdio.h>
#include <stdlib.h>
#include "sine_table.h"
#include "Notes.h"

void DAC_Channel2_Init(void);
void System_Clock_Init(void);
void TIM4_Init(void);
void play_twinkle(void);

volatile unsigned int CURRENT_ANGLE = 0;
volatile uint32_t frequency;


int main(void)
{
	DAC_Channel2_Init();
	System_Clock_Init();
	TIM4_Init();
	
	
	/* 2C Wait for hardware to be done updating the DAC */
	//while((DAC->SR & DAC_SR_BWST2) != 0);
	
	/* Set a 12-bit right-aligned value into the DAC */
	//DAC->DHR12R2 = 2048;
	
	/* Trigger an update that will move the value to the DAC */
	//DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG2;
	
	while(1)
	{
		play_twinkle();
	}
}

void System_Clock_Init(void){
	
	/*Set HSI Clock*/
	RCC->CR |= RCC_CR_HSION;
	
	/*Wait for HSI Clock to be Ready*/
	while((RCC->CR & RCC_CR_HSIRDY ) == 0);
	
	/* Make HSI the system clock */
	RCC->CFGR &= ~(RCC_CFGR_SW);		//clear bits with mask
	RCC->CFGR |= RCC_CFGR_SW_HSI;		//Set Value
}

void DAC_Channel2_Init()
{
	/* 1A Set up DAC clock */
	RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;
	
	/* 1B Disable the DAC to allow configuration */
	DAC->CR &= ~(DAC_CR_EN1);
	DAC->CR &= ~(DAC_CR_EN2);
	
	/* 1C Set up DAC channel 2 mode to be external pin with the buffer enabled */
	DAC->MCR &= ~DAC_MCR_MODE2;
	
	/* 1D Select the software trigger */
	DAC->CR |= DAC_CR_TSEL2_0;
	DAC->CR &= ~(DAC_CR_TSEL2_1);
	DAC->CR |= DAC_CR_TSEL2_2;
	
	/* 1E Enable the DAC channel 2 */
	DAC->CR |= DAC_CR_EN2;
	
	/* 1G Enable GPIOA Clock */
	RCC->CR |= RCC_AHB2ENR_GPIOAEN;
	
	/* 1H Set pin PA5 to be analog mode */
	GPIOA->MODER |= GPIO_MODER_MODER5;
}

void TIM4_Init()
{
	/* 2A Enable the TIM4 clock */
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	
	/* 2B Set edge-aligned mode */
	TIM4->CR1 &= ~(TIM_CR1_CMS);
	
	/* 2C Set the output to be trigger TRG0 */
	TIM4->CR2 &= ~(TIM_CR2_MMS);
	TIM4->CR2 |= TIM_CR2_MMS_2;
	
	/* 2D Enable the trigger and update interrupts for TIM4 */
	TIM4->DIER |= TIM_DIER_TIE;
	TIM4->DIER |= TIM_DIER_UIE;
	
	/* 2E Set PWM mode 1 output */
	TIM4->CCMR1 &=  ~(TIM_CCMR1_OC1M);
	TIM4->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
	
	/* 2F Set the prescaler */
	TIM4->PSC = 18;
	
	/* 2F Set the autoreload */
	TIM4->ARR = 18;
	
	/* 2G Set the dutycycle */
	TIM4->CCR1 = 9;
	
	TIM4->CCER |= TIM_CCER_CC1E;
	
	/* 2I  Enable the timer */
	TIM4->CR1 |= TIM_CR1_CEN;
	
	/* 2J  Enable the TIM4 interrupt in the interrupt handler */
	NVIC_SetPriority(TIM4_IRQn, 0);
	NVIC_EnableIRQ(TIM4_IRQn);
}

int lookup_sine(int angle)
{
		angle = angle % 360;
	//lookup up the angle in sine_lookup for every possible angle
	if(angle < 90)
		return sine_lookup[angle];
	if(angle < 180)
			return sine_lookup[180-angle];
	if(angle < 270)
		return 4096 - sine_lookup[angle-180];
	
	return 4096 - sine_lookup[360-angle];
}

//Play twinkle Twinkle little star
void play_twinkle(void)
{
	int i, j, duration;
	
	for(i = 0; i < NOTES; i++)
	{
		/* To play the proper frequency, calculate the stepsize */
		frequency = 44300000/twinkle_freq[i];
		duration = twinkle_len[i];
		CURRENT_ANGLE = 0;
		for(j = 0; j < 750000*duration; j++); //delay for longer time base for note
	}
}


void TIM4_IRQHandler() 
{
	uint32_t stepSize=360*1000000/frequency;
	
	if((TIM4->SR & TIM_SR_CC1IF) != 0)
	{
		
		CURRENT_ANGLE = (CURRENT_ANGLE + stepSize) % 360000;
		
		DAC->DHR12R2 = lookup_sine(CURRENT_ANGLE/1000); //Set value for DAC regsiter
		
		//Clear the flag to acknowledge interrupt
		TIM4->SR &= ~TIM_SR_CC1IF;
	}
	//Acknowledge flag
	if((TIM4->SR & TIM_SR_UIF) != 0)
		TIM4->SR &= ~TIM_SR_UIF;
}



//440 Hz sin wave
/*
void TIM4_IRQHandler() 
{
	//int stepSize = 3576;
	if((TIM4->SR & TIM_SR_CC1IF) != 0)
	{
		
		CURRENT_ANGLE += stepSize;
		
		DAC->DHR12R2 = lookup_sine(CURRENT_ANGLE/1000);
		
		if(CURRENT_ANGLE >= 360000)
			CURRENT_ANGLE = 0;
		
		TIM4->SR &= ~TIM_SR_CC1IF;
	}
	
	if((TIM4->SR & TIM_SR_UIF) != 0)
		TIM4->SR &= ~TIM_SR_UIF;
}
*/




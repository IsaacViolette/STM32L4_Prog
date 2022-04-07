//Isaac Violette
//ECE 271
//Lab 9: Input Capture and Distance Sensor
//4/06/22

#include <stdint.h>
#include "stm32l476xx.h"
#include "LCD.h"
#include <stdio.h>
#include <stdlib.h>

void System_Clock_Init(void);
void TIM4_CH1_Init(void);
void TIM4_IRQHandler(void);
void TIM1_CH2_Init(void);

volatile uint32_t pulse_width = 0;
volatile uint32_t last_captured = 0;
volatile uint32_t signal_polarity = 0;
volatile uint32_t num_of_overflows = 0;
volatile int distance_in = 0;
volatile uint32_t distance_cm = 0;


int main(void){
	char inches[7] = {' ',' ',' ',' ',' ',' '};
	char inches2[2] = {' ',' '};
	
	System_Clock_Init();
	TIM4_CH1_Init();
	TIM1_CH2_Init();
	
	LCD_Clock_Init();
	LCD_Pin_Init();
	LCD_Configure();

	
	while(1)
	{
		distance_in = pulse_width/148; //calculate distance in inches
		distance_cm = pulse_width/58;  //claculate distance in cm
		
		sprintf(inches, "%d", distance_in); //convert distance to an array of chars
		
		inches2[0] = inches[0];
		inches2[1] = inches[1];
		
		LCD_Display_String(inches2); //print to LCD screen
		
		inches[1] = ' '; //reset number 2 in buffer
		inches2[1] = ' '; //reset number 2 in buffer
		
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

void TIM4_CH1_Init(void)
{
	/* Enable TIM4 Clock for GPIO port B */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	/* Set GPIOB pin 6 to be alternate function */
	GPIOB->MODER &= ~(GPIO_MODER_MODER6);
	GPIOB->MODER |= GPIO_MODER_MODER6_1;
	
	/* Setting GPIOB pin 6 to Alternate Function for TIM4 */
	GPIOB->AFR[0] &= 0xF0FFFFFF;
	GPIOB->AFR[0] |= 0x02000000;
	
	
	/* Enable clock for timer 4 */
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	
	/* Set PSC to slow down 16Mhz clcok to 1 MHz */
	TIM4->PSC = 15;
	
	/* Set ARR to the maximum 16-bit value */
	TIM4->ARR = 65535;
	
	/* Set the direction of channel 1 as input and set input to 1 */
	TIM4->CCMR1 &= ~(TIM_CCMR1_CC1S); //MASK
	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;	//SET
	
	/*Set the input filter duration to 0 */
	TIM4->CCMR1 &= ~(TIM_CCMR1_IC1F);
	
	/* Set the capture to be on both rising and falling */
	TIM4->CCER |= TIM_CCER_CC1P | TIM_CCER_CC1NP;
	
	/* Clear the prescaler to capture each transition */
	TIM4->CCMR1 &= ~(TIM_CCMR1_IC1PSC);
	
	/* Enable capture for channel 1 */
	TIM4->CCER |= TIM_CCER_CC1E;
	
	/* Enable capture interrupt generation for TIM4 channel 1 */
	TIM4->DIER |= TIM_DIER_CC1IE;
	
	/* Enable overflow interupt */
	TIM4->DIER |= TIM_DIER_UIE;
	
	/* Enable timer 4 */
	TIM4->CR1 |= TIM_CR1_CEN;
	
	/* Set priority of interrupt to 0 or highest urgency */
	NVIC_SetPriority(TIM4_IRQn, 0);
	
	/* Enable the Timer 4 interrupt in the interrupt controller */
	NVIC_EnableIRQ(TIM4_IRQn);
}

void TIM1_CH2_Init(void)
{
	/* Enable clock for timer 1 */
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	/* Enable GPIOE Clock */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	
	/* Set PE11 to be alternate mode */
	GPIOE->MODER &= ~(GPIO_MODER_MODER11); //MASK
	GPIOE->MODER |= GPIO_MODER_MODER11_1;  //VALUE
	
	/* Set alternate function of Pin 11 to be TIM1_CH2 */
	GPIOE->AFR[1] &= 0xFFFF0FFF;	//MASK
	GPIOE->AFR[1] |= 0x00001000;	//VALUE
	
	/* Set Pin 11 to be Push-Pull */
	GPIOE->OTYPER &= ~(GPIO_OTYPER_OT_11);
	
	/* Set Pin 11 for no pull-up/pull-down */
	GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPDR11);
	
	/* Setting the counting direction up */
	TIM1->CR1 &= ~(TIM_CR1_DIR);
	
	/* Set prescaler for TIM1->PSC to count from 16MHz to 1MHz */
	TIM1->PSC = 15;
	
	/* Make the maximum possible period */
	TIM1->ARR = 65535;
	
	/* Clear the OC2M field and select PWM Mode 1 */
	TIM1->CCMR1 &= ~(TIM_CCMR1_OC2M);	//MASK
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;	//MASK
	
	/* Enable Output 2 preload enable */
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;
	
	/* Select the output polarity by clearing CC2P field */
	TIM1->CCER &= ~(TIM_CCER_CC2P);
	
	/* Enable the output of Channel 2 */
	TIM1->CCER |= TIM_CCER_CC2E;
	
	/* Set the main output enable */
	TIM1->BDTR |= TIM_BDTR_MOE;
	
	/* Set output compare register for 10us for each period */
	TIM1->CCR2 = 10;
	
	/* Enable the counter for pin 11 */
	TIM1->CR1 |= TIM_CR1_CEN;
	
}

void TIM4_IRQHandler(void)
{
	uint32_t current_captured;
	
	/* check if interrupt flag is set */
	if((TIM4->SR & TIM_SR_CC1IF) != 0)
	{
		//Reading CCR1 clears CC1IF interrupt flag
		current_captured = TIM4->CCR1;
		
		/* Toggle the polarity flag */
		signal_polarity = 1 - signal_polarity;
		
		/* Calculate when current output is low */
		if(signal_polarity == 0)
				pulse_width = current_captured - last_captured + (num_of_overflows * 65535);
		else
			num_of_overflows = 0;
		
		last_captured = current_captured;
	}
	
	/* Check if overflow has taken place */
	if((TIM4->SR & TIM_SR_UIF) != 0)
	{
		/* Clear UIF flag to avoid re-entry */
		TIM4->SR &= ~(TIM_SR_UIF);
		
		/* Number of overflows */
		num_of_overflows++;
		
	}
}




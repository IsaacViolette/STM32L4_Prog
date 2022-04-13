//Isaac Violette
//ECE 271
//Lab 10: Analog to Digital Converter
//4/13/22

#include <stdint.h>
#include "stm32l476xx.h"
#include "LCD.h"
#include <stdio.h>
#include <stdlib.h>


void System_Clock_Init(void);
void ADC1_Wakeup(void);
void ADC_Init(void);

volatile int VALUE;

int main(void){
	char distance[7] = {' ',' ',' ',' ',' ',' '};
	
	System_Clock_Init();
	ADC1_Wakeup();
	ADC_Init();
	
	LCD_Clock_Init();
	LCD_Pin_Init();
	LCD_Configure();
	
	while(1)
	{
		ADC1->CR |= ADC_CR_ADSTART;
		while((ADC123_COMMON->CSR & ADC_CSR_EOC_MST) == 0);
		VALUE = ADC1->DR;
		
		sprintf(distance, "%d", VALUE);
		
		LCD_Display_String(distance);
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

void ADC1_Wakeup(void){
	int wait_time;
	
	/* DEEPPWD = 0: ADC not in deep-power down */
	/* DEEPPWD = 1: ADC in deep-power-down (default state) */
	if((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD)
		ADC1->CR &= ~(ADC_CR_DEEPPWD); //Exit deep power down mode if still in that state
	
	/* Enable the ADC internal voltage regulator */
	ADC1->CR |= ADC_CR_ADVREGEN;
	
	wait_time = 20 * (80000000 / 1000000);
	
	/* Wait for ADC Voltage regulator start-up time */
	while(wait_time != 0)
		wait_time--;
}

void ADC_Init(void){
	//Pin A1 Initialize
	/* Set clock for GPIOA */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	/* Set value for pin 1 to be analog */
	GPIOA->MODER |= GPIO_MODER_MODER1;
	
	/* Hook analog pin to the ADC */
	GPIOA->ASCR |= GPIO_ASCR_EN_1;
	
	
	/*A) Enable ADC Clock Bit */
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	
	/*B) Disable ADC1 */
	ADC1->CR &= ~(ADC_CR_ADEN);
	
	/*C) Enable the I/O analog switches voltage booster */
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;
	
	/*D) Enable conversion of internal channels */
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;
	
	/*E) Configure the ADC prescaler to have the clock not divided */
	ADC123_COMMON->CCR &= ~(ADC_CCR_PRESC);
	
	/*F) Select synchronous clock mode (HCLK/1) */
	 ADC123_COMMON->CCR &= ~(ADC_CCR_CKMODE);
	 ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;
	
	/*G) Configure all ADCs as independent */
	ADC123_COMMON->CCR &= ~(ADC_CCR_DUAL);
	
	/*H) CALL WAKEUP FUNCTION */
	ADC1_Wakeup();
	
	/*I) Configure the ADC to have 12-bit resolution */
	ADC1->CFGR &= ~(ADC_CFGR_RES);
	
	/*J) Set right-alignment of the 12-bit result inside the results register */
	ADC1->CFGR &= ~(ADC_CFGR_ALIGN);
	
	/*K,L,M) Select 1 conversion in the regular channel conversion sequence with by clearing */
	ADC1->SQR1 &= ~(ADC_SQR1_L);
	ADC1->SQR1 &= ~(ADC_SQR1_SQ1);
	ADC1->SQR1 |=  (6U << 6);
	ADC1->DIFSEL &= ~(ADC_DIFSEL_DIFSEL_6);
	
	/*N) Select the ADC sampling time */
	ADC1->SMPR1 &= ~(ADC_SMPR1_SMP6);
	ADC1->SMPR1 |= ADC_SMPR1_SMP6_0;
	
	/*O) Select ADC to discontinuous mode */
	ADC1->CFGR &= ~(ADC_CFGR_CONT);
	
	/*P) Select software trigger */
	ADC1->CFGR &= ~(ADC_CFGR_EXTEN);
	
	/*Q) Select software trigger */
	ADC1->CR |= ADC_CR_ADEN;
	
	/*R) Select software trigger */
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0);
}


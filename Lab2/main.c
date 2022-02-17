//Isaac Violette
//ECE 271
//Lab 2: The Liquid Crystal Display (LCD)
//2/03/22

#include <stdint.h>
#include "stm32l476xx.h"
#include "lcd.h"

void System_Clock_Init(void);

int main(void){
	
	//USED FOR TEST LCD FUNCTION FOR LOOP DELAY
	//int i;
	
	System_Clock_Init();

	LCD_Clock_Init();

	LCD_Pin_Init();

	LCD_Configure();
	
	//USED IN LAB 2: PART A
	//LCD_Display_Name(); 
	
	//USED IN LAB 2: PART B
	//LCD_Display_String("ISV");
	
	//USED IN LAB 2: PART C
	LCD_Display_Long("Electric");
	
	//TEST CLEAR LCD FUNCTION
	//for(i = 0; i < 10000000; i++); 
	//LCD_Clear();
	
	
	/* Loop forever */
	while(1);

}

void System_Clock_Init(void){

	/* Enable the HSI clock */
	RCC->CR |= RCC_CR_HSION;

	/* Wait until HSI is ready */
	while ( (RCC->CR & RCC_CR_HSIRDY) == 0 );

	/* Select HSI as system clock source  */
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;  /* 01: HSI16 oscillator used as system clock */

	/* Wait till HSI is used as system clock source */
	while ((RCC->CFGR & RCC_CFGR_SWS) == 0 );

}


//Isaac Violette
//ECE 271
//Lab 5: Stepper Motor in C
//3/2/22

#include <stdint.h>
#include "stm32l476xx.h"
#include "lcd.h"

void System_Clock_Init(void);
unsigned char keypad_scan(void);
void Keypad_Pin_Init(void);
void Stepper_Pin_Init(void);
void Stepper_Full_Step(int angle);
void Stepper_Half_Step(int angle);

int main(void){
	//char print[7] = {' ',' ',' ',' ',' ',' '}; //inizialize buffer
	//int i = 0; //index initializer for place in buffer
	//int k; //used to move every value in buffer back one in the display
	//unsigned char key; //used to implement # sign 
	
	System_Clock_Init();

	LCD_Clock_Init();

	LCD_Pin_Init();

	LCD_Configure();
	
	Keypad_Pin_Init();
	
	Stepper_Pin_Init();
	
	Stepper_Full_Step(180);
	Stepper_Half_Step(180);
	
	//USED IN LAB 2: PART A
	//LCD_Display_Name(); 
	
	//USED IN LAB 2: PART B
	//LCD_Display_String("ISV");
	
	//USED IN LAB 2: PART C
	//LCD_Display_Long("Electric");
	
	/* Loop forever */
	//Testing Lab 3 Part 2 
	 /*while(1){
		print = keypad_scan();
		LCD_Display_String(&print); //print value
	 }
	*/
	//LCD_Clear(); //clear LCD to start
	
	/*
	while(1)
	{
		
		key = keypad_scan();
		
		if(key != 0xFF)//if the keypad is pressed
		{
			if(key == '#')//something cool: repeat previous input with #
				print[i] = print[i-1]; //set last input to current input
			else
				print[i] = key;//set keypadscan to a certain value in an array
			while(1)
			{
				if(keypad_scan() == 0xFF)//software debouncer to print only one value
					break;//break out of loop to display string
			}
				
			LCD_Display_String(print);//place value on certain spot on LCD
			i++;//move to next spot on LCD
			
			if(i == 6)//when the index of the array is 6
			{
				i = 5;//set to 5
				for(k = 1; k < 6; k++)
				{
					print[k-1] = print[k];//move each index back one to display new input on index 6
				}
			}
			
		}
	}
	*/
}

void Keypad_Pin_Init(void)
{
	//Set Clock for GPIOA+E
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOEEN);
	//set GPIOA and GPIOE pins
	GPIOA->MODER &= ~(GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3 | GPIO_MODER_MODER5);
	GPIOE->MODER &= ~(GPIO_MODER_MODER10 | GPIO_MODER_MODER11 | GPIO_MODER_MODER12 | GPIO_MODER_MODER13);
	GPIOE->MODER |= (GPIO_MODER_MODER10_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0);
}

//key map used to print each button that is pressed
unsigned char key_map [4][4] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}		
};

unsigned char keypad_scan(void)
{
		//variables
		char column;
		char row = 0;
		int i;
		
		//clear the GPIOE bits
		GPIOE->ODR &= ~(GPIO_ODR_ODR_10 | GPIO_ODR_ODR_11 | GPIO_ODR_ODR_12 | GPIO_ODR_ODR_13);
		for(i = 0; i < 50000; i++); //delay
	
		//check if nothing is pressed
		if(((GPIOA->IDR & (GPIO_IDR_IDR_1 | GPIO_IDR_IDR_2 | GPIO_IDR_IDR_3 | GPIO_IDR_IDR_5))) == (GPIO_IDR_IDR_1 | GPIO_IDR_IDR_2 | GPIO_IDR_IDR_3 | GPIO_IDR_IDR_5))
			return 0xFF;	
	
		//check each column
		//column 1
		if((GPIOA->IDR & GPIO_IDR_IDR_1) == 0)
			column = 0;
		//column 2
		if((GPIOA->IDR & GPIO_IDR_IDR_2) == 0)
			column = 1;
		//column 3
		if((GPIOA->IDR & GPIO_IDR_IDR_3) == 0)
			column = 2;
		//column 4
		if((GPIOA->IDR & GPIO_IDR_IDR_5) == 0)
			column = 3;
		
		//check the rows
		//row 1
		GPIOE->ODR &= ~(GPIO_ODR_ODR_10);
		GPIOE->ODR |= (GPIO_ODR_ODR_11 | GPIO_ODR_ODR_12 | GPIO_ODR_ODR_13);
		for(i = 0; i < 10000; i++);
		if((GPIOA->IDR & (GPIO_IDR_IDR_1 | GPIO_IDR_IDR_2 | GPIO_IDR_IDR_3 | GPIO_IDR_IDR_5)) != 0x2E)
			row = 0;
		
		//row 2
		GPIOE->ODR &= ~(GPIO_ODR_ODR_11);
		GPIOE->ODR |= (GPIO_ODR_ODR_10 | GPIO_ODR_ODR_12 | GPIO_ODR_ODR_13);
		for(i = 0; i < 10000; i++);
		if((GPIOA->IDR & (GPIO_IDR_IDR_1 | GPIO_IDR_IDR_2 | GPIO_IDR_IDR_3 | GPIO_IDR_IDR_5)) != 0x2E)
			row = 1;
		
		//row 3
		GPIOE->ODR &= ~(GPIO_ODR_ODR_12);
		GPIOE->ODR |= (GPIO_ODR_ODR_10 | GPIO_ODR_ODR_11 | GPIO_ODR_ODR_13);
		for(i = 0; i < 10000; i++);
		if((GPIOA->IDR & (GPIO_IDR_IDR_1 | GPIO_IDR_IDR_2 | GPIO_IDR_IDR_3 | GPIO_IDR_IDR_5)) != 0x2E)
			row = 2;
		
		//row 4
		GPIOE->ODR &= ~(GPIO_ODR_ODR_13);
		GPIOE->ODR |= (GPIO_ODR_ODR_10 | GPIO_ODR_ODR_11 | GPIO_ODR_ODR_12);
		for(i = 0; i < 10000; i++);
		if((GPIOA->IDR & (GPIO_IDR_IDR_1 | GPIO_IDR_IDR_2 | GPIO_IDR_IDR_3 | GPIO_IDR_IDR_5)) != 0x2E)
			row = 3;
		
		return key_map[row][column];//return row and column value for 2d array function
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

void Stepper_Pin_Init(void)
{
	//intitialize GPIOB Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	//Set GPIOB pin 2,3,6,7 as digital outputs
	GPIOB->MODER &= ~(GPIO_MODER_MODER2);
	GPIOB->MODER &= ~(GPIO_MODER_MODER3);
	GPIOB->MODER &= ~(GPIO_MODER_MODER6);
	GPIOB->MODER &= ~(GPIO_MODER_MODER7);
	
	GPIOB->MODER |= (GPIO_MODER_MODER2_0);
	GPIOB->MODER |= (GPIO_MODER_MODER3_0);
	GPIOB->MODER |= (GPIO_MODER_MODER6_0);
	GPIOB->MODER |= (GPIO_MODER_MODER7_0);
}

void Stepper_Full_Step(int angle)
{
	int i;
	int delay;
	int rotation = ((angle)*512.0)/360;
	
	for(i = 0; i < rotation; i++)
	{
		//Step 0
		GPIOB->BSRR |= GPIO_BSRR_BR_6;
		GPIOB->BSRR |= GPIO_BSRR_BR_3;
		GPIOB->BSRR |= GPIO_BSRR_BS_7;
		GPIOB->BSRR |= GPIO_BSRR_BS_2;
		for(delay = 0; delay < 6000; delay++);
		
		//Step 1
		GPIOB->BSRR |= GPIO_BSRR_BR_7;
		GPIOB->BSRR |= GPIO_BSRR_BR_3;
		GPIOB->BSRR |= GPIO_BSRR_BS_6;
		GPIOB->BSRR |= GPIO_BSRR_BS_2;
		for(delay = 0; delay < 6000; delay++);
		
		//Step 2
		GPIOB->BSRR |= GPIO_BSRR_BR_7;
		GPIOB->BSRR |= GPIO_BSRR_BR_2;
		GPIOB->BSRR |= GPIO_BSRR_BS_6;
		GPIOB->BSRR |= GPIO_BSRR_BS_3;
		for(delay = 0; delay < 6000; delay++);
		
		//Step 3
		GPIOB->BSRR |= GPIO_BSRR_BR_6;
		GPIOB->BSRR |= GPIO_BSRR_BR_2;
		GPIOB->BSRR |= GPIO_BSRR_BS_7;
		GPIOB->BSRR |= GPIO_BSRR_BS_3;
		for(delay = 0; delay < 6000; delay++);
	}
}
	
void Stepper_Half_Step(int angle)
{
	int i;
	int delay;
	int rotation = ((angle)*512.0)/360;
	
	for(i = 0; i < rotation; i++)
	{
		//Step 0
		GPIOB->BSRR |= GPIO_BSRR_BR_6;
		GPIOB->BSRR |= GPIO_BSRR_BR_3;
		GPIOB->BSRR |= GPIO_BSRR_BS_7;
		GPIOB->BSRR |= GPIO_BSRR_BS_2;
		for(delay = 0; delay < 3000; delay++);
		
		//Step 1
		GPIOB->BSRR |= GPIO_BSRR_BR_7;
		GPIOB->BSRR |= GPIO_BSRR_BR_6;
		GPIOB->BSRR |= GPIO_BSRR_BR_3;
		GPIOB->BSRR |= GPIO_BSRR_BS_2;
		for(delay = 0; delay < 3000; delay++);
		
		//Step 2
		GPIOB->BSRR |= GPIO_BSRR_BR_7;
		GPIOB->BSRR |= GPIO_BSRR_BR_2;
		GPIOB->BSRR |= GPIO_BSRR_BS_6;
		GPIOB->BSRR |= GPIO_BSRR_BS_2;
		for(delay = 0; delay < 3000; delay++);
		
		//Step 3
		GPIOB->BSRR |= GPIO_BSRR_BR_7;
		GPIOB->BSRR |= GPIO_BSRR_BR_3;
		GPIOB->BSRR |= GPIO_BSRR_BR_2;
		GPIOB->BSRR |= GPIO_BSRR_BS_6;
		for(delay = 0; delay < 3000; delay++);
		
		//Step 4
		GPIOB->BSRR |= GPIO_BSRR_BR_7;
		GPIOB->BSRR |= GPIO_BSRR_BR_2;
		GPIOB->BSRR |= GPIO_BSRR_BS_6;
		GPIOB->BSRR |= GPIO_BSRR_BS_3;
		for(delay = 0; delay < 3000; delay++);
		
		//Step 5
		GPIOB->BSRR |= GPIO_BSRR_BR_7;
		GPIOB->BSRR |= GPIO_BSRR_BR_6;
		GPIOB->BSRR |= GPIO_BSRR_BR_2;
		GPIOB->BSRR |= GPIO_BSRR_BS_3;
		for(delay = 0; delay < 3000; delay++);
		
		//Step 6
		GPIOB->BSRR |= GPIO_BSRR_BR_6;
		GPIOB->BSRR |= GPIO_BSRR_BR_2;
		GPIOB->BSRR |= GPIO_BSRR_BS_7;
		GPIOB->BSRR |= GPIO_BSRR_BS_3;
		for(delay = 0; delay < 3000; delay++);
		
		//Step 7
		GPIOB->BSRR |= GPIO_BSRR_BR_6;
		GPIOB->BSRR |= GPIO_BSRR_BR_3;
		GPIOB->BSRR |= GPIO_BSRR_BR_2;
		GPIOB->BSRR |= GPIO_BSRR_BS_7;
		for(delay = 0; delay < 3000; delay++);
		
	}
}



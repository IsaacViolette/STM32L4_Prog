//Isaac Violette
//ECE 271
//Lab 5: Stepper Motor in C
//3/2/22

#include <stdint.h>
#include "stm32l476xx.h"
#include "lcd.h"

void System_Clock_Init(void);
void Stepper_Pin_Init(void);
void Stepper_Full_Step(int angle);
void Stepper_Half_Step(int angle);
void Something_Cool(void);

int main(void){
	
	System_Clock_Init();
	
	Stepper_Pin_Init();
	
	LCD_Clock_Init();
	
	LCD_Pin_Init();

	LCD_Configure();

	
	//Lab 5: Half and Full Step Parts
	//Stepper_Full_Step(180);
	//Stepper_Half_Step(180);
	
	Something_Cool();
	
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

//Full Step
void Stepper_Full_Step(int angle)
{
	int i;
	int delay;
	int rotation = ((angle)*512.0)/360; //claculates each degree to rotate
	
	for(i = 0; i < rotation; i++) //rotates each degree until the condition is hit
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

//Half Step
void Stepper_Half_Step(int angle)
{
	int i;
	int delay;
	int rotation = ((angle)*512.0)/360; //calculates each degree
	
	for(i = 0; i < rotation; i++) //moves each degree until the condition is hit
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

void Something_Cool(void)
{
	//Inintialize clock for GPIOA (Joystick)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
		
	//Initialize the Joystick GPIOs to be Inputs
	GPIOA->MODER &= 0xFFFFF300;
	
	//Initialize the Joystick GPIOs to be PullDown
	GPIOA->PUPDR &= 0xFFFFF300; //Mask
	GPIOA->PUPDR |= 0x8AA; //Set

	while(1)
	{
			LCD_Display_String("      "); //Sets the initial LCD Screen to be blank
			
			//The Joystick is held right
			while((GPIOA->IDR >> 2) & 0x1)
			{
				int i;
				LCD_Display_String("RIGHT");
				//Runs through half Step and moves clockwise
				//Step 0
				GPIOB->BSRR |= GPIO_BSRR_BR_6;
				GPIOB->BSRR |= GPIO_BSRR_BR_3;
				GPIOB->BSRR |= GPIO_BSRR_BS_7;
				GPIOB->BSRR |= GPIO_BSRR_BS_2;
				for(i = 0; i < 3000; i++);
				
				//Step 1
				GPIOB->BSRR |= GPIO_BSRR_BR_7;
				GPIOB->BSRR |= GPIO_BSRR_BR_6;
				GPIOB->BSRR |= GPIO_BSRR_BR_3;
				GPIOB->BSRR |= GPIO_BSRR_BS_2;
				for(i = 0; i < 3000; i++);
				
				//Step 2
				GPIOB->BSRR |= GPIO_BSRR_BR_7;
				GPIOB->BSRR |= GPIO_BSRR_BR_2;
				GPIOB->BSRR |= GPIO_BSRR_BS_6;
				GPIOB->BSRR |= GPIO_BSRR_BS_2;
				for(i = 0; i < 3000; i++);
				
				//Step 3
				GPIOB->BSRR |= GPIO_BSRR_BR_7;
				GPIOB->BSRR |= GPIO_BSRR_BR_3;
				GPIOB->BSRR |= GPIO_BSRR_BR_2;
				GPIOB->BSRR |= GPIO_BSRR_BS_6;
				for(i = 0; i < 3000; i++);
				
				//Step 4
				GPIOB->BSRR |= GPIO_BSRR_BR_7;
				GPIOB->BSRR |= GPIO_BSRR_BR_2;
				GPIOB->BSRR |= GPIO_BSRR_BS_6;
				GPIOB->BSRR |= GPIO_BSRR_BS_3;
				for(i = 0; i < 3000; i++);
				
				//Step 5
				GPIOB->BSRR |= GPIO_BSRR_BR_7;
				GPIOB->BSRR |= GPIO_BSRR_BR_6;
				GPIOB->BSRR |= GPIO_BSRR_BR_2;
				GPIOB->BSRR |= GPIO_BSRR_BS_3;
				for(i = 0; i < 3000; i++);
				
				//Step 6
				GPIOB->BSRR |= GPIO_BSRR_BR_6;
				GPIOB->BSRR |= GPIO_BSRR_BR_2;
				GPIOB->BSRR |= GPIO_BSRR_BS_7;
				GPIOB->BSRR |= GPIO_BSRR_BS_3;
				for(i = 0; i < 3000; i++);
				
				//Step 7
				GPIOB->BSRR |= GPIO_BSRR_BR_6;
				GPIOB->BSRR |= GPIO_BSRR_BR_3;
				GPIOB->BSRR |= GPIO_BSRR_BR_2;
				GPIOB->BSRR |= GPIO_BSRR_BS_7;
				for(i = 0; i < 3000; i++);
			}
			
			//The Joystick is held left
			while((GPIOA->IDR >> 1) & 0x1)
			{
				int i;
				LCD_Display_String("LEFT");
				//Runs the half step and moves counterclockwise
				//Step 7
				GPIOB->BSRR |= GPIO_BSRR_BR_6;
				GPIOB->BSRR |= GPIO_BSRR_BR_3;
				GPIOB->BSRR |= GPIO_BSRR_BR_2;
				GPIOB->BSRR |= GPIO_BSRR_BS_7;
				for(i = 0; i < 3000; i++);
				
				//Step 6
				GPIOB->BSRR |= GPIO_BSRR_BR_6;
				GPIOB->BSRR |= GPIO_BSRR_BR_2;
				GPIOB->BSRR |= GPIO_BSRR_BS_7;
				GPIOB->BSRR |= GPIO_BSRR_BS_3;
				for(i = 0; i < 3000; i++);
				
				//Step 5
				GPIOB->BSRR |= GPIO_BSRR_BR_7;
				GPIOB->BSRR |= GPIO_BSRR_BR_6;
				GPIOB->BSRR |= GPIO_BSRR_BR_2;
				GPIOB->BSRR |= GPIO_BSRR_BS_3;
				for(i = 0; i < 3000; i++);
				
				//Step 4
				GPIOB->BSRR |= GPIO_BSRR_BR_7;
				GPIOB->BSRR |= GPIO_BSRR_BR_2;
				GPIOB->BSRR |= GPIO_BSRR_BS_6;
				GPIOB->BSRR |= GPIO_BSRR_BS_3;
				for(i = 0; i < 3000; i++);
				
				//Step 3
				GPIOB->BSRR |= GPIO_BSRR_BR_7;
				GPIOB->BSRR |= GPIO_BSRR_BR_3;
				GPIOB->BSRR |= GPIO_BSRR_BR_2;
				GPIOB->BSRR |= GPIO_BSRR_BS_6;
				for(i = 0; i < 3000; i++);
				
				//Step 2
				GPIOB->BSRR |= GPIO_BSRR_BR_7;
				GPIOB->BSRR |= GPIO_BSRR_BR_2;
				GPIOB->BSRR |= GPIO_BSRR_BS_6;
				GPIOB->BSRR |= GPIO_BSRR_BS_2;
				for(i = 0; i < 3000; i++);
				
				//Step 1
				GPIOB->BSRR |= GPIO_BSRR_BR_7;
				GPIOB->BSRR |= GPIO_BSRR_BR_6;
				GPIOB->BSRR |= GPIO_BSRR_BR_3;
				GPIOB->BSRR |= GPIO_BSRR_BS_2;
				for(i = 0; i < 3000; i++);
				
				//Step 0
				GPIOB->BSRR |= GPIO_BSRR_BR_6;
				GPIOB->BSRR |= GPIO_BSRR_BR_3;
				GPIOB->BSRR |= GPIO_BSRR_BS_7;
				GPIOB->BSRR |= GPIO_BSRR_BS_2;
				for(i = 0; i < 3000; i++);
			}
			
			//The Joystick is pressed down
			while(((GPIOA->IDR >> 5) & 0x1))
			{
				while(1)
				{
					int i;
					LCD_Display_String("DOWN");
					//Runs half step counter clcokwise infinitely 
					//Step 7
					GPIOB->BSRR |= GPIO_BSRR_BR_6;
					GPIOB->BSRR |= GPIO_BSRR_BR_3;
					GPIOB->BSRR |= GPIO_BSRR_BR_2;
					GPIOB->BSRR |= GPIO_BSRR_BS_7;
					for(i = 0; i < 3000; i++);
					
					//Step 6
					GPIOB->BSRR |= GPIO_BSRR_BR_6;
					GPIOB->BSRR |= GPIO_BSRR_BR_2;
					GPIOB->BSRR |= GPIO_BSRR_BS_7;
					GPIOB->BSRR |= GPIO_BSRR_BS_3;
					for(i = 0; i < 3000; i++);
					
					//Step 5
					GPIOB->BSRR |= GPIO_BSRR_BR_7;
					GPIOB->BSRR |= GPIO_BSRR_BR_6;
					GPIOB->BSRR |= GPIO_BSRR_BR_2;
					GPIOB->BSRR |= GPIO_BSRR_BS_3;
					for(i = 0; i < 3000; i++);
					
					//Step 4
					GPIOB->BSRR |= GPIO_BSRR_BR_7;
					GPIOB->BSRR |= GPIO_BSRR_BR_2;
					GPIOB->BSRR |= GPIO_BSRR_BS_6;
					GPIOB->BSRR |= GPIO_BSRR_BS_3;
					for(i = 0; i < 3000; i++);
					
					//Step 3
					GPIOB->BSRR |= GPIO_BSRR_BR_7;
					GPIOB->BSRR |= GPIO_BSRR_BR_3;
					GPIOB->BSRR |= GPIO_BSRR_BR_2;
					GPIOB->BSRR |= GPIO_BSRR_BS_6;
					for(i = 0; i < 3000; i++);
					
					//Step 2
					GPIOB->BSRR |= GPIO_BSRR_BR_7;
					GPIOB->BSRR |= GPIO_BSRR_BR_2;
					GPIOB->BSRR |= GPIO_BSRR_BS_6;
					GPIOB->BSRR |= GPIO_BSRR_BS_2;
					for(i = 0; i < 3000; i++);
					
					//Step 1
					GPIOB->BSRR |= GPIO_BSRR_BR_7;
					GPIOB->BSRR |= GPIO_BSRR_BR_6;
					GPIOB->BSRR |= GPIO_BSRR_BR_3;
					GPIOB->BSRR |= GPIO_BSRR_BS_2;
					for(i = 0; i < 3000; i++);
					
					//Step 0
					GPIOB->BSRR |= GPIO_BSRR_BR_6;
					GPIOB->BSRR |= GPIO_BSRR_BR_3;
					GPIOB->BSRR |= GPIO_BSRR_BS_7;
					GPIOB->BSRR |= GPIO_BSRR_BS_2;
					for(i = 0; i < 3000; i++);
					
					if((GPIOA->IDR >> 0) & 0x1) //Stops when center button is pressed
						break;
				}
			}
			
			while((GPIOA->IDR >> 3) & 0x1)
			{
				while(1)
				{
					int i;
					LCD_Display_String("UP");
					//Half step moving clockwise infinitely
					//Step 0
					GPIOB->BSRR |= GPIO_BSRR_BR_6;
					GPIOB->BSRR |= GPIO_BSRR_BR_3;
					GPIOB->BSRR |= GPIO_BSRR_BS_7;
					GPIOB->BSRR |= GPIO_BSRR_BS_2;
					for(i = 0; i < 3000; i++);
					
					//Step 1
					GPIOB->BSRR |= GPIO_BSRR_BR_7;
					GPIOB->BSRR |= GPIO_BSRR_BR_6;
					GPIOB->BSRR |= GPIO_BSRR_BR_3;
					GPIOB->BSRR |= GPIO_BSRR_BS_2;
					for(i = 0; i < 3000; i++);
					
					//Step 2
					GPIOB->BSRR |= GPIO_BSRR_BR_7;
					GPIOB->BSRR |= GPIO_BSRR_BR_2;
					GPIOB->BSRR |= GPIO_BSRR_BS_6;
					GPIOB->BSRR |= GPIO_BSRR_BS_2;
					for(i = 0; i < 3000; i++);
					
					//Step 3
					GPIOB->BSRR |= GPIO_BSRR_BR_7;
					GPIOB->BSRR |= GPIO_BSRR_BR_3;
					GPIOB->BSRR |= GPIO_BSRR_BR_2;
					GPIOB->BSRR |= GPIO_BSRR_BS_6;
					for(i = 0; i < 3000; i++);
					
					//Step 4
					GPIOB->BSRR |= GPIO_BSRR_BR_7;
					GPIOB->BSRR |= GPIO_BSRR_BR_2;
					GPIOB->BSRR |= GPIO_BSRR_BS_6;
					GPIOB->BSRR |= GPIO_BSRR_BS_3;
					for(i = 0; i < 3000; i++);
					
					//Step 5
					GPIOB->BSRR |= GPIO_BSRR_BR_7;
					GPIOB->BSRR |= GPIO_BSRR_BR_6;
					GPIOB->BSRR |= GPIO_BSRR_BR_2;
					GPIOB->BSRR |= GPIO_BSRR_BS_3;
					for(i = 0; i < 3000; i++);
					
					//Step 6
					GPIOB->BSRR |= GPIO_BSRR_BR_6;
					GPIOB->BSRR |= GPIO_BSRR_BR_2;
					GPIOB->BSRR |= GPIO_BSRR_BS_7;
					GPIOB->BSRR |= GPIO_BSRR_BS_3;
					for(i = 0; i < 3000; i++);
					
					//Step 7
					GPIOB->BSRR |= GPIO_BSRR_BR_6;
					GPIOB->BSRR |= GPIO_BSRR_BR_3;
					GPIOB->BSRR |= GPIO_BSRR_BR_2;
					GPIOB->BSRR |= GPIO_BSRR_BS_7;
					for(i = 0; i < 3000; i++);
					
					if((GPIOA->IDR >> 0) & 0x1) //stops when center button is pressed
						break;
				}
			}
			
	}
			
}



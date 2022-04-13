#ifndef __STM32L476G_DISCOVERY_LCD_H
#define __STM32L476G_DISCOVERY_LCD_H

void LCD_Display_Long(char *string); //Prototype for Display long used in main.c
void LCD_Clock_Init(void);
void LCD_Pin_Init(void);
void LCD_Configure(void);
void LCD_Display_Name(void);
void LCD_Display_String(char *string);
void LCD_Clear(void);


void LCD_WriteChar(uint8_t ch, int point, int colon, uint8_t position);

#endif /* __STM32L476G_DISCOVERY_LCD_H */

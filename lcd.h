/*
 * lcd.h
 *
 * Created: 5/19/2019 8:29:50 PM
 *  Author: Edward-Laptop
 */ 


#ifndef LCD_H_
#define LCD_H_

void LCD_Init();
void LCD_ClearScreen(void);
void LCD_WriteCommand (unsigned char command);
void LCD_WriteData(unsigned char data);
void LCD_Cursor (unsigned char column);
void LCD_DisplayString(unsigned char column ,/*const unsigned*/ char *string);
void delay_ms(int miliSec);
void LCD_WriteCustomData(unsigned char column, unsigned char *bytes);
void LCD_WriteCustomString(unsigned char column, unsigned char **string);

void LCD_DisplayMenu4(char *item1, char *item2, char *item3, char *item4);
void LCD_DisplayMenu2(char *item1, char *item2);

#endif /* LCD_H_ */
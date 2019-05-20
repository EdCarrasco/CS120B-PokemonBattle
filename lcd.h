/*
 * lcd.h
 *
 * Created: 5/19/2019 8:29:50 PM
 *  Author: Edward-Laptop
 */ 


#ifndef LCD_H_
#define LCD_H_

void LCD_init();
void LCD_ClearScreen(void);
void LCD_WriteCommand (unsigned char Command);
void LCD_Cursor (unsigned char column);
void LCD_DisplayString(unsigned char column ,/*const unsigned*/ char *string);
void delay_ms(int miliSec);

#endif /* LCD_H_ */
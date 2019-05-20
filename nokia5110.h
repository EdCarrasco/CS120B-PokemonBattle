/* Nokia 5110 LCD AVR Library
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 * 
 * Updated by Edward Carrasco
 */


#ifndef NOKIA5110_H_
#define NOKIA5110_H_

#include <avr/pgmspace.h>
#include <stdint.h>

// LCD port and pins
#define PORT_LCD PORTB
#define DDR_LCD DDRB
#define LCD_SCE PB2
#define LCD_RST PB3
#define LCD_DC PB4
#define LCD_DIN PB5
#define LCD_CLK PB6
#define LCD_CONTRAST 0x40

void NokiaLCD_Init(void);
void NokiaLCD_Clear(void);
void NokiaLCD_SetPower(uint8_t bool);
void NokiaLCD_SetPixel(uint8_t x, uint8_t y, uint8_t bool);
void NokiaLCD_SetScale(uint8_t scale);
void NokiaLCD_WriteChar(char code);
void NokiaLCD_WriteString(const char *str);
void NokiaLCD_SetCursor(uint8_t x, uint8_t y);
void NokiaLCD_Render(void);

#endif /* NOKIA5110_H_ */
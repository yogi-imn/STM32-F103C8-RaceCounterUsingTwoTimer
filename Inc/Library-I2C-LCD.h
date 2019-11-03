/*
 * Library-I2C-LCD.h
 *
 *  Created on: Okt 27, 2019
 *      Author: Yogi Iman
 */

#ifndef LIBRARY_I2C_LCD_H_
#define LIBRARY_I2C_LCD_H_

#include "stm32f1xx_hal.h"

void lcd_init (void);   // initialize lcd
void lcd_send_cmd (char cmd);  // send command to the lcd
void lcd_send_data (char data);  // send data to the lcd
void lcd_send_string (char *str);  // send string to the lcd

#endif /* LIBRARY_I2C_LCD_H_ */

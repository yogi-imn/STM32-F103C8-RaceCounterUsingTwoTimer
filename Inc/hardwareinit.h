/*
 * hardwareinit.h
 *
 *  Created on: Okt 27, 2019
 *      Author: Yogi Iman
 */

#ifndef HARDWAREINIT_H_
#define HARDWAREINIT_H_

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_I2C2_Init(void);
void MX_USART1_UART_Init(void);
void MX_TIM2_Init(void);
void MX_TIM4_Init(uint32_t Periode);
void System_Init(void);

#endif /* HARDWAREINIT_H_ */

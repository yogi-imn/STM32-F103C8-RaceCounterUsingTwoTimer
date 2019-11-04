/*
 * device.h
 *
 *  Created on: Okt 27, 2019
 *      Author: Yogi Iman
 */

#ifndef DEVICE_H_
#define DEVICE_H_
#include "stm32f1xx_hal.h"

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;
UART_HandleTypeDef huart1;

/* Private defines -----------------------------------------------------------*/
#define LED3_Pin GPIO_PIN_13
#define LED3_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_15
#define LED1_GPIO_Port GPIOC
#define Buzzer_Pin GPIO_PIN_1
#define Buzzer_GPIO_Port GPIOB
#define Sensor3_Pin GPIO_PIN_13
#define Sensor3_GPIO_Port GPIOB
#define Sensor2_Pin GPIO_PIN_14
#define Sensor2_GPIO_Port GPIOB
#define Sensor1_Pin GPIO_PIN_15
#define Sensor1_GPIO_Port GPIOB
#define PBReset_Pin GPIO_PIN_8
#define PBReset_GPIO_Port GPIOB
#define PBOn_Pin GPIO_PIN_9
#define PBOn_GPIO_Port GPIOB

#endif /* DEVICE_H_ */

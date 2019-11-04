/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <main.h>
#include <device.h>
#include <hardwareinit.h>
#include <Library-I2C-LCD.h>
#include <sensor.h>
#include <stm32f1xx_hal.h>

int main(void)
{
  /* Initialize all configured peripherals */
  System_Init();
  MX_TIM4_Init(2);

  HAL_TIM_Base_Start_IT(&htim4);
  lcd_init();

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

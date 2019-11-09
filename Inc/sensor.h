/*
 * sensor.h
 *
 *  Created on: Okt 27, 2019
 *      Author: Yogi Iman
 */

#ifndef SENSOR_H_
#define SENSOR_H_

void PBOn(void);
void PBReset(void);
void Sensor1(void);
void Sensor2(void);
void Sensor3(void);
void DearLCD(void);
void LCDAwal(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* SENSOR_H_ */

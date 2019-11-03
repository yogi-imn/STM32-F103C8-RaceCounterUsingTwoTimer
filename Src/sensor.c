/*
 * sensor.c
 *
 *  Created on: Okt 27, 2019
 *      Author: Yogi Iman
 */

#include "main.h"
#include "stm32f1xx_hal.h"
#include "Library-I2C-LCD.h"
#include "device.h"
#include "sensor.h"
#include "stdio.h"

unsigned int milisec=0,sec=0,min=0;
char buffer[18],buffertimer[10],buffs[13];
unsigned int runstop=0;

unsigned int lapA=0,lapB=0,lapC=0,totlap=5;
unsigned int milisec1,sec1,min1,miliseclast1=0,seclast1=0,minlast1=0;
unsigned int milisec2,sec2,min2,miliseclast2=0,seclast2=0,minlast2=0;
unsigned int milisec3,sec3,min3,miliseclast3=0,seclast3=0,minlast3=0;

unsigned char bouncing	=0xFF;
unsigned char bouncing2	=0xFF;
unsigned char bouncing3	=0xFF;
unsigned char bouncing4	=0xFF;
unsigned char bouncing5	=0xFF;

uint32_t Timeout_loop 	= 0;
uint32_t Timeout_value 	= 200;
uint32_t Timeout_loop2 	= 0;
uint32_t Timeout_value2 = 200;
uint32_t Timeout_loop3 	= 0;
uint32_t Timeout_value3 = 200;
uint32_t Timeout_loop4 	= 0;
uint32_t Timeout_value4 = 200;
uint32_t Timeout_loop5 	= 0;
uint32_t Timeout_value5 = 200;

void PBOn(void)
{
	if(HAL_GPIO_ReadPin(PBOn_GPIO_Port,PBOn_Pin)== GPIO_PIN_RESET && (Timeout_loop++<=Timeout_value)){
		bouncing=(bouncing<<1);
	}

	else if(HAL_GPIO_ReadPin(PBOn_GPIO_Port,PBOn_Pin)== GPIO_PIN_RESET && (Timeout_loop++>Timeout_value)){
		if(Timeout_loop>=Timeout_value+50){
			Timeout_loop=Timeout_value+1;
			HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rPBOn Error: 0xFE",25,10); //,25,10 iki opo?
		}
	}

	else if(HAL_GPIO_ReadPin(PBOn_GPIO_Port,PBOn_Pin)== GPIO_PIN_SET && Timeout_loop<=Timeout_value){
		bouncing = (bouncing<<1)|1;
	}
	else{
		Timeout_loop=0;
		bouncing=0xFF;
	}

	if (bouncing==0x01){
		if(runstop==0){
			HAL_TIM_Base_Start_IT(&htim2);
			runstop=1;
		}
		else{
			HAL_TIM_Base_Stop_IT(&htim2);
			runstop=0;
		}
	}
}

void PBReset(void)
{
	if(HAL_GPIO_ReadPin(PBReset_GPIO_Port,PBReset_Pin)== GPIO_PIN_RESET && (Timeout_loop2++<=Timeout_value2)){
		bouncing2=(bouncing2<<1);

	}

	else if(HAL_GPIO_ReadPin(PBReset_GPIO_Port,PBReset_Pin)== GPIO_PIN_RESET && (Timeout_loop2++>Timeout_value2)){
		if(Timeout_loop2>=Timeout_value2+50){
			Timeout_loop2=Timeout_value2+1;
			HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rPBReset Error: 0xEF",23,10); //,23,10 iki opo?
		}
	}

	else if(HAL_GPIO_ReadPin(PBReset_GPIO_Port,PBReset_Pin)== GPIO_PIN_SET && Timeout_loop2<=Timeout_value2){
		bouncing2 = (bouncing2<<1)|1;
	}

	else{
		Timeout_loop2=0;
		bouncing2=0xFF;
	}

	if (bouncing2==0x01){

		HAL_TIM_Base_Stop_IT(&htim2);

		milisec=0;	sec=0;	min=0;	runstop=0;
		milisec1=0;	sec1=0;	min1=0;
		milisec2=0;	sec2=0;	min2=0;
		milisec3=0;	sec3=0;	min3=0;
		lapA=0;	lapB=0;	lapC=0;

	}
}

void Sensor1(void)
{
	if(HAL_GPIO_ReadPin(Sensor1_GPIO_Port,Sensor1_Pin)== GPIO_PIN_RESET && (Timeout_loop3++<=Timeout_value3)){
		bouncing3=(bouncing3<<1);
		///
	}

	else if(HAL_GPIO_ReadPin(Sensor1_GPIO_Port,Sensor1_Pin)== GPIO_PIN_RESET && (Timeout_loop3++>Timeout_value3)){
		if(Timeout_loop3>=Timeout_value3+50){
			Timeout_loop3=Timeout_value3+1;
			HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rSensor1 Error: 0xFD",18,10);
		}
	}

	else if(HAL_GPIO_ReadPin(Sensor1_GPIO_Port,Sensor1_Pin)== GPIO_PIN_SET && Timeout_loop3<=Timeout_value3){
		bouncing3 = (bouncing3<<1)|1;
	}

	else{
		Timeout_loop3=0;
		bouncing3=0xFF;
	}

	if (bouncing3==0x01){

		milisec1=milisec-miliseclast1;
		miliseclast1=milisec;

		sec1=sec-seclast1;
		seclast1=sec;

		min1=min-minlast1;
		minlast1=min;

		if(runstop==1)	lapA++;
		else 			lapA=0;

		if(lapA<5){
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapA,(min1/10),(min1%10),(sec1/10),(sec1%10),(milisec1/10),(milisec1%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack A Lap:",14,10);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
		}
		else{
			lapA=5;
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack A = FINISH",18,10);
		}



	}
}

void Sensor2(void)
{
	if(HAL_GPIO_ReadPin(Sensor2_GPIO_Port,Sensor2_Pin)== GPIO_PIN_RESET && (Timeout_loop4++<=Timeout_value4)){
		bouncing4=(bouncing4<<1);

	}

	else if(HAL_GPIO_ReadPin(Sensor2_GPIO_Port,Sensor2_Pin)== GPIO_PIN_RESET && (Timeout_loop4++>Timeout_value4)){
		if(Timeout_loop4>=Timeout_value4+50){
			Timeout_loop4=Timeout_value4+1;
			HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rSensor2 Error: 0xFB",18,10);
		}
	}

	else if(HAL_GPIO_ReadPin(Sensor2_GPIO_Port,Sensor2_Pin)== GPIO_PIN_SET && Timeout_loop4<=Timeout_value4){
		bouncing4 = (bouncing4<<1)|1;
	}

	else{
		Timeout_loop4=0;
		bouncing4=0xFF;
	}

	if (bouncing4==0x01){

		milisec2=milisec-miliseclast2;
		miliseclast2=milisec;

		sec2=sec-seclast2;
		seclast2=sec;

		min2=min-minlast2;
		minlast2=min;

		if(runstop==1)	lapB++;
		else 			lapB=0;

		if(lapB<5){
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapB,(min2/10),(min2%10),(sec2/10),(sec2%10),(milisec2/10),(milisec2%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack B Lap:",14,10);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
		}
		else{
			lapB=5;
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack B = FINISH",18,10);
		}
	}
}

void Sensor3(void)
{
	if(HAL_GPIO_ReadPin(Sensor3_GPIO_Port,Sensor3_Pin)== GPIO_PIN_RESET && (Timeout_loop5++<=Timeout_value5)){
		bouncing5=(bouncing5<<1);

	}

	else if(HAL_GPIO_ReadPin(Sensor3_GPIO_Port,Sensor3_Pin)== GPIO_PIN_RESET && (Timeout_loop5++>Timeout_value5)){
		if(Timeout_loop5>=Timeout_value5+50){
			Timeout_loop5=Timeout_value5+1;
			HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rErr: 005-Sensor3",18,10);
		}
	}

	else if(HAL_GPIO_ReadPin(Sensor3_GPIO_Port,Sensor3_Pin)== GPIO_PIN_SET && Timeout_loop5<=Timeout_value5){
		bouncing5 = (bouncing5<<1)|1;
	}

	else{
		Timeout_loop5=0;
		bouncing5=0xFF;
	}

	if (bouncing5==0x01){

		milisec3=milisec-miliseclast3;
		miliseclast3=milisec;

		sec3=sec-seclast3;
		seclast3=sec;

		min3=min-minlast3;
		minlast3=min;

		if(runstop==1)	lapC++;
		else 			lapC=0;

		if(lapC<5){
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapC,(min3/10),(min3%10),(sec3/10),(sec3%10),(milisec3/10),(milisec3%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack C Lap:",14,10);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
		}
		else{
			lapC=5;
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack C = FINISH",18,10);
		}


	}
}

void DearLCD(void)
{
	///////////////////////////////////////////////////////////////////LCD ATAS
	lcd_send_cmd(0x80);
	sprintf(buffer,"    %d%d:%d%d:%d%d    ",(min/10),(min%10),(sec/10),(sec%10),(milisec/10),(milisec%10));
	lcd_send_string(buffer);
	lcd_send_cmd(0xe2);
	sprintf(buffer," ");
	lcd_send_string(buffer);
	if(lapA<totlap){
		lcd_send_cmd(0xd4);
		sprintf(buffer,"Lap%dA=%d%d:%d%d:%d%d ",(lapA),(min1/10),(min1%10),(sec1/10),(sec1%10),(milisec1/10),(milisec1%10));
		lcd_send_string(buffer);
	}
	else{
		lcd_send_cmd(0xd4);
		sprintf(buffer,"A = Finish");
		lcd_send_string(buffer);
	}
	///////////////////////////////////////////////////////////////////LCD BAWAH
	if(lapB<totlap && lapC<totlap){
		lcd_send_cmd(0xc0);
		sprintf(buffer,"B%d=%d%d:%d%dC%d=%d%d:%d%d",(lapB),(min2/10),(min2%10),(sec2/10),(sec2%10),(lapC),(min3/10),(min3%10),(sec3/10),(sec3%10));
		lcd_send_string(buffer);
	}
	else if(lapB<totlap && lapC>=totlap){
		lcd_send_cmd(0xc0);
		sprintf(buffer,"B%d=%d%d:%d%dC=FINISH",(lapB),(min2/10),(min2%10),(sec2/10),(sec2%10));
		lcd_send_string(buffer);
	}
	else if(lapB>=totlap && lapC<totlap){
		lcd_send_cmd(0xc0);
		sprintf(buffer,"B=FINISHC%d=%d%d:%d%d",(lapC),(min3/10),(min3%10),(sec3/10),(sec3%10));
		lcd_send_string(buffer);
	}
	else{
		lcd_send_cmd(0xc0);
		lcd_send_string("B=FINISHC=FINISH");
	}

	///////////////////////////////////////////////////////////////////STOP Timer2
	if(lapA>=totlap && lapB>=totlap && lapC>=totlap)HAL_TIM_Base_Stop_IT(&htim2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2){
		milisec++;
		if(milisec>=100){
			sec++;
			milisec=0;
		}
		if(sec>=60){
			min++;
			sec=0;
		}
		if(min>=100){
			milisec=0;
			sec=0;
			min=0;
		}
	}

	if(htim->Instance==TIM4){

		///////////////////////////////Baca Tombol Dan Sensor
		PBOn();
		PBReset();
		Sensor1();
		Sensor2();
		Sensor3();

		///////////////////////////////Kirim LCD
		DearLCD();
	}
}

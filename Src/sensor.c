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
//uint2_t milisec=0,sec=0,min=0;
char buffer[21],buffertimer[10],buffs[13];
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
uint32_t Timeout_value 	= 400;
uint32_t Timeout_loop2 	= 0;
uint32_t Timeout_value2 = 400;
uint32_t Timeout_loop3 	= 0;
uint32_t Timeout_value3 = 400;
uint32_t Timeout_loop4 	= 0;
uint32_t Timeout_value4 = 400;
uint32_t Timeout_loop5 	= 0;
uint32_t Timeout_value5 = 400;

void PBOn(void)
{
	if(HAL_GPIO_ReadPin(PBOn_GPIO_Port,PBOn_Pin)== GPIO_PIN_RESET && (Timeout_loop++<=Timeout_value)){
		bouncing=(bouncing<<1);
	}

	else if(HAL_GPIO_ReadPin(PBOn_GPIO_Port,PBOn_Pin)== GPIO_PIN_RESET && (Timeout_loop++>Timeout_value)){
		if(Timeout_loop>=Timeout_value+50){
			Timeout_loop=Timeout_value+1;

			HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);

			HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
			HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET);

			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rPBOn Error: 0x01",18,10);
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

			HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET);

			HAL_GPIO_TogglePin(Buzzer_GPIO_Port,Buzzer_Pin);
			HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
			HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET);

			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rPBReset Error: 0x02",21,10);
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

		lcd_send_cmd(0x80);
		sprintf(buffer," [Lap Time] ");
		lcd_send_string(buffer);
		lcd_send_cmd(0x8c);
		sprintf(buffer,"00");
		lcd_send_string(buffer);
		lcd_send_cmd(0x8e);
		sprintf(buffer,":");
		lcd_send_string(buffer);
		lcd_send_cmd(0x8f);
		sprintf(buffer,"00");
		lcd_send_string(buffer);
		lcd_send_cmd(0x91);
		sprintf(buffer,":");
		lcd_send_string(buffer);
		lcd_send_cmd(0x92);
		sprintf(buffer,"00");
		lcd_send_string(buffer);
		//TrackA
		lcd_send_cmd(0xc0);
		sprintf(buffer,"TrackA=");
		lcd_send_string(buffer);
		lcd_send_cmd(0xc7);
		sprintf(buffer,"00:00:00 ");
		lcd_send_string(buffer);
		lcd_send_cmd(0xd0);
		sprintf(buffer,"Lap%d",lapA);
		lcd_send_string(buffer);
		//TrackB
		lcd_send_cmd(0x94);
		sprintf(buffer,"TrackB=");
		lcd_send_string(buffer);
		lcd_send_cmd(0x9b);
		sprintf(buffer,"00:00:00 ");
		lcd_send_string(buffer);
		lcd_send_cmd(0xa4);
		sprintf(buffer,"Lap%d",lapB);
		lcd_send_string(buffer);
		//TrackC
		lcd_send_cmd(0xd4);
		sprintf(buffer,"TrackC=");
		lcd_send_string(buffer);
		lcd_send_cmd(0xdb);
		sprintf(buffer,"00:00:00 ");
		lcd_send_string(buffer);
		lcd_send_cmd(0xe4);
		sprintf(buffer,"Lap%d",lapC);
		lcd_send_string(buffer);
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

			HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);

			HAL_GPIO_TogglePin(Buzzer_GPIO_Port,Buzzer_Pin);
			HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
			HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
			HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET);

			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rSensor1 Error: 0x03",21,10);
		}
	}

	else if(HAL_GPIO_ReadPin(Sensor1_GPIO_Port,Sensor1_Pin)== GPIO_PIN_SET && Timeout_loop3<=Timeout_value3){
		bouncing3 = (bouncing3<<1)|1;
	}

	else{
		Timeout_loop3=0;
		bouncing3=0xFF;
	}

	if (bouncing3==0xFE){

		milisec1=milisec;
		//milisec1=milisec-miliseclast1;
		miliseclast1=milisec;

		sec1=sec;
		//sec1=sec-seclast1;
		seclast1=sec;

		min1=min;
		//min1=min-minlast1;
		minlast1=min;

		if(runstop==1)	lapA++;
		else 			lapA=0;

		if(lapA<5){
			//sprintf(buffs,"%d = %d:%d:%d",lapA,min1,sec1,milisec1);
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

			HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET);

			HAL_GPIO_TogglePin(Buzzer_GPIO_Port,Buzzer_Pin);
			HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
			HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET);

			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rSensor2 Error: 0x04",21,10);
		}
	}

	else if(HAL_GPIO_ReadPin(Sensor2_GPIO_Port,Sensor2_Pin)== GPIO_PIN_SET && Timeout_loop4<=Timeout_value4){
		bouncing4 = (bouncing4<<1)|1;
	}

	else{
		Timeout_loop4=0;
		bouncing4=0xFF;
	}

	if (bouncing4==0xFE){

		milisec2=milisec;
		//milisec2=milisec-miliseclast2;
		miliseclast2=milisec;

		sec2=sec;
		//sec2=sec-seclast2;
		seclast2=sec;

		min2=min;
		//min2=min-minlast2;
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

			HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);

			HAL_GPIO_TogglePin(Buzzer_GPIO_Port,Buzzer_Pin);
			HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
			HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
			HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET);

			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rSensor3 Error: 0x05",21,10);
		}
	}

	else if(HAL_GPIO_ReadPin(Sensor3_GPIO_Port,Sensor3_Pin)== GPIO_PIN_SET && Timeout_loop5<=Timeout_value5){
		bouncing5 = (bouncing5<<1)|1;
	}

	else{
		Timeout_loop5=0;
		bouncing5=0xFF;
	}

	if (bouncing5==0xFE){

		milisec3=milisec;
		//milisec3=milisec-miliseclast3;
		miliseclast3=milisec;

		sec3=sec;
		//sec3=sec-seclast3;
		seclast3=sec;

		min3=min;
		//min3=min-minlast3;
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

void LCDAwal(void)
{
	lcd_send_cmd(0x80);
	sprintf(buffer," [Lap Time] ");
	lcd_send_string(buffer);
	lcd_send_cmd(0x8c);
	sprintf(buffer,"00");
	lcd_send_string(buffer);
	lcd_send_cmd(0x8e);
	sprintf(buffer,":");
	lcd_send_string(buffer);
	lcd_send_cmd(0x8f);
	sprintf(buffer,"00");
	lcd_send_string(buffer);
	lcd_send_cmd(0x91);
	sprintf(buffer,":");
	lcd_send_string(buffer);
	lcd_send_cmd(0x92);
	sprintf(buffer,"00");
	lcd_send_string(buffer);
	//TrackA
	lcd_send_cmd(0xc0);
	sprintf(buffer,"TrackA=");
	lcd_send_string(buffer);
	lcd_send_cmd(0xc7);
	sprintf(buffer,"00:00:00 ");
	lcd_send_string(buffer);
	lcd_send_cmd(0xd0);
	sprintf(buffer,"Lap%d",lapA);
	lcd_send_string(buffer);
	//TrackB
	lcd_send_cmd(0x94);
	sprintf(buffer,"TrackB=");
	lcd_send_string(buffer);
	lcd_send_cmd(0x9b);
	sprintf(buffer,"00:00:00 ");
	lcd_send_string(buffer);
	lcd_send_cmd(0xa4);
	sprintf(buffer,"Lap%d",lapB);
	lcd_send_string(buffer);
	//TrackC
	lcd_send_cmd(0xd4);
	sprintf(buffer,"TrackC=");
	lcd_send_string(buffer);
	lcd_send_cmd(0xdb);
	sprintf(buffer,"00:00:00 ");
	lcd_send_string(buffer);
	lcd_send_cmd(0xe4);
	sprintf(buffer,"Lap%d",lapC);
	lcd_send_string(buffer);
}

void DearLCD(void)
{
	//LAP TIME
	lcd_send_cmd(0x80);
	//sprintf(buffer," [Lap Time] %d%d:%d%d:%d%d",(min/10),(min%10),(sec/10),(sec%10),(milisec/10),(milisec%10));
	sprintf(buffer," [Lap Time] ");
	lcd_send_string(buffer);

	lcd_send_cmd(0x8c);
	sprintf(buffer,"%d",min);
	lcd_send_string(buffer);
	lcd_send_cmd(0x8e);
	sprintf(buffer,":");
	lcd_send_string(buffer);
	lcd_send_cmd(0x8f);
	sprintf(buffer,"%d",sec);
	lcd_send_string(buffer);
	lcd_send_cmd(0x91);
	sprintf(buffer,":");
	lcd_send_string(buffer);
	lcd_send_cmd(0x92);
	sprintf(buffer,"%d",milisec);
	lcd_send_string(buffer);

	//TrackA
	if(lapA<totlap){
		lcd_send_cmd(0xc0);
		//sprintf(buffer,"TrackA=%d%d:%d%d:%d%d Lap%d",(min1/10),(min1%10),(sec1/10),(sec1%10),(milisec1/10),(milisec1%10),(lapA));
		//		sprintf(buffer,"TrackA=%d:%d:%d",min1,sec1,milisec1);
		//		lcd_send_string(buffer);
		//		lcd_send_cmd(0xd0);
		//		sprintf(buffer,"Lap%d",lapA);
		//		lcd_send_string(buffer);
		sprintf(buffer,"TrackA=");
		lcd_send_string(buffer);
		lcd_send_cmd(0xc7);
		sprintf(buffer,"%d",min1);
		lcd_send_string(buffer);
		lcd_send_cmd(0xc9);
		sprintf(buffer,":");
		lcd_send_string(buffer);
		lcd_send_cmd(0xca);
		sprintf(buffer,"%d",sec1);
		lcd_send_string(buffer);
		lcd_send_cmd(0xcc);
		sprintf(buffer,":");
		lcd_send_string(buffer);
		lcd_send_cmd(0xcd);
		sprintf(buffer,"%d",milisec1);
		lcd_send_string(buffer);

		lcd_send_cmd(0xd0);
		sprintf(buffer,"Lap%d",lapA);
		lcd_send_string(buffer);
	}
	else if(lapA==totlap){
		lcd_send_cmd(0xc0);
		sprintf(buffer,"TrackA = FINISH YEAY");
		lcd_send_string(buffer);
	}
	//TrackB
	if(lapB<totlap){
		lcd_send_cmd(0x94);
		//sprintf(buffer,"TrackB=%d%d:%d%d:%d%d Lap%d",(min2/10),(min2%10),(sec2/10),(sec2%10),(milisec2/10),(milisec2%10),(lapB));
		//		sprintf(buffer,"TrackB=%d:%d:%d",min2,sec2,milisec2);
		//		lcd_send_string(buffer);
		sprintf(buffer,"TrackB=");
		lcd_send_string(buffer);
		lcd_send_cmd(0x9b);
		sprintf(buffer,"%d",min2);
		lcd_send_string(buffer);
		lcd_send_cmd(0x9d);
		sprintf(buffer,":");
		lcd_send_string(buffer);
		lcd_send_cmd(0x9e);
		sprintf(buffer,"%d",sec2);
		lcd_send_string(buffer);
		lcd_send_cmd(0xa0);
		sprintf(buffer,":");
		lcd_send_string(buffer);
		lcd_send_cmd(0xa1);
		sprintf(buffer,"%d",milisec2);
		lcd_send_string(buffer);

		lcd_send_cmd(0xa4);
		sprintf(buffer,"Lap%d",lapB);
		lcd_send_string(buffer);
	}
	else if(lapB==totlap){
		lcd_send_cmd(0x94);
		sprintf(buffer,"TrackB = FINISH YEAY");
		lcd_send_string(buffer);
	}
	//TrackC
	if(lapC<totlap){
		lcd_send_cmd(0xd4);
		//sprintf(buffer,"TrackC=%d%d:%d%d:%d%d Lap%d",(min3/10),(min3%10),(sec3/10),(sec3%10),(milisec2/10),(milisec2%10),(lapC));
//		sprintf(buffer,"TrackC=%d:%d:%d",min3,sec3,milisec3);
//		lcd_send_string(buffer);
		sprintf(buffer,"TrackC=");
		lcd_send_string(buffer);
		lcd_send_cmd(0xdb);
		sprintf(buffer,"%d",min3);
		lcd_send_string(buffer);
		lcd_send_cmd(0xdd);
		sprintf(buffer,":");
		lcd_send_string(buffer);
		lcd_send_cmd(0xde);
		sprintf(buffer,"%d",sec3);
		lcd_send_string(buffer);
		lcd_send_cmd(0xe0);
		sprintf(buffer,":");
		lcd_send_string(buffer);
		lcd_send_cmd(0xe1);
		sprintf(buffer,"%d",milisec3);
		lcd_send_string(buffer);

		lcd_send_cmd(0xe4);
		sprintf(buffer,"Lap%d",lapC);
		lcd_send_string(buffer);
	}
	else if(lapC==totlap){
		lcd_send_cmd(0xd4);
		sprintf(buffer,"TrackC = FINISH YEAY");
		lcd_send_string(buffer);
	}

	///////////////////////////////////////////////////////////////////STOP Timer2
	if(lapA>=totlap && lapB>=totlap && lapC>=totlap)HAL_TIM_Base_Stop_IT(&htim2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//if(htim->Instance!=TIM2 && htim->Instance!=TIM4){
	LCDAwal();
	//}

	if(htim->Instance==TIM2){
		milisec++;
		if(milisec>99){
			sec++;
			milisec=0;
		}
		if(sec>60){
			min++;
			sec=0;
		}
		if(min>60){
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

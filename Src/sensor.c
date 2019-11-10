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
#include "stdlib.h"

unsigned int milisec=0,sec=0,min=0;
char buffer[21],buffs[13],buffg[6];
unsigned int runstop=0;
unsigned int lapA=0,lapB=0,lapC=0,totlap=5;

unsigned int milisecA=0,secA=0,minA=0;
unsigned int milisecAB=0,secAB=0,minAB=0;
unsigned int selisihmsAB=0,selisihsAB=0,selisihmAB=0;
unsigned int milisecAC=0,secAC=0,minAC=0;
unsigned int selisihmsAC=0,selisihsAC=0,selisihmAC=0;
unsigned int milisecB=0,secB=0,minB=0;
unsigned int milisecBA=0,secBA=0,minBA=0;
unsigned int selisihmsBA=0,selisihsBA=0,selisihmBA=0;
unsigned int milisecBC=0,secBC=0,minBC=0;
unsigned int selisihmsBC=0,selisihsBC=0,selisihmBC=0;
unsigned int milisecC=0,secC=0,minC=0;
unsigned int milisecCA=0,secCA=0,minCA=0;
unsigned int selisihmsCA=0,selisihsCA=0,selisihmCA=0;
unsigned int milisecCB=0,secCB=0,minCB=0;
unsigned int selisihmsCB=0,selisihsCB=0,selisihmCB=0;

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
			HAL_GPIO_TogglePin(Buzzer_GPIO_Port,Buzzer_Pin);

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
		milisecA=0;	secA=0;	minA=0;
		milisecB=0;	secB=0;	minB=0;
		milisecC=0;	secC=0;	minC=0;
		lapA=0;		lapB=0;	lapC=0;

		LCDAwal();
		HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_GPIO_Port,LED2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET);
	}
}

void Sensor1(void)
{
	if(HAL_GPIO_ReadPin(Sensor1_GPIO_Port,Sensor1_Pin)== GPIO_PIN_RESET && (Timeout_loop3++<=Timeout_value3)){
		bouncing3=(bouncing3<<1);
	}

	else if(HAL_GPIO_ReadPin(Sensor1_GPIO_Port,Sensor1_Pin)== GPIO_PIN_RESET && (Timeout_loop3++>Timeout_value3)){
		if(Timeout_loop3>=Timeout_value3+50){
			Timeout_loop3=Timeout_value3+1;

			HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);

			HAL_GPIO_TogglePin(Buzzer_GPIO_Port,Buzzer_Pin);
			HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
			HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);

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
		minA=min;
		minAB=minA-minB;
		minAC=minA-minC;

		secA=sec;
		selisihsAB=secA-secB;
		selisihsAC=secA-secC;
//		secAB=secA-secB;
//		secAC=secA-secC;

		milisecA=milisec;
		selisihmsAB=milisecA-milisecB;
		selisihmsAC=milisecA-milisecC;

		if(selisihsAB>=0){
			secAB=abs(selisihsAB);
		}else{//(selisihsAB<0)
			if(minAB != 0){
				secAB=100-secB+secA;
				minAB=minAB-1;
			}else if(minAB == 0){
				secAB=abs(selisihsAB);
			}
		}

		if(selisihsAC>=0){
			secAC=abs(selisihsAC);
		}else{
			if(minAC != 0){
				secAC=100-secC+secA;
				minAC=minAC-1;
			}else if(minAC == 0){
				secAC=abs(selisihsAC);
			}
		}

		if(selisihmsAB>=0){
			milisecAB=abs(selisihmsAB);
		}else{
			if(secAB != 0){
				milisecAB=100-milisecB+milisecA;
				secAB=secAB-1;
			}else if(secAB == 0){
				milisecAB=abs(selisihmsAB);
			}
		}

		if(selisihmsAC>=0){
			milisecAC=abs(selisihmsAC);
		}else{
			if(secAC != 0){
				milisecAC=100-milisecC+milisecA;
				secAC=secAC-1;
			}else if(secAC == 0){
				milisecAC=abs(selisihmsAC);
			}
		}

		if(runstop==1)	lapA++;
		else 			lapA=0;

		if(lapA<5){
			HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_SET);
			//sprintf(buffs,"%d = %d:%d:%d",lapA,minA,secA,milisecA);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack A Lap:",14,10);
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapA,(minA/10),(minA%10),(secA/10),(secA%10),(milisecA/10),(milisecA%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
			HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET);
		}
		else if(lapA==5){
			//lapA=5;
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack A = FINISH",18,10);
			//HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack A Lap:",14,10);
			sprintf(buffs," = %d%d:%d%d:%d%d",(minA/10),(minA%10),(secA/10),(secA%10),(milisecA/10),(milisecA%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
			//Selisih A dan B
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rSelisih mobil A dengan B = ",27,10);
			sprintf(buffg,"%d:%d:%d",minAB,secAB,milisecAB);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffg,sizeof(buffg),10);
			//Selisih A dan C
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rSelisih mobil A dengan C = ",27,10);
			sprintf(buffg,"%d:%d:%d",minAC,secAC,milisecAC);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffg,sizeof(buffs),10);
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
		milisecB=milisec;
		//milisecB=milisec-milisecBA;
		milisecBA=milisecB-milisecA;
		milisecBC=milisecB-milisecC;

		secB=sec;
		//secB=sec-secBA;
		secBA=secB-secA;
		secBC=secB-secC;

		minB=min;
		//minB=min-minBA;
		minBA=minB-minA;
		minBC=minB-minC;

		if(runstop==1)	lapB++;
		else 			lapB=0;

		if(lapB<5){
			HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_SET);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack B Lap:",14,10);
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapB,(minB/10),(minB%10),(secB/10),(secB%10),(milisecB/10),(milisecB%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
			HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET);
		}
		else if (lapB==5){
			//lapB=5;
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack B = FINISH",18,10);
			//Selisih B dan A
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rSelisih mobil B dengan A = ",27,10);
			sprintf(buffg,"%d:%d:%d",minBA,secBA,milisecBA);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffg,sizeof(buffg),10);
			//Selisih B dan C
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rSelisih mobil B dengan C = ",27,10);
			sprintf(buffg,"%d:%d:%d",minBC,secBC,milisecBC);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffg,sizeof(buffg),10);
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
		milisecC=milisec;
		//milisecC=milisec-milisecCA;
		milisecCA=milisecC-milisecA;
		milisecCB=milisecC-milisecB;

		secC=sec;
		//secC=sec-secCA;
		secCA=secC-secA;
		secCB=secC-secB;

		minC=min;
		//minC=min-minCA;
		minCA=minC-minA;
		minCB=minC-minB;

		if(runstop==1)	lapC++;
		else 			lapC=0;

		if(lapC<5){
			HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_SET);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack C Lap:",14,10);
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapC,(minC/10),(minC%10),(secC/10),(secC%10),(milisecC/10),(milisecC%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
			HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET);
		}
		else if (lapC==5){
			//lapC=5;
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rTrack C = FINISH",18,10);
			//Selisih C dan A
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rSelisih mobil C dengan A = ",27,10);
			sprintf(buffg,"%d:%d:%d",minCA,secCA,milisecCA);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffg,sizeof(buffg),10);
			//Selisih C dan B
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rSelisih mobil C dengan B = ",27,10);
			sprintf(buffg,"%d:%d:%d",minCB,secCB,milisecCB);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffg,sizeof(buffg),10);
		}
	}
}

void LCDAwal(void)
{
	lcd_send_cmd(0x80);
	sprintf(buffer," [Lap Time] ");
	lcd_send_string(buffer);
	lcd_send_cmd(0x8c);
	sprintf(buffer,"  ");
	lcd_send_string(buffer);
	lcd_send_cmd(0x8e);
	sprintf(buffer,":");
	lcd_send_string(buffer);
	lcd_send_cmd(0x8f);
	sprintf(buffer,"  ");
	lcd_send_string(buffer);
	lcd_send_cmd(0x91);
	sprintf(buffer,":");
	lcd_send_string(buffer);
	lcd_send_cmd(0x92);
	sprintf(buffer,"  ");
	lcd_send_string(buffer);
	//TrackA
	lcd_send_cmd(0xc0);
	sprintf(buffer,"TrackA=");
	lcd_send_string(buffer);
	lcd_send_cmd(0xc7);
	sprintf(buffer,"  :  :   ");
	lcd_send_string(buffer);
	lcd_send_cmd(0xd0);
	sprintf(buffer,"Lap%d",lapA);
	lcd_send_string(buffer);
	//TrackB
	lcd_send_cmd(0x94);
	sprintf(buffer,"TrackB=");
	lcd_send_string(buffer);
	lcd_send_cmd(0x9b);
	sprintf(buffer,"  :  :   ");
	lcd_send_string(buffer);
	lcd_send_cmd(0xa4);
	sprintf(buffer,"Lap%d",lapB);
	lcd_send_string(buffer);
	//TrackC
	lcd_send_cmd(0xd4);
	sprintf(buffer,"TrackC=");
	lcd_send_string(buffer);
	lcd_send_cmd(0xdb);
	sprintf(buffer,"  :  :   ");
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
		//sprintf(buffer,"TrackA=%d%d:%d%d:%d%d Lap%d",(minA/10),(minA%10),(secA/10),(secA%10),(milisecA/10),(milisecA%10),(lapA));
		//		sprintf(buffer,"TrackA=%d:%d:%d",minA,secA,milisecA);
		//		lcd_send_string(buffer);
		//		lcd_send_cmd(0xd0);
		//		sprintf(buffer,"Lap%d",lapA);
		//		lcd_send_string(buffer);
		sprintf(buffer,"TrackA=");
		lcd_send_string(buffer);
		lcd_send_cmd(0xc7);
		sprintf(buffer,"%d",minA);
		lcd_send_string(buffer);
		lcd_send_cmd(0xc9);
		sprintf(buffer,":");
		lcd_send_string(buffer);
		lcd_send_cmd(0xca);
		sprintf(buffer,"%d",secA);
		lcd_send_string(buffer);
		lcd_send_cmd(0xcc);
		sprintf(buffer,":");
		lcd_send_string(buffer);
		lcd_send_cmd(0xcd);
		sprintf(buffer,"%d",milisecA);
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
		//sprintf(buffer,"TrackB=%d%d:%d%d:%d%d Lap%d",(minB/10),(minB%10),(secB/10),(secB%10),(milisecB/10),(milisecB%10),(lapB));
		//		sprintf(buffer,"TrackB=%d:%d:%d",minB,secB,milisecB);
		//		lcd_send_string(buffer);
		sprintf(buffer,"TrackB=");
		lcd_send_string(buffer);
		lcd_send_cmd(0x9b);
		sprintf(buffer,"%d",minB);
		lcd_send_string(buffer);
		lcd_send_cmd(0x9d);
		sprintf(buffer,":");
		lcd_send_string(buffer);
		lcd_send_cmd(0x9e);
		sprintf(buffer,"%d",secB);
		lcd_send_string(buffer);
		lcd_send_cmd(0xa0);
		sprintf(buffer,":");
		lcd_send_string(buffer);
		lcd_send_cmd(0xa1);
		sprintf(buffer,"%d",milisecB);
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
		//sprintf(buffer,"TrackC=%d%d:%d%d:%d%d Lap%d",(minC/10),(minC%10),(secC/10),(secC%10),(milisecB/10),(milisecB%10),(lapC));
//		sprintf(buffer,"TrackC=%d:%d:%d",minC,secC,milisecC);
//		lcd_send_string(buffer);
		sprintf(buffer,"TrackC=");
		lcd_send_string(buffer);
		lcd_send_cmd(0xdb);
		sprintf(buffer,"%d",minC);
		lcd_send_string(buffer);
		lcd_send_cmd(0xdd);
		sprintf(buffer,":");
		lcd_send_string(buffer);
		lcd_send_cmd(0xde);
		sprintf(buffer,"%d",secC);
		lcd_send_string(buffer);
		lcd_send_cmd(0xe0);
		sprintf(buffer,":");
		lcd_send_string(buffer);
		lcd_send_cmd(0xe1);
		sprintf(buffer,"%d",milisecC);
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

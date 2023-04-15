/*
 * display.c
 *
 *  Created on: Apr 11, 2023
 *      Author: jaziba
 */




#include "main.h"
#include "display.h"

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart2;

char buffer[100];

void makeMenu(int length, char* options[100])
{
	clearDisplay();//TODO delete
	uint8_t box[] = {0x7C ,0x0F, 0x14 , 0x75, 0x8C , 0x7F, 0x01};
	//uint8_t clear[2] = {0x7C,0x00};
	uint8_t buff[10] = {0x7C, 0x0C, 0x00 , 0x0A , 0x32 , 0x3C, 0x01};
	uint8_t xoff[5] = {0x7C, 0x18, 0x50 - 120};
	uint8_t yoff[5] = {0x7C, 0x19, 0x50+40};
	clearDisplay();


	for(int i=0; i<length; i++){
		// char buffer[100];
		// sprintf(buffer,options[i]);
		// HAL_UART_Transmit (&huart3, box, sizeof (box), 150);
		// box[3] = box[3] - 20;
		// box[5] = box[5] - 20;
		sprintf(buffer,options[i]);
		HAL_UART_Transmit (&huart3, xoff, sizeof (xoff), 150);
		HAL_UART_Transmit (&huart3, yoff, sizeof (yoff), 150);
		HAL_UART_Transmit(&huart3,(uint8_t*)buffer,strlen((const char*)buffer),10);
		yoff[2] = yoff[2] - 14;

	}
	HAL_Delay(1000);
	HAL_Delay(1000);
	// HAL_Delay(1000);
	// HAL_Delay(1000);
	// HAL_Delay(1000);
	// HAL_Delay(1000);
	// HAL_Delay(1000);
	// HAL_Delay(1000);
	//  clearDisplay();//TODO delete

}

void clearDisplay(){
	uint8_t clear[2] = {0x7C,0x00};
	HAL_UART_Transmit (&huart3, clear, sizeof (clear), 150);
}

int sendInformation(char* data){
	int ack = 0;
	for(int i=0; i<5; i++){
		sprintf(buffer, data);
		HAL_UART_Transmit(&huart2, buffer, strlen(buffer), 500);
		HAL_UART_Receive(&huart2, buffer, 12, 1000);
		buffer[3] = '\0';
		if(strcmp(buffer, "ack") == 0){
			ack = 1;
			break;
		}
	}
	return ack;
}

//FREE DATA AFTER
char* sendInformationGetData(char* data, char* destination, int length, int timeRecieving){
	for(int i=0; i<5; i++){
		sprintf(buffer, data);
		HAL_UART_Transmit(&huart2, buffer, strlen(buffer), 500);
		HAL_UART_Receive(&huart2, buffer, 3+length-1, timeRecieving);
		if(buffer[0] == 'a' && buffer[1] == 'c' && buffer[2] == 'k'){
			strncpy(destination, &buffer[3], length);
			destination[length-1] = '\0';
			return destination;
		}
	}
	return '\0';
}

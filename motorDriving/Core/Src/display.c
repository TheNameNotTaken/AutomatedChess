#include "main.h"
#include "display.h"


void makeMenu(int length, char* options[100])
{
	uint8_t box2[] = {0x7C ,0x0F, 0x14 , 0x75 - 14, 0x8C , 0x7F - 14, 0x01};
	uint8_t box[] = {0x7C ,0x0F, 0x14 , 0x75, 0x8C , 0x7F, 0x01};

	for(int i=0; i<length; i++){
		char buffer[100];
		sprintf(buffer,options[i]);
//		HAL_UART_Transmit(&huart3,(uint8_t*)buffer,strlen((const char*)buffer),10);
	}



//  for(int i = 0; i < length; i++){
//	  char* curChar = options[i];
//	  while(*curChar != '/0'){
//		  HAL_UART_Transmit (&huart3, *curChar, sizeof(*curChar), 150);
//		  curChar++;
//	  }
//	  //HAL_UART_Transmit (&huart3, box, sizeof (box), 150);
////		  char str[1024] = "";
////		  char tmp = '.';
////
////		  strcat(str, tmp);
//	// uint8_t xoff[5] = {0x7C, 0x18+i, 0x50++i};
////	HAL_UART_Transmit (&huart3, box, sizeof (box), 150);
//	box[3] = box[3] - 11;
//	box[5] = box[5] - 11;
//	char* test = {0x4a, 0x61, 0x7a, 0x69, 0x62, 0x00};
//	HAL_UART_Transmit (&huart3, "Jazib", 6, 150);
//	uint8_t* xy = {0x7C, 0x18, box[3], 0x7C, 0x19, box[4]};
//	HAL_UART_Transmit (&huart3, xy, 16, 150);
//  }
  clearDisplay();//TODO delete
}

void clearDisplay(){
	uint8_t clear[2] = {0x7C,0x00};
//	HAL_UART_Transmit (&huart3, clear, sizeof (clear), 150);
}

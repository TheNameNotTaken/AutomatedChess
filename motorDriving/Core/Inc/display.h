#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include <stdint.h>
#include "main.h"

//extern UART_HandleTypeDef huart3;

//uint8_t clear[2] = {0x7C,0x00};
//uint8_t buff[10] = {0x7C, 0x0C, 0x00 , 0x0A , 0x32 , 0x3C, 0x01};
//uint8_t circle[10] = {0x7C, 0x03, 0x50 ,0x40, 0x0A, 0x01};
////0x7C 0x18 0x50
//uint8_t xoff[5] = {0x7C, 0x18, 0x50};
//
//uint8_t demo[3] = {0x7C, 0x12};
//uint8_t box[] = {0x7C ,0x0F, 0x14 , 0x75, 0x8C , 0x7F, 0x01};
//
//uint8_t box3[] = {0x7C ,0x0F, 0x14 , 0x75- 28, 0x8C , 0x7F - 28, 0x01};
//
//uint8_t box4[] = {0x7C ,0x0F, 0x14 , 0x75 - 20, 0x8C , 0x7F - 20, 0x01};
//uint8_t box5[] = {0x7C ,0x0F, 0x14 , 0x75, 0x8C , 0x7F, 0x01};
//
//uint8_t box6[] = {0x7C ,0x0F, 0x14 , 0x75 - 20, 0x8C , 0x7F - 20, 0x01};
//uint8_t bo7[] = {0x7C ,0x0F, 0x14 , 0x75, 0x8C , 0x7F, 0x01};
//uint8_t toggle[] = {0x7C,0x12};

void makeMenu(int length, char **options);

void clearDisplay();

#endif

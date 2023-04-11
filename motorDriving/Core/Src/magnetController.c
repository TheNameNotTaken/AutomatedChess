
/*
 * elec_mag_ctrl.c
 *
 *  Created on: Apr 4, 2023
 *      Author: xuzhekai
 */


#include "main.h"
#include "magnetController.h"

#define whitePort GPIOB
#define whitePin GPIO_PIN_0
#define blackPort GPIOE
#define blackPin GPIO_PIN_0

void setMagnet(int direction){
	if (direction == BLACK){
		HAL_GPIO_WritePin(blackPort, blackPin, 1);
		HAL_GPIO_WritePin(whitePort, whitePin, 0);
	}
	else if(direction == WHITE){
		HAL_GPIO_WritePin(blackPort, blackPin, 0);
		HAL_GPIO_WritePin(whitePort, whitePin, 1);
	}
	else if(direction == STOP){
		HAL_GPIO_WritePin(blackPort, blackPin, 0);
		HAL_GPIO_WritePin(whitePort, whitePin, 0);
	}
}

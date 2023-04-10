
/*
 * elec_mag_ctrl.c
 *
 *  Created on: Apr 4, 2023
 *      Author: xuzhekai
 */


#include "main.h"
#include "magnetController.h"

void setMagnet(int direction){
	if (direction == BLACK){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
	}
	else if(direction == WHITE){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
	}
	else if(direction == STOP){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
	}
}

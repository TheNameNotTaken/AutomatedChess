/*
 * elec_mag_ctrl.c
 *
 *  Created on: Apr 4, 2023
 *      Author: xuzhekai
 */


#include "main.h"
#include "elec_mag_ctrl.h"

void Set_Direction(int direction){
	if (direction == BLACK){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
	}else{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
	}
}


void Set_Magnitude(uint16_t magnitude){
	TIM4->CCR3 = magnitude;

}

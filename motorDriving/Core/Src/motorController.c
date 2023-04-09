#include "motorController.h"
#include <stdio.h>

#define dirBig GPIO_PIN_4
#define dirBigPort GPIOC
#define stepBig GPIO_PIN_1
#define stepBigPort GPIOC
#define slpBig GPIO_PIN_3
#define slpBigPort GPIOC
#define rstBig GPIO_PIN_0
#define rstBigPort GPIOC
#define switchBig GPIO_PIN_5
#define switchBigPort GPIOC
#define turnBig GPIO_PIN_2
#define turnBigPort GPIOB

#define dirSmall GPIO_PIN_7
#define dirSmallPort GPIOD
#define stepSmall GPIO_PIN_6
#define stepSmallPort GPIOD
#define slpSmall GPIO_PIN_5
#define slpSmallPort GPIOD
#define rstSmall GPIO_PIN_4
#define rstSmallPort GPIOD
#define switchSmall GPIO_PIN_5
#define switchSmallPort GPIOD
#define turnSmall GPIO_PIN_2
#define turnSmallPort GPIOD

//Motor Variables
#define smallLength 2090 //TODO update for smaller steps
#define bigLength 2140 //TODO update for smaller steps

//X refers to larger space
struct coordinate curPosition = {0,0};

void motorSetup(){
	HAL_GPIO_WritePin(rstBigPort, rstBig, 1);
	HAL_GPIO_WritePin(slpBigPort, slpBig, 1);
	HAL_GPIO_WritePin(dirBigPort, dirBig, 0);

	HAL_GPIO_WritePin(rstSmallPort, rstSmall, 1);
	HAL_GPIO_WritePin(slpSmallPort, slpSmall, 1);
	HAL_GPIO_WritePin(dirSmallPort, dirSmall, 0);
}


//Square is input of form "{letter}{number}" for example "A4"
struct coordinate convertToCoord(char* square){
	struct coordinate coord;
	coord.x = (2*(square[0] - 'A') + 1)*(bigLength/16.0);
	coord.y = (2*(atoi(&square[1])-1) + 1)*(smallLength/16.0);
	if(coord.x<0 || coord.x>bigLength || coord.y<0 || coord.y>smallLength){
		printf("square %s doesn't exist or handled incorrectly", square);
		exit(1);
	}
	return coord;
}

//Square is input of form "{letter}{number}" for example "A4"
void moveToCoord(char* square){
	struct coordinate pos = convertToCoord(square);
	int deltaX = pos.x - curPosition.x;
	int deltaY = pos.y - curPosition.y;
	curPosition.x += deltaX;
	curPosition.y += deltaY;


	//Update direction moving
	if(deltaX < 0){
		HAL_GPIO_WritePin(dirBigPort, dirBig, 1);
		deltaX = deltaX*-1;
	}
	else{
		HAL_GPIO_WritePin(dirBigPort, dirBig, 0);
	}
	if(deltaY < 0){
		HAL_GPIO_WritePin(dirSmallPort, dirSmall, 1);
		deltaY = deltaY*-1;
	}
	else{
		HAL_GPIO_WritePin(dirSmallPort, dirSmall, 0);
	}


	//Move according to deltas
	while(deltaX > 0 || deltaY > 0){
		if(deltaX > 0){
			HAL_GPIO_WritePin(stepBigPort, stepBig, 1);
		}
		if(deltaY > 0){
			HAL_GPIO_WritePin(stepSmallPort, stepSmall, 1);
		}
		HAL_Delay(1);
		if(deltaX > 0){
			HAL_GPIO_WritePin(stepBigPort, stepBig, 0);
			deltaX--;
		}
		if(deltaY > 0){
			HAL_GPIO_WritePin(stepSmallPort, stepSmall, 0);
			deltaY--;
		}
	}
}

////instructions come in the form of two squares, followed by B/W for black
////and white and P(awn)/R(ook)/H(orse)/B(ishop)/K(ing)/Q(ueen) ex "E2E4BR"
//void executeInstruction(char* instructions){
//	//TODO turn off electromagnet just in case
//	char square1[2];
//	char square2[2];
//
//	strncpy(square1, instructions, 2);
//	strncpy(square2, instructions+2, 2);
//
//	//no matter what kind of move we're making we
//
//	if(square1)
//
//}



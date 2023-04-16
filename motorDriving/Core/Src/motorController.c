#include "motorController.h"
#include "magnetController.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define dirBig GPIO_PIN_5
#define dirBigPort GPIOF
#define stepBig GPIO_PIN_3
#define stepBigPort GPIOF
#define slpBig GPIO_PIN_2
#define slpBigPort GPIOD
#define rstBig GPIO_PIN_12
#define rstBigPort GPIOC
#define switchBig GPIO_PIN_5
#define switchBigPort GPIOD
#define turnBig GPIO_PIN_2
#define turnBigPort GPIOD

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
#define smallLength 2090*8 //TODO update for smaller steps
#define bigLength 2140*8 //TODO update for smaller steps

//#define largestX 2140*16
//#define smallestX 23*16
//#define largestY 1802*16
//#define smallestY 151*16

#define largestX 16800
#define smallestX 19
#define largestY 13500
#define smallestY 1000

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
	coord.x = abs((square[0] - 'A')-7)*((largestX-smallestX)/7)+smallestX;
	coord.y = (atoi(&square[1])-1)*((largestY-smallestY)/7)+smallestY;
		if(coord.x<smallestX || coord.x>largestX || coord.y<smallestY || coord.y>largestY){
		printf("square %s doesn't exist or handled incorrectly", square); //TODO display on board
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
	int counter = 0;

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
			if(counter == 5) counter = 0;
			HAL_GPIO_WritePin(stepSmallPort, stepSmall, 1);
			counter++;
		}
		HAL_Delay(1);
		if(deltaX > 0){
			HAL_GPIO_WritePin(stepBigPort, stepBig, 0);
			deltaX--;
		}
		if(deltaY > 0 && counter != 5){
			HAL_GPIO_WritePin(stepSmallPort, stepSmall, 0);
			deltaY--;
		}
	}
}


void moveToExactCoord(struct coordinate pos){
	int deltaX = pos.x - curPosition.x;
	int deltaY = pos.y - curPosition.y;
	curPosition.x += deltaX;
	curPosition.y += deltaY;
	int counter = 0;

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
			if(counter == 5) counter = 0;
			HAL_GPIO_WritePin(stepSmallPort, stepSmall, 1);
			counter++;
		}
		HAL_Delay(1);
		if(deltaX > 0){
			HAL_GPIO_WritePin(stepBigPort, stepBig, 0);
			deltaX--;
		}
		if(deltaY > 0 && counter != 5){
			HAL_GPIO_WritePin(stepSmallPort, stepSmall, 0);
			deltaY--;
		}
	}
}

// ASSUMES MOVES ARE VALID ALREADY
// move come in the form of two squares, ex "e2e4"
// color is an enum from magnetController BLACK, WHITE, or STOP
// piece is an enum from motorController PAWN ROOK KNIGHT BISHOP QUEEN or KING
void executeInstruction(char* move, int color, int piece){
	setMagnet(STOP);
	char start[3];
	char goal[3];
	char curSquare[3];

	strncpy(start, move, 2);
	start[2] = '\0';
	strncpy(goal, move+2, 2);
	goal[2] = '\0';
	strncpy(curSquare, start, 2);
	curSquare[2] = '\0';

	moveToCoord(start);

	setMagnet(color);

	if(piece == KNIGHT){
		//TODO integrate with display
		int xDelta = goal[0]-start[0];
		int yDelta = goal[1]-start[1];
		if(abs(xDelta)>abs(yDelta)){
			curSquare[0] += xDelta;
			moveToCoord(curSquare);
			curSquare[1] += yDelta;
			moveToCoord(curSquare);
		}
		else{
			curSquare[1] += yDelta;
			moveToCoord(curSquare);
			curSquare[0] += xDelta;
			moveToCoord(curSquare);
		}
	}
	else{
		while(strcmp(curSquare, goal) != 0){
			if(curSquare[0]!=goal[0]){
				curSquare[0] += (goal[0]-curSquare[0] > 0) ? 1:-1;
			}
			if(curSquare[1]!=goal[1]){
				curSquare[1] += (goal[1]-curSquare[1] > 0) ? 1:-1;
			}
			moveToCoord(curSquare);
		}
	}
	setMagnet(STOP);
}

void stepForward(){
	HAL_GPIO_WritePin(dirSmallPort, dirSmall, 0);
	HAL_GPIO_WritePin(stepSmallPort, stepSmall, 1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(stepSmallPort, stepSmall, 0);
	curPosition.y++;
	int y = curPosition.y;
}
void stepBack(){
	HAL_GPIO_WritePin(dirSmallPort, dirSmall, 1);
	HAL_GPIO_WritePin(stepSmallPort, stepSmall, 1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(stepSmallPort, stepSmall, 0);
	curPosition.y--;
	if(curPosition.y < 0) curPosition.y = 0;
	int y = curPosition.y;
}
void stepRight(){
	HAL_GPIO_WritePin(dirBigPort, dirBig, 0);
	HAL_GPIO_WritePin(stepBigPort, stepBig, 1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(stepBigPort, stepBig, 0);
	curPosition.x++;
	int x = curPosition.x;
}
void stepLeft(){
	HAL_GPIO_WritePin(dirBigPort, dirBig, 1);
	HAL_GPIO_WritePin(stepBigPort, stepBig, 1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(stepBigPort, stepBig, 0);
	curPosition.x--;
	if(curPosition.x < 0) curPosition.x = 0;
	int x = curPosition.x;
}



#ifndef motorController_H_
#define motorController_H_

#include "main.h"

//X is the big coordinates Y is the small one
struct coordinate{
	int x;
	int y;
};

//enum to define pieces
enum PIECE{PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING};


void motorSetup();
struct coordinate convertToCoord(char* square);

#endif

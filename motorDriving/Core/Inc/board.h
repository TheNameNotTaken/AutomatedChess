///*
// * board.h
// *
// *  Created on: Apr 4, 2023
// *      Author: Jeremy
// */
//
#ifndef SRC_BOARD_H_
#define SRC_BOARD_H_
//
#include "stm32l4r5xx.h"
#include "stm32l4xx_hal.h"
#include "board.h"
#include <stdlib.h>
#include <stdint.h>
//
//struct move{
//	char start[2]; //letter and number indicating starting location (ex E2)
//	char end[2]; //letter and number indicating ending location (ex E4)
//	char piece; //piece moving, represented all uppercase, following legend below
//	char side; //W for white B for black
//	int capture; //1 if is a capture of another piece 0 otherwise
//};
//


/*
 * chess board has default value shown below
 * uppercase letters mean that a piece is white and undercase means black
 * legend is following:
 * r/R = rook
 * n/N = knight
 * b/B = bishop
 * k/K = king
 * q/Q = queen
 * p/P = pawn
 * - = empty space
 */
extern char board[8][8];

//
//// reads the state of the new board and creates
//// a DYNAMICALLY allocated board that has to be deleted
char** readCurrentBoard();
//
////update board to new board given full board
void updateBoard(char** newBoard);
//
////update board to new board given move
//void updateBoardMove(struct move change);
//
////check if the current reading of the board is the same
//int compareBoard(char** newBoard);
//
////given a new board and current board, find the move made
//struct move findMove(char** newBoard);
//
//=======
///*
// * board.h
// *
// *  Created on: Apr 9, 2023
// *      Author: isabel
// */
//
//typedef struct {
//	Mux** muxArr; // array of mux pointers
//
//	uint32_t *** muxVal; // 12 x 8 x 8 -- some sort of global variable, I'm leaving this in here so the rest of the code makes sense
//	char** rows;
//	char* values;
//} Board;
//
typedef struct {
	struct Pin* sel0;
	struct Pin* sel1;
	struct Pin* sel2;
} Mux;

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} Pin;


// add pin assignments
extern Pin masterSel0;
extern Pin masterSel1;
extern Pin masterSel2;
extern Mux masterMux;

extern Pin colSel0;
extern Pin colSel1;
extern Pin colSel2;
extern Mux colMux;


//Pin *new_pin(GPIO_TypeDef *_port, uint16_t _pin);
int readPin(Pin* pin);
void writePin(Pin* pin, int val);

//Mux* newMux(Pin* pin0, Pin* pin1, Pin* pin2);
void setMuxVal(uint16_t val, Mux* mux);

//Returns board as FEN
char* boardAsFEN();

//// might also need a readMuxVal
//
////char** readNewBoard();
//struct Board* readNewBoard();
//void updateBoard(char** newBoard);
//
#endif /* SRC_BOARD_H_ */

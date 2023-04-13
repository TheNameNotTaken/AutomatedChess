/////*
//// * board.h
//// *
//// *  Created on: Apr 4, 2023
//// *      Author: Jeremy
//// */
////
//#ifndef SRC_BOARD_H_
//#define SRC_BOARD_H_
////
////#include "main.h"
//#include <stdlib.h>
////
////struct move{
////	char start[2]; //letter and number indicating starting location (ex E2)
////	char end[2]; //letter and number indicating ending location (ex E4)
////	char piece; //piece moving, represented all uppercase, following legend below
////	char side; //W for white B for black
////	int capture; //1 if is a capture of another piece 0 otherwise
////};
////
//
///*
// * chess board has default value shown below
// * uppercase letters mean that a piece is white and undercase means black
// * legend is following:
// * r/R = rook
// * n/N = knight
// * b/B = bishop
// * k/K = king
// * q/Q = queen
// * p/P = pawn
// * - = empty space
// */
//char board[8][8] = 	{{'r', 'n', 'b', 'k', 'q', 'b', 'n', 'r'},
//					 {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
//					 {'-', '-', '-', '-', '-', '-', '-', '-'},
//					 {'-', '-', '-', '-', '-', '-', '-', '-'},
//					 {'-', '-', '-', '-', '-', '-', '-', '-'},
//					 {'-', '-', '-', '-', '-', '-', '-', '-'},
//					 {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
//					 {'R', 'N', 'B', 'K', 'Q', 'B', 'N', 'R'}};
//
//
////Next to move, 'w' for white 'b' for black
//char activeColor = 'w';
//
//// add pin assignments
//Mux masterMux = {};
//Mux muxA = {};
//Mux muxB = {};
//Mux muxC = {};
//Mux muxD = {};
//Mux muxE = {};
//Mux muxF = {};
//Mux muxG = {};
//Mux muxH = {};
//Mux muxArr[9] = {masterMux, muxA, muxB, muxC, muxD, muxE, muxF, muxG, muxH};
//
//char pieces[24] = {'p', 'r', 'n', 'b', 'q', 'k', 'P', 'R', 'N', 'B', 'Q', 'K'};
//
//uint32_t hallValues[12][8][8];
////
////// reads the state of the new board and creates
////// a DYNAMICALLY allocated board that has to be deleted
//char** readCurrentBoard();
////
//////update board to new board given full board
//void updateBoard(char** newBoard);
////
//////update board to new board given move
////void updateBoardMove(struct move change);
////
//////check if the current reading of the board is the same
////int compareBoard(char** newBoard);
////
//////given a new board and current board, find the move made
////struct move findMove(char** newBoard);
////
////=======
/////*
//// * board.h
//// *
//// *  Created on: Apr 9, 2023
//// *      Author: isabel
//// */
////
////typedef struct {
////	Mux** muxArr; // array of mux pointers
////
////	uint32_t *** muxVal; // 12 x 8 x 8 -- some sort of global variable, I'm leaving this in here so the rest of the code makes sense
////	char** rows;
////	char* values;
////} Board;
////
//typedef struct {
//	Pin* sel0;
//	Pin* sel1;
//	Pin* sel2;
//} Mux;
////
//typedef struct {
//    GPIO_TypeDef *port;
//    uint16_t pin;
//} Pin;
////
////Pin *new_pin(GPIO_TypeDef *_port, uint16_t _pin);
//int readPin(Pin* pin);
//void writePin(Pin* pin, int val);
//
////Mux* newMux(Pin* pin0, Pin* pin1, Pin* pin2);
//void setMuxVal(uint16_t val, Mux* mux);
//
////Returns board as FEN
//char* boardAsFEN(){
//
//}
////// might also need a readMuxVal
////
//////char** readNewBoard();
////struct Board* readNewBoard();
////void updateBoard(char** newBoard);
////
//#endif /* SRC_BOARD_H_ */

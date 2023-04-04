/*
 * board.h
 *
 *  Created on: Apr 4, 2023
 *      Author: Jeremy
 */

#ifndef SRC_BOARD_H_
#define SRC_BOARD_H_

#include "main.h"

struct move{
	char start[2]; //letter and number indicating starting location (ex E2)
	char end[2]; //letter and number indicating ending location (ex E4)
	char piece; //piece moving, represented all uppercase, following legend below
	char side; //W for white B for black
	int capture; //1 if is a capture of another piece 0 otherwise
};

/*
 * chess board has default value shown below
 * uppercase letters mean that a piece is white and undercase means black
 * legend is following:
 * r/R = rook
 * h/H = horse/knight
 * b/B = bishop
 * k/K = king
 * q/Q = queen
 * p/P = pawn
 * - = empty space
 */
char board[8][8] = 	{{'r', 'h', 'b', 'k', 'q', 'b', 'h', 'r'},
					 {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
					 {'-', '-', '-', '-', '-', '-', '-', '-'},
					 {'-', '-', '-', '-', '-', '-', '-', '-'},
					 {'-', '-', '-', '-', '-', '-', '-', '-'},
					 {'-', '-', '-', '-', '-', '-', '-', '-'},
					 {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
					 {'R', 'H', 'B', 'K', 'Q', 'B', 'H', 'R'}};

// reads the state of the new board and creates
// a DYNAMICALLY allocated board that has to be deleted
char** readCurrentBoard();

//update board to new board given full board
void updateBoard(char** newBoard);

//update board to new board given move
void updateBoard(struct move change);

//given a new board and current board, find the move made
struct move findMove(char** newBoard);

#endif /* SRC_BOARD_H_ */

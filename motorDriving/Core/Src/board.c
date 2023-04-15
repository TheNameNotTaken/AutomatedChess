#include "board.h"
#include "main.h"

#define TOLERANCE 5

extern ADC_HandleTypeDef hadc1;

// add pin assignments
Pin masterSel0 = {GPIOC, GPIO_PIN_6};
Pin masterSel1 = {GPIOB, GPIO_PIN_15};
Pin masterSel2 = {GPIOB, GPIO_PIN_13};
Mux masterMux = {&masterSel0, &masterSel1, &masterSel2};

Pin colSel0 = {GPIOB, GPIO_PIN_12};
Pin colSel1 = {GPIOA, GPIO_PIN_4};
Pin colSel2 = {GPIOB, GPIO_PIN_4};
Mux colMux = {&colSel0, &colSel1, &colSel2};

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
char board[8][8] = 	{{'r', 'n', 'b', 'k', 'q', 'b', 'n', 'r'},
					 {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
					 {'-', '-', '-', '-', '-', '-', '-', '-'},
					 {'-', '-', '-', '-', '-', '-', '-', '-'},
					 {'-', '-', '-', '-', '-', '-', '-', '-'},
					 {'-', '-', '-', '-', '-', '-', '-', '-'},
					 {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
					 {'R', 'N', 'B', 'K', 'Q', 'B', 'N', 'R'}};


//Next to move, 'w' for white 'b' for black
char activeColor = 'w';

char playerColor = 'w';

char pieces[24] = {'p', 'r', 'n', 'b', 'q', 'k', 'P', 'R', 'N', 'B', 'Q', 'K'};

uint32_t hallValues[12][8][8];


//Must free new board afterwards
char** readCurrentBoard(){

	char** newBoard = malloc(8*sizeof(char*));
	//TODO read board with hall sensors'

	uint32_t ADC_VAL = 0;
	// don't worry about enable pins
	for (int i = 0; i < 8; ++i) {
		setMuxVal(i, &masterMux); // 0 -> master mux
		for (int j = 0; j < 8; ++j) {
			setMuxVal(j, &colMux);

			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, 0xffffffff);
			ADC_VAL = HAL_ADC_GetValue(&hadc1);

			uint8_t match = 0;
			for (int k = 0; k < 12; ++k) {
				if (abs((int)ADC_VAL - (int)hallValues[k][i][j]) < TOLERANCE) {
					newBoard[i][j] = pieces[k];
					match = 1;
					break;
				}
			}
			if (!match) {
				newBoard[i][j] = '-';
			}
		}
	}
	//return rows;
	return newBoard;
}



//update board to new board given full board
void updateBoard(char** newBoard){
	for(int i=0; i<8; i++){
		strncpy(board[i], newBoard[i], 8);
	}
}


Mux* newMux(Pin* pin0, Pin* pin1, Pin* pin2) {
	Mux *mux = (Mux *) malloc(sizeof(Mux));
	mux->sel0 = pin0;
	mux->sel1 = pin1;
	mux->sel2 = pin2;
}

void setMuxVal(uint16_t val, Mux* mux) {
	writePin(mux->sel0, val << 2);
	writePin(mux->sel1, (val >> 1) & 1);
	writePin(mux->sel2, val >> 2);
}

Pin *new_pin(GPIO_TypeDef* _port, uint16_t _pin) {
    Pin *pin = (Pin *) malloc(sizeof(Pin));
    pin->port = _port;
    pin->pin = _pin;

    return pin;
}

int readPin(Pin* pin) {
	return HAL_GPIO_ReadPin(pin->port, pin->pin);
}

void writePin(Pin* pin, int val) {
	HAL_GPIO_WritePin(pin->port, pin->pin, val);
}

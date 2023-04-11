//#include "board.h"
//#include "main.h"
//
//#define TOLERANCE 5
//
//
//Board *readNewBoard(Mux** _muxArr){
//	Board *board = (Board *) malloc(sizeof(Board));
//
//	char* _values = calloc(8*8, sizeof(char));
//	board->values = _values;
//
//	char** _rows = malloc(8*sizeof(char*));
//	board->rows = _rows;
//
//	//TODO read board with hall sensors'
//
//	board->muxArr = _muxArr; // mux array initialized in main?
//
//	uint32_t ADC_VAL = 0;
//	// don't worry about enable pins
//	for (int i = 0; i < 8; ++i) {
//		setMuxVal(i, muxArr[0]); // 0 -> master mux
//		for (int j = 0; j < 8; ++j) {
//			setMuxVal(j, muxArr[i+1]);
//
//			HAL_ADC_Start(&hadc1);
//			HAL_ADC_PollForConversion(&hadc1, 0xffffffff);
//			ADC_VAL = HAL_ADC_GetValue(&hadc1);
//
//			for (int k = 0; k < 12; ++k) {
//				if (abs(ADC_VAL - muxVal[k][i][j]) < TOLERANCE) {
//					rows[i][j] = values[k];
//				}
//			}
//		}
//	}
//	//return rows;
//	return board;
//}
//
//
//
////update board to new board given full board
//void updateBoard(char** newBoard){
//
//}
//
//
//Mux* newMux(Pin* pin0, Pin* pin1, Pin* pin2) {
//	Mux *mux = (Mux *) malloc(sizeof(Mux));
//	mux->sel0 = pin0;
//	mux->sel1 = pin1;
//	mux->sel2 = pin2;
//}
//
//void setMuxVal(uint16_t val, Mux* mux) {
//	writePin(mux->sel0, val << 2);
//	writePin(mux->sel1, (val >> 1) & 1);
//	writePin(mux->sel2, val >> 2);
//}
//
//Pin *new_pin(GPIO_TypeDef *_port, uint16_t _pin) {
//    Pin *pin = (Pin *) malloc(sizeof(Pin));
//    pin->port = _port;
//    pin->pin = _pin;
//
//    return pin;
//}
//
//int readPin(Pin* pin) {
//	return HAL_GPIO_ReadPin(pin->port, pin->pin);
//}
//
//void writePin(Pin* pin, int val) {
//	HAL_GPIO_WritePin(pin->port, pin->pin, val);
//}

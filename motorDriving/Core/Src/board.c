//#include "board.h"
//#include "main.h"
//
//#define TOLERANCE 5
//
//
//char** readCurrentBoard(){
//
//	char** newBoard = malloc(8*sizeof(char*));
//	//TODO read board with hall sensors'
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
//			uint8_t match = 0;
//			for (int k = 0; k < 12; ++k) {
//				if (abs((int)ADC_VAL - (int)hallValues[k][i][j]) < TOLERANCE) {
//					newBoard[i][j] = pieces[k];
//					match = 1;
//					break;
//				}
//			}
//			if (!match) {
//				newBoard[i][j] = '-';
//			}
//		}
//	}
//	//return rows;
//	return newBoard;
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

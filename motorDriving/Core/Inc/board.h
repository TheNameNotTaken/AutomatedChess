/*
 * board.h
 *
 *  Created on: Apr 9, 2023
 *      Author: isabel
 */

typedef struct {
	Mux** muxArr; // array of mux pointers

	uint32_t *** muxVal; // 12 x 8 x 8 -- some sort of global variable, I'm leaving this in here so the rest of the code makes sense
	char** rows;
	char* values;
} Board;

typedef struct {
	Pin* sel0;
	Pin* sel1;
	Pin* sel2;
} Mux;

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} Pin;

Pin *new_pin(GPIO_TypeDef *_port, uint16_t _pin);
bool readPin(Pin* pin);
bool writePin(Pin* pin, bool val);

Mux* newMux(Pin* pin0, Pin* pin1, Pin* pin2);
void setMuxVal(uint16_t val, Mux* mux);
// might also need a readMuxVal

//char** readNewBoard();
Board* readNewBoard();
void updateBoard(char** newBoard);

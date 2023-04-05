/*
 * elec_mag_ctrl.h
 *
 *  Created on: Apr 4, 2023
 *      Author: xuzhekai
 */

#ifndef INC_ELEC_MAG_CTRL_H_
#define INC_ELEC_MAG_CTRL_H_

#include "main.h"
enum Direction{BLACK, WHITE};

void Set_Direction(int direction);

void Set_Magnitude(uint16_t magnitude);

#endif /* INC_ELEC_MAG_CTRL_H_ */

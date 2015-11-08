/*
 * Dbg.h
 *
 *  Created on: Oct 24, 2015
 *      Author: armin
 */

#ifndef SRC_DBG_DBG_H_
#define SRC_DBG_DBG_H_
#include "Uart.h"

void Dbg_ReadRegister(uint8 UartHwUnit, uint8 str[], uint8 address);

void Dbg_ReadVariableInteger(uint8 str[], uint32 variable);
void Dbg_ReadVariableFloat(uint8 str[], float32 variable);
#endif /* SRC_DBG_DBG_H_ */

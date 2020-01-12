/*
 * pit.h
 *
 *  Created on date 19/09/2017
 *      Author:Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#ifndef PIT_H_
#define PIT_H_

#include "derivative.h"
#include "util.h"

void initPIT(uint8_t div4, unsigned int periodo);
void enablePITInterrup(void);
void disablePITInterrup(void);

#endif /* PIT_H_ */

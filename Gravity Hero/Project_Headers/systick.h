/*
 * \file systick.h
 *
 * \date 19/09/2017
 * \author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "derivative.h"

void initSysTick (unsigned int periodo);

void enableSysTickInterrup(void);

void disableSysTickInterrup(void);

#endif /* SYSTICK_H_ */

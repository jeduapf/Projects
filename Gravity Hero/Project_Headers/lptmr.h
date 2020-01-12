/*
 * \file lptmr.h
 *
 * \date Aug 11, 2017
 * \author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#ifndef LPTMR_H_
#define LPTMR_H_

#include "derivative.h"

uint8_t initLPTMR0 (uint8_t pbyp, uint8_t pre, uint8_t fonte, uint16_t valor);

void enableInterrupLPTMR0 (void);

#endif /* LPTMR_H_ */

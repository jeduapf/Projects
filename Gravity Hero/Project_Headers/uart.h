/*
 * \file uart.h
 *
 * \date Feb 16, 2017
 * \author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#ifndef UART_H_
#define UART_H_

#include "derivative.h"

uint8_t initUART(uint8_t taxa, unsigned int baud_rate);
void enableTEInterrup (void);
void enableRIEInterrup (void);
void enableREInterrup (void);
void disableREInterrup (void);
void disableTEInterrup (void);
void enableRE (void);
void enableTE (void);
void disableRE (void);
void disableTE (void);

#endif /* UART_H_ */

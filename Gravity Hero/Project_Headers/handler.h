/*
 * \file handler.h
 *
 * \date 10/10/2017
 * \author Jose Eduardo Alves Peraira Filho and Joao Vitor Santos Prado
 */

#ifndef HANDLER_H_
#define HANDLER_H_

#include "derivative.h"
#include "util.h"

void PORTA_IRQHandler(void);
void SysTick_Handler(void);
void PIT_IRQHandler(void);
void NMI_Handler (void);
void UART0_IRQHandler(void);
void FTM1_IRQHandler(void);
void FTM0_IRQHandler(void);
void atualizatela(uint16_t);
void acertou(void);

#define MAX_MUSICA 3;

#endif /* HANDLER_H_ */

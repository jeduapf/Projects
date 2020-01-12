/*
 * \file nvic.h
 *
 * \date Feb 14, 2017
 * \author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#ifndef NVIC_H_
#define NVIC_H_

#include "derivative.h"
#include "util.h"

void enableNVICPTA(uint8_t priority);

void enableNVICPIT(uint8_t priority);

void enableNVICUART(uint8_t priority);

void enableNVICTPM0(uint8_t priority);

void enableNVICTPM1(uint8_t priority);

void enableNVICTPM2(uint8_t priority);

void enableNVICADC0(uint8_t priority);

void enableNVICLPTMR0(uint8_t priority);

#endif /* NVIC_H_ */

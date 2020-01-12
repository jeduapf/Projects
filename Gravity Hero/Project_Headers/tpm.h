/*
 * tpm.h
 *
 *  Created on: August 04, 2017
 *      Author: Wu Shin Ting and Leandro Cavalcanti Silva and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#ifndef TPM_H_
#define TPM_H_
#define COUNTER_OVF 16384

#include "derivative.h"
#include "util.h"

void initTPM(uint8_t modulo, uint8_t pre, uint16_t counter, uint8_t cpwms);

uint8_t initTPM0Cn(uint8_t canal, uint8_t c_modo, unsigned short valor);
uint8_t initTPM1Cn(uint8_t canal, uint8_t c_modo, unsigned short valor);
uint8_t initTPM2Cn(uint8_t canal, uint8_t c_modo, unsigned short valor);

uint8_t reconfigModoTPM0Cn(uint8_t canal, uint8_t c_modo);
uint8_t reconfigModoTPM1Cn(uint8_t canal, uint8_t c_modo);
uint8_t reconfigModoTPM2Cn(uint8_t canal, uint8_t c_modo);

uint8_t reconfigValorTPM0Cn(uint8_t canal, unsigned short valor);
uint8_t reconfigValorTPM1Cn(uint8_t canal, unsigned short valor);
uint8_t reconfigValorTPM2Cn(uint8_t canal, unsigned short valor);

uint8_t enableTPMInterrup(uint8_t modulo);
uint8_t disableTPMInterrup(uint8_t modulo);

uint8_t enableTPM0CnInterrup(uint8_t canal);
uint8_t disableTPM0CnInterrup(uint8_t canal);
uint8_t enableTPM1CnInterrup(uint8_t canal);
uint8_t disableTPM1CnInterrup(uint8_t canal);
uint8_t enableTPM2CnInterrup(uint8_t canal);
uint8_t disableTPM2CnInterrup(uint8_t canal);

#endif /* TPM_H_ */

/*
 * \file port.h
 *
 * \date Mar 4, 2017
 * \author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#ifndef PORT_H_
#define PORT_H_

#include "derivative.h"

/*
 * Enumera&ccedil&atilde;o das portas do microcontrolador
 */
typedef enum _tipo_port {
	A,   /*!< A */
	B,   /*!< B */
	C,   /*!< C */
	D,   /*!< D */
	E    /*!< E */
} porta;

void initPort (porta p);

void setaMux(uint32_t volatile *reg, uint8_t mux);

void resetaMux(uint32_t volatile *reg);

void enableIRQ(uint32_t volatile *reg, uint8_t irq);
#endif /* PORT_H_ */

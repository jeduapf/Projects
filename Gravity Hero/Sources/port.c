/*
 * port.c
 *
 * \date 19/09/2017
 * \author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#include "port.h"
/*!
 * \brief Habilita o clock de um m&oacute;dulo PORT
 * \param[in] p porta
 */
void initPort (porta p)
{
	switch (p) {
	case A:
		SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
		break;
	case B:
		SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
		break;
	case C:
		SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
		break;
	case D:
		SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
		break;
	case E:
		SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	}
}

/*!
 * \brief seta a fun&ccedil;&atilde;o do pino
 * \param[in] reg endere&ccedil;o do registrador do pino n da PORTA x
 * \param[in] mux fun&ccedil;&atilde;o do pino
 */
void setaMux(uint32_t volatile *reg, uint8_t mux) {
	*reg &= ~PORT_PCR_MUX(0b111);      ///< limpa o campo MUX (0b000)
	
	*reg |= (PORT_PCR_ISF_MASK |       ///< ISF=PORTB_PCR18[14]: w1c (limpa a pendência)
			 PORT_PCR_MUX(mux) );      ///< seta fun&ccedil;&atilde;o 
}

/*!
 * \brief reseta o campo MUX do registrador do pino em 000
 * \param[in] reg endere&ccedil;o do registrador do pino n da PORTA x
 * \param[in] mux fun&ccedil;&atilde;o do pino
 */
void resetaMux(uint32_t volatile *reg) {
	*reg |= PORT_PCR_ISF_MASK;        ///< ISF=PORTB_PCR18[14]: w1c (limpa a pendência)
	*reg &=	~PORT_PCR_MUX(0b111);        ///< reseta MUX/desabilita pino  
}

void enableIRQ(uint32_t volatile *reg, uint8_t irq) {
	*reg &= ~PORT_PCR_IRQC(0b1111);       ///< zerar o campo (desabilitar IRQ)
	*reg |= (PORT_PCR_ISF_MASK | PORT_PCR_IRQC(irq)); ///< seta IRQ           
}

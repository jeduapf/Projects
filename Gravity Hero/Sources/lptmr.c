/*
 * \file lptmr.c
 *
 * \date Aug 11, 2017
 * \author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#include "lptmr.h"

/*!
 * \fn uint8_t initLPTMR(uint8_t pbyp, uint8_t pre, uint8_t fonte, uint16_t valor)
 * \param[in] pbyp bypassed (1) ou não (0) pr&eacute;-escala
 * \param[in] pre pr&eacute-escala
 * \param[in] fonte MCGIRCLK(0b00)/LPO(0b01)/ERC32CLK(0b10)/OSCERCLK(0b11)
 * \param[in] valor valor em que CNR &eacute; resetado
 * \return c&oacute;digo de erro (0-OK; 1-fonte n&atilde;o suportada)
 */
uint8_t initLPTMR0 (uint8_t pbyp, uint8_t pre, uint8_t fonte, uint16_t valor)
{
	SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

	/*!
	 * Selecionar o modo de contagem
	 */
	LPTMR0_CSR |= LPTMR_CSR_TPS(0b00);      ///< seleciona pino de entrada 0 

	LPTMR0_CSR &= ~(LPTMR_CSR_TPP_MASK   ///< Polaridade: contador CNR incrementa na borda de subida (0) 
			| LPTMR_CSR_TFC_MASK  ///< Contador CNR reseta quando CNR = CMR
			| LPTMR_CSR_TMS_MASK  ///< Modo de contagem por tempo
	);

	/*!
	 * Selecionar a base de tempo dentre MCGIRCLK(0b00)/LPO(0b01)/OSCERCLK(0b11)
	 * <a href="ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf#page=124">LPTMR Clocking</a>
	 */
	if (fonte == 0b10) return 1;   ///< modo n&atilde;o suportado

	LPTMR0_PSR |= LPTMR_PSR_PCS(fonte);    ///< fonte de base de tempo

	if (pbyp)
		LPTMR0_PSR |= LPTMR_PSR_PBYP_MASK;     ///<  bypass prescaler clock ou fonte de pulsos
	else {
		LPTMR0_PSR &= ~LPTMR_PSR_PBYP_MASK;     
		LPTMR0_PSR |= LPTMR_PSR_PRESCALE(pre); ///< Pr&eacute;-escala
	}

	LPTMR0_CMR = LPTMR_CMR_COMPARE(valor);  ///< setar o valor CMR

	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;   ///< habilitar LPTMR
	
	return 0;
}

void enableInterrupLPTMR0 (void) {
    LPTMR0_CSR |= ( LPTMR_CSR_TCF_MASK   ///< Limpar pend&ecirc;ncias (w1c)
                  | LPTMR_CSR_TIE_MASK   ///< Habilitar interrup&ccedil;&atilde;o
                  );
}

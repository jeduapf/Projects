/*
 * \file nvic.c
 *
 * \date Feb 14, 2017
 * \author: Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#include "nvic.h"

/*!
 * \brief Habilita interrup&ccedil;&otilde;es da PORTA no NVIC.
 * \param[in] priority level
 */
void enableNVICPTA(uint8_t priority) {
	/* 
	 * "Non-core interrupt source count" (IRQ = Vector Number (46) - 16, 
	 * Table 3-7 de KL-25 Sub-Family Reference Manual) &eacute; habilitado
	 * ao escrever 1 no bit correpondente do registrador NVIC_ISER 
	 * (B3.4.3 de ARMv6-M Architecture Reference Manual) e a solicita&ccedil;&atilde;o de 
	 * interrup&ccedil;&atilde;o &eacute; limpa ao escrever 1 no bit correspondente do 
	 * registrador NVIC_ICPR (B3.4.6 de ARMv6-M Architecture Reference Manual) 
	 */
	/*! PORTA: Vetor 46, IRQ30, prioridade em NVICIPR7[23:16]
	 * Se&ccedil;&atilde;o 3.3.2.3 em 
	 * <a href="ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf" target=_manual>ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf</a>
	 */
    NVIC_ISER |= 1 << (46-16);            ///< Habilita interrup&ccedil;&atilde;o PORTA
    NVIC_ICPR |= 1 << (46-16);
    NVIC_IPR7 |= NVIC_IP_PRI_30(priority << 6);    ///< N&iacute;vel de prioridade = 3
}

/**
 * \brief Habilita interrup&ccedil;&atilde;o do m&oacute;dulo PIT 
 * (vetor de interrup&ccedil;&atilde;o = 38) no NVIC.
 * \param[in] priority level
 */
void enableNVICPIT(uint8_t priority) {
    NVIC_ISER |= 1 << (38-16);         ///< NVIC_ISER[22]=1 (habilita IRQ22)
    NVIC_ICPR |= 1 << (38-16);         ///< NVIC_ICPR[22]=1 (limpa as pendências)
    NVIC_IPR5 |= NVIC_IP_PRI_22(priority << 6); ///< NVIC IPR register number na Table 3-7 
    						           ///< de KL-25 Sub-Family Reference Manual
    		                           ///< seta prioridade 1			
}

/*!
 * \brief Habilita interrup&ccedil;&atilde;o da UART via NVIC.
 * \param[in] priority level
 */
void enableNVICUART(uint8_t priority) {
	/* 
	 * "Non-core interrupt source count" (IRQ = Vector Number (46) - 16, 
	 * Table 3-7 de KL-25 Sub-Family Reference Manual) &eacute; habilitado
	 * ao escrever 1 no bit correpondente do registrador NVIC_ISER 
	 * (B3.4.3 de ARMv6-M Architecture Reference Manual) e a solicita&ccedil;&atilde;o de 
	 * interrup&ccedil;&atilde;o &eacute; limpa ao escrever 1 no bit correspondente do 
	 * registrador NVIC_ICPR (B3.4.6 de ARMv6-M Architecture Reference Manual) 
	 */
	/*! UART0: Vetor 28, IRQ12, prioridade em NVICIPR3[7:0]
	 * Se&ccedil;&atilde;o 3.3.2.3 em 
	 * <a href="ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf" target=_manual>ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf</a>
	 */
    NVIC_ISER |= 1 << (28-16);            // i) Habilite interrup&ccedil;&atilde;o UART0
    NVIC_ICPR |= 1 << (28-16);            // e limpe as pendencias
    NVIC_IPR3 |= NVIC_IP_PRI_12(priority << 6);    ///< j) Sete o nivel de prioridade em 1.
}

/**
 * \brief Habilita interrupção do módulo TPM0 (vetor de interrupção = 33) via NVIC.
 * \param[in] priority level
 *
 */
void enableNVICTPM0(uint8_t priority) {
    NVIC_ISER |= 1 << (33-16) ;      ///< NVIC_ISER[17]=1 (habilita IRQ17)
    NVIC_ICPR |= 1 << (33-16) ;      ///< NVIC_ICPR[17]=1 (limpa as pend&ecird;ncias)
    NVIC_IPR4 |= NVIC_IP_PRI_17(priority << 6); ///< seta prioridade 		
}

/**
 * \brief Habilita interrupção do módulo TPM1 (vetor de interrupção = 34) via NVIC.
 * \param[in] priority level
 *
 */
void enableNVICTPM1(uint8_t priority) {
    NVIC_ISER |= 1 << (34-16) ;      ///< NVIC_ISER[18]=1 (habilita IRQ18)
    NVIC_ICPR |= 1 << (34-16) ;      ///< NVIC_ICPR[18]=1 (limpa as pend&ecird;ncias)
    NVIC_IPR4 |= NVIC_IP_PRI_18(priority << 6); ///< seta prioridade  		
}

/**
 * \brief Habilita interrupção do módulo TPM0 (vetor de interrupção = 35) via NVIC.
 * \param[in] priority level
 *
 */
void enableNVICTPM2(uint8_t priority) {
    NVIC_ISER |= 1 << (35-16) ;      ///< NVIC_ISER[19]=1 (habilita IRQ19)
    NVIC_ICPR |= 1 << (35-16) ;      ///< NVIC_ICPR[19]=1 (limpa as pend&ecird;ncias)
    NVIC_IPR4 |= NVIC_IP_PRI_19(priority << 6); ///< seta prioridade 		
}

/*!
 * \fn enableNVICADC (void)
 * \brief Habilita interrupções via NVIC.
 */
void enableNVICADC0(uint8_t priority) {
	/* 
	 * "Non-core interrupt source count" (IRQ = Vector Number (46) - 16, 
	 * Table 3-7 de KL-25 Sub-Family Reference Manual) &eacute; habilitado
	 * ao escrever 1 no bit correpondente do registrador NVIC_ISER 
	 * (B3.4.3 de ARMv6-M Architecture Reference Manual) e a solicita&ccedil;&atilde;o de 
	 * interrup&ccedil;&atilde;o &eacute; limpa ao escrever 1 no bit correspondente do 
	 * registrador NVIC_ICPR (B3.4.6 de ARMv6-M Architecture Reference Manual) 
	 */
	/*! ADC0: Vetor 31, IRQ15, prioridade em NVICIPR3[7:0]
	 * Se&ccedil;&atilde;o 3.3.2.3 em 
	 * <a href="ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf" target=_manual>ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf</a>
	 */
	NVIC_ISER |= NVIC_ISER_SETENA(GPIO_PIN(31-16));   ///< Habilita interrup&ccedil;&atilde;o ADC0                               
	NVIC_ICPR |= NVIC_ICPR_CLRPEND(GPIO_PIN(31-16));  ///< Limpa as poss&iacute;veis pend&ecirc;ncias do ADC0                               
    NVIC_IPR3 |= NVIC_IP_PRI_15(priority << 6); ///< seta prioridade 		
}

/*!
 * \fn enableNVICLPTMR0 (void)
 * \brief Habilita interrupções via NVIC.
 */
void enableNVICLPTMR0(uint8_t priority) {
	/*! LPTMR0: Vetor 44, IRQ28, prioridade em NVICIPR7[7:0]
	 * Se&ccedil;&atilde;o 3.3.2.3 em 
	 * <a href="ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf" target=_manual>ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf</a>
	 */
	NVIC_ISER |= NVIC_ISER_SETENA(GPIO_PIN(44-16));   ///< Habilita interrup&ccedil;&atilde;o ADC0                               
	NVIC_ICPR |= NVIC_ICPR_CLRPEND(GPIO_PIN(44-16));  ///< Limpa as poss&iacute;veis pend&ecirc;ncias do ADC0                               
    NVIC_IPR7 |= NVIC_IP_PRI_28(priority << 6); ///< seta prioridade 		
}

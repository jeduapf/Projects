/*!
 * \file pit.c
 *
 * \date 19/09/2017
 * \author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#include "pit.h"

/**
 * \fn InitPIT (void)
 * 
 * \brief Configura o timer 0 do PIT para que gere um sinal periodico 2*periodo/20971520.
 * \param[in] div4 divisor de frequencia do nucleo
 * \param[in] periodo contagem maxima
 */
void initPIT(uint8_t div4, unsigned int periodo) {
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;        ///< TPM1=SIM_SCGC6[23]=1 (Clock gate de PIT habilitado)
	SIM_CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV4(0b111);  ///< OUTDIV1=SIM_CLKDIV1[18:16]=0b000
	SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV4(div4);  ///< OUTDIV1=SIM_CLKDIV1[18:16]=0b000 (dividido por 1)

	/*
	 * Reinicializa novo ciclo de contagem
	 */
	PIT_TCTRL0 &= ~(PIT_TCTRL_TEN_MASK | ///< TEN=PIT_CTRL0[31]=0 (desativa timer 0)
			PIT_TCTRL_TIE_MASK );    ///< TIE=PIT_CTRL0[30]=0 (desabilita interrupca do timer 0)
	PIT_LDVAL0 = PIT_LDVAL_TSV(periodo);       ///< TSV=PIT_LDVAL0[31:0]=x              
	
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK;      ///< TIF=PIT_TFLG0[31]: w1c (limpa pendências)

	PIT_TCTRL0 |= (PIT_TCTRL_TEN_MASK);  ///< TEN=PIT_CTRL0[31]=1 (ativa timer 0)

	PIT_TCTRL0 &= ~PIT_TCTRL_CHN_MASK;   ///< CHN=PIT_CTRL0[29]=0 (nao encadeado com o predecessor)
	
	PIT_MCR &= ~(PIT_MCR_FRZ_MASK |      ///< FRZ=PIT_MCR[31]=0 (continua contagem no modo Debug)
		PIT_MCR_MDIS_MASK );         ///< MDIS=PIT_MCR[30]=0 (habilita clock)
}

/*!
 * \brief Habilita a interrupcao do temporizador PIT
 */
void enablePITInterrup(void) {
	PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK;    ///< TIE=PIT_CTRL0[30]=1 (habilita interrupcao do timer 0)
}

/*!
 * \brief Desabilita a interrupcao do temporizador PIT
 */
void disablePITInterrup(void) {
	PIT_TCTRL0 &= ~PIT_TCTRL_TIE_MASK;    ///< TIE=PIT_CTRL0[30]=0 (desabilita interrupcao do timer 0)
}

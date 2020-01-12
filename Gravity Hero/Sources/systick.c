/*
 * \file systick.c
 *
 * \date 19/09/2017
 * \author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#include "systick.h"
extern uint8_t som_play;

/*!
 * \fn initSysTick (void)
 * \brief Inicializa o temporizador SysTick.
 */
void initSysTick (unsigned int periodo) {
	SYST_RVR = SysTick_RVR_RELOAD(periodo);   
	SYST_CVR = SysTick_CVR_CURRENT(0);   ///< zerar o contador             
	SYST_CSR |= (SysTick_CSR_CLKSOURCE_MASK |    ///< clock source = core clock 
			SysTick_CSR_ENABLE_MASK); ///< habilita SysTick           
}

/*!
 * \brief Habilita interrupcao do SysTick
 */
void enableSysTickInterrup (void){
	som_play = 1;
	SYST_CSR |= SysTick_CSR_TICKINT_MASK;	
}

/*!
 * \brief Desabilita interrupcao do SysTick
 */
void disableSysTickInterrup (void){
	som_play = 0;
	SYST_CSR &= ~SysTick_CSR_TICKINT_MASK;	
}

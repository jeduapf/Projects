/*
 * \file uart.c
 *
 * \date Feb 16, 2017
 * \author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#include "uart.h"

/*!
 * \fn initUART (void)
 * \brief Inicializa os pinos conectados a UART0 e ao OpenSDA.
 * \return codigo de erro (1=taxa invalida; 2=baud invalido; 3=combinacao invalida)
 */
uint8_t initUART(uint8_t taxa, unsigned int baud_rate) {
	
	/*! Validacoes */
	/*!
	 * Valida o valor da taxa 
	 */
	if (taxa < 4 || taxa > 32) return 1;
	/*!
	 * Valida o valor de baud
	 */
	if (baud_rate != 300 && baud_rate != 1200 && baud_rate != 2400 && baud_rate != 4800 
			&& baud_rate != 9600 && baud_rate != 19200 && baud_rate != 38400 
			&& baud_rate != 57600 && baud_rate != 115200) return 2;
	/*!
	 * Computar SBR valido
	 */
	uint32_t SBR = (uint32_t)(20971520./(baud_rate * taxa));
	
	if (SBR > 0x1FFF) return 3;
	
	/*!
	 * Configuracao de SIM
     */
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;		///< Configura o SIM para PORTA
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;		///< Configura o SIM para UART0
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(0b01);	///< configura a fonte de relógio (20.971520MHz)
	SIM_SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;	 
	                                         ///< no reset, o valor é zero (FLL)
	                                         ///< mas não deixa de ser uma boa
	                                         ///< prática de inicializar explicitamente
	
	/*!
	 * Configuracao dos pinos que servem UART0 
     */
	PORTA_PCR1 |= PORT_PCR_MUX(0b010);         ///< Configure pino com funcao UART0_RX
	PORTA_PCR2 |= PORT_PCR_MUX(0b010);         ///< Configure pino com funcao UART0_TX
		
	/*!
	 * Configuracao do modulo UART0 
         */
	UART0_C1 &= ~(UART0_C1_PE_MASK |			///< Configure "no paridade"
			UART0_C1_ILT_MASK |
			UART0_C1_WAKE_MASK |
			UART0_C1_M_MASK |                   ///< Configure "dados de 8 bits"
			UART0_C1_DOZEEN_MASK |
			UART0_C1_LOOPS_MASK );
	/*!
	 * Desabilita canal receptor e transmissor para configurar baud rate
	 */
	UART0_C2 &= ~(UART_C2_RE_MASK | UART_C2_TE_MASK);
	
	UART0_BDH &= ~(UART_BDH_SBNS_MASK |     ///< Configura numero de Stop Bits
				UART_BDH_RXEDGIE_MASK | 
				UART_BDH_LBKDIE_MASK);
	
	/*!
	 * Habilite BOTHEDGE se taxa de superamostragem é x4 a x7
	 */
	if (taxa < 8)
		UART0_C5 |= UART0_C5_BOTHEDGE_MASK;       ///< amostra as duas bordas nos dados do receptor (entre taxa de amostragem 4x e 7x)

	/*!
	 * Taxa de superamostragem. O padrao e 0x0F e os valores devem ser maiores que 3
	 */
	UART0_C4 |= ~(UART0_C4_OSR(0b00000));            ///< Resetar o campo 
	UART0_C4 &= (UART0_C4_OSR(taxa-1));				///< Por definicao a tava 4x, esta pre configurada para 0b00011
	
	/*!
	 * Setar SBR
	 */
	UART0_BDH &= ~UART_BDH_SBR(0b11111);                             
	UART0_BDL &= ~UART_BDL_SBR(0b11111111);            
	UART0_BDH |= UART_BDH_SBR((SBR & 0x1F00)>>8);                             
	UART0_BDL |= UART_BDL_SBR(SBR & 0x00FF);            
		
	UART0_S1 |= (UART0_S1_PF_MASK |              ///< Registradores de estado: w1c
				UART0_S1_FE_MASK |
				UART0_S1_NF_MASK |
				UART0_S1_OR_MASK |
				UART0_S1_IDLE_MASK);
	
	UART0_C2 |= (UART_C2_RE_MASK |         		///< habilita o canal receptor 
				 UART_C2_TE_MASK);	  			///< habilita o canal transmissor  
	
	return 0;
}

/*!
 * \brief Habilita a interrupcao do canal receptor
 */
void enableRIEInterrup (void) {
	UART0_C2 |= UART_C2_RIE_MASK;
	
}

/*!
 * \brief Habilita a interrupcao do canal transmissor
 */
void enableTEInterrup (void) {
	UART0_C2 |= UART_C2_TIE_MASK;
}

/*!
 * \brief Desabilita a interrupcao do canal receptor
 */
void disableRIEInterrup (void) {
	UART0_C2 &= ~UART_C2_RIE_MASK;
}

/*!
 * \brief Desabilita a interrupcao do canal transmissor
 */
void disableTEInterrup (void) {
	UART0_C2 &= ~UART_C2_TIE_MASK;
	
}

/*!
 * \brief Habilita o canal receptor
 */
void enableRE (void) {
	UART0_C2 |= UART_C2_RE_MASK;
	
}

/*!
 * \brief Habilita o canal transmissor
 */
void enableTE (void) {
	UART0_C2 |= UART_C2_TE_MASK;
}

/*!
 * \brief Desabilita o canal receptor
 */
void disableRE (void) {
	UART0_C2 &= ~UART_C2_RE_MASK;
}

/*!

 * \brief Desabilita o canal transmissor

 */
void disableTE (void) {
	UART0_C2 &= ~UART_C2_TE_MASK;
	
}

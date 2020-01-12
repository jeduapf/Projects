/*!
 * @file pushbutton.c
 *
 * @date 19/09/2017
 * @author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#include "pushbutton.h"
#include "atrasos.h"

/**
* @brief Inicializa botoeiras 
*/
void initPushbutton (void) {
    /*!  Habilita os clocks dos módulos */
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK) ;    

    /*! Configura pino A4 como GPIO (MUX = 001) */
    PORTA_PCR4 &= ~PORT_PCR_MUX(0b111); /*! Pois o pino A4 não inicia com 000..., e sim com 111*/
    PORTA_PCR4 |= PORT_PCR_MUX(0x1);

    /*! Configura pino A5 como GPIO (MUX = 001) */
    PORTA_PCR5 |= PORT_PCR_MUX(0x1);

    /*! Configura pino A12 como GPIO (MUX = 001) */
    PORTA_PCR12 |= PORT_PCR_MUX(0x1);

    /*! Configura a direcao dos pinos 4, 5 e 12 da PORTA como entrada */
    GPIOA_PDDR &= ~(GPIO_PDDR_PDD(GPIO_PIN(4)) |
    		GPIO_PDDR_PDD(GPIO_PIN(5)) |
    		GPIO_PDDR_PDD(GPIO_PIN(12)));
}

/**
 * @brief Habilita a interrupcao dos pinos em que pushbutton estao ligados
 */
void enablePushbuttonIRQA(void) {
	PORTA_PCR4 |= PORT_PCR_ISF_MASK | 
				  PORT_PCR_IRQC(0b1010);       ///< sensivel a borda de descida
	PORTA_PCR5 |= PORT_PCR_ISF_MASK | 
				  PORT_PCR_IRQC(0b1011);       ///< sensivel a duas bordas
	PORTA_PCR12 |= PORT_PCR_ISF_MASK | 
		      	  PORT_PCR_IRQC(0b1010);               ///< sensivel a borda de descida
}

/**
* @brief L&ecirc; o valor do pino 12 da porta A
* @param [in] pta 0 (PTA5) ou 1 (PTA12)
* @return estado do botao (0 apertado; 1 normal)
*/
short le_pta (enum _tipo_pta pta) {
	volatile unsigned int a;
	short tmp;

	a = GPIOA_PDIR;       /*! Registrador de entrada na PORTA (32 bits) */
	switch (pta) {
	case PTA4: tmp = (short) (a & GPIO_PIN(4));
	break;
	case PTA5: tmp = (short) (a & GPIO_PIN(5));
	break;
	case PTA12: tmp = (short) (a & GPIO_PIN(12));
	}
	return tmp;
}

/*! No pino 23 que vamos conectar o cabo de SOM entao precisamos gerar nele um sinal com a frequencia respectiva da nota*/
/*!
 * \fn initPORTE(void)
 * \brief Inicializa porta E
 */
void initPORTE(void) {
	SIM_SCGC5 = SIM_SCGC5 | (1<<13);    	/*!  Habilita clock GPIO do PORTE */
	PORTE_PCR23 = PORTE_PCR23 & 0xFFFFF8FF; /*! Zera bits 10, 9 e 8 (MUX) de PTE23 */
	PORTE_PCR23 = PORTE_PCR23 | 0x00000100; /*! Seta bit 8 do MUX de PTE23, assim os 3 bits de MUX serao 001 = funcao GPIO */
	GPIOE_PDDR  = GPIOE_PDDR  | (1<<23);    /*! Seta direcao do pino 23 de PORTE como saida */	
}

/*!
 * \fn ligaPORTE(void)
 * \brief Seta bit = 1
 */
void ligaPORTE(void) {
	GPIOE_PCOR = (1<<23); 	  /*! Limpa bit 23*/	
}
/*!
 * \fn apagaPORTE(void)
 * \brief Seta bit = 0
 */
void apagaPORTE(void) {
	GPIOE_PSOR = (1<<23);     /*! Seta bit 23*/
}

/*!
 * \fn alternaPORTE(void)
 * \brief Alterna bit
 */
void alternaPORTE(void) {
	GPIOE_PTOR = (1<<23);     /*! Alterna bit 23 */
}

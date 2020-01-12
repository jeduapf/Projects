/*!
 * @file ledRGB.c
 * @brief Gerencia o led RGB
 * \date 19/09/2017
 * \author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */
#include "ledRGB.h"

/*!
 * @brief Inicializa os registradores relacionados com o led R
 */
void initLedR() {
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK ;     /*! Habilita clock GPIO do PORTB */

	PORTB_PCR18 |= PORT_PCR_MUX(0x1);       /*! Seta bit 8 do MUX de PTB18, assim os 3 bits de MUX serao 001 = funcao GPIO */

	GPIOB_PDDR |= GPIO_PDDR_PDD(GPIO_PIN(18));   /*! Seta direcao do pino 18 de PORTB como saida */
}
/*!
 * @brief Inicializa os registradores relacionados com o led G
 */
void initLedG() {
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK ;     /*! Habilita clock GPIO do PORTB */

	PORTB_PCR19 |= PORT_PCR_MUX(0x1);       /*! Seta bit 8 do MUX de PTB19, assim os 3 bits de MUX serao 001 = funcao GPIO */

	GPIOB_PDDR |= GPIO_PDDR_PDD(GPIO_PIN(19));   /*! Seta direcao do pino 19 de PORTB como saida */
}
/*!
 * @brief Inicializa os registradores relacionados com o led B
 */
void initLedB() {
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK ;     /*! Habilita clock GPIO do PORTD */

	PORTB_PCR1 |= PORT_PCR_MUX(0x1);       /*! Seta bit 8 do MUX de PTB1, assim os 3 bits de MUX serao 001 = funcao GPIO */

	GPIOB_PDDR |= GPIO_PDDR_PDD(GPIO_PIN(1));   /*! Seta direcao do pino 1 de PORTD como saida */
}
/*
 * @brief Inicializa o led RGB
 */
void initLedRGB() {
	SIM_SCGC5 |= (SIM_SCGC5_PORTB_MASK |
			SIM_SCGC5_PORTD_MASK) ;    
	/*! Configura pino B18 como GPIO (MUX = 001) */
	PORTB_PCR18 |= PORT_PCR_MUX(0x1);
	/*! Configura pino B19 como GPIO (MUX = 001) */
	PORTB_PCR19 |= PORT_PCR_MUX(0x1);
	/*! Configura a direcao dos pinos 18 e 19 da PORTB como saida */
	GPIOB_PDDR |= GPIO_PDDR_PDD(GPIO_PIN(18)) |
			GPIO_PDDR_PDD(GPIO_PIN(19));
	/*! Inicializa os pinos 18 e 19 */
	GPIOB_PSOR = GPIO_PSOR_PTSO(GPIO_PIN(18)) |
			GPIO_PSOR_PTSO(GPIO_PIN(19));
	/*! Configura pino D1 como GPIO (MUX = 001) */
	PORTD_PCR1 |= PORT_PCR_MUX(0x1);
	/*! Configura a diirecao dos pino 1 da PORTD como saida */
	GPIOD_PDDR |= GPIO_PDDR_PDD(GPIO_PIN(1));
	/*! Inicializa o pino 1 */
	GPIOD_PSOR |= GPIO_PSOR_PTSO(GPIO_PIN(1));
}
/*!
 * @brief Liga led RGB
 * @note
 * Atribuir 0 nos bits do registrador PCOR nao afeta o estado dos pinos correspondentes.
 */
void ligaLedRGB() {
	GPIOB_PCOR |= GPIO_PCOR_PTCO(GPIO_PIN(18));  /*! Limpa bit 18, LED vermelho em PTB18 (acende) */
	GPIOB_PCOR |= GPIO_PCOR_PTCO(GPIO_PIN(19));  /*! Limpa bit 19, LED VERDE em PTB19 (acende) */
	GPIOD_PCOR |= GPIO_PCOR_PTCO(GPIO_PIN(1));  /*! Limpa bit 1, LED AZUL em PTD1 (acende) */
}
/*!
 * @brief Apaga led R
 * @note
 * Atribuir 0 nos bits do registrador PSOR nao afeta o estado dos pinos correspondentes.
 */
void apagaLedRGB() {
	GPIOB_PSOR |= GPIO_PSOR_PTSO(GPIO_PIN(18));  /*! SETA bit 18, LED vermelho em PTB18 (acende) */
	GPIOB_PSOR |= GPIO_PSOR_PTSO(GPIO_PIN(19));  /*! SETA bit 19, LED VERDE em PTB19 (acende) */
	GPIOD_PSOR |= GPIO_PSOR_PTSO(GPIO_PIN(1));  /*! SETA bit 1, LED AZUL em PTD1 (acende) */
}
/*!
 * @brief Alterna led RGB
 * @note
 * Atribuir 0 nos bits do registrador PTOR nao afeta o estado dos pinos correspondentes.
 */
void alternaLedRGB() {
	GPIOB_PSOR |= GPIO_PTOR_PTTO(GPIO_PIN(18));  /*! ALTERNA bit 18, LED vermelho em PTB18 (acende) */
	GPIOB_PSOR |= GPIO_PTOR_PTTO(GPIO_PIN(19));  /*! ALTERNA bit 19, LED VERDE em PTB19 (acende) */
	GPIOD_PSOR |= GPIO_PTOR_PTTO(GPIO_PIN(1));  /*! ALTERNA bit 1, LED AZUL em PTD1 (acende) */
}
/*!
 * @brief Liga led R
 * @note
 * Atribuir 0 nos bits do registrador PCOR nao afeta o estado dos pinos correspondentes.
 */
void ligaLedR() {
	GPIOB_PCOR |= GPIO_PCOR_PTCO(GPIO_PIN(18));  /*! Limpa bit 18, LED vermelho em PTB18 (acende) */	
}
/*!
 * @brief Apaga led R
 * @note
 * Atribuir 0 nos bits do registrador PSOR nao afeta o estado dos pinos correspondentes.
 */
void apagaLedR() {
	GPIOB_PSOR = GPIO_PSOR_PTSO(GPIO_PIN(18));     /*! Seta bit 18, LED vermelho em PTB18 (apaga) */
}
/*!
 * @brief Alterna led R
 * @note
 * Atribuir 0 nos bits do registrador PTOR nao afeta o estado dos pinos correspondentes.
 */
void alternaLedR() {
	GPIOB_PTOR = GPIO_PTOR_PTTO(GPIO_PIN(18));     /*! Seta bit 18, LED vermelho em PTB18 (alterna) */
}
/*!
 * @brief Liga led G
 * @note
 * Atribuir 0 nos bits do registrador PCOR nao afeta o estado dos pinos correspondentes.
 */
void ligaLedG() {
	GPIOB_PCOR |= GPIO_PCOR_PTCO(GPIO_PIN(19));  /*! Limpa bit 19, LED VERDE em PTB19 (acende) */
}
/*!
 * @brief Apaga led G
 * @note
 * Atribuir 0 nos bits do registrador PSOR nao afeta o estado dos pinos correspondentes.
 */
void apagaLedG() {
	GPIOB_PSOR = GPIO_PSOR_PTSO(GPIO_PIN(19));     /*! Seta bit 19, LED VERDE em PTB19 (apaga) */
}
/*!
 * @brief Alterna led G
 * @note
 * Atribuir 0 nos bits do registrador PTOR nao afeta o estado dos pinos correspondentes.
 */
void alternaLedG() {
	GPIOB_PTOR = GPIO_PTOR_PTTO(GPIO_PIN(1));     /*! Seta bit 1, LED VERDE em PTB19 (alterna) */
}
/*!
 * @brief Liga led B
 * @note
 * Atribuir 0 nos bits do registrador PCOR nao afeta o estado dos pinos correspondentes.
 */
void ligaLedB() {
	GPIOD_PCOR |= GPIO_PCOR_PTCO(GPIO_PIN(1));  /*! Limpa bit 19, LED AZUL em PTB19 (acende) */
}
/*!
 * @brief Apaga led B
 * @note
 * Atribuir 0 nos bits do registrador PSOR nao afeta o estado dos pinos correspondentes.
 */
void apagaLedB() {
	GPIOD_PSOR = GPIO_PSOR_PTSO(GPIO_PIN(1));     /*! Seta bit 1, LED AZUL em PTD1 (apaga) */
}
/*!
 * @brief Alterna led B
 * @note
 * Atribuir 0 nos bits do registrador PTOR nao afeta o estado dos pinos correspondentes.
 */
void alternaLedB() {
	GPIOD_PTOR = GPIO_PTOR_PTTO(GPIO_PIN(1));     /*! Seta bit 1, LED AZUL em PTD1 (alterna) */
}

///Ting: voce tem que inserir a variavel "estado" em param ...
/*!
 * @brief Seta o estado do led
 * @param[in] tipo vermelho, verde e azul
 * @param[in] estado aceso (1) ou apagado (0) 
 */
void led (enum _tipo_led tipo, enum _estado_led estado) {
	switch(tipo)
	{
	case VERMELHO:
		if (estado == ACESO)
			GPIOB_PCOR = GPIO_PIN(18); /*! Clear bit 18 de PTB, LED vermelho acende */
		else
			GPIOB_PSOR = GPIO_PIN(18); /*! Set bit 18 de PTB, LED vermelho apaga */
		break;
	case VERDE:
		if (estado == ACESO)
			GPIOB_PCOR = GPIO_PIN(19); /*! Clear bit 19 de PTB, LED verde acende */
		else
			GPIOB_PSOR = GPIO_PIN(19); /*! Set bit 19 de PTB, LED verde apaga */
		break;
	case AZUL:
		if (estado == ACESO)
			GPIOD_PCOR = GPIO_PIN(1); /*! Clear bit 1 de PTD, LED azul acende */
		else
			GPIOD_PSOR = GPIO_PIN(1); /*! Set bit 1 de PTD, LED azul apaga */
	}
}


/*!
 * \fn initLedRGBPWM(uint8_t ps, uint16_t counter, uint8_t up)
 * \brief inicializa o led RGB para operar no modo PWM
 * \param[in] ps divisor de frequ&ecirc;ncia
 * \param[in] counter contagem m&aacute;xima
 */
void initLedRGBPWM(uint8_t ps, uint16_t counter, uint8_t up) {
	/*! 
	 * Inicializa os tr&ecirc;s pinos dos leds para serem controlados por TPM
	 */
	initPort(B);                          ///< leds R e G
	initPort(D);                          ///< led B
	/*!
	 * Uso de macros para obter os endere&ccedil;os dos registradores
	 */
	setaMux(&PORT_PCR_REG(PORTB_BASE_PTR,18), 0x3);           ///< TPM2_C0
	setaMux(&PORT_PCR_REG(PORTB_BASE_PTR,19), 0x3);			  ///< TPM2_C1
	setaMux(&PORT_PCR_REG(PORTD_BASE_PTR,1), 0x4);			  ///< TPM0_C1

	/*!
	 * Configura TPM: 
	 * Pr&eacute;-escala=ps;
	 * Contagem M&aacute;xima=counter
	 * Contagem crescente
	 */
	initTPM(0,ps,counter,up);
	initTPM(2,ps,counter,up);
	
	/*! Apaga os leds: Inicializa os canais desabilitados */ 
	initTPM0Cn(1,0b0000,(unsigned short)(1.1*counter));                                /*! led azul */
    initTPM2Cn(0,0b0000,(unsigned short)(1.1*counter));                                /*! led vermelho */
    initTPM2Cn(1,0b0000,(unsigned short)(1.1*counter));                                /*! led verde */
}

/*!
 * \brief Liga leds RGB controlados por PWM
 * \param[in] potR pot&ecirc;ncia do led vermelho
 * \param[in] potG pot&ecirc;ncia do led verde
 * \param[in] potB pot&ecirc;ncia do led azul
 */
void ligaLedRGBPWM (float potR, float potG, float potB) {
	if (potR < 0.001)
		TPM2_C0SC &= ~(TPM_CnSC_MSB_MASK |      	///< MSB=TPM2_C0SC[5]=0
					TPM_CnSC_MSA_MASK |         	///< MSA=TPM2_C0SC[4]=0
					TPM_CnSC_ELSB_MASK |        	///< ELSB=TPM2_C0SC[3]=0
					TPM_CnSC_ELSA_MASK );       	///< ELSA=TPM2_C0SC[2]=0
	else {
		TPM2_C0SC |= (TPM_CnSC_MSB_MASK |         	///< MSB=TPM2_C1SC[5]=1
				TPM_CnSC_ELSB_MASK |          	///< ELSB=TPM2_C1SC[3]=1
				TPM_CnSC_ELSA_MASK );         	///< ELSA=TPM2_C1SC[2]=1
		TPM2_C0SC &= ~TPM_CnSC_MSA_MASK;          	///< MSA=TPM2_C1SC[4]=0
	}
	/*! Led G */
	if (potG < 0.001)
		TPM2_C1SC &= ~(TPM_CnSC_MSB_MASK |      	///< MSB=TPM2_C1SC[5]=0
					TPM_CnSC_MSA_MASK |         	///< MSA=TPM2_C1SC[4]=0
					TPM_CnSC_ELSB_MASK |        	///< ELSB=TPM2_C1SC[3]=0
					TPM_CnSC_ELSA_MASK );       	///< ELSA=TPM2_C1SC[2]=0
	else {
		TPM2_C1SC |= (TPM_CnSC_MSB_MASK |       	///< MSB=TPM2_C1SC[5]=1
				TPM_CnSC_ELSB_MASK |        	///< ELSB=TPM2_C1SC[3]=1
				TPM_CnSC_ELSA_MASK );       	///< ELSA=TPM2_C1SC[2]=1
		TPM2_C1SC &= ~TPM_CnSC_MSA_MASK;        	///< MSA=TPM2_C1SC[4]=0
	}
	/*! Led B */
	if (potB < 0.001)		
		TPM0_C1SC &= ~(TPM_CnSC_MSB_MASK |      ///< MSB=TPM0_C1SC[5]=0
					TPM_CnSC_MSA_MASK |         ///< MSA=TPM0_C1SC[4]=0
					TPM_CnSC_ELSB_MASK |        ///< ELSB=TPM0_C1SC[3]=0
					TPM_CnSC_ELSA_MASK );       ///< ELSA=TPM0_C1SC[2]=0
	else {
		TPM0_C1SC |= (TPM_CnSC_MSB_MASK |         	///< MSB=TPM2_C1SC[5]=1
				TPM_CnSC_ELSB_MASK |        	///< ELSB=TPM2_C1SC[3]=1
				TPM_CnSC_ELSA_MASK );       	///< ELSA=TPM2_C1SC[2]=1
		TPM0_C1SC &= ~TPM_CnSC_MSA_MASK;         	///< MSA=TPM2_C1SC[4]=0
	}			
}

/*!
 * \brief Apaga leds RGB controlados por PWM
 */
void apagaLedRGBPWM () {
	/*! Sele&ccedil;&atilde;o de modo, transi&ccedil;&atilde;o e n&iacute;vel (desabilita os 3 canais) */
	TPM2_C0SC &= ~(TPM_CnSC_MSB_MASK |      	///< MSB=TPM2_C0SC[5]=0
				TPM_CnSC_MSA_MASK |         	///< MSA=TPM2_C0SC[4]=0
				TPM_CnSC_ELSB_MASK |        	///< ELSB=TPM2_C0SC[3]=0
				TPM_CnSC_ELSA_MASK );       	///< ELSA=TPM2_C0SC[2]=0
	TPM2_C1SC &= ~(TPM_CnSC_MSB_MASK |      	///< MSB=TPM2_C1SC[5]=0
				TPM_CnSC_MSA_MASK |         	///< MSA=TPM2_C1SC[4]=0
				TPM_CnSC_ELSB_MASK |        	///< ELSB=TPM2_C1SC[3]=0
				TPM_CnSC_ELSA_MASK );       	///< ELSA=TPM2_C1SC[2]=0
	TPM0_C1SC &= ~(TPM_CnSC_MSB_MASK |      ///< MSB=TPM0_C1SC[5]=0
				TPM_CnSC_MSA_MASK |         ///< MSA=TPM0_C1SC[4]=0
				TPM_CnSC_ELSB_MASK |        ///< ELSB=TPM0_C1SC[3]=0
				TPM_CnSC_ELSA_MASK );       ///< ELSA=TPM0_C1SC[2]=0
}

/*!
 * \brief muda a cor dos leds RGB controlados por PWM
 * \param[in] potR pot&ecirc;ncia do led vermelho
 * \param[in] potG pot&ecirc;ncia do led verde
 * \param[in] potB pot&ecirc;ncia do led azul
 * 
 */
void alteraCorLedRGBPWM (uint16_t potR, uint16_t potG, uint16_t potB) {
	TPM2_C0V = TPM_CnV_VAL(potR);     ///< % R
	TPM2_C1V = TPM_CnV_VAL(potG);        ///< % G
	TPM0_C1V = TPM_CnV_VAL(potB);         ///< % B
}


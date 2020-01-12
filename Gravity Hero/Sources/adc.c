/*
 * \file adc.c
 *
 *  \date Aug 7, 2017
 *  \author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#include "adc.h"

/*!
 * \fn initADC (ADCConfig *config) 
 * \brief Configura os módulos de interface com os periféricos
 * \param[in] div_4 divisor de frequ&ecirc;ncia do n&uacute;cleo
 * \param[in] config par&acirc;metros de configur&ccedil;&atilde;o
 */
void initADC0(uint8_t div4, ADCConfig *config) 
{
	calibraADC0();         ///< calibra ADC
	configuraADC0 (div4, config); ///< configura modo de opera&ccedil;&atilde;o do ADC
	selecionaTriggerADC0 (config);
}

/*!
 * \fn calibraADC0 (void)
 * \brief Calibra o m&oacute;dulo ADC
 * "Prior to calibration, the user must configure the ADC's clock source and 
 * frequency, low power configuration, voltage reference selection, sample time, 
 * and high speed configuration according to the application's clock source 
 * availability and needs." (Se&ccedil;&atilde;o 28.4.6 de KL-25 Sub-Family 
 * Reference Manual)
 */
void calibraADC0 (void)
{
	uint16_t tmp=0;
	
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;      ///< habilita clock do m&oacute;dulo ADC0                             
	/*! Configura&ccedil;&atilde;o do rel&oacute;gio
	 * Os bits de ADC0_CFG1 s&atilde;o resetados em 0: 
	 * ADLSMP=0 (short sample time) 
	 */ 
	ADC0_CFG1 &= ~(ADC_CFG1_ADLPC_MASK); ///< opera&ccedil;&atilde;o normal
	ADC0_CFG1 |= ADC_CFG1_ADIV(0x3) |             ///< ADCK = (source clock)/8
	            ADC_CFG1_ADLSMP_MASK |            ///< intervalo de amostragem longo
	            ADC_CFG1_MODE(0x3) |             ///< convers&atilde;o em 16 bits
	            ADC_CFG1_ADICLK(0x1);            ///< (source clock) = (bus clock)/2
	/*! Configura&ccedil;&atilde;o dos ciclos adicionais 
	 * Os bits de ADC0_CFG2 s&atilde;o resetados em 0: MUXSEL = 0 (canais ADxxa)  
	 */
	ADC0_CFG2 &= ~(ADC_CFG2_ADACKEN_MASK |   ///< desabilita a sa&iacute;da do sinal de clock      
	             ADC_CFG2_ADLSTS(0x3));      ///< 24 (20 extra+4) ADCK ciclos por amostra
	ADC0_CFG2 |=  ADC_CFG2_ADHSC_MASK;       ///< sequ&ecirc;ncia de convers&atilde;o em alta velocidade com 2 ciclos adicionais
	/*! Os bits de ADC0_SC2 s&atilde;o resetados em 0: ACFE=0 (desabilitada fun&ccedil;&atilde;o de compara&ccedil;&atilde;o) */
	ADC0_SC2 &= ADC_SC2_REFSEL(0x0);    ///< Tens&otilde;es de refer&ecirc; VREFH e VREFL                                
	  
	/*! Os bits de ADC0_SC3 s&atilde;o resetados em 0 */
	ADC0_SC3 |= ADC_SC3_AVGE_MASK | ///< habilita fun&ccedil;&atilde;o em hardware
				ADC_SC3_AVGS(0x3);  ///< n&uacute;mero de amostras por m&eacute;dia = 32                
	ADC0_SC3 |= (ADC_SC3_CAL_MASK);  ///< inicializa a calibra&ccedil;&atilde;o	

	while (ADC0_SC3 & ADC_SC3_CAL_MASK)  {}  ///< aguarda a calibra&ccedil;&atilde;o

	/*! Gere as corre&ccedil;&otilde;es de erros de ganhos, conforme o procedimento
	 *  na Se&ccedil;&atilde;o 28.4.6 
	 */
	tmp = ADC0_CLP0 + ADC0_CLP1 + ADC0_CLP2 + ADC0_CLP3 + ADC0_CLP4 + ADC0_CLPS; ///< soma PS
	tmp = tmp/2;                            ///< divide por 2
	tmp |= (uint16_t)(0x8000);              ///< set o bit mais significativo
	ADC0_PG = tmp;                          ///< plus-side gain
	/*! Somente &uacute;til para modos diferenciais */
	tmp = ADC0_CLM0 + ADC0_CLM1 + ADC0_CLM2 + ADC0_CLM3 + ADC0_CLM4 + ADC0_CLMS; ///< soma PS
	tmp = tmp/2;                            ///< divide por 2
	tmp |= (uint16_t)(0x8000);              ///< set o bit mais significativo
	ADC0_MG = tmp;	                        ///< minus side gain
}

/*!
 * \fn configuraADC (uint8_t div4, ADCConfig *config)
 * \param[in] div_4 divisor de frequ&ecirc;ncia do n&uacute;cleo
 * \param[in] config par&acirc;metros de configur&ccedil;&atilde;o
 * \brief Configura o modo de opera&ccedil;&atilde;o do módulo ADC. Ver
 * <a href="ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KLQRUG.pdf#page=119">Refer&ecirc;ncia</a>
 */
void configuraADC0(uint8_t div4, ADCConfig *config) {
	/*!
	 * Configura&ccedil;&atilde;o de SIM
	 */
	SIM_CLKDIV1 &= ~(uint32_t)(SIM_CLKDIV1_OUTDIV4(0b111));  ////< bus_clock = clock/1
	SIM_CLKDIV1 |= (uint32_t)(SIM_CLKDIV1_OUTDIV4(div4));  ////< bus_clock = clock/1
	/*!
	 * Modo unipolar ou diferencial
	 */
	if (config->diff) ADC0_SC1A |= ADC_SC1_DIFF_MASK;
	else ADC0_SC1A &= ~ADC_SC1_DIFF_MASK;

	/*!
	 * Opera&ccedil;&atilde;o em baixa pot&ecirc;ncia
	 */
	ADC0_CFG1 |= ADC_CFG1_ADLPC_MASK;  

	/*!
	 * Intervalo de amostragem
	 */
	if (config->adlsmp) {
		ADC0_CFG1 |= ADC_CFG1_ADLSMP_MASK;	///< intervalo de amostragem longo
		/*!
		 * ADLSTS=00 (24 ciclos)
		 */ 
		ADC0_CFG2 &= ~ADC_CFG2_ADLSTS(0b11);  ///< setar o novo intervalo de amostragem
		ADC0_CFG2 |= ADC_CFG2_ADLSTS(config->adlsts);
	}
	else ADC0_CFG1 &= ~ADC_CFG1_ADLSMP_MASK;   ///< intervalo de amostragem curto

	/*!
	 * Resolu&ccedil;&atilde;o de convers&atilde;o
	 */
	ADC0_CFG1	&= ~(ADC_CFG1_MODE(0b11));     ///< zera o campo 
	ADC0_CFG1  |= ADC_CFG1_MODE(config->mode);        ///< seta a resolu&ccedil;&atilde;o 

	/*! 
	 * Base de tempo do ADCK = (bus clock)/2*8
	 */ 
	ADC0_CFG1 &= ~ADC_CFG1_ADICLK(0b11);     ///< seleciona clock
	ADC0_CFG1 |= ADC_CFG1_ADICLK(config->adiclk);    
	ADC0_CFG1 &= ~ADC_CFG1_ADIV(0b11);       ///< seleciona divisor do clock
	ADC0_CFG1 |= ADC_CFG1_ADIV(config->adiv);

	/*!
	 * Configura&ccedil;&atilde;o da velocidade: MUXSEL=0 (canais ADxxa)
	 * Canais entre 4 a 7 tem dois canais associados A e B
	 * Sele&ccedil;&atilde;o do canal (A==0 ou B==1)
	 */
	if (config->muxsel) ADC0_CFG2 |= ADC_CFG2_MUXSEL_MASK;  ///< Canal B
	else ADC0_CFG2 &= ~ADC_CFG2_MUXSEL_MASK;   ///< Canal A 

	/*! 
	 */
	ADC0_CFG2 &= ~ADC_CFG2_ADACKEN_MASK; ///< desabilitar sa&iacude;da do sinal de rel&oacute;gio

	ADC0_CFG2 &= ~ADC_CFG2_ADHSC_MASK;      ///< sequ&ecirc;ncia de convers&atilde;o normal      

	/*!
	 * Desabilita fun&ccedil;&otilde;es de compara&ccedil;&atilde;o e DMA
	 */
	ADC0_SC2 &= ~(ADC_SC2_ACFE_MASK |
			ADC_SC2_DMAEN_MASK);
	/*!
	 * Seleciona os sinais dos pinos VREFH e VREFL como sinais de refer&ecirc;ncia
	 */
	ADC0_SC2 &= ~ADC_SC2_REFSEL(0b11);

	/*!
	 * Modo de convers&atilde;o: cont&iacute;nuo ou unitário
	 */
	if (config->adco) ADC0_SC3 |= ADC_SC3_ADCO_MASK;
	else ADC0_SC3 &= ~ADC_SC3_ADCO_MASK;

	/*! 
	 * 
	 *  Habilita c&ocirc;mputo da m&eacute;dia das amostras 
	 */
	if (config->avge) {
		ADC0_SC3 |= ADC_SC3_AVGE_MASK;
		ADC0_SC3 &= ~ADC_SC3_AVGS (0b11);
		ADC0_SC3 |= ADC_SC3_AVGS (config->avgs);
	} else ADC0_SC3 &= ~ADC_SC3_AVGE_MASK;

	ADC0_SC1A |= ADC_SC1_ADCH(31); ///<  Disabilita o m&oacute;dulo 
	//	ADC0_SC3 |= ADC_SC3_CALF_MASK;      ///< baixa a bandeira de "calibration failed"
}

/*!
 * \fn selecionaTriggerADC0(ADCConfig *config)
 * \brief Seleciona o tipoe a fonte do trigger deconvers&atilde;o
 */
void selecionaTriggerADC0 (ADCConfig *config) {
	/*!
	 * Disparo para convers&atilde;o
	 */
	if (config->adtrg) {
		if (config->alternate) {
			/*!
			 * Trigger alternativo
			 */
			SIM_SOPT7 |= SIM_SOPT7_ADC0ALTTRGEN_MASK;
			/*!
			 * Pre-trigger
			 */
			if (config->pre) SIM_SOPT7 |= SIM_SOPT7_ADC0PRETRGSEL_MASK; ///< B
			else SIM_SOPT7 &= ~SIM_SOPT7_ADC0PRETRGSEL_MASK;    ///< A 
			/*!
			 * PIT/TPM/RTC/LPTMR0
			 */
			SIM_SOPT7 &= ~SIM_SOPT7_ADC0TRGSEL(0b1111);     ///< zerar o campo
			SIM_SOPT7 |= SIM_SOPT7_ADC0TRGSEL(config->fonte);     ///< setar a fonte
		}
		else SIM_SOPT7 &= ~SIM_SOPT7_ADC0ALTTRGEN_MASK;  ///< A <- TPM1_CH0; B <- TPM1_CH1
		ADC0_SC2 |= ADC_SC2_ADTRG_MASK;    ///< disparo por hardware
	} else ADC0_SC2 &= ~ADC_SC2_ADTRG_MASK;    ///< disparo por software	
}

/*!
 * \fn enableInterrupADC0 (void)
 * \brief Habilita interrup&ccedil;&atilde;o de convers&atilde;o completa do ADC
 */
void enableInterrupADC0 (void) {
	/*! ADC0_SC1A: COCO=0, DIFF=0 (medida unipolar) e ADCH=0b11111 (canais desabilitados) no reset */
	ADC0_SC1A |= ADC_SC1_AIEN_MASK;           ///< habilita interrup&ccedil;&atilde;o
}

/*!
 * \fn disableInterrupADC0 (void)
 * \brief Habilita interrup&ccedil;&atilde;o de convers&atilde;o completa do ADC
 */
void disableInterrupADC0 (void) {
	/*! ADC0_SC1A: COCO=0, DIFF=0 (medida unipolar) e ADCH=0b11111 (canais desabilitados) no reset */
	ADC0_SC1A &= ~ADC_SC1_AIEN_MASK;           ///< habilita interrup&ccedil;&atilde;o
}

/*!
 * \fn selecionaCanalADC0 (uint8_t x)
 * \brief Seleciona o canal de entrada. Consulte 
 * <a href="ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf#page=464">Manual</a>
 */
void selecionaCanalADC0 (uint8_t x) {
	ADC0_SC1A	&= ~(ADC_SC1_ADCH(0x1f)); ///< zera o campo ADCH (~ADCH=0b11111)
	ADC0_SC1A	|= ADC_SC1_ADCH(x);            ///< AD9 selecionado (PTB1)
}

/*
 * adc.h
 *
 *  Created on: Aug 7, 2017
 *      Author: Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#ifndef ADC_H_
#define ADC_H_

#include "derivative.h"

typedef struct _ADCConfiguracao {
	uint8_t diff;            ///< convers&atilde;o unipolar (0) ou diferencial (1)
	uint8_t adlsmp; 	     ///< tempo de convers&atilde;o: curto (0) ou longo (1)
	uint8_t adlsts; 		 ///< tipo de tempo longo
	uint8_t mode; 			 ///< resolu&ccedil;&atilde;o da convers&atilde;o: 8, 10, 12, 16
	uint8_t adiclk;          ///< clock do barramento
	uint8_t adiv; 			 ///< divisor de frequ&ecirc;ncia do clock
	uint8_t muxsel; 		 ///< sele&ccedil;&atilde;o de canal: A (0) e B (1)
	uint8_t adco; 			 ///< convers&atilde;o cont&iacute;nua (1) ou n&atilde;o (0)
	uint8_t avge; 			 ///< habilita m&eacute;dia das amostras
	uint8_t avgs; 			 ///< quantidade de amostras por m&eacute;dia
	uint8_t adtrg;           ///< disparo de uma conver&atilde;o
	uint8_t alternate;       ///< habilita fontes alternativas
	uint8_t pre; 			 ///< canal A (0) ou canal B (1)
	uint8_t fonte;			 ///< fonte
} ADCConfig;

void calibraADC0 (void);

void configuraADC0 (uint8_t div4, ADCConfig *config);

void initADC0 (uint8_t div4, ADCConfig *config);

void selecionaTriggerADC0 (ADCConfig *config);

void enableInterrupADC0 (void);

void disableInterrupADC0 (void);

void selecionaCanalADC0 (uint8_t x);

#endif /* ADC_H_ */

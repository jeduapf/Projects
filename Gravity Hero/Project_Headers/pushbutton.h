/*!
 * @file pushbotton.h
 *
 * @date 19/09/2017
 * @author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#ifndef PUSHBOTTON_H_
#define PUSHBOTTON_H_
#include <derivative.h>
#include "util.h"
/*
 * Enumera&ccedil&atilde;o da botoeira
 */
enum _tipo_pta {
	PTA4, 	  /*!< pino 4 da porta A */
	PTA5,     /*!< pino 5 da porta A */
	PTA12     /*!< pino 12 da porta A */
};

void initPushbutton (void);
void enablePushbuttonIRQA(void);
short le_pta (enum _tipo_pta pta);
void initPORTE(void);
void ligaPORTE(void);
void apagaPORTE(void);
void alternaPORTE(void);
uint8_t tempo_espera (enum _tipo_pta pta);
#endif /* PUSHBOTTON_H_ */

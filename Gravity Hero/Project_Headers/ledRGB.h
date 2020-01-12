/*
 * @file ledRGB.h
 * @note
 * Defini&ccedil;&atilde;o de macros referentes aos endere&ccedil;os dos registradores
 * do microcontrolador. O endere&ccedil;o em que cada registrador est&aacute; mapeado
 * pode ser consultado no manual de refer&ecirc;ncia
 * ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf.
 * Com o mecanismo de busca pelas macros no manual, encontra-se junto &aacute; 
 * descri&ccedil;&atilde;o de cada registrador o seu endere&ccedil;o no espa&ccedil;o
 * de mem&oacute;ria em hexadecimal.
 * 
 * @date Aug 3, 2017
 * @author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#ifndef LEDRGB_H_
#define LEDRGB_H_
#include <derivative.h>
#include "util.h"
#include "port.h"
#include "tpm.h"

#define MAX_COR 16

/*
 * Enumera&ccedil&atilde;o das cores do led
 */
enum _tipo_led {
	VERMELHO,   /*!< vermelho */
	VERDE,      /*!< verde */
	AZUL        /*! azul */
};

/*
 * Enumera&ccedil&atilde;o dos estados do led
 */
typedef enum _estado_led {
	ACESO,      /*!< aceso */
	APAGADO     /*!< apagado */
} estado_led;

/*
 * Estrutura para representar uma cor pelos estados dos 3 leds 
 */
typedef struct _cor {
	enum _estado_led vermelho;
	enum _estado_led verde;
	enum _estado_led azul;
} cor;

/*
 * Estrutura para representar uma cor pelos estados dos 3 leds 
 */
typedef struct _cor_pf {
	float vermelho;
	float verde;
	float azul;
} cor_pf;

void led (enum _tipo_led tipo, enum _estado_led estado);

void initLedRGBPWM (uint8_t ps, uint16_t counter, uint8_t up);

void ligaLedRGBPWM (float potR, float potG, float potB);

void alteraCorLedRGBPWM (uint16_t potR, uint16_t potG, uint16_t potB);

void apagaLedRGBPWM ();

void initLedR();

void initLedRGB();

void ligaLedR();

void apagaLedR();

void alternaLedR();

void ligaLedG();

void apagaLedG();

void alternaLedG();

void ligaLedB();

void apagaLedB();

void alternaLedB();

void apagaLedRGB();


#endif /* LEDRGB_H_ */

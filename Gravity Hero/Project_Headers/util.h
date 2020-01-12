/*!
 * @file util.h
 *
 * @date 19/09/2017
 * @author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#include <stdlib.h>

#ifndef UTIL_H_
#define UTIL_H_

#define GPIO_PIN(x)  ((1)<<(x)) ///< obtem o bit do pino x

void ConvUI2String (unsigned int j, char * s);

void ConvF2String (float j, char * s);

void ConvMMSS2String(uint8_t MM, float SS, char *s);

void ConvCronometro (float j, char * s);

void ConvRelogio (float j, char * s);

#endif /* UTIL_H_ */

/*
 * estrutura.h
 *
 *  Created on: Feb 27, 2017
 *      Author: Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#ifndef ESTRUTURA_H_
#define ESTRUTURA_H_

#include <stdlib.h>

typedef struct bufCircular {
	unsigned int head;
	unsigned int tail;
	unsigned int num;
	char *elementos;
} buffer_circular;

void bufCircular_cria (buffer_circular *c, unsigned int n);

uint8_t bufCircular_insere (buffer_circular *c, char v);

char bufCircular_remove (buffer_circular *c);

char bufCircular_ultimo (buffer_circular *c);

char bufCircular_primeiro (buffer_circular *c);

uint8_t bufCircular_cheia (buffer_circular *c);

uint8_t bufCircular_vazia (buffer_circular *c);

unsigned int bufCircular_tamanho (buffer_circular *c);

void bufCircular_deleta (buffer_circular *c);

#endif /* ESTRUTURA_H_ */

/*
 * @file util.c
 *
 * @date 19/09/2017
 * @author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#include "util.h"

/*!
 * \fn ConvUI2String(unsigned int j, char * s)
 * \brief Converte em ASCII o inteiro sem sinal j.
 * \param[in]  j valor inteiro sem sinal
 * \param[out] s representa&ccedil;&atilde;o em ASCII
 */
void ConvUI2String (unsigned int j, char * s) {
	short k = 0;
	char aux[11];
	/*!
	 * Passo 1: extrai os algarismos decimais
	 */
	if (j == 0) { ///< Caso especial
		aux[k] = 0;
		k++;
	}
	while (j) {
		aux[k] = j % 10;
		j = j / 10;
		k++;
	}
	/*!
	 * Passo 2: representa em ASCII os d&iacute;gitos decimais
	 */
	while (k > 0) {
		*s = (aux[k-1] + 0x30);
		s++;                    ///< incrementa o endere&ccedil;o por unidade do tipo de dados
		k--;
	}
	*s = 0;                     ///< adiciona o terminador '\0'
}

/*!
 * \fn ConvF2String(float j, char * s)
 * \brief Converte em ASCII um valor em ponto flutuante j com uma casa decimal.
 * \param[in]  j valor em ponto flutuante
 * \param[out] s representa&ccedil;&atilde;o em ASCII com uma casa decimal
 */
void ConvF2String (float j, char * s) {
    int aux;
    aux = j/1; /*! pegando a parte inteira de j */
    ConvUI2String(aux, s);
    
	while(*s!=0) s++; /*! Acha onde a parte inteira termina */
	*s = '.'; /*! colocar o separador decimal */
	s++;
    aux = ((int)(j*10))%10; /*! pegar a parte decimal */
    *s = (aux + 0x30); /*! Coloca na string o primeiro digito decimal */
    s++;
    *s = 0; /*! Adiciona o terminador '\0' */
}

/*!
 * \fn ConvMMSS2String(uint8_t MM, float SS, char * s)
 * \brief Converte no formato MM:SS.S em ASCII o valor em minutos e o valor em segundos.
 * \param[in]  MM valor em minutos
 * \param[in]  SS valor em segundos
 * \param[out] s representa&ccedil;&atilde;o em ASCII com uma casa decimal em segundos
 * 
 */
void ConvMMSS2String(uint8_t MM, float SS, char *s) {
	char a,b,c,d,e;
	a = MM/10;
	b = MM%10;
	c = ((char)(SS/10));
	d = ((char)SS)/1; 				///< Separar o numero em parte inteira
	e = (char)(SS*10);			///< (1)Separar o numero em parte decimal
	e = e%10;			///< (2)Separar o numero em parte decimal
	
	*s = (a + 0x30);		///< Adiciona parte da dezena de minuto na primeira posicao da string
	s++;                  	    ///< Incrementa o endereco por unidade do tipo de dados
	*s = (b + 0x30);		///< Adiciona parte do minuto na segunda posicao da string
	s++;                   		///< Incrementa o endereco por unidade do tipo de dados
	*s = ':';					///< Adiciona o separador ':' na terceira posicao da string
	s++;						///< Incrementa o endereco por unidade do tipo de dados
	*s = (c + 0x30);		///< Adiciona parte da dezena de segundo na quarta posicao da string
	s++;                  	    ///< Incrementa o endereco por unidade do tipo de dados
	*s = (d + 0x30);		///< Adiciona parte do segundo na quinta posicao da string
	s++;                        ///< Incrementa o endereco por unidade do tipo de dados
	*s = '.';					///< Adiciona o separador '." na sexta posicao da string
	s++;						///< Incrementa o endereco por unidade do tipo de dados
	*s = (e + 0x30);		///< Adiciona parte do decimo de segundo na setima posicao da string
	s++;						///< Incrementa o endereco por unidade do tipo de dados
	*s = 0;                     ///< Adiciona o terminador '\0'
	
}

void ConvCronometro (float j, char * s) {
	int aux[4];
	int flag;
	j= j*10;
	flag = (int)j;
	///< Calculo de cada unidade de tempo e armazenamento no vetor
	aux[0] = flag%10;				///< Calcula o decimo de segundo com base no inteiro, [0,9]
	aux[1] = (flag%100)/10;		///< Calcula o segundo com base no inteiro, [0,9]
	aux[2] = (flag%600)/100;		///< Calcula a dezena do segundo com base no inteiro [0,6]
	aux[3] = (flag%6000)/600;		///< Calcula o minuto com base no inteiro [0,6]
	aux[4] = (flag%42000)/6000;	///< Calcula a dezena do minuto com base no inteiro [0,6]
	
	*s = (aux[4] + 0x30);		///< Adiciona parte da dezena de minuto na primeira posicao da string
	s++;                  	    ///< Incrementa o endereco por unidade do tipo de dados
	*s = (aux[3] + 0x30);		///< Adiciona parte do minuto na segunda posicao da string
	s++;                   		///< Incrementa o endereco por unidade do tipo de dados
	*s = ':';					///< Adiciona o separador ':' na terceira posicao da string
	s++;						///< Incrementa o endereco por unidade do tipo de dados
	*s = (aux[2] + 0x30);		///< Adiciona parte da dezena de segundo na quarta posicao da string
	s++;                  	    ///< Incrementa o endereco por unidade do tipo de dados
	*s = (aux[1] + 0x30);		///< Adiciona parte do segundo na quinta posicao da string
	s++;                        ///< Incrementa o endereco por unidade do tipo de dados
	*s = '.';					///< Adiciona o separador '." na sexta posicao da string
	s++;						///< Incrementa o endereco por unidade do tipo de dados
	*s = (aux[0] + 0x30);		///< Adiciona parte do decimo de segundo na setima posicao da string
	s++;						///< Incrementa o endereco por unidade do tipo de dados
	*s = ' ';               	///< Adiciona espaço em branco
	s++;						///< Incrementa o endereco por unidade do tipo de dados
	*s = ' ';               	///< Adiciona espaço em branco
	s++;						///< Incrementa o endereco por unidade do tipo de dados
	*s = ' ';               	///< Adiciona espaço em branco
	s++;						///< Incrementa o endereco por unidade do tipo de dados
	*s = ' ';               	///< Adiciona espaço em branco
	s++;						///< Incrementa o endereco por unidade do tipo de dados
	*s = ' ';               	///< Adiciona espaço em branco
	s++;						///< Incrementa o endereco por unidade do tipo de dados
	*s = 0;                     ///< Adiciona o terminador '\0'
}

void ConvRelogio (float j, char * s) {
	int aux[6];
	int flag;
	j= j*10;
	flag = (int)j;
	///< Calculo de cada unidade de tempo e armazenamento no vetor
	aux[0] = flag%10;				///< Calcula o decimo de segundo com base no inteiro, [0,9]
	aux[1] = (flag%100)/10;		///< Calcula o segundo com base no inteiro, [0,9]
	aux[2] = (flag%600)/100;		///< Calcula a dezena do segundo com base no inteiro [0,6]
	aux[3] = (flag%6000)/600;		///< Calcula o minuto com base no inteiro [0,6]
	aux[4] = (flag%36000)/6000;	///< Calcula a dezena do minuto com base no inteiro [0,6]
	aux[5] = (flag%360000)/36000;		///< Calcula a hora com base no inteiro [0,6]
	aux[6] = (flag%3600000)/360000;	///< Calcula a dezena da hora com base no inteiro [0,6]
	
	*s = (aux[6] + 0x30);		///< Adiciona parte da dezena da hora na primeira posicao da string
	s++;                  	    ///< Incrementa o endereco por unidade do tipo de dados
	*s = (aux[5] + 0x30);		///< Adiciona parte da hora na segunda posicao da string
	s++;                   		///< Incrementa o endereco por unidade do tipo de dados
	*s = ':';					///< Adiciona o separador ':' na quinta posicao da string
	s++;						///< Incrementa o endereco por unidade do tipo de dados	
	*s = (aux[4] + 0x30);		///< Adiciona parte da dezena de minuto na terceira posicao da string
	s++;                  	    ///< Incrementa o endereco por unidade do tipo de dados
	*s = (aux[3] + 0x30);		///< Adiciona parte do minuto na quarta posicao da string
	s++;                   		///< Incrementa o endereco por unidade do tipo de dados
	*s = ':';					///< Adiciona o separador ':' na quinta posicao da string
	s++;						///< Incrementa o endereco por unidade do tipo de dados
	*s = (aux[2] + 0x30);		///< Adiciona parte da dezena de segundo na sexta posicao da string
	s++;                  	    ///< Incrementa o endereco por unidade do tipo de dados
	*s = (aux[1] + 0x30);		///< Adiciona parte do segundo na setima posicao da string
	s++;                        ///< Incrementa o endereco por unidade do tipo de dados
	*s = '.';					///< Adiciona o separador '." na oitava posicao da string
	s++;						///< Incrementa o endereco por unidade do tipo de dados
	*s = (aux[0] + 0x30);		///< Adiciona parte do decimo de nova na setima posicao da string
	s++;						///< Incrementa o endereco por unidade do tipo de dados
	*s = 0;                     ///< Adiciona o terminador '\0'
}

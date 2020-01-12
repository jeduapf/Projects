/*!
 * musica.c
 *
 * @author Jose Eduardo Alves Peraira Filho 176601 e Jo&atilde;o Vitor 176433
 * 
 * @date 07/11/2017
 */

#include "derivative.h" 
#include "stdlib.h"
#include "string.h"
#include "atrasos.h"
#include "lcdled.h"
#include "systick.h"
#include "pit.h"
#include "handler.h"
#include "acel_MMA8451Q.h"
#include "I2C.h"
#include "musica.h"

extern uint16_t tempo,tempo_musica,pontos;
extern uint8_t som_play, musica_play,flag_acertou,modo,ind_musica;  
extern unsigned char AccData[6];
extern short Xout_14_bit, Yout_14_bit, Zout_14_bit;
extern float Xout_g, Yout_g, Zout_g;
extern char DataReady;
extern char Xoffset, Yoffset, Zoffset;
extern char tela[16];
extern char acelerometro;
extern char musica1[];
extern char musica2[];
extern char musica3[];
extern char  *musica; 
extern int k;


/*!
 * Vetores das musicas
 */
/*! Musica 1 */
float v1[265] = {1,Do,Re,Mi,Fa,1,Fa,1,Fa,Do,Re,Do,Re,1,Re,1,Re,Do,Sol,Fa,Mi,1,Mi,1,Mi,Do,Re,Mi,Fa,1,Fa,1,Fa,1,Do,Re,Mi,Fa,1,Fa,1,Fa,Do,Re,Do,Re,1,Re,1,Re,Do,Sol,Fa,Mi,1,Mi,1,Mi,Do,Re,Mi,Fa,1,Fa,1,Fa,1,Do,Re,Mi,Fa,1,Fa,1,Fa,Do,Re,Do,Re,1,Re,1,Re,Do,Sol,Fa,Mi,1,Mi,1,Mi,Do,Re,Mi,Fa,1,Fa,1,Fa,1,Do,Re,Mi,Fa,1,Fa,1,Fa,Do,Re,Do,Re,1,Re,1,Re,Do,Sol,Fa,Mi,1,Mi,1,Mi,Do,Re,Mi,Fa,1,Fa,1,Fa,1,Do,Re,Mi,Fa,1,Fa,1,Fa,Do,Re,Do,Re,1,Re,1,Re,Do,Sol,Fa,Mi,1,Mi,1,Mi,Do,Re,Mi,Fa,1,Fa,1,Fa,1,Do,Re,Mi,Fa,1,Fa,1,Fa,Do,Re,Do,Re,1,Re,1,Re,Do,Sol,Fa,Mi,1,Mi,1,Mi,Do,Re,Mi,Fa,1,Fa,1,Fa,1,Do,Re,Mi,Fa,1,Fa,1,Fa,Do,Re,Do,Re,1,Re,1,Re,Do,Sol,Fa,Mi,1,Mi,1,Mi,Do,Re,Mi,Fa,1,Fa,1,Fa,1,Do,Re,Mi,Fa,1,Fa,1,Fa,Do,Re,Do,Re,1,Re,1,Re,Do,Sol,Fa,Mi,1,Mi,1,Mi,Do,Re,Mi,Fa,1,Fa,1,Fa,1}; 
float v2[265] = {4,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,TEMP0,0.3,TEMP0,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,TEMP0,0.3,TEMP0,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,TEMP0,0.3,TEMP0,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,TEMP0,0.3,TEMP0,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,TEMP0,0.3,TEMP0,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,TEMP0,0.3,TEMP0,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,TEMP0,0.3,TEMP0,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,TEMP0,0.3,TEMP0,TEMP0,0.1,TEMP0,0.1,TEMP0,TEMP0,TEMP0,TEMP0,TEMP1,0.1,TEMP0,0.1,TEMP0,0.1};
/*! Musica 2 */
float v3[160] = {1,Sol3,1,Sol3,1,Re,1,Re,1,Mi,1,Mi,1,Re,1,Do,1,Do,1,Si3,1,Si3,1,La3,1,Sol3,1,Re,1,Re,1,Do,1,Do,1,Si3,1,Si3,1,La3,1,Re,1,Re,1,Do,1,Do,1,Si3,1,Si3,1,La3,1,Sol3,1,Sol3,1,Re,1,Re,1,Mi,1,Mi,1,Re,1,Do,1,Do,1,Si3,1,Si3,1,La3,1,Sol3,1,Sol3,1,Sol3,1,Re,1,Re,1,Mi,1,Mi,1,Re,1,Do,1,Do,1,Si3,1,Si3,1,La3,1,Sol3,1,Re,1,Re,1,Do,1,Do,1,Si3,1,Si3,1,La3,1,Re,1,Re,1,Do,1,Do,1,Si3,1,Si3,1,La3,1,Sol3,1,Sol3,1,Re,1,Re,1,Mi,1,Mi,1,Re,1,Do,1,Do,1,Si3,1,Si3,1,La3,1,Sol3}; 
float v4[160] = {4,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP2,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP2,0.01,TEMP2,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP2,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP2,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP2,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP2,0.01,TEMP2,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP2,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP2,0.01,TEMP2,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP2,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP2,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP2,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP1,0.01,TEMP2,0.01,TEMP2};
/*! Musica 3 */
float v5[204]={1,Fa,1,Mi,Mi,1,Re,Re,1,Do,1,Mi,1,La,1,La,1,Si,1,Fa,1,Mi3,1,Mi3,1,Sol,1,Fa,Fa,1,Re,Re,Mi3,1,Mi3,1,Sol,1,Fa,Fa,1,Re,Re,Fa,1,Mi,1,Si3,Si3,Si3,1,Mi,1,Sol,1,La,1,Mi,Mi,1,Sol,1,Fa,1,Sol,1,La,1,Re,1,Si,1,La,1,Fa,1,Fa,1,Mi,1,Si3,Si3,1,Mi,1,Sol,1,La,1,Mi,Mi,1,Sol,1,Fa,1,Sol,1,La,1,Re,1,Mi,1,1,Mi,1,Fa,1,Sol,1,La,1,Si,1,Mi,1,Sol,Sol,1,Fa,Fa,1,Mi,1,Sol,1,La,1,Sol,1,Fa,Fa,1,Si,1,Si,1,La,1,La,1,Si,1,Mi,1,Sol,Sol,1,Fa,Fa,1,Mi,1,Sol,1,La,1,Sol,1,Fa,Fa,1,Si3,1,Mi,Mi,1,La3,1,La3,1,Si3,1,Mi3,1,Sol3,Sol3,1,Fa3,Fa3,1,Mi3,1,Sol3,1,Re,1,Si3,Si3,1,Sol,1,La,1,Sol,1,Fa,Fa,1,Si3,1,Mi,1};
float v6[204]={4,TEMP3,0.01,TEMP3,TEMP3,0.01,TEMP3,TEMP3,0.01,TEMP2,0.01,TEMP1,0.01,TEMP1,0.01,TEMP3,0.01,TEMP2,0.01,TEMP2,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,TEMP0,0.01,TEMP0,TEMP1,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,TEMP0,0.01,TEMP0,TEMP1,TEMP0/2.,0.01,TEMP0/2.,0.01,TEMP0,TEMP1,TEMP1,0.01,TEMP0,0.01,TEMP0,0.01,TEMP1,0.01,TEMP2,TEMP1,0.01,TEMP2,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP2,0.01,TEMP1,0.01,TEMP1,0.01,TEMP0/2.,0.01,TEMP0/2.,0.01,TEMP0,TEMP2,0.01,TEMP0/2.,0.01,TEMP0/2.,0.01,TEMP1,0.01,TEMP2,TEMP1,0.01,TEMP2,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP3,0.01,TEMP0,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,TEMP0,0.01,TEMP0,TEMP1,0.01,TEMP2,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,TEMP1,0.01,TEMP1,0.01,TEMP2,0.01,TEMP2,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,TEMP0,0.01,TEMP0,TEMP1,0.01,TEMP1,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,TEMP1,0.01,TEMP1,0.01,TEMP2,TEMP3,0.01,TEMP3,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,TEMP0,0.01,TEMP0,TEMP1,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,TEMP2,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,0.01,TEMP0,TEMP1,0.01,TEMP1,0.01,TEMP3,0.01};

/*!
 * \fn periodo (int)
 * \brief Dá os valores que devem ser carregados nos contadores.
 * \Entrada: Frequencia desejada
 * \Saida: periodo que deve ser inicializado o Systick/PIT
 */
int periodo(int frequencia){
	return 20971520/frequencia;
}

/*!
 * \fn PlaySOM(int)
 * \brief Faz tocar a nota desejada.
 * \Entrada: Frequencia desejada
 */
void PlaySOM(float frequencia,float tempo){
	if(!som_play){
	initSysTick(((int)periodo(2*frequencia))); ///< Iniciar no Systick a frequencia da nota
	enableSysTickInterrup();
		if (tempo != 0) {  ///< Se o tempo for zero, toca indefinidamente,senao toca pelo valor
			delay10us(100000*tempo);
			disableSysTickInterrup();
		}
	
	}
}

/*!
 * \fn PlayMUSICA(int)
 * \brief Faz tocar uma Musica.
 * \Entrada: Frequencia desejada
 */
void PlayMUSICA(int musica){
	/*!
	 * Variaveis de tempo base da musica
	 */
	int aux=musica; ///< Congela musica escolhida
	
	if( aux == 0){																///< Musica 1
		for(k = 0; k < 264; k++){
			if(musica_play == 0 && flag_acertou == 1) PlaySOM(v1[k],v2[k]);///< Acertou -> Som
			else if(musica_play == 0 && flag_acertou == 0) PlaySOM(1,v2[k]);///< Errou -> Sem Som
		}	
	}
	if(aux == 1){																///< Musica 2 
		for(k = 0; k < 80; k++){
			if(musica_play == 0 && flag_acertou == 1) PlaySOM(v3[k],v4[k]);///< Acertou -> Som
			else if(musica_play == 0 && flag_acertou == 0) PlaySOM(1,v4[k]);///< Errou -> Sem Som
		}
	}
	if(aux == 2){																///< Musica 3
		for(k = 0; k < 204; k++){
			if(musica_play == 0 && flag_acertou == 1) PlaySOM(v5[k],v6[k]);///< Acertou -> Som
			else if(musica_play == 0 && flag_acertou == 0) PlaySOM(1,v6[k]);///< Errou -> Sem Som
		}
	}
}









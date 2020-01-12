/*!
 * @file main.c
 * 
 * @brief 
 * 
 * @mainpage Projeto Final 
 * 
 * @details 
 * 			 
 * @author Jose Eduardo Alves Peraira Filho 176601 e Jo&atilde;o Vitor 176433
 * 
 * @date 07/11/2017
 * 
 *       
 * @attention 
 * 
 */

#include "derivative.h" 
#include "stdlib.h"
#include "string.h"
#include "util.h"
#include "nvic.h"
#include "atrasos.h"
#include "pushbutton.h"
#include "ledRGB.h"
#include "lcdled.h"
#include "systick.h"
#include "pit.h"
#include "handler.h"
#include "acel_MMA8451Q.h"
#include "I2C.h"
#include "musica.h"

/*! Variaveis Globais*/
uint16_t tempo,tempo_musica,pontos; ///< Variaveis de controle
uint8_t som_play, musica_play,flag_acertou,modo,ind_musica;  ///< Flags 
int k;

char str[5];///< Para exibicao da pontuacao

/*! Acelerometro*/
unsigned char AccData[6];
short Xout_14_bit, Yout_14_bit, Zout_14_bit;
float Xout_g, Yout_g, Zout_g;
char DataReady;
char Xoffset, Yoffset, Zoffset;
char acelerometro;

/*! Tela*/
char tela[16];

/*! Musicas*/
char musica1[] = "                >>>>>   <<<<<   ^^^^^   >>>>>   vvvvv   ^^^^^   >>>>>   <<<<<   vvvvv   ^^^^^   >>>>>   vvvvv   >>>>>   <<<<!                " ;
char musica2[] = "                    vvvvvv   ^^^^^   <<<<<<   >>>>>>   vvvvvv   >>>>>>   <<<<<<   vvvvvv   >>>>>>      >!                " ;
char musica3[] = "                    >>>>>>>>    <<<<<<<<    vvvvvvvv    ^^^^^^^^    >>>>>>>>    vvvvvvvv    <<<<<<<<    >>>>>>>>    vvvvvvvv       !                ";
char  *musica; 


/*! Funcoes*/
void inicializa(void);
void processo_inicial(void);
void configura_musica(void);
void run_jogo(void);
void pontuacao(void);

int main(void){
	
	/*! Inicializa*/
	inicializa();
	
	/*! Execucao do programa*/
	for(;;){
			if(modo == 0) processo_inicial();
			if(modo == 1) configura_musica();
			if(modo == 2) run_jogo();
			if(modo == 3) pontuacao();
		}
	
	return 1;
}            

/*!
 * \fn processo_inicial(void)
 * \brief Exibe inicio no LCD
 */
void processo_inicial(void){
	enviaLCDr0(0b10000000,4);
	mandaString("**Gravity HERO**");
	enviaLCDr0(0b11000000,4);
	mandaString("PTA 5/12 to PLAY");
}

/*!
 * \fn configura_musica(void)
 * \brief Escolha de musica do usuario
 */
void configura_musica(void){
	enviaLCDr0(0b10000000,4);
	mandaString("Escolha musica: ");
	enviaLCDr0(0b11000000,4);
	
	switch(ind_musica){
	case 0:
		mandaString("Do,Re,Mi,Fa     ");
		musica = musica1;
		tempo_musica = 125;///<58.8s  + 4s parado
		pontos = 0;
		break;
	case 1:
		mandaString("Brilha - Brilha ");
		musica = musica2;
		tempo_musica = 105;///<48,78s + 4s parado
		pontos = 0;
		break;
	case 2:
		mandaString("Sadness & Sorrow");
		musica = musica3;
		tempo_musica = 132;///< 62.2s + 4s parado
		pontos = 0;
		break;
	}
}

/*!
 * \fn run_jogo(void)
 * \brief  Inicio da partida
 */
void run_jogo(void){
	PlayMUSICA(ind_musica);
}

/*!
 * \fn pontuacao(void)
 * \brief Calculo final dos acertos do usuario
 */
void pontuacao(void){
	disablePITInterrup(); ///< Desabilita PIT, contador especifico da run_jogo() 
	
 	enviaLCDr0(0b10000000,4);
	mandaString(">--Pontuacao:--<");
	ConvUI2String(pontos,str);
	enviaLCDr0(0b11000000,4);
	mandaString("   ");
	mandaString(str);
	ConvUI2String(tempo_musica,str);
	mandaString(" de ");
	mandaString(str);
	mandaString("   ");
}

/*!
 * \fn inicializa(void)
 * \brief Inicializa todos os modulos e variaveis que serao nescessarios.
 */
void inicializa(void){
	/*! Inicializa Variaveis*/
	modo = 0;
	DataReady = 0;
	som_play = 0;
	
	/*! Inicializa Acelerometro*/
	MCU_Init(); ///< Inicializa modulo I2C para comunicar com acelerometro
	Accelerometer_Init();///< Processo de inicializacao do acelerometro
	Calibrate();

	/*! Inicializacoes do LCD*/
	initLcdledGPIO();
	initLCD();
	
	/*! Inicializa PORTE*/
	initPORTE();
		
	/*! Habilita as interrupcoes no controlador NVIC com nivel de prioridade */
	enableNVICPTA(1);
	enableNVICPIT(2);
	
	/*! Inicializacoes dos pushbuttons*/
	initPushbutton ();
	enablePushbuttonIRQA(); ///< Habilita interrupcao dos pinos 4, 5 e 12 da PORTA*/
	
	/*! Inicializando o PIT*/
	initPIT(0b000,20971520/2);///< Interrupções de 0,5s
}

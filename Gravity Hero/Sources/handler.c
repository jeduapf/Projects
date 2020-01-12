/*!
 * \file handler.c
 *
 * \date 10/10/2017
 * \author Jose Eduardo Alves Peraira Filho and Joao Vitor Santos Prado
 */

#include "handler.h"
#include "ledRGB.h"
#include "lcdled.h"
#include "atrasos.h"
#include "acel_MMA8451Q.h"
#include "I2C.h"
#include "pit.h"
#include "musica.h"
#include "systick.h"
#include "pushbutton.h"

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

/*!
 * \fn acertou(void)
 * \brief Verifica se usuario posicionou a placa conforme a musica
 */
void acertou(void){
	acelerometro = dadosAcelerometro_char(); ///< captura dado do acelerometro
		if(tela[0] == acelerometro){
			flag_acertou = 1;
			pontos++;
		}else {
			flag_acertou = 0;
		}
}
/*!
 * \fn atualizatela(uint16_t tempo)
 * \brief Faz o efeito cascata da tela
 */
void atualizatela(uint16_t tempo){
	uint8_t i;
	uint16_t a = tempo;///< congela tempo para evitar conflito
	for(i = 0; i < 16; i++){
		tela[i] = musica[a+i];
	}
}
/*!
 * \fn SysTick_Handler (void)
 * \brief Rotina de serviço para excecao SysTick (Vetor 15)
 */
void SysTick_Handler(void) {
	alternaPORTE();			/* alterna PORTE*/
}

/*
 * \fn PIT_IRQHandler (void)
 * \brief Rotina de serviço para PIT (Vetor 38, IRQ22, prioridade em NVICIPR5[23:22])
 */
void PIT_IRQHandler(void) {
	if (tempo == tempo_musica){///< Quando rodou a tela inteira, encerra o jogo
		modo = 3;///< Modo pontuacao
		musica_play = 1;///< Faz a musica nao tocar
	} 
	acertou();
	tempo++;
	atualizatela(tempo);
	if(modo != 0){///< para evitar conflitos no final do jogo, impede atualizacao da tela
		enviaLCDr0(0b10000000,4);
		mandaString(tela);
		enviaLCDr0(0b11000000,4);
		mandaString(tela);
	}
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK;     ///< TIF=PIT_TFLG0[31]: w1c (limpa pendências)
}

/*!
 * \fn PORTA_IRQHandler (void)
 * \brief Rotina de serviço para GPIOA (Vetor 46, IRQ30, prioridade em NVICIPR7[31:30])
 */
void PORTA_IRQHandler(void) {
	if(PORTA_PCR14 & PORT_PCR_ISF_MASK){///< Aciona acelerometro quando interromper
		PORTA_PCR14 |= PORT_PCR_ISF_MASK;            ///< abaixa a bandeira de interrupcao
		DataReady = 1;
	}
	else {
		if (PORTA_PCR12 & PORT_PCR_ISF_MASK) {
			if (GPIOA_PDIR & GPIO_PIN(12)) {
				/*! Soltou! */ 			
			} else {
				/*! Apertou: */
				switch(modo){
					case 0:///< Modo inicio
						modo = 1;
						PlaySOM(Do,0.30);
						disableSysTickInterrup(); ///< Para nao emitir som
						break;
					case 1:///< Modo configura
						ind_musica = (ind_musica+1)%MAX_MUSICA;///< Escolhe musica
						PlaySOM(Fa,0.15);
						disableSysTickInterrup();///< Para nao emitir som
						break;
					case 3:///< Modo pontuacao
						modo = 0;
						break;
				}
			}
			PORTA_PCR12 |= PORT_PCR_ISF_MASK;            ///< abaixa a bandeira de interrupcao
		} else if (PORTA_PCR5 & PORT_PCR_ISF_MASK) {
			if (GPIOA_PDIR & GPIO_PIN(5)) {
				/*! Soltou */ 
			} else {
				/*! Apertou */
				switch(modo){
					case 0:///< Modo inicio
						modo = 1;
						PlaySOM(Do,0.30);
						disableSysTickInterrup();///< Para nao emitir som
						break;
					case 1:///< Modo configura
						modo = 2;
						tempo = 0;
						PlaySOM(Do,0.30);
						musica_play = 0;
						enablePITInterrup();///< Para comecar rodar o modo jogo	
						break;
					case 3:///< Modo pontuacao
						modo = 0;
						disablePITInterrup();///< Para nao atualizar a tela	
						break;
				}
			}
			PORTA_PCR5 |= PORT_PCR_ISF_MASK;            ///< abaixa a bandeira de interrupcao
		} else if (PORTA_PCR4 & PORT_PCR_ISF_MASK) {
			if (GPIOA_PDIR & GPIO_PIN(4)) {
				/*! Soltou */
			} else {
				/*! Apertou */
				switch(modo){
					case 2:///< Modo jogo
						musica_play = 1; ///< Para nao permitir que toque de manha
						disablePITInterrup();///< Para nao atualizar a tela
						enviaLCDr0(0b10000000,4);
						mandaString(" Jogo Cancelado ");
						enviaLCDr0(0b11000000,4);
						mandaString("                ");
						modo = 0;
						delay10us(12500); ///< Para permitir visualizacao
						disableSysTickInterrup();///< Para nao emitir som
						break;
				}
			}
			PORTA_PCR4 |= PORT_PCR_ISF_MASK;            ///< abaixa a bandeira de interrupcao
		} 
	}
}

void NMI_Handler (void) {
}

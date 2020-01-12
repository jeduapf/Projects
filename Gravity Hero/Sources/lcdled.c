/*!
 * \file lcdled.c
 *
 * \date 19/09/2017
 * \author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

#include "atrasos.h"
#include "lcdled.h"
#include "ledRGB.h"

extern uint8_t icon_seta_direita[8];		///<		>>
extern uint8_t icon_seta_esquerda[8];		///<		<<		
extern uint8_t icon_seta_baixo[8];			///<		v	
extern uint8_t icon_seta_cima[8];			///<		^

/**
 * \brief Inicializa como GPIO A12, B18, B19, C0-10 e D1 
 */
void initLcdledGPIO(void) {
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK ;     ///< Habilita os clocks dos módulos

	/*! Configura os pinos PORTC[10:0] como GPIO de saida */
	PORTC_PCR0 |= PORT_PCR_MUX(0x1);       ///< D0-D7 dos dados (GPIO)
	PORTC_PCR1 |= PORT_PCR_MUX(0x1);
	PORTC_PCR2 |= PORT_PCR_MUX(0x1);
	PORTC_PCR3 |= PORT_PCR_MUX(0x1);
	PORTC_PCR4 |= PORT_PCR_MUX(0x1);
	PORTC_PCR5 |= PORT_PCR_MUX(0x1);
	PORTC_PCR6 |= PORT_PCR_MUX(0x1);
	PORTC_PCR7 |= PORT_PCR_MUX(0x1);
	PORTC_PCR8 |= PORT_PCR_MUX(0x1);        ///< RS do LCD
	PORTC_PCR9 |= PORT_PCR_MUX(0x1);        ///< E do LCD
	PORTC_PCR10 |= PORT_PCR_MUX(0x1);       ///< /LE do latch

	GPIOC_PDDR |= GPIO_PDDR_PDD(GPIO_PIN(0x0) | GPIO_PIN(0x1) | GPIO_PIN(0x2) |GPIO_PIN(0x3) | 
			GPIO_PIN(0x4) | GPIO_PIN(0x5) | GPIO_PIN(0x6) | GPIO_PIN(0x7) | 
			GPIO_PIN(0x8) | GPIO_PIN(0x9) | GPIO_PIN(0xA));       ///< Configura como saidas
	GPIOC_PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(0x0) | GPIO_PIN(0x1) | GPIO_PIN(0x2) |GPIO_PIN(0x3) | 
			GPIO_PIN(0x4) | GPIO_PIN(0x5) | GPIO_PIN(0x6) | GPIO_PIN(0x7) | 
			GPIO_PIN(0x8) | GPIO_PIN(0x9) | GPIO_PIN(0xA));       ///< 

}

/*!
 * \fn pulso (char p)
 * \brief Gera um pulso "Enable" de t*1us.
 * \param[in] p para LCD (p=0) e para Leds (p=1).
 */
void pulso(tipo_enable p, uint8_t t) {
	GPIOC_PSOR = GPIO_PIN (9 + p);     ///< Seta 1 no PORTC[9+p]
	GPIOC_PCOR = GPIO_PIN (9 + p);       ///< Limpa em 0 o PORTC[9+p]
	delay10us(t);                        ///< mantem aprox. t*10us
}

/*!
 * \fn RS (uint8_t l)
 * \brief Envia ao LCD o sinal RS pelo pino PORTC[8].
 * \param[in] l valor do RS (0, byte de instrucao e 1, byte de dados).
 */
void RS(uint8_t l) {
	if(l) {                      ///< (l != 0)
		GPIOC_PSOR = GPIO_PIN(8);    ///< Seta o LCD no modo de dados
	} else {   
		GPIOC_PCOR = GPIO_PIN(8);     ///< Seta o LCD no modo de instrucao
	}
}

/*!
 * \fn enviaLCDr (char c)
 * \brief Envia ao LCD um byte pelos pinos PORTC[7:0]
 * \param[in] c caracter em ASCII.
 * \param[in] t tempo de processamento necessario.
 */
void enviaLCD(char c, uint8_t t) {
	GPIOC_PCOR = 0x000000FF;          ///< limpa em 0 PORTC[7:0]
	GPIOC_PSOR = (unsigned int)c;     ///< Seta os bits que devem ser setados
	pulso(0, t);                      ///< dispara o pulso "Enable" do LCD
}

/*!
 * \fn enviaLCDr0 (char c)
 * \brief Envia ao LCD um byte pelos pinos PORTC[7:0]
 * \param[in] c caracter em ASCII.
 * \param[in] t tempo de processamento necessario.
 */
void enviaLCDr0(char c, uint8_t t) {
	RS(0);
	GPIOC_PCOR = 0x000000FF;          ///< limpa em 0 PORTC[7:0]
	GPIOC_PSOR = (unsigned int)c;     ///< Seta os bits que devem ser setados
	pulso(0, t);                      ///< dispara o pulso "Enable" do LCD
}

/*!
 * \fn enviaLCDr1 (char c)
 * \brief Envia ao LCD um byte pelos pinos PORTC[7:0]
 * \param[in] c caracter em ASCII.
 * \param[in] t tempo de processamento necessario.
 */
///Ting: Talvez valesse a pena ponderar se eh de fato eficiente repetir as instrucoes RS(x) 
/// no envio de cada caractere ...
/// Quando voce seta RS em 1 o LCD se mantem neste estado ...
void enviaLCDr1(char c, uint8_t t) {
	RS(1);
	GPIOC_PCOR = 0x000000FF;          ///< limpa em 0 PORTC[7:0]
	GPIOC_PSOR = (unsigned int)c;     ///< Seta os bits que devem ser setados
	pulso(0, t);                      ///< dispara o pulso "Enable" do LCD
}


///Ting: Nao ha variavel t!!!
/*!
 * \fn enviaLed (char c)
 * \brief Envia ao LED um byte pelos pinos PORTC[7:0]
 * \param[in] c caracter em ASCII.
 * \param[in] t tempo de processamento necessario.
 */
void enviaLed(char c) {
	GPIOC_PCOR = 0x000000FF;           ///< limpa em 0 PORTC[7:0]
	GPIOC_PSOR = (unsigned int)c;     ///< Seta os bits que devem ser setados
	pulso(1, 1);                      ///< dispara o pulso "Enable" do Latch 74573
}


///Ting: Eh initLCD
/*!
 * \fn inicLCD (void) 
 * \brief Inicializa o LCD com a sequencia de instrucoes recomendada pelo fabricante
 */
void initLCD(void) {
	int k;
	lcd init_LCD[4];

	/*! 
	 * Instrucoes de inicializacao do LCD
	 */
	init_LCD[0].cop = LCD_FUNCTION_SET; ///< 39 us
	init_LCD[0].tempo = 4; 
	init_LCD[1].cop = LCD_DISPLAY_CONTROL; ///< 39 us
	init_LCD[1].tempo = 4;
	init_LCD[2].cop = LCD_DISPLAY_CLEAR; ///< 1.53 ms
	init_LCD[2].tempo = 153;
	init_LCD[3].cop = LCD_ENTRY_MODE_SET; ///< 39 us
	init_LCD[3].tempo = 4;

	delay10us(153);

	RS(0);                    ///< Seta o LCD no modo de instrucao
	for(k = 0; k < 4; k++) {  
		enviaLCDr0(init_LCD[k].cop, init_LCD[k].tempo);    ///< instrucao de inicializacao
	}	
}

/*!
 * \fn mandaString (char *s)
 * \brief Envia uma string de caracteres.
 * \param[in] s endereco inicial da string.
 */
void mandaString(char * s) {
	///Ting: Veja que voce setou RS=1 aqui ...
	RS(1);                            ///< Seta o LCD no modo de dados
	while (*s) {                      ///< enquanto o conteudo do endereco != 0
		///Ting: Nao sao 43us? Nao precisa repetir RS=1. O sinal ja esta configurado ...
//		enviaLCDr1(*s, 4);            ///< envia o byte
		enviaLCD(*s, 5);            ///< envia o byte
		s++;                          ///< incrementa o endereco
	}
}

/*!
 * \fn limpaLCD (void) 
 * \brief Envia a instrução "Clear Display" (0x01).
 */
void limpaLCD(void) {
	enviaLCDr0(LCD_DISPLAY_CLEAR,153);
}

///Ting: Eh criaBitmap
/*!
 * \fn criaBitmapI (uint8_t end)
 * \brief Cria um bitmap "i"
 * \param[in] end endereco onde o bitmap e gravado
 * \param[in] bitmap matriz do bitmap
 */
void criaBitmap(uint8_t end, uint8_t *bitmap){

		enviaLCDr0(0x40|(end*8),4);				///< define o enderecao em CGRAM
		RS(1);
		enviaLCD(bitmap[0], 5); 				///< carrega a linha 1 
		enviaLCD(bitmap[1], 5);				///< carrega a linha 2
		enviaLCD(bitmap[2], 5); 				///< carrega a linha 3
		enviaLCD(bitmap[3], 5); 				///< carrega a linha 4
		enviaLCD(bitmap[4], 5);				///< carrega a linha 5
		enviaLCD(bitmap[5], 5); 				///< carrega a linha 6
		enviaLCD(bitmap[6], 5); 				///< carrega a linha 7
		enviaLCD(bitmap[7], 5); 				///< carrega a linha 8	
}


/*
 * @file acel_MMA84151Q.c
 *
 * @date 01/11/2017
 * @author Jose Eduardo and Joao Vitor Santos Prado and Tomas Vaverka
 */

#include "derivative.h" 				// Include peripheral declarations 
#include "acel_MMA8451Q.h"
#include "I2C.h"

/*! Variaveis globais*/
extern unsigned char AccData[];
extern short Xout_14_bit, Yout_14_bit, Zout_14_bit;
extern float Xout_g, Yout_g, Zout_g;
extern char DataReady;
extern char Xoffset, Yoffset, Zoffset;

/*! Funcao de inicializacao do MCU*/

void MCU_Init(void)
{
	/*! Inicializacao do mÃ³dulo de comunicacao serial I2C*/
	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;		// Ligando o clock do mÃ³dulo I2C0 
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;		// Ligando o clock da PORTE 
	PORTE_PCR24 = PORT_PCR_MUX(5);			// o pino PTE24 e tambem da SCL line de I2C0  
	PORTE_PCR25 = PORT_PCR_MUX(5);			// o pino PTE25 e tambem da SDA line de I2C0  
	I2C0_F  = 0x14; 						// SDA hold time = 2.125us, SCL start hold time = 4.25us, SCL stop hold time = 5.125us *
	I2C0_C1 = I2C_C1_IICEN_MASK;    		// Habilita I2C0 module 
	
	/*! Configurando o pino 14 (conectado em INT1 de MMA8451Q) para interrupcµes na borda de descida*/
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;		// Ligando o clock da PORTA 
	PORTA_PCR14 |= (0|PORT_PCR_ISF_MASK|	// Limpa a flag de interrupcao 
					  PORT_PCR_MUX(0x1)|	// PTA14 e configurado para GPIO 
					  PORT_PCR_IRQC(0xA));	// PTA14 is configured for interrupcµes na borda de descida 
	
	/*! Habilitando interrupcµes na PORTA pelo NVIC*/
	NVIC_ICPR |= 1 << ((INT_PORTA - 16)%32); 
	NVIC_ISER |= 1 << ((INT_PORTA - 16)%32); 
}

/*! Funcao de inicializacao do Acelerometro*/ 

void Accelerometer_Init (void)
{
	unsigned char reg_val = 0;
	
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG2, 0x40);		// Resetando todos os registradores
	
	do		// Esperando o bit RST ficar limpo 
	{
		reg_val = I2C_ReadRegister(MMA845x_I2C_ADDRESS, CTRL_REG2) & 0x40; 
	} 	while (reg_val);
	
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, XYZ_DATA_CFG_REG, 0x00);		// +/-2g range -> 1g = 16384/4 = 4096 counts 
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG2, 0x02);		// Modo de Alta resolucao
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG1, 0x3D);	// ODR = 1.56Hz, Reduced noise, Active mode	
}

/*! Calibracao com base nos offset*/

void Calibrate (void)
{
	unsigned char reg_val = 0;
	
	while (!reg_val)		// Esperando receber o primeiro dado	 
	{
		reg_val = I2C_ReadRegister(MMA845x_I2C_ADDRESS, STATUS_REG) & 0x08; 
	} 	
	  	
	I2C_ReadMultiRegisters(MMA845x_I2C_ADDRESS, OUT_X_MSB_REG, 6, AccData);		// Leitura dos registradores de saida 0x01-0x06  
	  						
	Xout_14_bit = ((short) (AccData[0]<<8 | AccData[1])) >> 2;		// Computando os 14-bits referentes ao eixo x
	Yout_14_bit = ((short) (AccData[2]<<8 | AccData[3])) >> 2;		// Computando os 14-bits referentes ao eixo y
	Zout_14_bit = ((short) (AccData[4]<<8 | AccData[5])) >> 2;		// Computando os 14-bits referentes ao eixo z
	  					
	Xoffset = Xout_14_bit / 8 * (-1);		// Computando valores referentes ao eixo x com correcao de offset
	Yoffset = Yout_14_bit / 8 * (-1);		// Computando valores referentes ao eixo y com correcao de offset
	Zoffset = (Zout_14_bit - SENSITIVITY_2G) / 8 * (-1);		// Computando valores referentes ao eixo z com correcao de offset
	  					
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG1, 0x00);		// Modo de espera para permitir a escrita aos registros de compensacao	
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, OFF_X_REG, Xoffset);		
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, OFF_Y_REG, Yoffset);	
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, OFF_Z_REG, Zoffset);	
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG3, 0x00);		// Push-pull, baixa interrupcao ativa 
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG4, 0x01);		// Ativar interrupcao DRDY
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG5, 0x01);		// Interrupcao DRDY roteada para INT1 - PTA14
	I2C_WriteRegister(MMA845x_I2C_ADDRESS, CTRL_REG1, 0x3D);		// ODR = 1.56Hz, ruÃ­do reduzido, modo ativo
}

/*!
 * \fn dadosAcelerometro(void)
 * \brief captura os dados do acelerometro.
 */
void dadosAcelerometro(void){
	DataReady = 0;
															
	I2C_ReadMultiRegisters(MMA845x_I2C_ADDRESS, OUT_X_MSB_REG, 6, AccData);		// Leitura dos registradores de dados de saida 
    
	Xout_14_bit = ((short) (AccData[0]<<8 | AccData[1])) >> 2;		// Computando o valor de 14-bit do eixo X
	Yout_14_bit = ((short) (AccData[2]<<8 | AccData[3])) >> 2;		// Computando o valor de 14-bit do eixo Y
	Zout_14_bit = ((short) (AccData[4]<<8 | AccData[5])) >> 2;		// Computando o valor de 14-bit do eixo Z
	
	Xout_g = ((float) Xout_14_bit)*RANGE / SENSITIVITY_2G;			// Computando o valor de g do eixo X (range = aumenta escala)
	Yout_g = ((float) Yout_14_bit)*RANGE / SENSITIVITY_2G;			// Computando o valor de g do eixo Y (range = aumenta escala)
	Zout_g = ((float) Zout_14_bit)*RANGE / SENSITIVITY_2G;			// Computando o valor de g do eixo Z (range = aumenta escala)
}

/*!
 * \fn dadosAcelerometro_char(void)
 * \obtem dados do acelerometro, e os transforma em caracteres
 */
char dadosAcelerometro_char(void){
	if (DataReady) dadosAcelerometro();
	if((Xout_g < 1800 && Xout_g > -1800) && (Yout_g < 1800 && Yout_g > -1800))
		return ' ';
	
	else if((Xout_g < 1000 && Xout_g > -1000)&&( Yout_g > 1500 || Yout_g <-1500))
	{
		if(Yout_g < 0)		return '>'; //<Esquerda
		else 				return '<';//<Direita
	}
	else {
		if(Xout_g < 0)		return 'v';//<Baixo
		else				return '^';//<Cima
	}
}

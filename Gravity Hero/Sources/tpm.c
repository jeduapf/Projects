/*
 * \file tpm.c
 *
 *  * \date 19/09/2017
 * \author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado and Leandro Cavalcanti Silva
 * \note Veja a rela&ccedil;&atilde;o dos campos dos registradores na Figura 31-1 de KL-25 Sub-Family Reference Manual
 *   Configura&ccedil;&atilde;o dos m&oacute;dulos TPM0, TPM1 e TPM2
 *   Periodo_{UP} = (MOD+1) * (PS/(frequ&ecirc;ncia de source clock))      (CPWMS=0);
 *   Periodo_{UP-DOWN} = (MOD) * (PS/(frequ&ecirc;ncia de source clock)) * (1/(1+CPWMS)) (CPWMS=1).
 *   Fórmula geral:
 *   Periodo_{TPMS} ~ (MOD) * (PS/(frequ&ecirc;ncia de source clock)) * (1/(1+CPWMS))
 *   frequência de MCGFLLCLK =  20.971520MHz 
 *   Vale observar que para programar um duty cycle 100%, MOD < 0xFFFF (Seção 31.4.6)
 *   
 *  
 */


/*!
 * \brief Configura o módulo TPM0.
 * \param[in] pre pr&eacute;-escala
 * \param[in] counter valor m&aacute;ximo de contagem
 * \param[in] cpwms alinhamento do meio de contagem
 *
 */

#include "util.h"
#include "tpm.h"

volatile uint32_t *tpm_sc;
volatile uint32_t *tpm_cnt;
volatile uint32_t *tpm_mod;
volatile uint32_t *tpm_cnsc;
volatile uint32_t *tpm_cnv;
volatile uint32_t *tpm_status;
volatile uint32_t *tpm_conf;

/*!
 * \brief initialize a TPM module
 * \param[in] modulo mudule ID
 * \param[in] pre pr&eacute;-scale
 * \param[in[ counter contagem m&aacute;xima do contador do m&oacute;dulo
 * \param[in] cpwms tipo de alinhamento da contagem
 * 
 * \attention H&aacute; uma s&eacute;rie de configura&ccedil;&otilde;es associadas a um m&oacute;dulo TPM.
 * S&oacute; ser&atilde;o setadas tr&ecirc;s delas nesta disciplina.
 */
void initTPM(uint8_t modulo, uint8_t pre, uint16_t counter, uint8_t cpwms) {
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(0b01);     ///< TPMSRC=SIM_SOPT2[25:24]=01 (MCGFLLCLK|MCGPLLCLK/2) 
	SIM_SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;  ///< PLLFLLSEL=SIM_SOPT2[16]=0 (MCGFLLCLK)		
	
	switch(modulo) {
	case 0:
		SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;        ///< TPM0=SIM_SCGC6[24]=1 (Clock gate de TPM0 habilitado)
		tpm_sc = &TPM_SC_REG(TPM0_BASE_PTR);	
		tpm_cnt = &TPM_CNT_REG(TPM0_BASE_PTR);
		tpm_mod = &TPM_MOD_REG(TPM0_BASE_PTR);
		tpm_status = &TPM_STATUS_REG(TPM0_BASE_PTR);
		tpm_conf = &TPM_CONF_REG(TPM0_BASE_PTR);
		break;
	case 1:
		SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;        ///< TPM1=SIM_SCGC6[25]=1 (Clock gate de TPM1 habilitado)
		tpm_sc = &TPM_SC_REG(TPM1_BASE_PTR);	
		tpm_cnt = &TPM_CNT_REG(TPM1_BASE_PTR);
		tpm_mod = &TPM_MOD_REG(TPM1_BASE_PTR);
		tpm_status = &TPM_STATUS_REG(TPM1_BASE_PTR);
		tpm_conf = &TPM_CONF_REG(TPM1_BASE_PTR);
		break;
	case 2:
		SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;        ///< TPM2=SIM_SCGC6[26]=1 (Clock gate de TPM2 habilitado)
		tpm_sc = &TPM_SC_REG(TPM2_BASE_PTR);	
		tpm_cnt = &TPM_CNT_REG(TPM2_BASE_PTR);
		tpm_mod = &TPM_MOD_REG(TPM2_BASE_PTR);
		tpm_status = &TPM_STATUS_REG(TPM2_BASE_PTR);
		tpm_conf = &TPM_CONF_REG(TPM2_BASE_PTR);
	}
	
	*tpm_sc |= (TPM_SC_TOF_MASK |            ///< TOF=TPMx_TOF[7]: w1c (limpa a pend&ecird;ncia do TPM0) 
				TPM_SC_CMOD(0x1) |           ///< CMOD=TPMx_SC[4:3]=0b01 (incrementa a cada pulso do LPTPM)
				TPM_SC_PS(pre) );            ///< PS=TPMx_SC[2:0]=0b000 (Fator Pr&eacute;-escala=32)
	if (cpwms)			
		*tpm_sc |= TPM_SC_CPWMS_MASK; 		 ///< CPWMS=TPMx_SC[5]=1 (modo de contagem crescente-decrescente)
	else 
		*tpm_sc &= ~TPM_SC_CPWMS_MASK; 	     ///< CPWMS=TPMx_SC[5]=0 (modo de contagem crescente)

	*tpm_sc &= ~(TPM_SC_DMA_MASK |               ///< DMA=TPMx_SC[8]=0 (desabilita DMA)
				TPM_SC_TOIE_MASK);     ///< TOIE=TPMx_SC[6]=0 (desabilita interrup&ccedil;&atilde;o "overflow")

	*tpm_cnt |= TPM_CNT_COUNT(0x0)  ;        ///< COUNT=TPMx_CNT[15:0]: w (qq acesso de escrita reseta LPTPM)

	*tpm_mod &= TPM_MOD_MOD(counter);       ///< MOD=TPMx_MOD[15:0]=COUNTER_OVF atualizado conforme CMOD e CPWMS
											 ///<  (Se&ccedil;&atilde;o 31.4.8.1 de KL25 Sub-Family Reference Manual)
	
	*tpm_conf &= ~(TPM_CONF_CROT_MASK |      ///< CROT=TPMx_CONF[18]=0 (n&atilde;o recarrega o valor de LPTPM no gatilho)
			TPM_CONF_CSOO_MASK |             ///< CSOO=TPMx_CONF[17]=0 (continua contagem em overflow)
			TPM_CONF_CSOT_MASK |             ///< CSOT=TPMx_CONF[16]=0 (inicia contagem quando habilitado)
			TPM_CONF_GTBEEN_MASK |			 ///< GTBEEN=TPMx_CONF[9]=0 (LPTPM contador como base de tempo)
			TPM_CONF_DOZEEN_MASK);           ///< DOZEEN=TPMx_CONF[5]=0 (continua contagem em hiberna&ccedil;&atilde;o)
	*tpm_conf |= TPM_CONF_DBGMODE(0b11);	 ///< DBGMODE=TPMx_CONF[7:6]=0b11 (continua contagem em Debug)
}

/*!
 * \brief Configure o canal n do m&oacute;dulo TPM0. 
 * \param[in] canal canal do m&oacute;dulo
 * \param[in] msb modo de opera&ccedil;&atilde;o do canal (d&iacute;gito b)
 * \param[in] msa modo de opera&ccedil;&atilde;o do canal (d&iacute;gito a) 
 * \param[in] elsb borda/n&iacute;vel (d&iacute;gito b)
 * \param[in] elsa borda/n&iacute;vel (d&iacute;gito a)
 * \param[in] valor valor do canal
 */
uint8_t initTPM0Cn(uint8_t canal, uint8_t c_modo, unsigned short valor) {
	
	switch(canal) {
	case 0:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,0);
		tpm_cnv = &TPM_CnV_REG(TPM0_BASE_PTR,0);
		break;
	case 1:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,1);
		tpm_cnv = &TPM_CnV_REG(TPM0_BASE_PTR,1);
		break;
	case 2:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,2);
		tpm_cnv = &TPM_CnV_REG(TPM0_BASE_PTR,2);
		break;
	case 3:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,3);
		tpm_cnv = &TPM_CnV_REG(TPM0_BASE_PTR,3);
		break;
	case 4:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,4);
		tpm_cnv = &TPM_CnV_REG(TPM0_BASE_PTR,4);
		break;
	case 5:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,5);
		tpm_cnv = &TPM_CnV_REG(TPM0_BASE_PTR,5);
		break;
	default:
		return 0;
	}
	
	/*!
	 * Configura&ccedil;&atilde;o do canal 0 (led B): CPWMS(0), MSnB:MSnA(10), ELSnB:ELSnA(11) - edge-aligned PWM
	 */
	*tpm_cnsc |= (TPM_CnSC_CHF_MASK);       ///< CHF=TPM0_CnSC[7]: w1c (limpa a pend&ecirc;ncia do canal) 
	*tpm_cnsc &= ~(TPM_CnSC_CHIE_MASK |     ///< CHIE=TPM0_CnSC[6]=0 (desabilita interrup&ccedil;&atilde;o)
				TPM_CnSC_DMA_MASK);         ///< DMA=TPM0_CnSC[0]=0 (desabilita DMA)
	*tpm_cnv = TPM_CnV_VAL(valor);  ///< VAL=valor

	/*! Sele&ccedil;&atilde;o de modo (Se&ccedil;&atilde;o 31.3.4 em 
	 * ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf 
	 */
	*tpm_cnsc &= ~(TPM_CnSC_MSB_MASK |      ///< MSB=TPM2_C0SC[5]=0
				TPM_CnSC_MSA_MASK |         ///< MSA=TPM2_C0SC[4]=0
				TPM_CnSC_ELSB_MASK |        ///< ELSB=TPM2_C0SC[3]=0
				TPM_CnSC_ELSA_MASK );       ///< ELSA=TPM2_C0SC[2]=0

	*tpm_cnsc |= (c_modo << 2);
	
	return 1;
}

/*!
 * \brief Configure o canal n do módulo TPM1. 
 * \param[in] canal canal do m&oacute;dulo
 * \param[in] msb modo de opera&ccedil;&atilde;o do canal (d&iacute;gito b)
 * \param[in] msa modo de opera&ccedil;&atilde;o do canal (d&iacute;gito a) 
 * \param[in] elsb borda/n&iacute;vel (d&iacute;gito b)
 * \param[in] elsa borda/n&iacute;vel (d&iacute;gito a)
 * \param[in] valor valor do canal
 */
uint8_t initTPM1Cn(uint8_t canal, uint8_t c_modo, unsigned short valor) {
	
	switch(canal) {
	case 0:
		tpm_cnsc = &TPM_CnSC_REG(TPM1_BASE_PTR,0);
		tpm_cnv = &TPM_CnV_REG(TPM1_BASE_PTR,0);
		break;
	case 1:
		tpm_cnsc = &TPM_CnSC_REG(TPM1_BASE_PTR,1);
		tpm_cnv = &TPM_CnV_REG(TPM1_BASE_PTR,1);
		break;
	default:
		return 0;
	}
	
	/*!
	 * Configura&ccedil;&atilde;o do canal 0 (led B): CPWMS(0), MSnB:MSnA(10), ELSnB:ELSnA(11) - edge-aligned PWM
	 */
	*tpm_cnsc |= (TPM_CnSC_CHF_MASK);       ///< CHF=TPM0_CnSC[7]: w1c (limpa a pend&ecirc;ncia do canal) 
	*tpm_cnsc &= ~(TPM_CnSC_CHIE_MASK |     ///< CHIE=TPM0_CnSC[6]=0 (desabilita interrup&ccedil;&atilde;o)
				TPM_CnSC_DMA_MASK);         ///< DMA=TPM0_CnSC[0]=0 (desabilita DMA)
	*tpm_cnv = TPM_CnV_VAL(valor);  ///< VAL=valor

	/*! Sele&ccedil;&atilde;o de modo (Se&ccedil;&atilde;o 31.3.4 em 
	 * ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf 
	 */
	*tpm_cnsc &= ~(TPM_CnSC_MSB_MASK |      ///< MSB=TPM2_C0SC[5]=0
				TPM_CnSC_MSA_MASK |         ///< MSA=TPM2_C0SC[4]=0
				TPM_CnSC_ELSB_MASK |        ///< ELSB=TPM2_C0SC[3]=0
				TPM_CnSC_ELSA_MASK );       ///< ELSA=TPM2_C0SC[2]=0

	*tpm_cnsc |= (c_modo << 2);

	return 1;
}

/*!
 * \brief Configure o canal n do módulo TPM2. 
 * \param[in] canal canal do m&oacute;dulo
 * \param[in] msb modo de opera&ccedil;&atilde;o do canal (d&iacute;gito b)
 * \param[in] msa modo de opera&ccedil;&atilde;o do canal (d&iacute;gito a) 
 * \param[in] elsb borda/n&iacute;vel (d&iacute;gito b)
 * \param[in] elsa borda/n&iacute;vel (d&iacute;gito a)
 * \param[in] valor valor do canal
 */
uint8_t initTPM2Cn(uint8_t canal, uint8_t c_modo, unsigned short valor) {
	
	switch(canal) {
	case 0:
		tpm_cnsc = &TPM_CnSC_REG(TPM2_BASE_PTR,0);
		tpm_cnv = &TPM_CnV_REG(TPM2_BASE_PTR,0);
		break;
	case 1:
		tpm_cnsc = &TPM_CnSC_REG(TPM2_BASE_PTR,1);
		tpm_cnv = &TPM_CnV_REG(TPM2_BASE_PTR,1);
		break;
	default:
		return 0;
	}
	
	/*!
	 * Configura&ccedil;&atilde;o do canal 0 (led B): CPWMS(0), MSnB:MSnA(10), ELSnB:ELSnA(11) - edge-aligned PWM
	 */
	*tpm_cnsc |= (TPM_CnSC_CHF_MASK);       ///< CHF=TPM0_CnSC[7]: w1c (limpa a pend&ecirc;ncia do canal) 
	*tpm_cnsc &= ~(TPM_CnSC_CHIE_MASK |     ///< CHIE=TPM0_CnSC[6]=0 (desabilita interrup&ccedil;&atilde;o)
				TPM_CnSC_DMA_MASK);         ///< DMA=TPM0_CnSC[0]=0 (desabilita DMA)
	*tpm_cnv = TPM_CnV_VAL(valor);  ///< VAL=valor

	/*! Sele&ccedil;&atilde;o de modo (Se&ccedil;&atilde;o 31.3.4 em 
	 * ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf 
	 */
	*tpm_cnsc &= ~(TPM_CnSC_MSB_MASK |      ///< MSB=TPM2_C0SC[5]=0
				TPM_CnSC_MSA_MASK |         ///< MSA=TPM2_C0SC[4]=0
				TPM_CnSC_ELSB_MASK |        ///< ELSB=TPM2_C0SC[3]=0
				TPM_CnSC_ELSA_MASK );       ///< ELSA=TPM2_C0SC[2]=0

	*tpm_cnsc |= (c_modo << 2);
	
	return 1;
}

/*!
 * \brief Configure o canal n do m&oacute;dulo TPM0. 
 * \param[in] canal canal do m&oacute;dulo
 * \param[in] msb modo de opera&ccedil;&atilde;o do canal (d&iacute;gito b)
 * \param[in] msa modo de opera&ccedil;&atilde;o do canal (d&iacute;gito a) 
 * \param[in] elsb borda/n&iacute;vel (d&iacute;gito b)
 * \param[in] elsa borda/n&iacute;vel (d&iacute;gito a)
 */
uint8_t reconfigModoTPM0Cn(uint8_t canal, uint8_t c_modo) {
	
	switch(canal) {
	case 0:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,0);
		break;
	case 1:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,1);
		break;
	case 2:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,2);
		break;
	case 3:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,3);
		break;
	case 4:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,4);
		break;
	case 5:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,5);
		break;
	default:
		return 0;
	}
	
	/*!
	 * Desabilita o canal
	 */
	*tpm_cnsc &= ~(TPM_CnSC_MSB_MASK |      ///< MSB=TPM2_C0SC[5]=0
				TPM_CnSC_MSA_MASK |         ///< MSA=TPM2_C0SC[4]=0
				TPM_CnSC_ELSB_MASK |        ///< ELSB=TPM2_C0SC[3]=0
				TPM_CnSC_ELSA_MASK );       ///< ELSA=TPM2_C0SC[2]=0

	/*!
	 * Conforme
	 * ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf#page=555
	 * "When switching from one channel mode to a different channel mode, 
	 * the channel must first be disabled and this must be acknowledged in 
	 * the LPTPM counter clock domain."
	 */
	*tpm_cnsc |= (c_modo << 2);

	return 1;
}

/*!
 * \brief Configure o canal n do módulo TPM1. 
 * \param[in] canal canal do m&oacute;dulo
 * \param[in] msb modo de opera&ccedil;&atilde;o do canal (d&iacute;gito b)
 * \param[in] msa modo de opera&ccedil;&atilde;o do canal (d&iacute;gito a) 
 * \param[in] elsb borda/n&iacute;vel (d&iacute;gito b)
 * \param[in] elsa borda/n&iacute;vel (d&iacute;gito a)
 */
uint8_t reconfigModoTPM1Cn(uint8_t canal, uint8_t c_modo) {
	
	switch(canal) {
	case 0:
		tpm_cnsc = &TPM_CnSC_REG(TPM1_BASE_PTR,0);
		break;
	case 1:
		tpm_cnsc = &TPM_CnSC_REG(TPM1_BASE_PTR,1);
		break;
	default:
		return 0;
	}
	
	/*!
	 * Desabilita o canal
	 */
	*tpm_cnsc &= ~(TPM_CnSC_MSB_MASK |      ///< MSB=TPM2_C0SC[5]=0
				TPM_CnSC_MSA_MASK |         ///< MSA=TPM2_C0SC[4]=0
				TPM_CnSC_ELSB_MASK |        ///< ELSB=TPM2_C0SC[3]=0
				TPM_CnSC_ELSA_MASK );       ///< ELSA=TPM2_C0SC[2]=0

	/*! Sele&ccedil;&atilde;o de novo modo do (Se&ccedil;&atilde;o 31.3.4 em 
	 * ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf 
	 */
	/*!
	 * Conforme
	 * ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf#page=555
	 * "When switching from one channel mode to a different channel mode, 
	 * the channel must first be disabled and this must be acknowledged in 
	 * the LPTPM counter clock domain."
	 */
	*tpm_cnsc |= (c_modo << 2);

	return 1;
}

/*!
 * \brief Configure o canal n do módulo TPM2. 
 * \param[in] canal canal do m&oacute;dulo
 * \param[in] msb modo de opera&ccedil;&atilde;o do canal (d&iacute;gito b)
 * \param[in] msa modo de opera&ccedil;&atilde;o do canal (d&iacute;gito a) 
 * \param[in] elsb borda/n&iacute;vel (d&iacute;gito b)
 * \param[in] elsa borda/n&iacute;vel (d&iacute;gito a)
 */
uint8_t reconfigModoTPM2Cn(uint8_t canal, uint8_t c_modo) {
	
	switch(canal) {
	case 0:
		tpm_cnsc = &TPM_CnSC_REG(TPM2_BASE_PTR,0);
		break;
	case 1:
		tpm_cnsc = &TPM_CnSC_REG(TPM2_BASE_PTR,1);
		break;
	default:
		return 0;
	}
	
	/*!
	 * Desabilita o canal
	 */
	*tpm_cnsc &= ~(TPM_CnSC_MSB_MASK |      ///< MSB=TPM2_C0SC[5]=0
				TPM_CnSC_MSA_MASK |         ///< MSA=TPM2_C0SC[4]=0
				TPM_CnSC_ELSB_MASK |        ///< ELSB=TPM2_C0SC[3]=0
				TPM_CnSC_ELSA_MASK );       ///< ELSA=TPM2_C0SC[2]=0

	/*!
	 * Conforme
	 * ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf#page=555
	 * "When switching from one channel mode to a different channel mode, 
	 * the channel must first be disabled and this must be acknowledged in 
	 * the LPTPM counter clock domain."
	 */
	*tpm_cnsc |= (c_modo << 2);

	return 1;
}

/*!
 * \brief Configure o valor do canal n do m&oacute;dulo TPM0. 
 * \param[in] canal canal do m&oacute;dulo
 * \param[in] valor novo valor 
 */
uint8_t reconfigValorTPM0Cn(uint8_t canal, unsigned short valor) {
	
	switch(canal) {
	case 0:
		tpm_cnv = &TPM_CnV_REG(TPM0_BASE_PTR,0);
		break;
	case 1:
		tpm_cnv = &TPM_CnV_REG(TPM0_BASE_PTR,1);
		break;
	case 2:
		tpm_cnv = &TPM_CnV_REG(TPM0_BASE_PTR,2);
		break;
	case 3:
		tpm_cnv = &TPM_CnV_REG(TPM0_BASE_PTR,3);
		break;
	case 4:
		tpm_cnv = &TPM_CnV_REG(TPM0_BASE_PTR,4);
		break;
	case 5:
		tpm_cnv = &TPM_CnV_REG(TPM0_BASE_PTR,5);
		break;
	default:
		return 0;
	}
	
	TPM0_SC |= 	TPM_SC_CMOD(0x0);           ///< desabilita clock LPTPM (Se&ccedil;&atilde;o 31.4.8.2 em ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf)

	/*!
	 * Configura&ccedil;&atilde;o do canal 0 (led B): CPWMS(0), MSnB:MSnA(10), ELSnB:ELSnA(11) - edge-aligned PWM
	 */
	*tpm_cnv = TPM_CnV_VAL(valor);  ///< VAL=valor

	TPM0_SC |= 	TPM_SC_CMOD(0x1);           ///< habilita clock LPTPM contagem crescente
	
	return 1;
}

/*!
 * \brief Configure o novo valor do canal n do módulo TPM1. 
 * \param[in] canal canal do m&oacute;dulo
 * \param[in] valor novo valor do canal 
 */
uint8_t reconfigValorTPM1Cn(uint8_t canal, unsigned short valor) {
	
	switch(canal) {
	case 0:
		tpm_cnv = &TPM_CnV_REG(TPM1_BASE_PTR,0);
		break;
	case 1:
		tpm_cnv = &TPM_CnV_REG(TPM1_BASE_PTR,1);
		break;
	default:
		return 0;
	}
		
	TPM1_SC |= 	TPM_SC_CMOD(0x0);           ///< desabilita clock LPTPM (Se&ccedil;&atilde;o 31.4.8.2 em ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf)

	/*!
	 * Configura&ccedil;&atilde;o do canal 0 (led B): CPWMS(0), MSnB:MSnA(10), ELSnB:ELSnA(11) - edge-aligned PWM
	 */
	*tpm_cnv = TPM_CnV_VAL(valor);  ///< VAL=valor

	TPM1_SC |= 	TPM_SC_CMOD(0x1);           ///< habilita clock LPTPM em contagem crescente 

	return 1;
}

/*!
 * \brief Configure o valor do canal n do módulo TPM2. 
 * \param[in] canal canal do m&oacute;dulo
 * \param[in] valor novo valor do canal
 */
uint8_t reconfigValorTPM2Cn(uint8_t canal, unsigned short valor) {
	
	switch(canal) {
	case 0:
		tpm_cnv = &TPM_CnV_REG(TPM2_BASE_PTR,0);
		break;
	case 1:
		tpm_cnv = &TPM_CnV_REG(TPM2_BASE_PTR,1);
		break;
	default:
		return 0;
	}
	
	TPM2_SC |= 	TPM_SC_CMOD(0x0);           ///< desabilita clock LPTPM (Se&ccedil;&atilde;o 31.4.8.2 em ftp://ftp.dca.fee.unicamp.br/pub/docs/ea871/ARM/KL25P80M48SF0RM.pdf)

	/*!
	 * Configura&ccedil;&atilde;o do canal 0 (led B): CPWMS(0), MSnB:MSnA(10), ELSnB:ELSnA(11) - edge-aligned PWM
	 */
	*tpm_cnv = TPM_CnV_VAL(valor);  ///< VAL=valor

	TPM2_SC |= 	TPM_SC_CMOD(0x1);           ///< habilita clock LPTPM em contagem crescente 

	return 1;
}

/**
 * @brief Habilita a interrup&ccedil;&atilde;o do contador de um m&oacute;lo
 */
uint8_t enableTPMInterrup(uint8_t modulo) {
	switch (modulo) {
	case 0:
		tpm_sc = &TPM_SC_REG(TPM0_BASE_PTR);	
		break;
	case 1:
		tpm_sc = &TPM_SC_REG(TPM1_BASE_PTR);	
		break;
	case 2:
		tpm_sc = &TPM_SC_REG(TPM2_BASE_PTR);	
		break;
	default:
		return 0;
	}
	*tpm_sc |= TPM_SC_TOIE_MASK;         
	
	return 1;
}

/**
 * @brief Desabilita a interrup&ccedil;&atilde;o do contador de um m&oacute;lo
 */
uint8_t disableTPMInterrup(uint8_t modulo) {
	switch (modulo) {
	case 0:
		tpm_sc = &TPM_SC_REG(TPM0_BASE_PTR);	
		break;
	case 1:
		tpm_sc = &TPM_SC_REG(TPM1_BASE_PTR);	
		break;
	case 2:
		tpm_sc = &TPM_SC_REG(TPM2_BASE_PTR);	
		break;

	default:
		return 0;
	}
	*tpm_sc &= ~TPM_SC_TOIE_MASK;         
	
	return 1;
}

/**
 * @brief Habilita a interrup&ccedil;&atilde;o do canal n do m&oacute;lo TPM0
 */
uint8_t enableTPM0CnInterrup(uint8_t canal) {
	switch(canal) {
	case 0:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,0);
		break;
	case 1:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,1);
		break;
	case 2:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,2);
		break;
	case 3:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,3);
		break;
	case 4:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,4);
		break;
	case 5:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,5);
		break;
	default:
		return 0;
	}
	*tpm_cnsc |= TPM_CnSC_CHIE_MASK;       
	
	return 1;
}

/**
 * @brief Desabilita a interrup&ccedil;&atilde;o do canal do m&oacute;lo TPM0
 */
uint8_t disableTPM0CnInterrup(uint8_t canal) {
	switch(canal) {
	case 0:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,0);
		break;
	case 1:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,1);
		break;
	case 2:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,2);
		break;
	case 3:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,3);
		break;
	case 4:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,4);
		break;
	case 5:
		tpm_cnsc = &TPM_CnSC_REG(TPM0_BASE_PTR,5);
		break;
	default:
		return 0;
	}
	*tpm_cnsc &= ~TPM_CnSC_CHIE_MASK;         
	
	return 1;
}

/**
 * @brief Habilita a interrup&ccedil;&atilde;o do canal n do m&oacute;lo TPM1
 */
uint8_t enableTPM1CnInterrup(uint8_t canal) {
	switch(canal) {
	case 0:
		tpm_cnsc = &TPM_CnSC_REG(TPM1_BASE_PTR,0);
		break;
	case 1:
		tpm_cnsc = &TPM_CnSC_REG(TPM1_BASE_PTR,1);
		break;
	default:
		return 0;
	}
	*tpm_cnsc |= TPM_CnSC_CHIE_MASK;       
	
	return 1;
}

/**
 * @brief Desabilita a interrup&ccedil;&atilde;o do canal do m&oacute;lo TPM1
 */
uint8_t disableTPM1CnInterrup(uint8_t canal) {
	switch(canal) {
	case 0:
		tpm_cnsc = &TPM_CnSC_REG(TPM1_BASE_PTR,0);
		break;
	case 1:
		tpm_cnsc = &TPM_CnSC_REG(TPM1_BASE_PTR,1);
		break;
	default:
		return 0;
	}
	*tpm_cnsc &= ~TPM_CnSC_CHIE_MASK;         
	
	return 1;
}

/**
 * @brief Habilita a interrup&ccedil;&atilde;o do canal n do m&oacute;lo TPM2
 */
uint8_t enableTPM2CnInterrup(uint8_t canal) {
	switch(canal) {
	case 0:
		tpm_cnsc = &TPM_CnSC_REG(TPM2_BASE_PTR,0);
		break;
	case 1:
		tpm_cnsc = &TPM_CnSC_REG(TPM2_BASE_PTR,1);
		break;
	default:
		return 0;
	}
	*tpm_cnsc |= TPM_CnSC_CHIE_MASK;       
	
	return 1;
}

/**
 * @brief Desabilita a interrup&ccedil;&atilde;o do canal do m&oacute;lo TPM2
 */
uint8_t disableTPM2CnInterrup(uint8_t canal) {
	switch(canal) {
	case 0:
		tpm_cnsc = &TPM_CnSC_REG(TPM2_BASE_PTR,0);
		break;
	case 1:
		tpm_cnsc = &TPM_CnSC_REG(TPM2_BASE_PTR,1);
		break;
	default:
		return 0;
	}
	*tpm_cnsc &= ~TPM_CnSC_CHIE_MASK;         
	
	return 1;
}

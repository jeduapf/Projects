/*!
 * @file atrasos.c
 * @brief Gerencia os atrasos por parametro de espera
 * @date 19/09/2017
 * @author Ting and Jose Eduardo Alves Peraira Filho and Joao Prado
 */

/*!
 * Gera um atraso correspondente a i iteraoes
 * @param i numero de iteraoes
 */
void delay( unsigned int i) 
{
	while (i) i--;        /*! Decrementa o numero de iteraoes ata zero */
}

/*!
 * Gera um atraso correspondente a t*10us
 * @param t multiplo de 10us
 */
void delay10us(unsigned int t)
{
	asm (
		"mov r1, %0 \n\t"		/*Carrega em R1 o valor do argumento  (1 ciclo)*/
		"carregaR2: \n\t"	
		"movs r2, #69 \n\t"		/*Carrega em R2 o valor 69, que gera um delay de 10us (1 ciclo) */
		"iteracao: \n\t"
		"sub r2,#1 \n\t"		/*Decrementa valor de R2, para execução de 69 iterações (1 ciclo) */
		"bne iteracao \n\t"		/*Teste de retorno do decremento de R2 (2 ciclos) */
		"sub r1,#1 \n\t"		/*Decremente o valor de R1, é uma multiplicação para o entre o argumento t e 10us (1 ciclo) */
		"bne carregaR2 \n\t"		/*Teste de retorno do decremento de R2 (2 ciclos) */
		 :
		 :"r" (t)
	);
}	


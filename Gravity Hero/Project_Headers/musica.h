/*
 * musica.h
 *
 *  Created on: Nov 4, 2017
 *      Author: JoaoPrado and Jose Eduardo Alves Peraira Filho
 */

#ifndef MUSICA_H_
#define MUSICA_H_

#define TEMP0 0.25
#define TEMP1 0.50
#define TEMP2 1.00
#define TEMP3 2.00
/*!
 * 2 oitava
 */
#define Do2		65.406
#define Re2		73.416
#define Mi2		82.407
#define Fa2		87.307
#define Sol2	97.999
#define La2		110.0
#define Si2		123.47
/*!
 * 3 oitava
 */
#define Do3		130.813
#define Re3		146.832
#define Mi3		164.814
#define Fa3		174.614
#define Sol3	195.997
#define La3		220.000
#define Si3		246.941
/*!
 * 4 oitava
 */
#define Do		261.63
#define Re		293.66
#define Mi		329.63
#define Fa		349.23
#define Sol		391.99
#define La		440.0
#define Si		493.88
/*!
 * 5 oitava
 */
#define Do5		523.25
#define Re5		587.33
#define Mi5		659.26
#define Fa5		698.46
#define Sol5	783.99
#define La5		880.0
#define Si5		987.77


//void decideSOM(float,float,float,float,float);
void PlaySOM(float, float);
int periodo(int);
void PlayMUSICA(int musica);

#endif /* MUSICA_H_ */

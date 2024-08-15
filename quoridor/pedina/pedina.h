/* **--------------File Info---------------------------------------------------------------------------------
** File name:               pedina.h
** Descriptions:            funzioni per gestione pedina
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Peppe
** Created date:            2023-12-27
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
*/

#ifndef _PEDINA_H_
#define _PEDINA_H_

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"

void spostaPedinaDown(int posRiga, int posColonna);
void spostaPedinaUp(int posRiga, int posColonna);
void spostaPedinaLeft(int posRiga, int posColonna);
void spostaPedinaRight(int posRiga, int posColonna);	
void confermaPosizionePedina(int posRiga, int posColonna);
void coloraPiccolo(int colore, int riga, int colonna, int padding,int Xin,int Yin,int lar,int alt);
void spostaPedinaUpRight(int posRiga, int posColonna);
void spostaPedinaUpLeft(int posRiga, int posColonna);
void spostaPedinaDownLeft(int posRiga, int posColonna);
void spostaPedinaDownRight(int posRiga, int posColonna);
void rimuoviPiccolo(int riga, int colonna);

#endif

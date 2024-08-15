/* **--------------File Info---------------------------------------------------------------------------------
** File name:               scacchiera.h
** Descriptions:            funzioni per creare scacchiera
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Peppe
** Created date:            2023-12-20
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
*/

#ifndef _SCACCHIERA_H_
#define _SCACCHIERA_H_

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "../GLCD/GLCD.h"


void disegna_scacchiera(int X, int Y, int padding,int Xin,int Yin,int dimX,int dimY);
void disegna_riquadri(int Xpos, int Ypos, int dimX, int dimY, int colore);
void posizione(int colore, int Xpos, int Ypos, int padding,int Xin,int Yin,int dimX,int dimY);
int convertiInPixel(int cordinata, int pixelIn, int padding, int dim);
void evidenzia_Human(int colore);
void evidenzia_NPC(int colore);

#endif

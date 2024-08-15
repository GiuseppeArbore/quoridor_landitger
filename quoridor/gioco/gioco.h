/* **--------------File Info---------------------------------------------------------------------------------
** File name:               gioco.h
** Descriptions:            funzioni per il gioco
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Peppe
** Created date:            2023-12-20
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
*/

#ifndef _GIOCO_H_
#define _GIOCO_H_

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"

typedef struct {
	int player;
	int stato; //0 se non ha ancora cliccato int0, 1 se ha cliccato int0
} stato_t;


void prepara_gioco(void);
void init_scacchiera(void);		
void init_stato_scheda(void);
void invia_messaggio(unsigned int move);
int start(int t);
int due_schede(void);
void cambiaTurno(int t);
void cambiaTurno2schede(int PlayerID, int PlayerMorW, int VorO, int Y, int X);
void selectPlayer(int X, int Y, int padding,int Xin,int Yin,int dimX,int dimY);
void evidenzia(int RigaP, int ColonnaP, int dimX, int dimY);
void controlloAdiacenti(int RigaP, int ColonnaP);
void coloraIndicatore(int colore, int riga, int colonna, int padding,int Xin,int Yin,int lar,int alt);

void colora(int colore, int riga, int colonna, int padding,int Xin,int Yin,int lar,int alt); //lar: larghezzaRiquadro, alt=altezza riquadro


void rimuoviColore(int posRiga, int posColonna);
void selectGameMode(void);
void selectOppositePlayerSingle(void);
void selectOppositePlayerTwo(void);

void DelayUS(uint32_t cnt);

//void restart(void);
#endif

/* **--------------File Info---------------------------------------------------------------------------------
** File name:               AI.h
** Descriptions:            funzioni per il gioco automatico
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Peppe
** Created date:            2024-01-12
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
*/

#ifndef _AI_H_
#define _AI_H_

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"

void ArtificialIntelligence(int io);
int selectMax(int u, int d, int l, int r);
int verificaPercorsoUp(int posRiga, int posColonna, int fine);
int verificaPercorsoLeft(int posRiga, int posColonna, int fine);
int verificaPercorsoRight(int posRiga, int posColonna, int fine);
int verificaPercorsoDown(int posRiga, int posColonna, int fine);
void azzeraVisitato(void);
int provaMuroSopra(void);
int provaMuroSotto(void);
int provaMuroDestra(void);
int provaMuroSinistra(void);
int ritornaMosseAV(int ms);
int ritornaMosseAI(int ms);


//void restart(void);
#endif

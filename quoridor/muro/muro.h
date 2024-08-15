/* **--------------File Info---------------------------------------------------------------------------------
** File name:               muro.h
** Descriptions:            funzioni per gestione muri
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Peppe
** Created date:            2023-12-27
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
*/

#ifndef _MURO_H_
#define _MURO_H_

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
typedef struct {
	int riga;
	int colonna;
} pos_t;


typedef struct {
	pos_t centro;
	int direzione;
} posMuro_t;

void spostaMuroUp(int posRiga, int posColonna, int direzione);
void spostaMuroDown(int posRiga, int posColonna, int direzione);
void spostaMuroLeft(int posRiga, int posColonna, int direzione);
void spostaMuroRight(int posRiga, int posColonna, int direzione);
void rimuovi_muro(int posRiga, int posColonna, int direzione);
void disegna_muro(int posRiga, int posColonna, int direzione);
void ruotaMuro(int posRiga, int posColonna, int direzione);
void confermaPosizioneMuro(int posRiga, int posColonna, int direzione);
void disegna_provvisorio(int posRiga, int posColonna, int direzione);
void disegna_provvisorioNotValid(int posRiga, int posColonna, int direzione);
int verificaMuroUp(int posRiga, int posColonna);
int verificaMuroDown(int posRiga, int posColonna);
int verificaMuroLeft(int posRiga, int posColonna);
int verificaMuroRight(int posRiga, int posColonna);
int verificaValido(int posRiga, int posColonna, int fine);
void confermaPosizioneMuroPerAI(int posRiga, int posColonna, int direzione);

int controllaPercorso(int posRiga,int posColonna,int direzione);
#endif

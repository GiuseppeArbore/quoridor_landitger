/* **--------------File Info---------------------------------------------------------------------------------
** File name:               pedina.c
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


/* Includes ------------------------------------------------------------------*/
#include "../GLCD/GLCD.h"
#include "../gioco/gioco.h"
#include "../scacchiera/scacchiera.h"
#include "../timer/timer.h"
#include "../muro/muro.h"
#include "pedina.h"
extern unsigned int function1(int playerId,int pMorW, int VorH, int Y, int X);


extern int turno;
extern int spostato;
extern pos_t posPlayer;
extern pos_t posPlayerIn;
extern pos_t posFermo;
extern pos_t P1pos;
extern pos_t P2pos;
extern int mossa;
extern int attivo;

void spostaPedinaDown(int posRiga, int posColonna){
		if(posRiga!=6 && ( (posRiga+1!=posFermo.riga) || posColonna!=posFermo.colonna) && verificaMuroDown(posRiga, posColonna)){ //&controllo
			if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
			}
			if(turno==1){
				coloraPiccolo(White, posRiga+1, posColonna, 4, 2, 2, 30, 30);
			}
			else{
				coloraPiccolo(Red,  posRiga+1, posColonna, 4, 2, 2, 30, 30);
			}
			mossa++;
			colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
			posPlayer.colonna=posColonna;
			posPlayer.riga=posRiga+1;
		}
				
		
		if(posRiga<5 && ( (posRiga+1==posFermo.riga) && posColonna==posFermo.colonna) && verificaMuroDown(posRiga, posColonna) && verificaMuroDown(posRiga+1, posColonna) ){ //&controllo
			if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
			}
			if(turno==1){
					coloraPiccolo(White, posRiga+2, posColonna, 4, 2, 2, 30, 30);
			}
			else{
					coloraPiccolo(Red,  posRiga+2, posColonna, 4, 2, 2, 30, 30);
			}
			mossa++;
			colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
			posPlayer.colonna=posColonna;
			posPlayer.riga=posRiga+2;
	}
}
void spostaPedinaRight(int posRiga, int posColonna){
	if(posColonna!=6 && ( (posColonna+1!=posFermo.colonna) || posRiga!=posFermo.riga) && verificaMuroRight(posRiga, posColonna)){//&controllo muri e se c'è pedina, salta
			if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
			}
			if(turno==1){
				coloraPiccolo(White, posRiga, posColonna+1, 4, 2, 2, 30, 30);
			}
			else{
				coloraPiccolo(Red,  posRiga, posColonna+1, 4, 2, 2, 30, 30);
			}
			mossa++;
			colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
			posPlayer.riga=posRiga;
			posPlayer.colonna=posColonna+1;
		}
		if(posColonna<5 && ( (posColonna+1==posFermo.colonna) && posRiga==posFermo.riga) && verificaMuroRight(posRiga, posColonna) && verificaMuroRight(posRiga, posColonna+1)){//&controllo muri e se c'è pedina, salta
			if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
			}
			if(turno==1){
				coloraPiccolo(White, posRiga, posColonna+2, 4, 2, 2, 30, 30);
			}
			else{
				coloraPiccolo(Red,  posRiga, posColonna+2, 4, 2, 2, 30, 30);
			}
			mossa++;
			colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
			posPlayer.riga=posRiga;
			posPlayer.colonna=posColonna+2;
		}
		
}
void spostaPedinaLeft(int posRiga, int posColonna){
	if(posColonna!=0 && ( (posColonna-1!=posFermo.colonna) || posRiga!=posFermo.riga) && verificaMuroLeft(posRiga, posColonna)){//&controllo muri
		if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
		}
		if(turno==1){
				coloraPiccolo(White, posRiga, posColonna-1, 4, 2, 2, 30, 30);
			}
			else{
				coloraPiccolo(Red,  posRiga, posColonna-1, 4, 2, 2, 30, 30);
			}
			mossa++;
			colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
			posPlayer.riga=posRiga;
			posPlayer.colonna=posColonna-1;
	} 
	if(posColonna>1 && ( (posColonna-1==posFermo.colonna) && posRiga==posFermo.riga) && verificaMuroLeft(posRiga, posColonna) && verificaMuroLeft(posRiga, posColonna-1)){//&controllo muri
			if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
			}
			if(turno==1){
				coloraPiccolo(White, posRiga, posColonna-2, 4, 2, 2, 30, 30);
			}
			else{
				coloraPiccolo(Red,  posRiga, posColonna-2, 4, 2, 2, 30, 30);
			}
			mossa++;
			colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
			posPlayer.riga=posRiga;
			posPlayer.colonna=posColonna-2;
	} 

}
void spostaPedinaUp(int posRiga, int posColonna){
	if(posRiga!=0 && ( (posRiga-1!=posFermo.riga) || posColonna!=posFermo.colonna) && verificaMuroUp(posRiga, posColonna)){ //&controllo
		if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
		}
		if(turno==1){
				coloraPiccolo(White, posRiga-1, posColonna, 4, 2, 2, 30, 30);
		}
		else{
				coloraPiccolo(Red,  posRiga-1, posColonna, 4, 2, 2, 30, 30);
		}
		mossa++;
		colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
		posPlayer.colonna=posColonna;
		posPlayer.riga=posRiga-1;
	}
	if(posRiga>1 && ( (posRiga-1==posFermo.riga) && posColonna==posFermo.colonna) && verificaMuroUp(posRiga, posColonna) && verificaMuroUp(posRiga-1, posColonna)){ //&controllo
		if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
		}
		if(turno==1){
				coloraPiccolo(White, posRiga-2, posColonna, 4, 2, 2, 30, 30);
		}
		else{
				coloraPiccolo(Red,  posRiga-2, posColonna, 4, 2, 2, 30, 30);
		}
		mossa++;
		colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
		posPlayer.colonna=posColonna;
		posPlayer.riga=posRiga-2;
	}

}	
void confermaPosizionePedina(int posRiga, int posColonna){ //posPlayer
	unsigned int vittoria;
	rimuoviColore(posPlayerIn.riga, posPlayerIn.colonna);
		if(turno==1){	
			P1pos.riga=posRiga;
			P1pos.colonna=posColonna;
			colora(White, P1pos.riga, P1pos.colonna, 4, 2, 2, 30, 30);
			spostato=1;
			
			if(P1pos.riga==6){
				disable_timer(0);
				GUI_Text(16, 242, (uint8_t *) "  The winner is Player 1  ", Black , White);
				attivo=0;
				vittoria=function1(turno-1,0,0,6, P1pos.colonna);
				invia_messaggio(vittoria);
			}
			else{			
				cambiaTurno(turno);
			}
				
		}
		else{			
			P2pos.riga=posRiga;
			P2pos.colonna=posColonna;
			colora(Red, P2pos.riga, P2pos.colonna, 4, 2, 2, 30, 30);
			spostato=1;
			
			if(P2pos.riga==0){
				disable_timer(0);
				GUI_Text(16, 242, (uint8_t *) "  The winner is Player 2  ", Black , Red);
				attivo=0;
				vittoria=function1(turno-1,0,0,0, P2pos.colonna);
				invia_messaggio(vittoria);	
			}else{
				cambiaTurno(turno);
			}
		}
		
}


void coloraPiccolo(int colore, int riga, int colonna, int padding,int Xin,int Yin,int lar,int alt){
	int i;
	colonna=convertiInPixel(colonna, Xin, padding, lar);
	riga=convertiInPixel(riga, Yin, padding, alt);
	riga++;
	for(i=10; i<15; i++){
		disegna_riquadri(colonna+i, riga+i,lar-(2*i), alt-(2*i), colore);
	}
}

void rimuoviPiccolo(int riga, int colonna){
	coloraPiccolo(sfondo, riga, colonna, 4, 2, 2, 30, 30);
}

void spostaPedinaUpRight(int posRiga, int posColonna){
	if(posRiga>0 && ( (posRiga-1==posFermo.riga) && posColonna==posFermo.colonna) && verificaMuroUp(posRiga, posColonna) && (!verificaMuroUp(posRiga-1, posColonna) || posRiga-1==0) ){
		if(posColonna<7 && verificaMuroRight(posFermo.riga, posFermo.colonna)){
			if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
			}
			if(turno==1){
					coloraPiccolo(White, posRiga-1, posColonna+1, 4, 2, 2, 30, 30);
			}
			else{
					coloraPiccolo(Red,  posRiga-1, posColonna+1, 4, 2, 2, 30, 30);
			}
			mossa++;
			colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
			posPlayer.colonna=posColonna+1;
			posPlayer.riga=posRiga-1;
		}	
		
	}else if(posColonna<6 && ( (posColonna+1==posFermo.colonna) && posRiga==posFermo.riga) && verificaMuroRight(posRiga, posColonna) && (!verificaMuroRight(posRiga, posColonna+1) || posColonna+1==6)){
		if(posRiga>0 && verificaMuroUp(posFermo.riga, posFermo.colonna)){
			if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
			}
			if(turno==1){
					coloraPiccolo(White, posRiga-1, posColonna+1, 4, 2, 2, 30, 30);
			}
			else{
					coloraPiccolo(Red,  posRiga-1, posColonna+1, 4, 2, 2, 30, 30);
			}
			mossa++;
			colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
			posPlayer.colonna=posColonna+1;
			posPlayer.riga=posRiga-1;
		}	
		
	}
	
}

void spostaPedinaUpLeft(int posRiga, int posColonna){
	if(posRiga>0 && ( (posRiga-1==posFermo.riga) && posColonna==posFermo.colonna) && verificaMuroUp(posRiga, posColonna) && (!verificaMuroUp(posRiga-1, posColonna) || posRiga-1==0)){
		if(posColonna>0 && verificaMuroLeft(posFermo.riga, posFermo.colonna)){
			if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
			}
			if(turno==1){
					coloraPiccolo(White, posRiga-1, posColonna-1, 4, 2, 2, 30, 30);
			}
			else{
					coloraPiccolo(Red,  posRiga-1, posColonna-1, 4, 2, 2, 30, 30);
			}
			mossa++;
			colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
			posPlayer.colonna=posColonna-1;
			posPlayer.riga=posRiga-1;
		}	
		
	}else if(posColonna>0 && ( (posColonna-1==posFermo.colonna) && posRiga==posFermo.riga) && verificaMuroLeft(posRiga, posColonna) && (!verificaMuroLeft(posRiga, posColonna-1) || posColonna-1==0)){
		if(posRiga>0 && verificaMuroUp(posFermo.riga, posFermo.colonna)){
			if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
			}
			if(turno==1){
					coloraPiccolo(White, posRiga-1, posColonna-1, 4, 2, 2, 30, 30);
			}
			else{
					coloraPiccolo(Red,  posRiga-1, posColonna-1, 4, 2, 2, 30, 30);
			}
			mossa++;
			colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
			posPlayer.colonna=posColonna-1;
			posPlayer.riga=posRiga-1;
		}	
		
	}
	
}

void spostaPedinaDownRight(int posRiga, int posColonna){
	if(posRiga<6 && ( (posRiga+1==posFermo.riga) && posColonna==posFermo.colonna) && verificaMuroDown(posRiga, posColonna)  && (!verificaMuroDown(posRiga+1, posColonna) || posRiga+1==6)){
		if(posColonna<7 && verificaMuroRight(posFermo.riga, posFermo.colonna)){
			if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
			}
			if(turno==1){
					coloraPiccolo(White, posRiga+1, posColonna+1, 4, 2, 2, 30, 30);
			}
			else{
					coloraPiccolo(Red,  posRiga+1, posColonna+1, 4, 2, 2, 30, 30);
			}
			mossa++;
			colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
			posPlayer.colonna=posColonna+1;
			posPlayer.riga=posRiga+1;
		}	
		
	}else if(posColonna<6 && ( (posColonna+1==posFermo.colonna) && posRiga==posFermo.riga) && verificaMuroRight(posRiga, posColonna) && (!verificaMuroRight(posRiga, posColonna+1) || posColonna+1==6)){
		if(posRiga<7 && verificaMuroDown(posFermo.riga, posFermo.colonna)){
			if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
			}
			if(turno==1){
					coloraPiccolo(White, posRiga+1, posColonna+1, 4, 2, 2, 30, 30);
			}
			else{
					coloraPiccolo(Red,  posRiga+1, posColonna+1, 4, 2, 2, 30, 30);
			}
			mossa++;
			colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
			posPlayer.colonna=posColonna+1;
			posPlayer.riga=posRiga+1;
		}	
		
	}
	
}

void spostaPedinaDownLeft(int posRiga, int posColonna){
	if(posRiga<6 && ( (posRiga+1==posFermo.riga) && posColonna==posFermo.colonna) && verificaMuroDown(posRiga, posColonna)  && (!verificaMuroDown(posRiga+1, posColonna) || posRiga+1==6)){
		if(posColonna>0 && verificaMuroLeft(posFermo.riga, posFermo.colonna)){
			if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
			}
			if(turno==1){
					coloraPiccolo(White, posRiga+1, posColonna-1, 4, 2, 2, 30, 30);
			}
			else{
					coloraPiccolo(Red,  posRiga+1, posColonna-1, 4, 2, 2, 30, 30);
			}
			mossa++;
			colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
			posPlayer.colonna=posColonna-1;
			posPlayer.riga=posRiga+1;
		}	
		
	}else if(posColonna>0 && ( (posColonna-1==posFermo.colonna) && posRiga==posFermo.riga) && verificaMuroLeft(posRiga, posColonna) && (!verificaMuroLeft(posRiga, posColonna-1) || posColonna-1==0)){
		if(posRiga<7 && verificaMuroDown(posFermo.riga, posFermo.colonna)){
			if(mossa>0){
					rimuoviPiccolo(posPlayer.riga, posPlayer.colonna);
			}
			if(turno==1){
					coloraPiccolo(White, posRiga+1, posColonna-1, 4, 2, 2, 30, 30);
			}
			else{
					coloraPiccolo(Red,  posRiga+1, posColonna-1, 4, 2, 2, 30, 30);
			}
			mossa++;
			colora(sfondo, posRiga, posColonna, 4, 2, 2, 30, 30);
			posPlayer.colonna=posColonna-1;
			posPlayer.riga=posRiga+1;
		}	
		
	}
	
}

/* **--------------File Info---------------------------------------------------------------------------------
** File name:               AI.c
** Descriptions:            funzioni per intelligenza
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Peppe
** Created date:            2024-01-12
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
*/


/* Includes ------------------------------------------------------------------*/
#include "AI.h"
#include "../GLCD/GLCD.h"
#include "../gioco/gioco.h"
#include "../scacchiera/scacchiera.h"
#include "../timer/timer.h"
#include "../pedina/pedina.h"
#include "../muro/muro.h"
#include "../RIT/RIT.h"
#define HUMAN 0
#define NPC 1
#define SingleBoard 0
#define TwoBoards 1
#define Sopra 1
#define Sotto -1
#define Destra 2
#define Sinistra -2
#define Horizontal 1
#define Vertical 0

int visitato[8][8];
int last;
int cntUp;
int cntDown;
int cntLeft;
int cntRight;
int lastAv;
int cntUpAv;
int cntDownAv;
int cntLeftAv;
int cntRightAv;
int scelta;

int miaFine;
int fineAvv;
extern int down1;
extern int down2;
extern int P1Wall;
extern int P2Wall;
extern int time;

extern int muroMosso;
extern int muroConferma;
extern int insMuro;
extern int gameMode;
extern int sceltaOpposite;
extern int muriFiniti;


extern int P1Wall;
extern int P2Wall;
extern pos_t posPlayer;
extern pos_t posPlayerIn;
extern pos_t posFermo;
extern pos_t P1pos;
extern pos_t P2pos;
extern posMuro_t posMuro;
extern int turno;
extern unsigned int function1(int playerId,int pMorW, int VorH, int Y, int X);


void ArtificialIntelligence(int io){
	int myw;
	int nrMosseIA=0;
	int nrMosseAv=0;
	int inseritoMuro=0;
	int capire;
	int myWall;
	cntUp=0;
	cntDown=0;
	cntLeft=0;
	cntRight=0;
	cntUpAv=0;
	cntDownAv=0;
	cntLeftAv=0;
	cntRightAv=0;
	azzeraVisitato();
	if(last==Sotto) cntUp=100;
	if(last==Sopra) cntDown=100;
	if(last==Destra) cntLeft=100;
	if(last==Sinistra)cntRight=100;

	if(io==1){
		miaFine=6;
		fineAvv=0;
		myWall=P1Wall;
	}else{
		miaFine=0;
		fineAvv=6;
		myWall=P2Wall;
	}
	myw=myWall;
	
	
	if(posPlayerIn.riga>0 && verificaMuroUp(posPlayerIn.riga, posPlayerIn.colonna)){
		capire=verificaPercorsoUp(posPlayerIn.riga, posPlayerIn.colonna, miaFine);
	}
	azzeraVisitato();
	if(posPlayerIn.colonna>0 && verificaMuroLeft(posPlayerIn.riga, posPlayerIn.colonna)){
		capire=verificaPercorsoLeft(posPlayerIn.riga, posPlayerIn.colonna, miaFine);
	}
	azzeraVisitato();
	if(posPlayerIn.colonna<6 && verificaMuroRight(posPlayerIn.riga, posPlayerIn.colonna)){
		capire=verificaPercorsoRight(posPlayerIn.riga, posPlayerIn.colonna, miaFine);
	}
	azzeraVisitato();
	if(posPlayerIn.riga<6 && verificaMuroDown(posPlayerIn.riga, posPlayerIn.colonna)){
		capire=verificaPercorsoDown(posPlayerIn.riga, posPlayerIn.colonna, miaFine);
	}
	last=selectMax(cntUp, cntDown, cntLeft, cntRight);
	nrMosseIA=ritornaMosseAI(last);
	
	
	//controllo su avversario
	if(myWall>0){
		azzeraVisitato();
		if(posFermo.riga>0 && verificaMuroUp(posFermo.riga, posFermo.colonna)){
			capire=verificaPercorsoUp(posFermo.riga, posFermo.colonna, fineAvv);
		}
		azzeraVisitato();
		if(posFermo.colonna>0 && verificaMuroLeft(posFermo.riga, posFermo.colonna)){
			capire=verificaPercorsoLeft(posFermo.riga, posFermo.colonna, fineAvv);
		}
		azzeraVisitato();
		if(posFermo.colonna<6 && verificaMuroRight(posFermo.riga, posFermo.colonna)){
			capire=verificaPercorsoRight(posFermo.riga, posFermo.colonna, fineAvv);
		}
		azzeraVisitato();
		if(posFermo.riga<6 && verificaMuroDown(posFermo.riga, posFermo.colonna)){
			capire=verificaPercorsoDown(posFermo.riga, posFermo.colonna, fineAvv);
		}
		lastAv=selectMax(cntUpAv, cntDownAv, cntLeftAv, cntRightAv);
		nrMosseAv=ritornaMosseAV(lastAv);
	}
	
	//controllo se meglio inserire muro o spostare pedina
	if( ((miaFine==0&&((nrMosseAv<nrMosseIA && posFermo.riga>4) || (nrMosseAv<nrMosseIA-1 && posFermo.riga<5)))
			||(miaFine==6 &&((nrMosseAv<nrMosseIA-1 && posFermo.riga>3) || (nrMosseAv<nrMosseIA && posFermo.riga<4)))) && myWall>0){
		switch(lastAv){ //provo ad inserire muro
			case Sopra: //muro sopra
				inseritoMuro=provaMuroSopra();
				break;
			case Sotto: //muro sotto
				inseritoMuro=provaMuroSotto();
				break;
			case Sinistra: //muro sx
				inseritoMuro=provaMuroSinistra();
				break;
			case Destra: //muro dx
				inseritoMuro=provaMuroDestra();
				break;
			default:
				break;
		}	
	}
	if(inseritoMuro==0){
		switch(last){//sposto pedina
			case Sopra: //spostare sopra
				spostaPedinaUp(posPlayerIn.riga, posPlayerIn.colonna);			
				last=Sopra;
				break;
			case Sotto: //spostare sotto
				spostaPedinaDown(posPlayerIn.riga, posPlayerIn.colonna);
				last=Sotto;
				break;
			case Sinistra: //spostare sx
				spostaPedinaLeft(posPlayerIn.riga, posPlayerIn.colonna);
				last=Sinistra;
				break;
			case Destra: //spostare dx
				spostaPedinaRight(posPlayerIn.riga, posPlayerIn.colonna);
				last=Destra;
				break;
			default:
				break;
		
		}
		confermaPosizionePedina(posPlayer.riga, posPlayer.colonna);
	}


}


int selectMax(int u, int d, int l, int r){
	if((u<=d || d<=0) && (u<=l||l<=0) && (u<=r||r<=0) &&u>0)
		return Sopra;
	if((r<=u || u<=0) && (r<=l||l<=0) && (r<=d||d<=0) && r>0)
		return Destra;
	if((l<=u || u<=0) && (l<=r || r<=0) && (l<=d||d<=0) && l>0)
		return Sinistra;
	return Sotto;
}

int ritornaMosseAI(int ms){
	if(ms==Sopra)
		return cntUp;
	if(ms==Sinistra)
		return cntLeft;
	if(ms==Destra)
		return cntRight;
	return cntDown;
}

int ritornaMosseAV(int ms){
	if(ms==Sopra)
		return cntUpAv;
	if(ms==Sinistra)
		return cntLeftAv;
	if(ms==Destra)
		return cntRightAv;
	return cntDownAv;
}

void azzeraVisitato(void){
	int i, j;
	for(i=0; i<8;i++)
		for(j=0; j<8;j++)
			visitato[i][j]=0;	
}


int verificaPercorsoUp(int posRiga, int posColonna, int fine){
	if(fine==fineAvv)
		cntUpAv++;
	else
		cntUp++;
	if(posRiga==fine)
		return 1;
	if(posRiga>=0 && posColonna<7 && posColonna>=0 && posRiga<7){
		visitato[posColonna][posRiga]=1;
	}else{
		return 0;
	}
	
	
	if(posRiga>0){
		if( visitato[posColonna][posRiga-1]==0 && verificaMuroUp(posRiga, posColonna)){
			if(verificaPercorsoUp(posRiga-1, posColonna, fine)){
				return 1;
			}
		}
	}
	if(posColonna>0){
		if(visitato[posColonna-1][posRiga]==0 && verificaMuroLeft(posRiga, posColonna)){
			if(verificaPercorsoUp(posRiga, posColonna-1, fine)){
				return 1;
			}else{ if(fine==6) cntUpAv--; else cntUp--;}
		}
	}
	if(posColonna<6 ){
		if(visitato[posColonna+1][posRiga]==0 && verificaMuroRight(posRiga, posColonna)){
			if(verificaPercorsoUp(posRiga, posColonna+1, fine)){
			return 1;
			}else{ if(fine==6) cntUpAv--; else cntUp--;}
		}
	}
	if(posRiga<6){
		if(visitato[posColonna][posRiga+1]==0 && verificaMuroDown(posRiga, posColonna)){
			if(verificaPercorsoUp(posRiga+1, posColonna, fine)){
				return 1;
			}else{ if(fine==6) cntUpAv--; else cntUp--;}

		}
	}

	return 0;
}

int verificaPercorsoDown(int posRiga, int posColonna, int fine){	
	if(fine==fineAvv) cntDownAv++; else cntDown++;
	if(posRiga==fine)
		return 1;

	if(posRiga>=0 && posColonna<7 && posColonna>=0 && posRiga<7){
		visitato[posColonna][posRiga]=1;
	}else{
		return 0;
	}
	
	if(posRiga<6){
		if(visitato[posColonna][posRiga+1]==0 && verificaMuroDown(posRiga, posColonna)){
			if(verificaPercorsoDown(posRiga+1, posColonna, fine)){
				return 1;
			}

		}
	}
	if(posColonna>0){
		if(visitato[posColonna-1][posRiga]==0 && verificaMuroLeft(posRiga, posColonna)){
			if(verificaPercorsoDown(posRiga, posColonna-1, fine)){
				return 1;
			}else{ if(fine==6) cntDownAv--; else cntDown--;}
		}
	}
	if(posRiga>0){
		if( visitato[posColonna][posRiga-1]==0 && verificaMuroUp(posRiga, posColonna)){
			if(verificaPercorsoDown(posRiga-1, posColonna, fine)){
				return 1;
			}else{ if(fine==6) cntDownAv--; else cntDown--;}
		}
	}
	if(posColonna<6 ){
		if(visitato[posColonna+1][posRiga]==0 && verificaMuroRight(posRiga, posColonna)){
			if(verificaPercorsoDown(posRiga, posColonna+1, fine)){
				return 1;
			}else{ if(fine==6) cntDownAv--; else cntDown--;}
		}
	}

	return 0;
}

int verificaPercorsoLeft(int posRiga, int posColonna, int fine){	
	if(fine==fineAvv) cntLeftAv++; else cntLeft++;
	if(posRiga==fine)
		return 1;
	if(posRiga>=0 && posColonna<7 && posColonna>=0 && posRiga<7){
		visitato[posColonna][posRiga]=1;
	}else{
		return 0;
	}
	
	
	if(posColonna>0){
		if(visitato[posColonna-1][posRiga]==0 && verificaMuroLeft(posRiga, posColonna)){
			if(verificaPercorsoLeft(posRiga, posColonna-1, fine)){
				return 1;
			}
		}
	}
	if(posRiga>0){
		if( visitato[posColonna][posRiga-1]==0 && verificaMuroUp(posRiga, posColonna)){
			if(verificaPercorsoLeft(posRiga-1, posColonna, fine)){
				return 1;
			}else{ if(fine==6) cntLeftAv--; else cntLeft--;}
		}
	}
	if(posRiga<6){
		if(visitato[posColonna][posRiga+1]==0 && verificaMuroDown(posRiga, posColonna)){
			if(verificaPercorsoLeft(posRiga+1, posColonna, fine)){
				return 1;
			}else{ if(fine==6) cntLeftAv--; else cntLeft--;}

		}
	}

	if(posColonna<6 ){
		if(visitato[posColonna+1][posRiga]==0 && verificaMuroRight(posRiga, posColonna)){
			if(verificaPercorsoLeft(posRiga, posColonna+1, fine)){
				return 1;
			}else{ if(fine==6) cntLeftAv--; else cntLeft--;}
		}
	}
	return 0;
}
	

int verificaPercorsoRight(int posRiga, int posColonna, int fine){	

	if(fine==fineAvv) cntRightAv++; else cntRight++;
	if(posRiga==fine)
		return 1;
	if(posRiga>=0 && posColonna<7 && posColonna>=0 && posRiga<7){
		visitato[posColonna][posRiga]=1;
	}else{
		return 0;
	}
	if(posColonna<6 ){
		if(visitato[posColonna+1][posRiga]==0 && verificaMuroRight(posRiga, posColonna)){
			if(verificaPercorsoRight(posRiga, posColonna+1, fine)){
				return 1;
			}
		}
	}
	
	
	if(posRiga>0){
		if( visitato[posColonna][posRiga-1]==0 && verificaMuroUp(posRiga, posColonna)){
			if(verificaPercorsoRight(posRiga-1, posColonna, fine)){
				return 1;
			}else{ if(fine==fineAvv) cntRightAv--; else cntRight--;}
		}
	}
	if(posRiga<6){
		if(visitato[posColonna][posRiga+1]==0 && verificaMuroDown(posRiga, posColonna)){
			if(verificaPercorsoRight(posRiga+1, posColonna, fine)){
				return 1;
			}else{ if(fine==fineAvv) cntRightAv--; else cntRight--;}

		}
	}
	if(posColonna>0){
		if(visitato[posColonna-1][posRiga]==0 && verificaMuroLeft(posRiga, posColonna)){
			if(verificaPercorsoRight(posRiga, posColonna-1, fine)){
				return 1;
			}else{ if(fine==fineAvv) cntRightAv--; else cntRight--;}
		}

	}
	return 0;
}

	
int provaMuroSopra(void){
	if(posFermo.riga==0)
		return 0;
	if(posFermo.colonna>0){
		if(controllaPercorso(posFermo.riga, posFermo.colonna, Horizontal)){
			insMuro=1;
			confermaPosizioneMuroPerAI(posFermo.riga, posFermo.colonna, Horizontal);
			return 1;
		}
	}
	if(posFermo.colonna<6){
		if(controllaPercorso(posFermo.riga, posFermo.colonna+1, Horizontal)){
			insMuro=1;
			confermaPosizioneMuroPerAI(posFermo.riga, posFermo.colonna+1, Horizontal);
			return 1;
		}
	}
	return 0;
	
}
int provaMuroSotto(){
	if(posFermo.riga==6)
		return 0;
	if(posFermo.colonna>0){
		if(controllaPercorso(posFermo.riga+1, posFermo.colonna, Horizontal)){
			insMuro=1;
			confermaPosizioneMuroPerAI(posFermo.riga+1, posFermo.colonna, Horizontal);
			return 1;
		}
	}
	if(posFermo.colonna<6){
		if(controllaPercorso(posFermo.riga+1, posFermo.colonna+1, Horizontal)){
			insMuro=1;
			confermaPosizioneMuroPerAI(posFermo.riga+1, posFermo.colonna+1, Horizontal);
			return 1;
		}
	}
	return 0;	
}
int provaMuroDestra(){
	if(posFermo.colonna==6)
		return 0;
	
	if(posFermo.riga>0){
		if(controllaPercorso(posFermo.riga, posFermo.colonna+1, Vertical)){
			insMuro=1;
			confermaPosizioneMuroPerAI(posFermo.riga, posFermo.colonna+1, Vertical);
			return 1;
		}
	}
	if(posFermo.riga<6){
		if(controllaPercorso(posFermo.riga+1, posFermo.colonna+1, Vertical)){
			insMuro=1;
			confermaPosizioneMuroPerAI(posFermo.riga+1, posFermo.colonna+1, Vertical);
			return 1;
		}
	}
	return 0;
	
}
int provaMuroSinistra(){
	if(posFermo.colonna==0)
		return 0;
	if(posFermo.riga>0){
		if(controllaPercorso(posFermo.riga, posFermo.colonna, Vertical)){
			insMuro=1;
			confermaPosizioneMuroPerAI(posFermo.riga, posFermo.colonna, Vertical);
			return 1;
		}
	}
	if(posFermo.riga<6){
		if(controllaPercorso(posFermo.riga+1, posFermo.colonna, Vertical)){
			insMuro=1;
			confermaPosizioneMuroPerAI(posFermo.riga+1, posFermo.colonna, Vertical);
			return 1;
		}
	}
	return 0;
	
}

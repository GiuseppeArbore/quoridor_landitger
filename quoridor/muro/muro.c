/* **--------------File Info---------------------------------------------------------------------------------
** File name:               muro.c
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
#include "../pedina/pedina.h"
#include "muro.h"
volatile int visitati[8][8];
int Vertical=0;
int Horizontal=1;
extern int P1Wall;
extern int P2Wall;
extern int muriDir[8][8];
extern posMuro_t posMuro;
extern pos_t posFermo;
extern pos_t posPlayer;
extern int turno;
int muroMosso=0;
int muroConferma=0;
extern int insMuro;

void spostaMuroUp(int posRiga, int posColonna, int direzione){
	if(posRiga>1){
		rimuovi_muro(posRiga, posColonna, direzione);
		posRiga--;
		posMuro.centro.riga=posRiga;
		if(controllaPercorso(posRiga, posColonna, posMuro.direzione))
			disegna_provvisorio(posRiga, posColonna, posMuro.direzione);
		else
			disegna_provvisorioNotValid(posRiga, posColonna, posMuro.direzione);
		muroMosso++;
	}

}
void spostaMuroDown(int posRiga, int posColonna, int direzione){
	if(posRiga<6){
		rimuovi_muro(posRiga, posColonna, direzione);
		posRiga++;
		posMuro.centro.riga=posRiga;
		if(controllaPercorso(posRiga, posColonna, posMuro.direzione))
			disegna_provvisorio(posRiga, posColonna, posMuro.direzione);
		else
			disegna_provvisorioNotValid(posRiga, posColonna, posMuro.direzione);
		muroMosso++;
	}
}
void spostaMuroLeft(int posRiga, int posColonna, int direzione){
	if(posColonna>1){
		rimuovi_muro(posRiga, posColonna, direzione);
		posColonna--;
		posMuro.centro.colonna=posColonna;
		if(controllaPercorso(posRiga, posColonna, posMuro.direzione))
			disegna_provvisorio(posRiga, posColonna, posMuro.direzione);
		else
			disegna_provvisorioNotValid(posRiga, posColonna, posMuro.direzione);
		muroMosso++;
	}
	
}	
void spostaMuroRight(int posRiga, int posColonna, int direzione){
	if(posColonna<6){
		rimuovi_muro(posRiga, posColonna, direzione);
		posColonna++;
		posMuro.centro.colonna=posColonna;
		if(controllaPercorso(posRiga, posColonna, posMuro.direzione))
			disegna_provvisorio(posRiga, posColonna, posMuro.direzione);
		else
			disegna_provvisorioNotValid(posRiga, posColonna, posMuro.direzione);		muroMosso++;
	}
}
	
void disegna_muro(int posRiga, int posColonna, int direzione){
	int mezzadimL=34;
	int mezzadimD=30;
	int larghezza=4;
	int i;
	posRiga=convertiInPixel(posRiga, 2, 4, 30);
	posColonna=convertiInPixel(posColonna, 2, 4, 30);
	
	for(i=0; i<larghezza-1; i++){
			if(direzione==Horizontal){
				posRiga--;		
				LCD_DrawLine(posColonna-mezzadimL,posRiga,posColonna+mezzadimD,posRiga, Black);
			}else{
				posColonna--;
				LCD_DrawLine(posColonna,posRiga-mezzadimL,posColonna,posRiga+mezzadimD, Black);
			}	
	}
}

void disegna_provvisorio(int posRiga, int posColonna, int direzione){
	int mezzadimL=34;
	int mezzadimD=30;
	int larghezza=4;
	int i;
	posRiga=convertiInPixel(posRiga, 2, 4, 30);
	posColonna=convertiInPixel(posColonna, 2, 4, 30);
	
	for(i=0; i<larghezza-1; i++){
			if(direzione==Horizontal){
				posRiga--;		
				LCD_DrawLine(posColonna-mezzadimL,posRiga,posColonna+mezzadimD,posRiga, Grey);
			}else{
				posColonna--;
				LCD_DrawLine(posColonna,posRiga-mezzadimL,posColonna,posRiga+mezzadimD, Grey);
			}	
	}
}

void disegna_provvisorioNotValid(int posRiga, int posColonna, int direzione){
	int mezzadimL=34;
	int mezzadimD=30;
	int larghezza=4;
	int i;
	posRiga=convertiInPixel(posRiga, 2, 4, 30);
	posColonna=convertiInPixel(posColonna, 2, 4, 30);
	
	for(i=0; i<larghezza-1; i++){
			if(direzione==Horizontal){
				posRiga--;		
				LCD_DrawLine(posColonna-mezzadimL,posRiga,posColonna+mezzadimD,posRiga, Red);
			}else{
				posColonna--;
				LCD_DrawLine(posColonna,posRiga-mezzadimL,posColonna,posRiga+mezzadimD, Red);
			}	
	}
}


void ruotaMuro(int posRiga, int posColonna, int direzione){
	if(direzione==Horizontal)
		posMuro.direzione=Vertical;
	else
		posMuro.direzione=Horizontal;
	muroMosso++;
	rimuovi_muro(posRiga, posColonna, direzione);
	if(controllaPercorso(posRiga, posColonna, posMuro.direzione))
			disegna_provvisorio(posRiga, posColonna, posMuro.direzione);
	else
			disegna_provvisorioNotValid(posRiga, posColonna, posMuro.direzione);

}

void confermaPosizioneMuro(int posRiga, int posColonna, int direzione){
	if(controllaPercorso(posRiga, posColonna, direzione)==1){
		disegna_muro(posRiga, posColonna, direzione);
		muriDir[posColonna][posRiga]=direzione;
		muroConferma=1;
	}else{
		if(turno==1)
				GUI_Text(16, 242, (uint8_t *) "    position not valid    ", Black , White);
		else
				GUI_Text(16, 242, (uint8_t *) "    position not valid    ", Black , Red);
		//DelayUS(10000 * 50);
	}
	cambiaTurno(turno);

}

void confermaPosizioneMuroPerAI(int posRiga, int posColonna, int direzione){
	disegna_muro(posRiga, posColonna, direzione);
	posMuro.centro.riga=posRiga;
	posMuro.centro.colonna=posColonna;
	posMuro.direzione=direzione;
	muriDir[posColonna][posRiga]=direzione;
	muroConferma=1;
	insMuro=1;
	cambiaTurno(turno);
}
	
void rimuovi_muro(int posRiga, int posColonna, int direzione){
	int mezzadimL=34;
	int mezzadimD=30;
	int larghezza=4;
	int i;
	int Riga=posRiga;
	int Colonna=posColonna;
	posRiga=convertiInPixel(posRiga, 2, 4, 30);
	posColonna=convertiInPixel(posColonna, 2, 4, 30);
	
	for(i=0; i<larghezza-1; i++){
			if(direzione==Horizontal){
				posRiga--;		
				LCD_DrawLine(posColonna-mezzadimL,posRiga,posColonna+mezzadimD,posRiga, sfondo);
			}else{
				posColonna--;
				LCD_DrawLine(posColonna,posRiga-mezzadimL,posColonna,posRiga+mezzadimD, sfondo);
			}	
	}
	
	if(muriDir[Colonna][Riga]>-1){
		disegna_muro(Riga, Colonna, muriDir[Colonna][Riga]);
	}
	if(direzione==Horizontal){
		if(muriDir[Colonna-1][Riga]>-1)
			if(muriDir[Colonna-1][Riga]==Horizontal)
						disegna_muro(Riga, Colonna-1, Horizontal);
		if(muriDir[Colonna+1][Riga]>-1)
			if(muriDir[Colonna+1][Riga]==Horizontal)
						disegna_muro(Riga, Colonna+1, Horizontal);
	}
	if(direzione==Vertical){
		if(muriDir[Colonna][Riga-1]>-1)
			if(muriDir[Colonna][Riga-1]==Vertical)
						disegna_muro(Riga-1, Colonna, Vertical);
		if(muriDir[Colonna][Riga+1]>-1)
			if(muriDir[Colonna][Riga+1]==Vertical)
						disegna_muro(Riga+1, Colonna, Vertical);
	}
	
	
	
}	

int controllaPercorso(int posRiga,int posColonna,int direzione){
	int i, j, tmp, res=0;
	
	if(muriDir[posColonna][posRiga]>-1)
		return 0;
	
	if(direzione==Horizontal){
		if(posRiga==0 || posRiga==7)
			return 0;
		if(muriDir[posColonna+1][posRiga]>-1){
			if(muriDir[posColonna+1][posRiga]==Horizontal)
				return 0;
		}
		if(muriDir[posColonna-1][posRiga]>-1){
			if(muriDir[posColonna-1][posRiga]==Horizontal)
				return 0;
		}
		
	}
	if(direzione==Vertical){
		if(posColonna==0 || posColonna==7)
			return 0;
		if(muriDir[posColonna][posRiga+1]>-1){
			if(muriDir[posColonna][posRiga+1]==Vertical)
				return 0;
		}
		if(muriDir[posColonna][posRiga-1]>-1){
			if(muriDir[posColonna][posRiga-1]==Vertical)
				return 0;
		}
	}
	tmp=muriDir[posColonna][posRiga];
	muriDir[posColonna][posRiga]=direzione;
	for(i=0; i<7; i++){
		for(j=0;j<7;j++){
			visitati[i][j]=0;
		}
	}
	
	if(turno==1){
		res= verificaValido(posFermo.riga, posFermo.colonna, 0);
		for(i=0; i<7; i++){
			for(j=0;j<7;j++){
				visitati[i][j]=0;
			}
		}
		res= res & verificaValido(posPlayer.riga, posPlayer.colonna, 6);
	}
	else{
		res= verificaValido(posFermo.riga, posFermo.colonna, 6);
		for(i=0; i<7; i++){
			for(j=0;j<7;j++){
				visitati[i][j]=0;
			}
		}
		res= res & verificaValido(posPlayer.riga, posPlayer.colonna, 0);
	}
	
	muriDir[posColonna][posRiga]=tmp;
	return res;

}





int verificaValido(int posRiga, int posColonna, int fine){
	int a=0,b=0,c=0,d=0;	
	
	if(posRiga==fine)
		return 1;
	if(posRiga>=0 && posColonna<7 && posColonna>=0 && posRiga<7){
		visitati[posColonna][posRiga]=1;
	}else{
		return 0;
	}
	
	
	if(posRiga>0){
		if( visitati[posColonna][posRiga-1]==0 && verificaMuroUp(posRiga, posColonna)){
			a=verificaValido(posRiga-1, posColonna, fine);
			if(a==1)
				return a;
		}
	}
	if(posRiga<6){
		if(visitati[posColonna][posRiga+1]==0 && verificaMuroDown(posRiga, posColonna)){
			b=verificaValido(posRiga+1, posColonna, fine);
			if(b==1)
				return b;
		}
	}
	if(posColonna>0){
		if(visitati[posColonna-1][posRiga]==0 && verificaMuroLeft(posRiga, posColonna)){
			c=verificaValido(posRiga, posColonna-1, fine);
			if(c==1)
				return c;
		}
	}
	if(posColonna<6 ){
		if(visitati[posColonna+1][posRiga]==0 && verificaMuroRight(posRiga, posColonna)){
			d=verificaValido(posRiga, posColonna+1, fine);
			if(d==1)
				return d;
		}
	}
	return 0;
}



int verificaMuroUp(int posRiga, int posColonna){
	if (muriDir[posColonna][posRiga]>-1)
		if (muriDir[posColonna][posRiga]==Horizontal)
			return 0;
	if(posColonna<6)
		if (muriDir[posColonna+1][posRiga]>-1)
			if (muriDir[posColonna+1][posRiga]==Horizontal)
				return 0;	
			
	return 1;
}

int verificaMuroDown(int posRiga, int posColonna){
	posRiga++;
	if (muriDir[posColonna][posRiga]>-1)
		if (muriDir[posColonna][posRiga]==Horizontal)
			return 0;
	if(posColonna<6)
		if (muriDir[posColonna+1][posRiga]>-1)
			if (muriDir[posColonna+1][posRiga]==Horizontal)
				return 0;	
	return 1;
}

int verificaMuroLeft(int posRiga, int posColonna){
	if (muriDir[posColonna][posRiga]>-1)
		if (muriDir[posColonna][posRiga]==Vertical)
			return 0;
	if(posRiga<6)
		if (muriDir[posColonna][posRiga+1]>-1)
			if (muriDir[posColonna][posRiga+1]==Vertical)
				return 0;	
	return 1;
}

int verificaMuroRight(int posRiga, int posColonna){
	posColonna++;
	if (muriDir[posColonna][posRiga]>-1)
		if (muriDir[posColonna][posRiga]==Vertical)
			return 0;
	if(posRiga<6)
		if (muriDir[posColonna][posRiga+1]>-1)
			if (muriDir[posColonna][posRiga+1]==Vertical)
				return 0;	
	return 1;
}

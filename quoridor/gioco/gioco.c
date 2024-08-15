/* **--------------File Info---------------------------------------------------------------------------------
** File name:               gioco.c
** Descriptions:            funzioni per giocare
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Peppe
** Created date:            2023-12-20
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
*/


/* Includes ------------------------------------------------------------------*/
#include "../GLCD/GLCD.h"
#include "gioco.h"
#include "../scacchiera/scacchiera.h"
#include "../timer/timer.h"
#include "../pedina/pedina.h"
#include "../muro/muro.h"
#include "../RIT/RIT.h"
#include "../AI/AI.h"
#include "../CAN/CAN.h"
#define HUMAN 0
#define NPC 1
#define SingleBoard 0
#define TwoBoards 1

int whoami;
char W2[2];
char W1[2];
extern int P1Wall;
extern int P2Wall;
extern int time;
int muriDir[8][8];

int spostato=0;
int mossa=0; //controllare se ha spostato e non confermato
extern int muroMosso;
extern int muroConferma;
extern int insMuro;
extern int gameMode;
extern int sceltaOpposite;
extern int muriFiniti;
extern int attivo;
int tempoFinito;

int P1Wall;
int P2Wall;
pos_t posPlayer;
pos_t posPlayerIn;
pos_t posFermo;
pos_t P1pos;
pos_t P2pos;
posMuro_t posMuro;
stato_t myscheda;
extern int turno;
extern unsigned int function1(int playerId,int pMorW, int VorH, int Y, int X);
unsigned int move;
int fine=0;


void prepara_gioco(){
	LCD_Clear(sfondo);
	disegna_scacchiera(7, 7, 4, 2, 2, 30, 30); //righe, colonne, spazioPerWall, Xin, Yin, Xdim, Ydim
	disegna_scacchiera(1, 3, 10, 5, 260, 50, 70);
	GUI_Text(12, 265, (uint8_t *) "P1 Wall", Black, sfondo);
	GUI_Text(172, 265, (uint8_t *) "P2 Wall", Black, sfondo);
}



void init_scacchiera(void){
	int i;
	int j;
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
			muriDir[i][j]=-1;
}

void init_stato_scheda(void){
	myscheda.player=-1;
	myscheda.stato=-1;
}


int start(int t){ 
	colora(White, P1pos.riga, P1pos.colonna, 4, 2, 2, 30, 30);
	colora(Red, P2pos.riga, P2pos.colonna, 4, 2, 2, 30, 30);
	P1Wall=8;
	W1[0]=	P1Wall+'0';
	GUI_Text(38, 284, (uint8_t *) W1 , Black, sfondo);
	P2Wall=8;
	W2[0]= P2Wall+'0';
	GUI_Text(198, 284, (uint8_t *) W2 , Black, sfondo);
	tempoFinito=0;
	
	if(t==1){
		posPlayer.riga=P1pos.riga;
		posPlayer.colonna=P1pos.colonna;
		posPlayerIn.riga=P1pos.riga;
		posPlayerIn.colonna=P1pos.colonna;
		posFermo.riga=P2pos.riga;
		posFermo.colonna=P2pos.colonna;
	}
	else{
		t=2;
		posPlayer.riga=P2pos.riga;
		posPlayer.colonna=P2pos.colonna;
		posPlayerIn.riga=P2pos.riga;
		posPlayerIn.colonna=P2pos.colonna;
		posFermo.riga=P1pos.riga;
		posFermo.colonna=P1pos.colonna;
	}
	if((t==whoami && gameMode==TwoBoards && sceltaOpposite!=NPC) || gameMode==SingleBoard){
		controlloAdiacenti(posPlayer.riga, posPlayer.colonna);
		if(t==1)
			GUI_Text(16, 242, (uint8_t *) "     Fai la tua mossa     ", Black, White);
		else
			GUI_Text(16, 242, (uint8_t *) "     Fai la tua mossa     ", Black, Red);
	}else if(gameMode==TwoBoards && t!=whoami){
		if(t==1)
			GUI_Text(16, 242, (uint8_t *) "    Attendi avversario    ", Black, Red);
		else
			GUI_Text(16, 242, (uint8_t *) "    Attendi avversario    ", Black, White);
	}
	else if((t==whoami && gameMode==TwoBoards && sceltaOpposite==NPC)){
		ArtificialIntelligence(t);
	}
	
	NVIC_EnableIRQ(EINT1_IRQn);
	LPC_PINCON->PINSEL4		|= (1<<22);
	return t;
}

int due_schede(void){
	//-1 nessuno ha cliccato nessuno; 0 lui ha cliccato e io no; 1 io ho cliccato e lui no; 2 entrambi abbiamo cliccato
	//se altro player ha già cliccato int0 --> sono player2
	//se altro player non ha ancora cliccato int0 --> sono player1, attendo con timer e ciclo che clicchi
	//se altro player clicca int0, giochiamo
	//se non clicca int0, messaggio di errore
	CAN_TxMsg.id=1;
	CAN_TxMsg.len=2;
	CAN_TxMsg.data[0]=0xFF;
	CAN_TxMsg.data[1]=1;
	CAN_TxMsg.format=STANDARD_FORMAT;
	CAN_TxMsg.type=DATA_FRAME;
	CAN_wrMsg(1, &CAN_TxMsg);
	
	if(myscheda.stato==-1){ //sono il primo ad aver cliccato
		GUI_Text(16, 242, (uint8_t *) "      waiting player2     ", Black , White);
		myscheda.stato=1; //in attesa di altro
		myscheda.player=1;
		enable_timer(1);
		while(myscheda.stato==1){ //aggiornato a -1 se scade timer, a 2 se altro player clicca
		}
		if(myscheda.stato==2){
			whoami=1;
			disable_timer(1);
			return 1;
		}
		if(myscheda.stato==-1){
			LCD_Clear(Black);
			GUI_Text(16, 242, (uint8_t *) "Altro player: non connesso", Red, White);
			return -1;
		}
	}
	else if(myscheda.stato==0){ //altro ha già cliccato
		whoami=2;
		myscheda.stato=2;
		myscheda.player=2;
		GUI_Text(16, 242, (uint8_t *) "Entrambi connessi", Red, White);
		return 1;
	}
	return -1;
}


 void cambiaTurno(int t){ 
	int playerId;
	int playerMoveOrWall;
	int VerticalHorizontal=0;
	int Y;
	int X;
	
	playerId=turno-1;
	playerMoveOrWall=insMuro;
	
	
	disable_timer(0);
	if(insMuro==0){		//scelto di muovere pedina
		if (spostato==0 && mossa==0){
				rimuoviColore(posPlayer.riga, posPlayer.colonna);
		}
		else if(spostato==0 && mossa>0){
				if(t==1){
					rimuoviColore(P1pos.riga, P1pos.colonna);
					colora(White, P1pos.riga, P1pos.colonna, 4, 2, 2, 30, 30);
				}
				else{
					rimuoviColore(P2pos.riga, P2pos.colonna);
					colora(Red, P2pos.riga, P2pos.colonna, 4, 2, 2, 30, 30);
				}
		}
		Y=posPlayer.riga;
		X=posPlayer.colonna;
	} else{		//scelto di inserire muro
			
		NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 24);
		if(muroConferma==0){	//muroNonConfermato
			rimuovi_muro(posMuro.centro.riga, posMuro.centro.colonna, posMuro.direzione);
		}else{ //muroInserito
			playerMoveOrWall=1;
			if(turno==1){
				P1Wall--;
				W1[0]=	P1Wall+'0';
				GUI_Text(38, 284, (uint8_t *) W1 , Black, sfondo);
			}else{
				P2Wall--;
				W2[0]= P2Wall+'0';
				GUI_Text(198, 284, (uint8_t *) W2 , Black, sfondo);
			}
			VerticalHorizontal=posMuro.direzione; 
		}
		Y=posMuro.centro.riga;
		X=posMuro.centro.colonna;
		
	}
	if(t==1){		
			t=2;
			posPlayer.riga=P2pos.riga;
			posPlayer.colonna=P2pos.colonna;
			posPlayerIn.riga=P2pos.riga;
			posPlayerIn.colonna=P2pos.colonna;
			posFermo.riga=P1pos.riga;
			posFermo.colonna=P1pos.colonna;
	}else{
			t=1;
			posPlayer.riga=P1pos.riga;
			posPlayer.colonna=P1pos.colonna;
			posPlayerIn.riga=P1pos.riga;
			posPlayerIn.colonna=P1pos.colonna;
			posFermo.riga=P2pos.riga;
			posFermo.colonna=P2pos.colonna;
	}
	if (tempoFinito==1){
		tempoFinito=0;
		playerMoveOrWall=0;
		VerticalHorizontal=1;
	}
	if(playerMoveOrWall==1){//inserito muro
		move=function1(playerId,playerMoveOrWall,VerticalHorizontal,Y-1, X-1);
	}else{//inserimento pedina
		move=function1(playerId,playerMoveOrWall,VerticalHorizontal,Y, X);
	}
	if(gameMode==TwoBoards)
	invia_messaggio(move);
	muriFiniti=0;
	spostato=0;
	mossa=0;
	muroMosso=0;
	muroConferma=0;
	insMuro=0;
	time=20;
	turno= t;
	
	if( gameMode==SingleBoard || (gameMode==TwoBoards && whoami==t)){
		if(t==2 && gameMode==SingleBoard && sceltaOpposite==NPC ){ //automatico su single
			GUI_Text(16, 242, (uint8_t *) "           Wait           ", sfondo, sfondo);
			ArtificialIntelligence(2);
		}else if(gameMode==TwoBoards && sceltaOpposite==NPC && whoami==t){
			ArtificialIntelligence(whoami);
		}else  if((gameMode==SingleBoard) || (gameMode==TwoBoards && sceltaOpposite!=NPC && turno==whoami)){
			controlloAdiacenti(posPlayer.riga, posPlayer.colonna);
			reset_timer(0);
			enable_timer(0);
			if(t==1)
				GUI_Text(16, 242, (uint8_t *) "     Fai la tua mossa     ", Black, White);
			else
				GUI_Text(16, 242, (uint8_t *) "     Fai la tua mossa     ", Black, Red);
		}
	}
}
	

void invia_messaggio(unsigned int move){
	CAN_TxMsg.id=1;
	CAN_TxMsg.len=4;
	CAN_TxMsg.data[0]= ((move>>24) & 0xff);
	CAN_TxMsg.data[1]= ((move>>16) & 0xff);
	CAN_TxMsg.data[2]= ((move>>8) & 0xff);
	CAN_TxMsg.data[3]= (move & 0xff);
	CAN_TxMsg.format=STANDARD_FORMAT;
	CAN_TxMsg.type=DATA_FRAME;
	CAN_wrMsg(1, &CAN_TxMsg);	
}
	
void cambiaTurno2schede(int PlayerID, int PlayerMorW, int VorO, int Y, int X){
	if(PlayerID==1){ //se vecchio era 1
		turno=2;
	}
	else{
		turno=1;
	}

		
	if(PlayerMorW==0 && VorO==1){ //scaduto il timer dell'avversario

	}		
	else if(PlayerMorW==0){ //spostato pedina
		colora(sfondo, posPlayerIn.riga, posPlayerIn.colonna, 4, 2, 2, 30, 30);
		if(PlayerID==1){
			P1pos.riga=Y;
			P1pos.colonna=X;
			colora(White, Y, X, 4, 2, 2, 30, 30);
			if(Y==6){
				GUI_Text(16, 242, (uint8_t *) "  The winner is player 1  ", Black, White);
				disable_RIT();
				disable_timer(0);
				fine=1;
				attivo=0;
			}
		}else{
			P2pos.riga=Y;
			P2pos.colonna=X;
			colora(Red, Y, X, 4, 2,2, 30, 30);
			if(Y==0){
				GUI_Text(16, 242, (uint8_t *) "  The winner is player 2  ", Black, Red);
				disable_RIT();
				disable_timer(0);
				fine=1;
				attivo=0;
			}
		}
	}
	else{ //spostato muro
		muriDir[X][Y]=VorO;
		disegna_muro(Y, X, VorO);
		if(turno==2){
			P1Wall--;
			W1[0]=	P1Wall+'0';
			GUI_Text(38, 284, (uint8_t *) W1 , Black, sfondo);
		}else{
			P2Wall--;
			W2[0]= P2Wall+'0';
			GUI_Text(198, 284, (uint8_t *) W2 , Black, sfondo);
		}
	}
	if(fine==0){
		posPlayer.riga=posFermo.riga;
		posPlayer.colonna=posFermo.colonna;
		
		if(PlayerMorW==0 && VorO==0){
			posFermo.riga=Y;
			posFermo.colonna=X;
		}else{
			posFermo.riga=posPlayerIn.riga;
			posFermo.colonna=posPlayerIn.colonna;
		}
		posPlayerIn.riga=posPlayer.riga;
		posPlayerIn.colonna=posPlayer.colonna;
		
			
		tempoFinito=0;
		muriFiniti=0;
		spostato=0;
		mossa=0;
		muroMosso=0;
		muroConferma=0;
		insMuro=0;
		time=20;
		if(turno==2 && gameMode==SingleBoard && sceltaOpposite==NPC ){ //sistemare sta roba
			GUI_Text(16, 242, (uint8_t *) "           Wait           ", sfondo, sfondo);
			ArtificialIntelligence(2);
		}else if(gameMode==TwoBoards && sceltaOpposite==NPC && turno==whoami){ //e io sono un npc
			ArtificialIntelligence(whoami);
		}else if((gameMode==SingleBoard) || (gameMode==TwoBoards && sceltaOpposite!=NPC && turno==whoami)){
			controlloAdiacenti(posPlayer.riga, posPlayer.colonna);
			reset_timer(0);
			enable_timer(0);
			if(turno==1)
				GUI_Text(16, 242, (uint8_t *) "     Fai la tua mossa     ", Black, White);
			else
				GUI_Text(16, 242, (uint8_t *) "     Fai la tua mossa     ", Black, Red);
		}
	}
}






void controlloAdiacenti(int posRiga, int posColonna){
	//controllo adiacente
	if(posRiga!=0 && ( (posRiga-1!=posFermo.riga) || posColonna!=posFermo.colonna) && verificaMuroUp(posRiga, posColonna) )
		coloraIndicatore(Yellow, posRiga-1, posColonna, 4, 2, 2, 30, 30);
	if(posRiga!=6 && ( (posRiga+1!=posFermo.riga) || posColonna!=posFermo.colonna) && verificaMuroDown(posRiga, posColonna) ) 
		coloraIndicatore(Yellow, posRiga+1, posColonna, 4, 2, 2, 30, 30);
	if(posColonna!=0 && ( (posColonna-1!=posFermo.colonna) || posRiga!=posFermo.riga) && verificaMuroLeft(posRiga, posColonna) ) 
		coloraIndicatore(Yellow, posRiga, posColonna-1, 4, 2, 2, 30, 30);
	if(posColonna!=6 && ( (posColonna+1!=posFermo.colonna) || posRiga!=posFermo.riga) && verificaMuroRight(posRiga, posColonna) )
		coloraIndicatore(Yellow, posRiga, posColonna+1, 4, 2, 2, 30, 30);
	
	//controllo dietro altra pedina
	if(posRiga>1 && ( (posRiga-1==posFermo.riga) && posColonna==posFermo.colonna) && verificaMuroUp(posRiga, posColonna) && verificaMuroUp(posRiga-1, posColonna) ) //&controllo
		coloraIndicatore(Yellow, posRiga-2, posColonna, 4, 2, 2, 30, 30);
	if(posRiga<5 && ( (posRiga+1==posFermo.riga) && posColonna==posFermo.colonna) && verificaMuroDown(posRiga, posColonna)  && verificaMuroDown(posRiga+1, posColonna)) //&controllo muri
		coloraIndicatore(Yellow, posRiga+2, posColonna, 4, 2, 2, 30, 30);
	if(posColonna>1 && ( (posColonna-1==posFermo.colonna) && posRiga==posFermo.riga) && verificaMuroLeft(posRiga, posColonna) && verificaMuroLeft(posRiga, posColonna-1)) //&controllo
		coloraIndicatore(Yellow, posRiga, posColonna-2, 4, 2, 2, 30, 30);
	if(posColonna<5 && ( (posColonna+1==posFermo.colonna) && posRiga==posFermo.riga) && verificaMuroRight(posRiga, posColonna) && verificaMuroRight(posRiga, posColonna+1)) //&controllo muri
		coloraIndicatore(Yellow, posRiga, posColonna+2, 4, 2, 2, 30, 30);
	
	//controllo dietro pedina nel caso in cui ci sia un muro dietro o campo finito
	if(posRiga>0 && ( (posRiga-1==posFermo.riga) && posColonna==posFermo.colonna) && verificaMuroUp(posRiga, posColonna) && (!verificaMuroUp(posRiga-1, posColonna) || posRiga-1==0) ){
		if(posColonna<7 && verificaMuroRight(posFermo.riga, posFermo.colonna))
			coloraIndicatore(Yellow, posFermo.riga, posFermo.colonna+1, 4, 2, 2, 30, 30);
		if(posColonna>0 && verificaMuroLeft(posFermo.riga, posFermo.colonna))
			coloraIndicatore(Yellow, posFermo.riga, posFermo.colonna-1, 4, 2, 2, 30, 30);
	} //muro sopra alla pedina dell'avversario

	if(posRiga<6 && ( (posRiga+1==posFermo.riga) && posColonna==posFermo.colonna) && verificaMuroDown(posRiga, posColonna)  && (!verificaMuroDown(posRiga+1, posColonna) || posRiga+1==6)){
		if(posColonna<7 && verificaMuroRight(posFermo.riga, posFermo.colonna))
			coloraIndicatore(Yellow, posFermo.riga, posFermo.colonna+1, 4, 2, 2, 30, 30);
		if(posColonna>0 && verificaMuroLeft(posFermo.riga, posFermo.colonna))
			coloraIndicatore(Yellow, posFermo.riga, posFermo.colonna-1, 4, 2, 2, 30, 30);
	} 
	if(posColonna>0 && ( (posColonna-1==posFermo.colonna) && posRiga==posFermo.riga) && verificaMuroLeft(posRiga, posColonna) && (!verificaMuroLeft(posRiga, posColonna-1) || posColonna-1==0)){
		if(posRiga<7 && verificaMuroDown(posFermo.riga, posFermo.colonna)) //bassosx
			coloraIndicatore(Yellow, posFermo.riga+1, posFermo.colonna, 4, 2, 2, 30, 30);
		if(posRiga>0 && verificaMuroUp(posFermo.riga, posFermo.colonna)) //altosx
			coloraIndicatore(Yellow, posFermo.riga-1, posFermo.colonna, 4, 2, 2, 30, 30);
	} 
	if(posColonna<6 && ( (posColonna+1==posFermo.colonna) && posRiga==posFermo.riga) && verificaMuroRight(posRiga, posColonna) && (!verificaMuroRight(posRiga, posColonna+1) || posColonna+1==6)){
		if(posRiga<7 && verificaMuroDown(posFermo.riga, posFermo.colonna)) //bassodx
			coloraIndicatore(Yellow, posFermo.riga+1, posFermo.colonna, 4, 2, 2, 30, 30);
		if(posRiga>0 && verificaMuroUp(posFermo.riga, posFermo.colonna))  //altodx
			coloraIndicatore(Yellow, posFermo.riga-1, posFermo.colonna, 4, 2, 2, 30, 30);
	}
	

}


void colora(int colore, int riga, int colonna, int padding,int Xin,int Yin,int lar,int alt){
	int i;
	colonna=convertiInPixel(colonna, Xin, padding, lar);
	riga=convertiInPixel(riga, Yin, padding, alt);
	riga++;
	for(i=0; i<alt-1; i++){
		LCD_DrawLine(colonna+1, riga, colonna+lar-1, riga,colore);
		riga++;
	}
}


void rimuoviColore(int posRiga, int posColonna){
	if(posRiga!=0 && ( (posRiga-1!=posFermo.riga) || posColonna!=posFermo.colonna)) //&controllo
		coloraIndicatore(sfondo, posRiga-1, posColonna, 4, 2, 2, 30, 30);
	if(posRiga!=6 && ( (posRiga+1!=posFermo.riga) || posColonna!=posFermo.colonna)) //&controllo muri
		coloraIndicatore(sfondo, posRiga+1, posColonna, 4, 2, 2, 30, 30);
	if(posColonna!=0 && ( (posColonna-1!=posFermo.colonna) || posRiga!=posFermo.riga)) //&controllo
		coloraIndicatore(sfondo, posRiga, posColonna-1, 4, 2, 2, 30, 30);
	if(posColonna!=6 && ( (posColonna+1!=posFermo.colonna) || posRiga!=posFermo.riga)) //&controllo muri
		coloraIndicatore(sfondo, posRiga, posColonna+1, 4, 2, 2, 30, 30);
	
	if(posRiga>1 && ( (posRiga-1==posFermo.riga) && posColonna==posFermo.colonna)) //&controllo
		coloraIndicatore(sfondo, posRiga-2, posColonna, 4, 2, 2, 30, 30);
	if(posRiga<5 && ( (posRiga+1==posFermo.riga) && posColonna==posFermo.colonna)) //&controllo muri
		coloraIndicatore(sfondo, posRiga+2, posColonna, 4, 2, 2, 30, 30);
	if(posColonna>1 && ( (posColonna-1==posFermo.colonna) && posRiga==posFermo.riga)) //&controllo
		coloraIndicatore(sfondo, posRiga, posColonna-2, 4, 2, 2, 30, 30);
	if(posColonna<5 && ( (posColonna+1==posFermo.colonna) && posRiga==posFermo.riga)) //&controllo muri
		coloraIndicatore(sfondo, posRiga, posColonna+2, 4, 2, 2, 30, 30);
	
	//ulteriore CONTROLLO
	if(posRiga>0 && ( (posRiga-1==posFermo.riga) && posColonna==posFermo.colonna) && verificaMuroUp(posRiga, posColonna) && !verificaMuroUp(posRiga-1, posColonna) ){
		if(posColonna<7 && verificaMuroRight(posFermo.riga, posFermo.colonna))
			coloraIndicatore(sfondo, posFermo.riga, posFermo.colonna+1, 4, 2, 2, 30, 30);
		if(posColonna>0 && verificaMuroLeft(posFermo.riga, posFermo.colonna))
			coloraIndicatore(sfondo, posFermo.riga, posFermo.colonna-1, 4, 2, 2, 30, 30);
	} //muro sopra alla pedina dell'avversario

	if(posRiga<6 && ( (posRiga+1==posFermo.riga) && posColonna==posFermo.colonna) && verificaMuroDown(posRiga, posColonna)  && !verificaMuroDown(posRiga+1, posColonna)){
		if(posColonna<7 && verificaMuroRight(posFermo.riga, posFermo.colonna))
			coloraIndicatore(sfondo, posFermo.riga, posFermo.colonna+1, 4, 2, 2, 30, 30);
		if(posColonna>0 && verificaMuroLeft(posFermo.riga, posFermo.colonna))
			coloraIndicatore(sfondo, posFermo.riga, posFermo.colonna-1, 4, 2, 2, 30, 30);
	}
	if(posColonna>0 && ( (posColonna-1==posFermo.colonna) && posRiga==posFermo.riga) && verificaMuroLeft(posRiga, posColonna) && !verificaMuroLeft(posRiga, posColonna-1)){
		if(posRiga<7 && verificaMuroUp(posFermo.riga, posFermo.colonna))
			coloraIndicatore(sfondo, posFermo.riga+1, posFermo.colonna, 4, 2, 2, 30, 30);
		if(posRiga>0 && verificaMuroDown(posFermo.riga, posFermo.colonna))
			coloraIndicatore(sfondo, posFermo.riga-1, posFermo.colonna, 4, 2, 2, 30, 30);
	} 

	if(posColonna<6 && ( (posColonna+1==posFermo.colonna) && posRiga==posFermo.riga) && verificaMuroRight(posRiga, posColonna) && !verificaMuroRight(posRiga, posColonna+1)){
		if(posRiga<7 && verificaMuroUp(posFermo.riga, posFermo.colonna))
			coloraIndicatore(sfondo, posFermo.riga+1, posFermo.colonna, 4, 2, 2, 30, 30);
		if(posRiga>0 && verificaMuroDown(posFermo.riga, posFermo.colonna))
			coloraIndicatore(sfondo, posFermo.riga-1, posFermo.colonna, 4, 2, 2, 30, 30);
	}
	
}


void coloraIndicatore(int colore, int riga, int colonna, int padding,int Xin,int Yin,int lar,int alt){
	int i;
	colonna=convertiInPixel(colonna, Xin, padding, lar);
	riga=convertiInPixel(riga, Yin, padding, alt);
	riga++;
	lar--;
	for(i=1; i<5; i++){
		disegna_riquadri(colonna+i, riga+i,lar-(2*i), alt-(2*i), colore);
	}
}




void selectGameMode(void){
	
	GUI_Text(17, 60, (uint8_t *) "        Select the       ", Black , sfondo);
	GUI_Text(17, 80, (uint8_t *) "        GAME  MODE       ", Black , sfondo);
	//nrRighe, nrColonne, padding, daSx, daSopra, altezza, larghezza
  disegna_scacchiera(2, 1, 20,60,160,40,120);
	GUI_Text(70, 172, (uint8_t *) "Single Board", Black , sfondo);
	GUI_Text(70, 232, (uint8_t *) " Two  Boards", Black , sfondo);

}

void selectOppositePlayerSingle(void)
{
	GUI_Text(17, 60, (uint8_t *) "  Single Board: select  ", Black , sfondo);
	GUI_Text(17, 80, (uint8_t *) "  the opposite  player   ", Black , sfondo);
	//nrRighe, nrColonne, padding, daSx, daSopra, altezza, larghezza
  disegna_scacchiera(2, 1, 20,60,160,40,120);
	GUI_Text(100, 172, (uint8_t *) "Human", Black , sfondo);
	GUI_Text(106, 232, (uint8_t *) "NPC", Black , sfondo);

}

void selectOppositePlayerTwo(void)
{
	GUI_Text(17, 60, (uint8_t *) "   Two Boards:  select   ", Black , sfondo);
	GUI_Text(17, 80, (uint8_t *) "       your player       ", Black , sfondo);
	//nrRighe, nrColonne, padding, daSx, daSopra, altezza, larghezza
  disegna_scacchiera(2, 1, 20,60,160,40,120);
	GUI_Text(100, 172, (uint8_t *) "Human", Black , sfondo);
	GUI_Text(106, 232, (uint8_t *) "NPC", Black , sfondo);

}






/*******************************************************************************
* Function Name  : DelayUS
* Description    : ÑÓÊ±1us
* Input          : - cnt: ÑÓÊ±Öµ
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DelayUS(uint32_t cnt)
{
  uint32_t i;
  for(i = 0;i<cnt;i++)
  {
     uint8_t us = 12; /* ÉèÖÃÖµÎª12£¬´óÔ¼ÑÓ1Î¢Ãë */    
     while (us--)     /* ÑÓ1Î¢Ãë	*/
     {
       ;   
     }
  }
}

/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h"
#include "../scacchiera/scacchiera.h"
#include "../gioco/gioco.h"
#include "../timer/timer.h"
#include "../pedina/pedina.h"
#include "../muro/muro.h"
#define HUMAN 0
#define NPC 1
#define SingleBoard 0
#define TwoBoards 1

extern int attivo;
extern int insMuro;
extern int muroMosso;
extern int mossa;
volatile int down0=0;						//P2.10  INT0  PINSEL4 20-21
volatile int down1=0;						//P2.11  KEY1  PINSEL4 22-23
volatile int down2=0;						//P2.12  KEY2  PINSEL4 24-25
extern int turno;
extern int insMuro;
volatile int down=0;
extern pos_t posPlayer;
extern pos_t posPlayerIn;
extern posMuro_t posMuro;
extern int Horizontal;
int gameMode=-1;
int sceltaOpposite=-1;
int startPlay=-1;
int iniziato=0;


int gm=-1;
int so=-1;

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


void RIT_IRQHandler (void)
{					
	static int Jselect=0;		//P1.25		PINSEL3 18 19
	static int Jdown=0;			//P1.26		PINSEL3 20 21
	static int Jleft=0;			//P1.27   PINSEL3 22 23
	static int Jright=0;		//P1.28   PINSEL3 24 25
	static int Jup=0;				//P1.29		PINSEL3 26 27
	static int JupLeft=0;
	static int JupRight=0;
	static int JdownLeft=0;
	static int JdownRight=0;
	disable_RIT();
	reset_RIT();
	
	
	if(down0!=0){		//INT0
		down0++;
		if((LPC_GPIO2->FIOPIN & (1<<10))  == 0){
			switch(down0){
				case 2:
					if(gameMode==-1){
						iniziato=1;
						GUI_Text(16, 242, (uint8_t *) "                          ", Black , sfondo);
						selectGameMode();
						init_stato_scheda();
					}
					else{
						prepara_gioco();
						init_scacchiera();
						if(gameMode==TwoBoards){
							//comunicazione ad altra scheda che è stato premuto int0
							turno=due_schede();
							if(turno!=-1){
								turno=start(turno);
								attivo=1;
							}else{
								attivo=0;
							}
						}
						else{		
							attivo=1;
							turno=start(1);
							enable_timer(0);
						}
						
					}
					
					break;
				default: 
					break;
			}
		}
		else{
			down0=0;
			if(attivo==0){
				NVIC_EnableIRQ(EINT0_IRQn);
				LPC_PINCON->PINSEL4		|= (1<<20);
			}
		}
	}
	
	
	
	if(down1!=0){		//KEY1
		down1++;
		if((LPC_GPIO2->FIOPIN & (1<<11))  == 0){
			switch(down1){
				case 2:
					if(insMuro==1 && attivo==1){
						NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
						LPC_PINCON->PINSEL4    &= ~(1 << 24);
						rimuovi_muro(posMuro.centro.riga, posMuro.centro.colonna, posMuro.direzione);
						insMuro=0;
						controlloAdiacenti(posPlayerIn.riga, posPlayerIn.colonna);
					}
					else if (attivo==1){
						insMuro=1;
						if(mossa>0){
							mossa=0;
							rimuoviPiccolo(posPlayer.riga,posPlayer.colonna);
							if(turno==1)
								colora(White,posPlayerIn.riga, posPlayerIn.colonna, 4, 2, 2, 30, 30);
							else
								colora(Red,posPlayerIn.riga, posPlayerIn.colonna, 4, 2, 2, 30, 30);
						}
						if(controllaPercorso(4, 4, Horizontal))
							disegna_provvisorio(4, 4, Horizontal); //Centroriga, Centrocolonna, direzione
						else
							disegna_provvisorioNotValid(4, 4, Horizontal); //Centroriga, Centrocolonna, direzione
						rimuoviColore(posPlayerIn.riga, posPlayerIn.colonna);
						posPlayer.riga=posPlayerIn.riga;
						posPlayer.colonna=posPlayerIn.colonna;
						posMuro.centro.riga=4;
						posMuro.centro.colonna=4;
						posMuro.direzione=Horizontal;
						NVIC_EnableIRQ(EINT2_IRQn);
						LPC_PINCON->PINSEL4		|= (1<<24);
					}
					
					break;
				default: 
					break;
			}
		}
		else{
			down1=0;
			NVIC_EnableIRQ(EINT1_IRQn);
			LPC_PINCON->PINSEL4		|= (1<<22);
		}
	}	
	
	if(down2!=0){		//KEY2
		down2++;
		if((LPC_GPIO2->FIOPIN & (1<<12))  == 0){
			switch(down2){
				case 2:
					ruotaMuro(posMuro.centro.riga, posMuro.centro.colonna, posMuro.direzione);
					break;
				default: 
					break;
			}
		}
		else{
			down2=0;
			NVIC_EnableIRQ(EINT2_IRQn);
			LPC_PINCON->PINSEL4		|= (1<<24);
		}
	}	
	
	
	
	if(((LPC_GPIO1->FIOPIN & (1<<29)) == 0)  &&  ((LPC_GPIO1->FIOPIN & (1<<27)) != 0) &&  ((LPC_GPIO1->FIOPIN & (1<<28)) != 0)   ){	/* Joytick UP pressed */
		Jup++;
		switch(Jup){
			case 1:
				break;
			case 2:
				if(gameMode==-1 && iniziato==1){
					gm=SingleBoard;
					evidenzia_Human(Yellow);
					evidenzia_NPC(sfondo);
					
				}else if(sceltaOpposite==-1 && iniziato==1){
					so=HUMAN;
					evidenzia_Human(Yellow);
					evidenzia_NPC(sfondo);
				}
				else if(iniziato==1){
					if(insMuro==1 && attivo==1){
					spostaMuroUp(posMuro.centro.riga, posMuro.centro.colonna, posMuro.direzione);
					}
				else if(attivo==1){
						spostaPedinaUp(posPlayerIn.riga, posPlayerIn.colonna);			
					}
				}

				break;
			default:
				if (Jup % 15 ==0){
					if(insMuro==1 && attivo==1)
						spostaMuroUp(posMuro.centro.riga, posMuro.centro.colonna, posMuro.direzione);
				}
				break;
		}
	}
	else{
			Jup=0;
	}
	
	
	if( ((LPC_GPIO1->FIOPIN & (1<<26)) == 0)  &&  ((LPC_GPIO1->FIOPIN & (1<<27)) != 0) &&  ((LPC_GPIO1->FIOPIN & (1<<28)) != 0)    ){			/* Joytick down pressed */
		Jdown++;
		switch(Jdown){
			case 1:
				break;
			case 2:
				if(gameMode==-1 && iniziato==1){
					gm=TwoBoards;
					evidenzia_Human(sfondo);
					evidenzia_NPC(Yellow);
				}else if(sceltaOpposite==-1 && iniziato==1){
					so=NPC;
					evidenzia_Human(sfondo);
					evidenzia_NPC(Yellow);
				}
				else{
					if(insMuro==1 && attivo==1)
						spostaMuroDown(posMuro.centro.riga, posMuro.centro.colonna, posMuro.direzione);
					else if (attivo==1){
						spostaPedinaDown(posPlayerIn.riga, posPlayerIn.colonna);
					}
				}
				
				break;
			default:
				if (Jdown % 15 ==0){ 
					if(insMuro==1 && attivo==1)
						spostaMuroDown(posMuro.centro.riga, posMuro.centro.colonna, posMuro.direzione);
				}
				break;
		}
	}
	else{
			Jdown=0;
	}
	
	if(((LPC_GPIO1->FIOPIN & (1<<27)) == 0) && ((LPC_GPIO1->FIOPIN & (1<<29)) != 0) && ((LPC_GPIO1->FIOPIN & (1<<26)) != 0)){		/* Joytick left pressed */
		Jleft++;
		switch(Jleft){
			case 1:
				break;
			case 2:
				if(gameMode!=-1 && attivo==1){
					if(insMuro==1){
						spostaMuroLeft(posMuro.centro.riga, posMuro.centro.colonna, posMuro.direzione);
					}
					else{
						spostaPedinaLeft(posPlayerIn.riga, posPlayerIn.colonna);
					
					}
				}
				break;
			default:
				if (Jleft % 15 ==0){ 
					if(insMuro==1 && attivo==1)
						spostaMuroLeft(posMuro.centro.riga, posMuro.centro.colonna, posMuro.direzione);
				}
				break;
		}
	}
	else{
			Jleft=0;
	}
	
	if(((LPC_GPIO1->FIOPIN & (1<<28)) == 0 && ((LPC_GPIO1->FIOPIN & (1<<29)) != 0) && ((LPC_GPIO1->FIOPIN & (1<<26)) != 0))){	/* Joytick right pressed */
		Jright++;
		switch(Jright){
			case 1:
				break;
			case 2:
				if(gameMode!=-1 && attivo==1){
					if(insMuro==1){
						spostaMuroRight(posMuro.centro.riga, posMuro.centro.colonna, posMuro.direzione);
					}
					else{
						spostaPedinaRight(posPlayerIn.riga, posPlayerIn.colonna);
					}
				}
				break;
			default:
				if (Jright % 15 ==0 && attivo==1){
					if(insMuro==1)
						spostaMuroRight(posMuro.centro.riga, posMuro.centro.colonna, posMuro.direzione);
				}
				break;
		}
	}
	else{
			Jright=0;
	}
	
	if(((LPC_GPIO1->FIOPIN & (1<<28)) == 0 && ((LPC_GPIO1->FIOPIN & (1<<29)) == 0) )){	/* Joytick UpRight pressed */
		JupRight++;
		switch(JupRight){
			case 1:
				break;
			case 2:
				if(gameMode!=-1 && attivo==1)
					spostaPedinaUpRight(posPlayerIn.riga, posPlayerIn.colonna);

				break;
			default:
				break;
		}
	}
	else{
			JupRight=0;
	}
	
	if(((LPC_GPIO1->FIOPIN & (1<<27)) == 0 && ((LPC_GPIO1->FIOPIN & (1<<29)) == 0) )){	/* Joytick UpRight pressed */
		JupLeft++;
		switch(JupLeft){
			case 1:
				break;
			case 2:
				if(gameMode!=-1 && attivo==1)
					spostaPedinaUpLeft(posPlayerIn.riga, posPlayerIn.colonna);

				break;
			default:
				break;
		}
	}
	else{
			JupLeft=0;
	}
	
	if(((LPC_GPIO1->FIOPIN & (1<<28)) == 0 && ((LPC_GPIO1->FIOPIN & (1<<26)) == 0) )){	/* Joytick UpRight pressed */
		JdownRight++;
		switch(JdownRight){
			case 1:
				break;
			case 2:
				if(gameMode!=-1 && attivo==1)
					spostaPedinaDownRight(posPlayerIn.riga, posPlayerIn.colonna);

				break;
			default:
				break;
		}
	}
	else{
			JdownRight=0;
	}
	
	if(((LPC_GPIO1->FIOPIN & (1<<27)) == 0 && ((LPC_GPIO1->FIOPIN & (1<<26)) == 0) )){	/* Joytick UpRight pressed */
		JdownLeft++;
		switch(JdownLeft){
			case 1:
				break;
			case 2:
				if(gameMode!=-1 && attivo==1)
					spostaPedinaDownLeft(posPlayerIn.riga, posPlayerIn.colonna);

				break;
			default:
				break;
		}
	}
	else{
			JdownLeft=0;
	}
	
	
	
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){		/* Joytick pressed */
		Jselect++;
		switch(Jselect){
			case 1:
				break;
			case 2:
				if(gameMode==-1 && gm!=-1 && iniziato==1){
					gameMode=gm;
					LCD_Clear(sfondo);
					if(gm==SingleBoard){
						selectOppositePlayerSingle();
					}else{
						selectOppositePlayerTwo();
					}
				}else if(sceltaOpposite==-1 && so!=-1 && iniziato==1){
					sceltaOpposite=so;
					LCD_Clear(sfondo);
					GUI_Text(16, 242, (uint8_t *) "    press INT0 to play    ", Black , sfondo);

				}
				else{
					if(insMuro==1 && attivo==1){
						confermaPosizioneMuro(posMuro.centro.riga, posMuro.centro.colonna, posMuro.direzione);
					}
					else if(attivo==1){
						confermaPosizionePedina(posPlayer.riga, posPlayer.colonna);
					}
				}
				break;
			default:
				break;
		}
	}
	else{
			Jselect=0;
	}
	
	enable_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

#include "button.h"
#include "../timer/timer.h"
#include "lpc17xx.h"
#include "../GLCD/GLCD.h" 
#include "../gioco/gioco.h"
#include "../scacchiera/scacchiera.h"
#include "../RIT/RIT.h"
#include "../CAN/CAN.h"

int attivo=0;
extern int turno;
extern int down0;
extern int down1;
extern int down2;
extern int P1Wall;
extern int P2Wall;
int insMuro=0;
int muriFiniti=0;


void EINT0_IRQHandler (void)	  	/* INT0			P2.10  PINSEL4 20-21		 */
{	
	down0=1;
	 
	enable_RIT();
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);

	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1	P2.11   PINSEL4 22-23 */
{
	if( ((turno==1 && P1Wall>0) || (turno==2 && P2Wall>0)) ){
		down1=1;
		enable_RIT();
		NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	}else if(muriFiniti==0) {
		muriFiniti=1;
		disable_timer(0);
		if(turno==1)
			GUI_Text(16, 242, (uint8_t *) " No walls, move the token " , Black, White);
		if(turno==2)
			GUI_Text(16, 242, (uint8_t *) " No walls, move the token " , Black, Red);
		enable_timer(0);
	}


	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{

	if(insMuro==1){
		down2=1;
		enable_RIT();
		NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
	}
	LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}



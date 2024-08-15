/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
//#include "../TouchPanel/TouchPanel.h"
#include "../gioco/gioco.h"
#include "../muro/muro.h"
#include "../pedina/pedina.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern int time;
extern int turno;
extern int tempoFinito;
extern pos_t posPlayer;
extern int mossa;
extern int spostato;
extern stato_t myscheda;



void TIMER0_IRQHandler (void)
{
	char s[6];
	time--;
	if(time>=0){
		s[0]= 32;
		s[1]=time/10 + '0';
		s[2]= time%10 + '0';
		s[3]= 32;
		s[4]= 115;
		s[5]= 32;
		if(turno==1)
			GUI_Text(97, 284, (uint8_t *) s , Black, White);
		else
			GUI_Text(97, 284, (uint8_t *) s , Black, Red);
	}
	else{
		disable_timer(0);
		if(mossa>0 && spostato==0)
			rimuoviPiccolo(posPlayer.riga,posPlayer.colonna);
		if(turno==1)
				GUI_Text(16, 242, (uint8_t *) "       time expired       ", Black , White);
		else
				GUI_Text(16, 242, (uint8_t *) "       time expired       ", Black , Red);
		DelayUS(10000 * 50);
		
		tempoFinito=1;
		cambiaTurno(turno);
	}
	

  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	myscheda.stato=-1;
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include "scacchiera/scacchiera.h"
#include "pedina/pedina.h"
#include "muro/muro.h"
#include "gioco/gioco.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"
#include "button_EXINT/button.h"
#include "CAN/CAN.h"

#define SIMULATOR 1
int turno=0;
int time;

extern pos_t posPlayer;
extern pos_t P1pos;
extern pos_t P2pos;
extern int down0;
extern int attivo;


posMuro_t Walls[16];

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	BUTTON_init_disable();
	joystick_init();											/* Joystick Initialization            */
	CAN_Init();
  LCD_Initialization();

	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec   */
	
	P1pos.riga=0;
	P1pos.colonna=3;
	P2pos.riga=6;
	P2pos.colonna=3;
	attivo=0;
	down0=0;
  enable_RIT();													/* RIT enabled												*/
	LCD_Clear(sfondo);
	
	init_timer(0, 0x017D7840);	//1s=0x017D7840 20s*25MHz 1DCD 6500
	init_timer(1, 0x05F5E100);	//5s

	NVIC_EnableIRQ(EINT0_IRQn);
	LPC_PINCON->PINSEL4		|= (1<<20);

	GUI_Text(16, 60, (uint8_t *) "      Q U O R I D O R     ", White , sfondo);
	GUI_Text(16, 242, (uint8_t *) "    press INT0 to start   ", Black , White);

	//timer 0e timer 1 a 25MHz
	time=20;

	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

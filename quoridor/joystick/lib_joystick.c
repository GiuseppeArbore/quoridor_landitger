/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/

void joystick_init(void) {
	//P1.25 per select posizione 20-21 in pinsel3
	//P1.26 per  down  posizione 20-21 in pinsel3
	//P1.27 per  left  posizione 22-23 in pinsel3
	//p1.28 per  right  posizione 24-24 in pinsel3
	//P1.29 per  up  posizione 26-27 in pinsel3
	
	
	/* joystick select functionality */
  LPC_PINCON->PINSEL3 &= ~(3<<26);	//PIN mode GPIO (00b value per P1.29) 
	LPC_GPIO1->FIODIR   &= ~(1<<29);	//P1.25 Input (joysticks on PORT1 defined as Input) 
	
	LPC_PINCON->PINSEL3 &= ~(3<<24);	//pin mode GPIO
	LPC_GPIO1->FIODIR   &= ~(1<<28);  //configurata in input
	
	LPC_PINCON->PINSEL3 &= ~(3<<22);	//pin mode GPIO
	LPC_GPIO1->FIODIR   &= ~(1<<27);  //configurata in input
	
	LPC_PINCON->PINSEL3 &= ~(3<<20);	//pin mode GPIO
	LPC_GPIO1->FIODIR   &= ~(1<<26);  //configurata in input
	
	LPC_PINCON->PINSEL3 &= ~(3<<18);	//pin mode GPIO
	LPC_GPIO1->FIODIR   &= ~(1<<25);  //configurata in input
	
}

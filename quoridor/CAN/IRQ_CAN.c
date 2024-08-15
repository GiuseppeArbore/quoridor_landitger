/*----------------------------------------------------------------------------
 * Name:    Can.c  pagina 352
 * Purpose: CAN interface for for LPC17xx with MCB1700
 * Note(s): see also http://www.port.de/engl/canprod/sv_req_form.html
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <lpc17xx.h>                  /* LPC17xx definitions */
#include "CAN.h"                      /* LPC17xx CAN adaption layer */
#include "../GLCD/GLCD.h"
#include "../gioco/gioco.h"

extern uint8_t icr ; 										//icr and result must be global in order to work with both real and simulated landtiger.
extern uint32_t result;
extern CAN_msg       CAN_TxMsg;    /* CAN message for sending */
extern CAN_msg       CAN_RxMsg;    /* CAN message for receiving */  
extern stato_t myscheda;





/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler (void)  {
	int turno;
	int mossaEffettuata;
	int VerticalOrr;
	int Y;
	int X;

  /* check CAN controller 1 */
	icr = 0;
  icr = (LPC_CAN1->ICR | icr) & 0xFF;               /* clear interrupts */

	
  if (icr & (1 << 0)) {                          		/* CAN Controller #1 meassage is received */
		CAN_rdMsg (1, &CAN_RxMsg);	                		/* Read the message */
    LPC_CAN1->CMR = (1 << 2);                    		/* Release receive buffer */
		if(CAN_RxMsg.data[0]==0xFF){ //se sto facendo handshake
			if(myscheda.stato==-1){//-1 nessuno ha cliccato; 0 lui ha cliccato e io no; 1 io ho cliccato e lui no; 2 entrambi abbiamo cliccato
				myscheda.stato=0;
				GUI_Text(16, 242, (uint8_t *) "Altro player: connesso", Red, White);
			}
			else if(myscheda.stato==1){
				myscheda.stato=2;
				GUI_Text(16, 242, (uint8_t *) "Siete entrambi connessi", Red, White);
			}
			else{
				GUI_Text(16, 242, (uint8_t *) "Errore: situazione anomala", Red, White);
			}
		}else{
			turno=CAN_RxMsg.data[0]+1;
			mossaEffettuata=((CAN_RxMsg.data[1]>>4) &0xf);
			VerticalOrr=(CAN_RxMsg.data[1] &0xf);
			Y=(CAN_RxMsg.data[2]);
			X=CAN_RxMsg.data[3];
			if(mossaEffettuata==1){
				Y++;
				X++;
			}		
			cambiaTurno2schede(turno, mossaEffettuata, VerticalOrr, Y, X);
		}		
	}
		

}

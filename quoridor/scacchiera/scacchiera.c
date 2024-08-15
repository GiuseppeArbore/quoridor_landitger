/* **--------------File Info---------------------------------------------------------------------------------
** File name:               scacchiera.c
** Descriptions:            funzioni per creare scacchiera
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
#include "../scacchiera/scacchiera.h"

void disegna_scacchiera(int X, int Y, int padding,int Xin,int Yin,int dimX,int dimY){//nrRighe, nrColonne, padding, daSx, daSopra, altezza, larghezza
	int i, j;
	int posX=Xin;
	int posY=Yin;
	for (i=0; i<Y; i++){
		posY=Yin;
		for(j=0; j<X; j++){
			disegna_riquadri(posX, posY, dimX, dimY, Black);
			posY+=dimX;
			posY+=padding;
		}
		posX+=dimY;
		posX+=padding;
	}
}
void disegna_riquadri(int Xpos, int Ypos, int dimX, int dimY, int colore){
	  LCD_DrawLine(Xpos,Ypos,Xpos+dimY,Ypos,colore);
		LCD_DrawLine(Xpos,Ypos,Xpos,Ypos+dimX,colore);
		LCD_DrawLine(Xpos+dimY,Ypos,Xpos+dimY,Ypos+dimX,colore);
		LCD_DrawLine(Xpos,Ypos+dimX,Xpos+dimY,Ypos+dimX,colore);
}



int convertiInPixel(int cordinata, int pixelIn, int padding, int dim){
		return pixelIn+((padding+dim)*cordinata);
}


void evidenzia_Human(int colore){  //int Xpos, int Ypos, int dimX, int dimY, int colore
	int i;
	for(i=0; i<4; i++){
		disegna_riquadri(61+i,161+i, 38-(2*i), 118-(2*i), colore);
	}
}
	
void evidenzia_NPC(int colore){  
	int i;
	for(i=0; i<4; i++)
		disegna_riquadri(61+i,221+i, 38-(2*i), 118-(2*i), colore);
}


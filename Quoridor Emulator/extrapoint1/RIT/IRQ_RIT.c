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
#include "LPC17xx.h"
#include "../GLCD/GLCD.h" 
#include "../button_EXINT/button.h"
#include "../timer/timer.h"
#include "../joystick/joystick.h"
#include "../Gioco/gioco.h"
#include <string.h>
#include <stdbool.h>
#include "../Gioco/gioco.h"
#include "../Gioco/muri.h"
/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down=0;
volatile int down2 =0;
extern mossa giocatoreincorso;
extern mossa g1;
extern mossa g2;
extern mossa ultimamossa;
extern muro muroattuale;
extern int giocatore;
extern int mg1;
extern int mg2;
int up= 0, left =0, right = 0, giu = 0;
int saltag;
int posmuro;
void RIT_IRQHandler (void)
{	
	bool controllo;
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		/* Joytick UP pressed */
	if(posmuro){
		spostamuro(muroattuale, 0, -2);

		}
		else{
				aggiornamossa(&up,&giu,&left, &right, &saltag);
				
				controllo = controllacaselle(giocatoreincorso, 0, -2);
				if(controllo){
					coloracasellascelta(giocatoreincorso.x, giocatoreincorso.y-2);
					up = 1;
					ultimamossa.x = giocatoreincorso.x;
					ultimamossa.y = giocatoreincorso.y-2;
				}
				saltag = scavalcagiocatore(giocatoreincorso, 0,-2);
				if(saltag){
					if(controllacaselle(giocatoreincorso, 0, -4)){
						coloracasellascelta(giocatoreincorso.x, giocatoreincorso.y-4);
						up = 1;
						ultimamossa.x = giocatoreincorso.x;
						ultimamossa.y = giocatoreincorso.y-4;
						saltag = 1;
					}
				}
			}
	}
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	//destra ->
		if(posmuro){
			spostamuro(muroattuale, 2, 0);
			
			
		}
		else{
		aggiornamossa(&up,&giu,&left, &right, &saltag);
		controllo = controllacaselle(giocatoreincorso, 2, 0);
		if(controllo){
			coloracasellascelta(giocatoreincorso.x+2, giocatoreincorso.y);
			right =1;
			ultimamossa.x = giocatoreincorso.x+2;
			ultimamossa.y = giocatoreincorso.y;
			}
			saltag = scavalcagiocatore(giocatoreincorso, 2,0);
			if(saltag){
				if(controllacaselle(giocatoreincorso, 4, 0)){
					coloracasellascelta(giocatoreincorso.x+4, giocatoreincorso.y);
					right = 1;
					ultimamossa.x = giocatoreincorso.x+4;
					ultimamossa.y = giocatoreincorso.y;
					saltag = 1;
				}
			}
		}
	}
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){ //sinistra <-
		if(posmuro){
			spostamuro(muroattuale, -2, 0);
			
			
			
		}
		else{
			aggiornamossa(&up,&giu,&left, &right, &saltag);
			controllo = controllacaselle(giocatoreincorso, -2, 0);
		if(controllo){
			coloracasellascelta(giocatoreincorso.x-2, giocatoreincorso.y);
			left =1;
			ultimamossa.x = giocatoreincorso.x-2;
			ultimamossa.y = giocatoreincorso.y;
			}
			saltag = scavalcagiocatore(giocatoreincorso, -2,0);
			if(saltag){
				if(controllacaselle(giocatoreincorso, -4, 0)){
					coloracasellascelta(giocatoreincorso.x-4, giocatoreincorso.y);
					left = 1;
					ultimamossa.x = giocatoreincorso.x-4;
					ultimamossa.y = giocatoreincorso.y;
					saltag = 1;
				}
			}
		}
	}
 	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	//giu
		if(posmuro){
			spostamuro(muroattuale, 0, 2);
		}
		else{
		aggiornamossa(&up,&giu,&left, &right, &saltag);
		controllo = controllacaselle(giocatoreincorso, 0, 2);
		if(controllo){
			coloracasellascelta(giocatoreincorso.x, giocatoreincorso.y+2);
			giu = 1;
			ultimamossa.x = giocatoreincorso.x;
			ultimamossa.y = giocatoreincorso.y+2;
		}
			saltag = scavalcagiocatore(giocatoreincorso, 0,2);
			if(saltag){
				if(controllacaselle(giocatoreincorso, 0, 4)){
					coloracasellascelta(giocatoreincorso.x, giocatoreincorso.y+4);
					giu = 1;
					ultimamossa.x = giocatoreincorso.x;
					ultimamossa.y = giocatoreincorso.y+4;
					saltag = 1;
				}
			}
		}
	}
 	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	//select
		if(posmuro){
			if(posizionamuro(muroattuale)){
				cambiaturno();
			}
		}
		else{
 		if(up || giu || left || right){
			if(saltag){
				if(up){
					decoloracasella(giocatoreincorso.x,giocatoreincorso.y-4);
				}
				else if(giu){
					decoloracasella(giocatoreincorso.x,giocatoreincorso.y+4);
				}
				else if(left){
					decoloracasella(giocatoreincorso.x-4,giocatoreincorso.y);
				}
				else if(right){
					decoloracasella(giocatoreincorso.x+4,giocatoreincorso.y);
				}
			}
 			confermagiocata(ultimamossa, giocatore);
			saltag = 0;	
			up = 0;
			giu = 0;
			left = 0;
			right = 0;
		}
	}
}
		/*button 1*/
	if(down!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
			if(giocatore == 1 && mg1 == 0){
				GUI_Text(130, 240, (uint8_t *) "MURI FINITI", Red, White);
				return;
			}
			else if(giocatore == 2 && mg2 == 0){
				GUI_Text(130, 240, (uint8_t *) "MURI FINITI", Red, White);
				return;
			}
			down++;
			switch(down){				
				case 2:				/* pay attention here: please see slides 19_ to understand value 2 */
					if(!posmuro){
						posmuro = 1;
						identificacaselle2(giocatoreincorso.x, giocatoreincorso.y);
						if(saltag){
								if(up){
									decoloracasella(giocatoreincorso.x,giocatoreincorso.y-4);
								}
								else if(giu){
									decoloracasella(giocatoreincorso.x,giocatoreincorso.y+4);
								}
								else if(left){
									decoloracasella(giocatoreincorso.x-4,giocatoreincorso.y);
								}
								else if(right){
									decoloracasella(giocatoreincorso.x+4,giocatoreincorso.y);
								}
							}
						up = 0;
						giu = 0;
						left = 0;
						right = 0;
						saltag = 0;
						muroattuale.x = 6;
						muroattuale.y = 5;
						creamuro(muroattuale);
					}
					else{
						posmuro = 0;
						cancellamuro(muroattuale);
						identificacaselle(giocatoreincorso.x,giocatoreincorso.y);
					}
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	/*button 2*/
	if(down2!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
			down2++;
			switch(down2){				
				case 2:				/* pay attention here: please see slides 19_ to understand value 2 */
					if(posmuro){
						cancellamuro(muroattuale);
						if(muroattuale.x %2 == 0){
							muroattuale.x = muroattuale.x+1;
							muroattuale.y = muroattuale.y-1;
						}
						else{
							muroattuale.x = muroattuale.x-1;
							muroattuale.y = muroattuale.y+1;
						}
						creamuro(muroattuale);
					}
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down2=0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}
	

void aggiornamossa(int *up, int *giu, int *left, int *right, int *s){
    if(*up){
		if(*s){
			coloracasella(giocatoreincorso.x, giocatoreincorso.y-4);
			*s = 0;
		}
		else{coloracasella(giocatoreincorso.x, giocatoreincorso.y-2);
		}
		*up = 0;
	}
	else if(*giu){
		if(*s){
			coloracasella(giocatoreincorso.x, giocatoreincorso.y+4);
			*s = 0;
		}
		else{coloracasella(giocatoreincorso.x, giocatoreincorso.y+2);}
		*giu = 0;
	}
	else if(*left ){
		if(*s){
			coloracasella(giocatoreincorso.x-4, giocatoreincorso.y);
			*s = 0;
		}
		else{coloracasella(giocatoreincorso.x-2, giocatoreincorso.y);}
		*left = 0;
	}
	else if(*right){
		if(*s){
			coloracasella(giocatoreincorso.x+4, giocatoreincorso.y);
			*s = 0;
		}
		else{coloracasella(giocatoreincorso.x+2, giocatoreincorso.y);}
		*right = 0;
	}
	return;
}
/******************************************************************************
**                            End Of File
******************************************************************************/

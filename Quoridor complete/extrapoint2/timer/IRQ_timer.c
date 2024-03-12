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
#include "../TouchPanel/TouchPanel.h"
#include "../Gioco/gioco.h"
#include <stdio.h>


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
int tempo=20;
char time_in_char[5] = "";
extern mossa giocatoreincorso;
int scaduto;
void TIMER0_IRQHandler (void)
{
	int i=0;
	tempo--;
	sprintf(time_in_char,"%d s   ", tempo);	
	GUI_Text(95, 280, (uint8_t *) time_in_char, Black, White);
	if(tempo == 0){
		sprintf(time_in_char,"Tempo scaduto");	
		GUI_Text(130, 240, (uint8_t *) time_in_char, Black, White);
		for(i=0; i< 15; i++) 	LCD_DrawLine(130,240+i,240,240+i,White);
		identificacaselle2(giocatoreincorso.x, giocatoreincorso.y);
		scaduto = 1;
		cambiaturno();
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
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

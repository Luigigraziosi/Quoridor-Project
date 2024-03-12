#include "twoboard.h"
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include <stdbool.h>
#include <stdio.h>
#include "Gioco/gioco.h"
#include "Gioco/muri.h"
#include <stdlib.h>

volatile int miogiocatore = 0;
extern uint32_t mossac;



void controllascheda(void){
	CAN_TxMsg.data[0] = 0xFF;
	CAN_TxMsg.len = 4;
	CAN_TxMsg.id = 1;
	CAN_TxMsg.format = STANDARD_FORMAT;
	CAN_TxMsg.type = DATA_FRAME;
	CAN_wrMsg(1, &CAN_TxMsg);
	return;
}

void mandamessaggiopronto(void){
	iniziagioco2();
	CAN_TxMsg.data[0] = 0xFF;
	CAN_TxMsg.len = 4;
	CAN_TxMsg.id = 1;
	CAN_TxMsg.format = STANDARD_FORMAT;
	CAN_TxMsg.type = DATA_FRAME;
	CAN_wrMsg(1, &CAN_TxMsg);
	return;
}

void inviamossa(void){
	CAN_TxMsg.data[0] = (mossac & 0xFF000000)>>24;
	CAN_TxMsg.data[1] = (mossac & 0x00FF0000)>>16;
	CAN_TxMsg.data[2] = (mossac & 0x0000FF00)>>8;
	CAN_TxMsg.data[3] = (mossac & 0x000000FF);
	CAN_TxMsg.len = 4;
	CAN_TxMsg.id = 1;
	CAN_TxMsg.format = STANDARD_FORMAT;
	CAN_TxMsg.type = DATA_FRAME;
	CAN_wrMsg(1, &CAN_TxMsg);
}



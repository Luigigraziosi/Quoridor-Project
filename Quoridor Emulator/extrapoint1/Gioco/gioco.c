#include "LPC17xx.h"
#include "../GLCD/GLCD.h" 
#include "../button_EXINT/button.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "../joystick/joystick.h"
#include <stdbool.h>
#include <stdio.h>
#include "gioco.h"
#include "muri.h"

uint32_t mossac;
extern muro muroattuale;
extern int posmuro;
mossa ultimamossa;
mossa g1;
mossa g2;
int mg1 = 8, mg2 = 8;
int moveOrPlace = 0;
mossa giocatoreincorso;
int giocatore;
char strg[20] = "";
extern int tempo;
extern int scaduto;
int m[13][13] ={0};

void coloragiocatore(int x, int y, uint16_t color){
	int i;
	int x1= x/2*6 + x/2*28+4+4;
	int y1=  y/2*6+ y/2*28+4+4;

	for(i = 0; i<21; i++){
		LCD_DrawLine(x1,y1+i,x1+21,y1+i,color);
	}		
	return;
}

void coloracasella(int x, int y){
	int i;
	int x1= x/2*6 + x/2*28+4+2;
	int y1=  y/2*6+ y/2*28+4+2;
	for(i = 0; i<26; i++){
		LCD_DrawLine(x1,y1+i,x1+25,y1+i,Yellow);
	}		
	return;
}
void quadratogiocatore(int g){
	int i = 0;
	if (g ==2){
		for(i=0;i<14;i++){
			LCD_DrawLine(100,241+i,114,241+i,Red);
		}
	}
	else{
		LCD_DrawLine(100,241,114,241,Black);
		LCD_DrawLine(100,241,100,255,Black);
		LCD_DrawLine(114,241,114,255,Black);
		LCD_DrawLine(100,255,114,255,Black);
	}
}
void identificacaselle(int x, int y){
	if(( x+2 < 13) && (m[x+2][y] == 0) && ( x+2 >= 0) && (m[x+1][y] == 0)) coloracasella(x+2,y);
	if(( y+2 < 13) && (m[x][y+2] == 0) && ( y+2 >= 0) && (m[x][y+1] == 0)) coloracasella(x,y+2);
	if(y-2 >= 0){
		if(( y-2 < 13) && (m[x][y-2] == 0) && (m[x][y-1] == 0)) coloracasella(x,y-2);
		if(y-4 >= 0){
			if(( y-4 < 13) && ((m[x][y-2] == 1)|| (m[x][y-2] == 2)) && (m[x][y-1] == 0)) coloracasella(x,y-4);
			}
	}
	if( x-2 >= 0){
		if(( x-2 < 13) && (m[x-2][y] == 0) && (m[x-1][y] == 0)) coloracasella(x-2,y);
		if( x-4 >= 0){
			if(( x-4 < 13) && ((m[x-2][y] == 1)|| (m[x-2][y] == 2)) && (m[x-1][y] == 0) ) coloracasella(x-4,y);
		}
	}
	if(( x+4 < 13) && ((m[x+2][y] == 1)|| (m[x+2][y] == 2)) && ( x+4 >= 0) && (m[x+1][y] == 0) ) coloracasella(x+4,y);
	if(( y+4 < 13) && ((m[x][y+2] == 1)|| (m[x][y+2] == 2)) && ( y+4 >= 0) && (m[x][y+1] == 0) ) coloracasella(x,y+4);
	return;
}
void decoloracasella(int x, int y){
	int i;
	int x1= x/2*6 + x/2*28+4+1;
	int y1=  y/2*6+ y/2*28+4+1;
	for(i = 0; i<27; i++){
		LCD_DrawLine(x1,y1+i,x1+26,y1+i,Blue2);
	}		
	return;
}
void identificacaselle2(int x, int y){
	if(( x+2 < 13) && (m[x+2][y] == 0) && ( x+2 >= 0)) decoloracasella(x+2,y);
	if(( y+2 < 13) && (m[x][y+2] == 0) && ( y+2 >= 0)) decoloracasella(x,y+2);	
	if(y-2 >= 0){
		if(( y-2 < 13) && (m[x][y-2] == 0)) decoloracasella(x,y-2);
		if(y-4 >= 0){
			if(( y-4 < 13) && ((m[x][y-2] == 1)|| (m[x][y-2] == 2))) decoloracasella(x,y-4);
			}
	}
	if( x-2 >= 0){
		if(( x-2 < 13) && (m[x-2][y] == 0)) decoloracasella(x-2,y);
		if( x-4 >= 0){
			if(( x-4 < 13) && ((m[x-2][y] == 1)|| (m[x-2][y] == 2))) decoloracasella(x-4,y);
		}
	}
	if(( x+4 < 13) && ((m[x+2][y] == 1)|| (m[x+2][y] == 2)) && ( x+4 >= 0)) decoloracasella(x+4,y);
	if(( y+4 < 13) && ((m[x][y+2] == 1)|| (m[x][y+2] == 2)) && ( y+4 >= 0)) decoloracasella(x,y+4);
	return;
}

void preparagioco(void){
	int i;
	LCD_Clear(Cyan);
	for(i=0;i<20;i++){
		LCD_DrawLine(95,40+i,115,40+i,Red);
	}
	for(i=0;i<20;i++){
		LCD_DrawLine(120,65+i,140,65+i,Red);
	}
	LCD_DrawLine(120,40,140,40,Black);
	LCD_DrawLine(120,60,140,60,Black);
	LCD_DrawLine(120,40,120,60,Black);
	LCD_DrawLine(140,40,140,60,Black);
	for(i=0;i<19;i++){
		LCD_DrawLine(121,41+i,139,41+i,White);
	}
	LCD_DrawLine(95,65,115,65,Black);
	LCD_DrawLine(95,85,115,85,Black);
	LCD_DrawLine(95,65,95,85,Black);
	LCD_DrawLine(115,65,115,85,Black);
	for(i=0;i<19;i++){
		LCD_DrawLine(96,66+i,114,66+i,White);
	}
	GUI_Text(78,90, (uint8_t *) " QUORIDOR ", Black, White);
	
	GUI_Text(10, 150, (uint8_t *) " We' re waiting for you...  ", Black, White);
	GUI_Text(0, 168, (uint8_t *) " Press the third button (INT0)", Black, White);
	
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
}

void iniziagioco(){
	int i,j;
	for(i=0;i<13;i++){
		for(j=0;j<13;j++){
			m[i][j] = 0;
		}
	}
	LCD_Clear2(Blue2);
	LCD_creagriglia();
	LCD_creamurietempo();
	g1.x = 6;
	g1.y = 0;
	g2.x = 6;
	g2.y = 12;
	giocatoreincorso = g1;
	m[6][0] = 1;
	m[6][12] = 2;
	coloragiocatore(6,0,White);
	coloragiocatore(6,12,Red);
	identificacaselle(6,0);
	giocatore = 1;
	tempo = 20;
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	init_timer(0, 0x17D7840 );						/* TIMER Initialization 1 s       	*/
	enable_timer(0);
	enable_RIT();
	sprintf(strg,"Giocatore %d:", giocatore);	
	GUI_Text(4, 240, (uint8_t *) "            ", Black, White);
	GUI_Text(4, 240, (uint8_t *) strg, Black, White);
	GUI_Text(95, 280, (uint8_t *) "20 s", Black, White);
	quadratogiocatore(giocatore);
	return;
}
bool controllacaselle(mossa g, int x1, int y1){
	if(x1 > 0){
		if(m[g.x+1][g.y] == 3) return false;
	}
	if(x1 < 0 && g.x-1 >= 0){
		if(m[g.x-1][g.y] == 3) return false;
	}
	if(y1 > 0){
		if(m[g.x][g.y+1] == 3) return false;
	}
	if(y1 < 0 && g.y-1 >= 0){
		if(m[g.x][g.y-1] == 3) return false;
	}
	if (g.x+x1 > 13) return false;
	if (g.x+x1 < 0) return false;
	if (g.y+y1 >=13) return false;
	if (g.y+y1 < 0) return false;
	if (m[g.x+x1][g.y+y1] != 0) return false;
	return true;
}
int scavalcagiocatore(mossa g, int x1, int y1){
	if(g.x+x1 < 0) return 0;
	if(g.y+y1 < 0) return 0;
	if (m[g.x+x1][g.y+y1] == 1 || m[g.x+x1][g.y+y1] == 2 ){
		return 1;
	}
	return 0;
}
void coloracasellascelta(int x, int y){
	int i;
	int x1= x/2*6 + x/2*28+4+2;
	int y1=  y/2*6+ y/2*28+4+2;
	
	for(i = 0; i<26; i++){
		LCD_DrawLine(x1,y1+i,x1+25,y1+i,Magenta);
	}		
	return;
}

void confermagiocata(mossa fatta, int giocatore){
	
	if(giocatore == 1){
		decoloracasella(g1.x,g1.y);
		identificacaselle2(g1.x, g1.y);
		m[g1.x][g1.y] = 0;
		g1.x = fatta.x;
		g1.y = fatta.y;
		m[g1.x][g1.y] = 1;
		coloragiocatore(g1.x,g1.y,White);
	}
	else {
		decoloracasella(g2.x,g2.y);
		identificacaselle2(g2.x, g2.y);
		m[g2.x][g2.y] = 0;
		g2.x = fatta.x;
		g2.y = fatta.y;
		m[g2.x][g2.y] = 2;
		coloragiocatore(g2.x,g2.y,Red);
	}
	cambiaturno();
return;
	
}


void cambiaturno(){
	codificamossa();

	if(g1.y == 12 || g2.y == 0){
		terminagioco(giocatore);
		return;
	}
	disable_timer(0);
	reset_timer(0);
	if(posmuro)	cancellamuro(muroattuale);
	posmuro = 0;
	if(giocatore == 1){
	giocatore = 2;
	giocatoreincorso.x = g2.x;
	giocatoreincorso.y = g2.y;
	}
	else{
	giocatore = 1;
	giocatoreincorso.x = g1.x;
	giocatoreincorso.y = g1.y;
	}
	sprintf(strg,"Giocatore %d:", giocatore);	
	GUI_Text(4, 240, (uint8_t *) "                            ", Black, White);
	GUI_Text(4, 240, (uint8_t *) strg, Black, White);
	quadratogiocatore(giocatore);
	tempo = 20;
	GUI_Text(95, 280, (uint8_t *) "20 s   ", Black, White);
	enable_timer(0);
	identificacaselle(giocatoreincorso.x,giocatoreincorso.y);
}

void terminagioco(int g){
	disable_timer(0);
	disable_RIT();
	LCD_Clear(Cyan);
	sprintf(strg," Complimeti giocatore  %d ", g);
	GUI_Text(20, 120, (uint8_t *) strg, Blue, Green);
	GUI_Text(80, 135, (uint8_t *) "HAI VINTO", Blue, Green);
}

uint8_t muroIntrappolaGiocatore(uint8_t g, uint8_t x, uint8_t y, uint8_t t, uint8_t s[DIM_SCACCHIERA][DIM_SCACCHIERA]){
	if(g == 0 && y == 12) // condizione finale
		return 0;
	else if(g == 1 && y == 0)
		return 0;
	s[x/2][y/2] = 1;
	// chiamate ricorsive
	if(g == 0){
		if(y + 2 < 13 && m[x][y+1] != 3 && t){ // down
			if(!s[x/2][(y+2)/2]){
				t = muroIntrappolaGiocatore(g, x, y + 2, t, s);
			}
		}
		if(x - 2 >= 0 && m[x-1][y] != 3 && t){ // left
			if(!s[(x-2)/2][y/2]){
				t = muroIntrappolaGiocatore(g, x - 2, y, t, s);
			}
		}
		if(x + 2 < 13 && m[x+1][y] != 3 && t){ // right
			if(!s[(x+2)/2][y/2]){
				t = muroIntrappolaGiocatore(g, x + 2, y, t, s);
			}
		}
		if(y - 2 >= 0 && m[x][y-1] != 3 && t){ // up
			if( !s[x/2][(y-2)/2]){
				t = muroIntrappolaGiocatore(g, x, y - 2, t, s);
			}
		}
	}
	else{
		if(y - 2 >= 0 && m[x][y-1] != 3 && t){ // up
			if(!s[x/2][(y-2)/2]){
				t = muroIntrappolaGiocatore(g, x, y - 2, t, s);
			}
		}
		if(x - 2 >= 0 && m[x-1][y] != 3 && t){ // left
			if( !s[(x-2)/2][y/2]){
				t = muroIntrappolaGiocatore(g, x - 2, y, t, s);
			}
		}
		if(x + 2 < 13 && m[x+1][y] != 3 && t){ // right
			if(!s[(x+2)/2][y/2]){
				t = muroIntrappolaGiocatore(g, x + 2, y, t, s);
			}
		}
		if(y + 2 < 13 && m[x][y+1] != 3 && t){ // down
			if(!s[x/2][(y + 2)/2]){
				t = muroIntrappolaGiocatore(g, x, y + 2, t, s);
			}
		}
	}
	return t;
}

int muroIntrappolaGiocatori(void){
	uint8_t x, y, i, j, scacchieraGiaEsplorata[DIM_SCACCHIERA][DIM_SCACCHIERA];
	x = muroattuale.x;
	y = muroattuale.y;
	for(i = 0; i < DIM_SCACCHIERA; i++)
		for(j = 0; j < DIM_SCACCHIERA; j++)
			scacchieraGiaEsplorata[i][j] = 0;
	if(x%2 == 0){
		m[x][y] = 3;
		m[x+1][y] = 3;
		m[x+2][y] = 3;
	}
	else{
		m[x][y] = 3;
		m[x][y+1] = 3;
		m[x][y+2] = 3;
	}
	if(muroIntrappolaGiocatore(0, g1.x, g1.y, 1, scacchieraGiaEsplorata)){
		if(x%2 == 0){
			m[x][y] = 0;
			m[x+1][y] = 0;
			m[x+2][y] = 0;
		}
		else{
			m[x][y] = 0;
			m[x][y+1] = 0;
			m[x][y+2] = 0;
		}
		return 1;
	}
	for(i = 0; i < DIM_SCACCHIERA; i++)
		for(j = 0; j < DIM_SCACCHIERA; j++)
			scacchieraGiaEsplorata[i][j] = 0;
	if(muroIntrappolaGiocatore(1, g2.x, g2.y, 1, scacchieraGiaEsplorata)){
		if(x%2 == 0){
			m[x][y] = 0;
			m[x+1][y] = 0;
			m[x+2][y] = 0;
		}
		else{
			m[x][y] = 0;
			m[x][y+1] = 0;
			m[x][y+2] = 0;
		}
		return 1;
	}
	if(x%2 == 0){
			m[x][y] = 0;
			m[x+1][y] = 0;
			m[x+2][y] = 0;
		}
		else{
			m[x][y] = 0;
			m[x][y+1] = 0;
			m[x][y+2] = 0;
		}
	return 0;
}


void codificamossa(){
	mossac = 0;
	if(giocatore == 1) mossac &= 0<<24;
	else mossac |= 1<<24;
	if(scaduto){
		mossac &= 0<<24;
		mossac |= 1<<20;
		scaduto = 0;
		moveOrPlace = 0;
		return;
	}
	if(moveOrPlace == 1){
		mossac |= 1<<20;
		
		if(muroattuale.x%2 == 0){
			mossac |= 1<<16 ;
			mossac |= (muroattuale.y)<<8;
			mossac |= (muroattuale.x+1);

		}
		else{
			mossac &= 0<<16 ;
			mossac |= (muroattuale.y+1)<<8;
			mossac |= (muroattuale.x);

		}
	}
	else{
		mossac &= 0<<20;
		mossac &= 0<<16;
		if(giocatore == 1){
			mossac |= g1.y<<8;
			mossac |= g1.x;
		}
		else{
			mossac |= g2.y<<8;
			mossac |= g2.x;
		}	
	}
  scaduto = 0;
  moveOrPlace = 0;
	return;
}





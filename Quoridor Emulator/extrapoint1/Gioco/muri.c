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

muro muroattuale;
extern int m[13][13];
extern int giocatore;
extern int mg1;
extern int mg2;
char strm[10];
extern int moveOrPlace;

int posizionamuro(muro ma){
	int x1,y1,i,l;
	if(ma.x%2 == 0){
		if(m[ma.x][ma.y] != 0)	return 0;
		if(m[ma.x+1][ma.y] != 0)	return 0;
		if(m[ma.x+2][ma.y] != 0)	return 0;
	}
	else{
		if(m[ma.x][ma.y] != 0)	return 0;
		if(m[ma.x][ma.y+1] != 0)	return 0;
		if(m[ma.x][ma.y+2] != 0)	return 0;
	}
	if(muroIntrappolaGiocatori()) return 0;
	
	if(giocatore == 1){
		mg1--;
		sprintf(strm,"  %d  ", mg1);	
		GUI_Text(23, 285, (uint8_t *) strm, Black, White);
	}
	else{
		mg2--;
		sprintf(strm,"  %d  ", mg2);	
		GUI_Text(173, 285, (uint8_t *) strm, Black, White);

	}
	if(ma.x%2 == 0){
		x1 = 4+28*ma.x/2+6*ma.x/2+1;
		y1 = 4+(ma.y+1)/2*28+ma.y/2*6+1;
		l = 28+28+4;
		for(i = 0;i<4;i++){
			LCD_DrawLine(x1,y1+i,x1+l,y1+i,Cyan);
		}
		m[ma.x][ma.y] = 3;
		m[ma.x+1][ma.y] = 3;
		m[ma.x+2][ma.y] = 3;
	}
	else{		
		x1 = 28*(ma.x+1)/2+4+6*(ma.x-1)/2+1;
		y1 = 4+ma.y/2*28+ma.y/2*6+1;
		l = 28+28+4;
		for(i =0;i<4;i++){
			LCD_DrawLine(x1+i,y1,x1+i,y1+l,Cyan);
		}
		m[ma.x][ma.y] = 3;
		m[ma.x][ma.y+1] = 3;
		m[ma.x][ma.y+2] = 3;
	}
	moveOrPlace = 1;

	return 1;
}

void creamuro(muro ma){
	int x1,y1,i,l;
	if(ma.x%2 == 0){
		x1 = 4+28*ma.x/2+6*ma.x/2+1;
		y1 = 4+(ma.y+1)/2*28+ma.y/2*6+1;
		l = 28+28+4;
		for(i = 0;i<5;i++){
			LCD_DrawLine(x1,y1+i,x1+l,y1+i,Green);
		}
	}
	else{		
		x1 = 28*(ma.x+1)/2+4+6*(ma.x-1)/2+1;
		y1 = 4+ma.y/2*28+ma.y/2*6+1;
		l = 28+28+4;
		for(i =0;i<5;i++){
			LCD_DrawLine(x1+i,y1,x1+i,y1+l,Green);
		}
	}	
	return;
}
void cancellamuro(muro ma){
	int x1,y1,l,i,j;
	for(j=0;j<3;j++){
		if((ma.x)%2 == 0){
			if(m[ma.x+j][ma.y] == 0){
				if(j%2 == 0){
					x1 = 4+28*(ma.x+j)/2+6*(ma.x+j)/2+1;
					y1 = 4+(ma.y+1)/2*28+(ma.y)/2*6+1;
					l = 28;
				}
				else{
					x1 = x1+28;
					l=6;
				}
				for(i = 0;i<5;i++){
					LCD_DrawLine(x1,y1+i,x1+l,y1+i,Blue2);
				}
			}
			else if(m[ma.x+j][ma.y] == 3){
				if(j%2 == 0){
					x1 = 4+28*(ma.x+j)/2+6*(ma.x+j)/2+1;
					y1 = 4+(ma.y+1)/2*28+(ma.y)/2*6+1;
					l = 28;
				}
				else{
					x1 = x1+28;
					l =6;
				}
				for(i = 0;i<5;i++){
					LCD_DrawLine(x1,y1+i,x1+l,y1+i,Cyan);
				}
			}
		}
		else{		
			if(m[ma.x][ma.y+j] == 0){
				if(j%2 == 0){
						x1 = 28*(ma.x+1)/2+4+6*(ma.x-1)/2+1;
						y1 = 4+(ma.y+j)/2*28+(ma.y+j)/2*6+1;
						l = 28;
					}
				else{
					y1 = y1+28;
					l =6;
					}
				for(i =0;i<5;i++){
					LCD_DrawLine(x1+i,y1,x1+i,y1+l,Blue2);
				}
			}
			else if(m[ma.x][ma.y+j] == 3){
				if(j%2 == 0){
						x1 = 28*(ma.x+1)/2+4+6*(ma.x-1)/2+1;
						y1 = 4+(ma.y+j)/2*28+(ma.y+j)/2*6+1;
						l = 28;
					}
				else{
					y1 = y1+28;
					l =6;
					}
				for(i =0;i<5;i++){
					LCD_DrawLine(x1+i,y1,x1+i,y1+l,Cyan);
				}
			}
		}
	}

	return;
}

void spostamuro(muro ma, int x, int y){
	if(ma.x%2 == 0){
		if(ma.x+x > 10) return;
		if(ma.x+x < 0) return;
		if(ma.y+y < 0) return;
		if(ma.y+y > 12) return;
		cancellamuro(ma);
		muroattuale.x+=x;
		muroattuale.y+=y;
		creamuro(muroattuale);
	}
	else{		
		if(ma.x+x > 12) return;
		if(ma.x+x < 0) return;
		if(ma.y+y < 0) return;
		if(ma.y+y > 10) return;
		cancellamuro(ma);
		muroattuale.x+=x;
		muroattuale.y+=y;
		creamuro(muroattuale);
	}	
	return;
}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

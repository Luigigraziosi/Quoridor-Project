#ifndef GIOCO_H
#define GIOCO_H

#include <stdbool.h>
#define DIM_SCACCHIERA 7
#include "muri.h"

typedef struct {
    int x;
    int y;
} mossa;

void coloragiocatore(int x, int y, uint16_t color);
void identificacaselle(int x, int y);
void coloracasella(int x, int y);
void iniziagioco(void);
void coloracasellascelta(int x, int y);
void decoloracasella(int x, int y);
void identificacaselle2(int x, int y);
void cambiaturno(void);
int scavalcagiocatore(mossa g, int x1, int y1);
void confermagiocata(mossa fatta, int giocatore);
bool controllacaselle(mossa g, int x1, int y1);
void terminagioco(int g);
uint8_t muroIntrappolaGiocatore(uint8_t g, uint8_t x, uint8_t y, uint8_t t, uint8_t s[DIM_SCACCHIERA][DIM_SCACCHIERA]);
int muroIntrappolaGiocatori(muro ma);
void menuiniziale(void);
void preparagioco(void);
void secondomenu(void);
int wrapperpo(mossa g1, int gioc, int per[]);
int percorsoottimo(int pos, int val[], int sol[], int mark[], int n, int k, int count, mossa g, int p);
int controllo(int pos, int sol[], int val[], int i, mossa g);
void mossanpc(void);
void codificamossa(int gioc, int scad, int mOP, muro m, mossa giocator );
int controllanuovaposizione(mossa g, int x1, int y1);
void iniziagioco2(void);
void iniziagioco3(void);

#endif // GIOCO_H

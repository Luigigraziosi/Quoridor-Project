
#ifndef MURI_H
#define MURI_H
typedef struct {
    int x;
    int y;
} muro;
void creamuro(muro ma);
void cancellamuro(muro ma);
void spostamuro(muro ma, int x, int y);
int posizionamuro(muro ma);
int mettimuronpc(int percorso[], int k);
void trovadirezionemuro(int p1, int p2);



#endif // MURI_H

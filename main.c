#include <stdio.h>

const int VUOTO   = 0;
const int PLAYER1 = 1;
const int PLAYER2 = 2;

/* una stampa tutti vuoti e una seconda per assegnare i player */

void inizializzazione(int *piano, int RIGHE, int COLONNE) {
    int r, c;
    for (r = 0; r < RIGHE; r++) {
        for (c = 0; c < COLONNE; c++) {
            if ((r % 2 == 0 && c % 2 == 0) || (r % 2 != 0 && c % 2 !=0)) { /*se righe e colonne sono entrambe pari o entrambe dispari stampa player1*/
                if (r < 3) {
                    piano[r * COLONNE + c] = PLAYER1;
                } else if (r > 3) {
                    piano[r * COLONNE + c] = PLAYER2;
                } else {
                    piano[r * COLONNE + c] = VUOTO;
                }
            }
        }
    }
}



void stampa(int *piano, int RIGA, int COLONNA) { /*RIGA/COLONNA=numero righe/colonne*/
    int r=0, c;
    for (r=0; r < RIGA; r++) { /*r=inidce riga, c=indice colonna*/
        printf("%d|", r); (r*COLONNA+c);
        for (c=0; c < COLONNA; c++) {
            int casella = piano[r * COLONNA + c];
            if (casella==PLAYER1) {
                printf(" o ");
            }
            else if (casella==PLAYER2){
                printf(" x ");
            }
            else {printf("   ");
            }
        }
        printf("\n");
    }

    printf("  ");
    for (c=0; c < COLONNA; c++)
        printf(" %d ", c);
    printf("\n");
}

int main() {
    int piano[7][7];
    inizializzazione(&(piano[0][0]), 7, 7);
    stampa(&(piano[0][0]), 7, 7);
    return 0;
}

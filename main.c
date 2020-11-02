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
    int r,c;
    for (r=0; r < RIGA; r++) { /*r=inidce riga, c=indice colonna*/
        printf("%d|", r); /*(r*COLONNA+c);*/
        for (c=0; c < COLONNA; c++) {
            int casella = piano[r * COLONNA + c];
            if (casella==PLAYER1) {
                printf(" o ");
            }
            else if (casella==PLAYER2){
                printf(" x ");
            } else {printf("   ");
            }
        }
        printf("\n");
    }

    printf("  ");
    for (c=0; c < COLONNA; c++)
        printf(" %d ", c);
    printf("\n");
}

int seleziona_pedina (int* piano, int COLONNE, int x, int y, int turno){ /*controllo se pedina corrisponde al giocatore giusto*/
    if(piano[y*COLONNE+x]==turno){
        return 1;
    } else {
        printf("scelta sbagliata\n");
        return 0;
    }
}

int seleziona_mossa(int* piano, int COLONNE, int x, int y, int* turno, int move_x, int move_y){
    if(*turno==PLAYER1){
        if((move_y == y+1) && ((move_x == x+1) || (move_x == x-1))){/*verifica se destinazione è valida per il giocatore non promosso*/
         piano[move_y*COLONNE+move_x] = *turno; /*assegnamento alla casella del player*/
         piano[y*COLONNE+x] = VUOTO; /*svuota casella old*/
            *turno = PLAYER2;
            return 1;
        }
    } else{
        if((move_y == y-1) && ((move_x == x+1)||(move_x == x-1))){
            piano[move_y*COLONNE+move_x] = *turno;
            piano[y*COLONNE+x]= VUOTO;
            *turno = PLAYER1;
            return 1;
        }
    return 0;
    }
}


int main() {
    int piano[7][7];
    int x,y,move_x,move_y;
    int turno=PLAYER1;
    inizializzazione(&(piano[0][0]), 7, 7);
    stampa(&(piano[0][0]), 7, 7);
    while(1){
        printf("\nTURNO GIOCATORE %d \n",turno);
        printf("\nseleziona le coordinate della pedina x , y\n");
        scanf("%d %d",&x,&y); /*inserimento pedina da controllare*/
        if(seleziona_pedina(piano,7,x,y,turno)){
            printf("seleziona le coordinate in cui muovere la pedina x , y\n");
            scanf("%d %d",&move_x,&move_y); /*inserimento coordinate destinazione pedina*/
           if (seleziona_mossa(piano,7,x,y,&turno,move_x,move_y)){ /*se la mossa è valida stampa piano*/
               stampa(piano,7,7);
           }
        }
    }

    return 0;
}

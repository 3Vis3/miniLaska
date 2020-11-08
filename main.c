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

/*void cambio_turno(int* turno){
    if(*turno == PLAYER1){
        *turno = PLAYER2;
    }else{
        *turno = PLAYER1;
    }
}*/

int seleziona_pedina (int* piano, int COLONNE, int x, int y, int* turno){ /*controllo se pedina corrisponde al giocatore giusto*/
    if(piano[y*COLONNE+x]==*turno){
        return 1;
    } else {
        printf("scelta sbagliata\n");
        return 0;
    }
}

int seleziona_mossa(int* piano, int COLONNE, int x, int y, int* turno, int move_x, int move_y){
    if(*turno==PLAYER1){
        if((move_y == y+1) && ((move_x == x+1) || (move_x == x-1))){ /*verifica se destinazione è valida per il giocatore non promosso*/
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
/*TODO, array per passare più destinazioni di possibili mangiate con stessa pedina*/
void mangiata(int* piano, int COLONNE, int y, int x, int* turno, int dest_y, int dest_x){ /*controllo se destinazione è valida*/
    int move_x, move_y;
    printf("TURNO GIOCATORE %d\n", *turno);
    printf("Devi mangiare l'avversario con la pedina nelle coordinate x:%d y:%d\n", x, y);
    printf("Inserisci le coordinate in cui fare la mangiata\n");
    scanf("%d %d", &move_x, &move_y);
    if(piano[move_y* COLONNE + move_x] == VUOTO && move_y==dest_y && move_x==dest_x){
        piano[y*COLONNE + x] = VUOTO;
        piano[move_y*COLONNE + move_x] = *turno;
        if(*turno == PLAYER1){
            *turno = PLAYER2;
        }else if (*turno == PLAYER2) {
            *turno = PLAYER1;
        }
        stampa(piano,7,7);
        /*torrefazione*/
    }
    else {
        printf("Hai sbagliato selezione\n");
        mangiata(piano, COLONNE, y, x, turno, dest_y, dest_x);
    }
    /*cambio_turno(&turno);*/
}

void controllo_mangiata(int* piano, int RIGHE, int COLONNE, int* turno){
    int i, j;
    char selection; /*variabili char che da scanf riceve s oppure n e fa il confronto tra selezione e i caratteri n o s*/
    char no= 'n'; /*TODO SISTEMARE LOOP INFINITO SE INSERISCI SN*/
    char si= 's';
    int count=0; /*conta le pedine obbligate a mangiare*/
        for (i = 0; i < RIGHE; i++) {
            for (j = 0; j < COLONNE; j++) {
                if (piano[i * COLONNE + j] == *turno) {
                    if (*turno == PLAYER1 /*|| PLAYER2 == promoted*/) {
                        if (i < 5 && j > 1 && piano[(i + 1) * COLONNE + (j - 1)] != *turno &&
                            /*ho inserito degli and per evitare il controllo a sx se sei vicino al bordo sx e viceversa a dx*/
                            piano[(i + 1) * COLONNE + (j - 1)] != VUOTO) {
                            /*controllo diagonale sinistra che sia player avversario e non vuoto*/
                            if (piano[(i + 2) * COLONNE + (j - 2)] == VUOTO) {
                                /*TODO funzione che controlla se esce dall'array del piano*/
                                /*passo i e j come coordinate per mangiata obbligatoria*/
                                count++; /*incrementa il conteggio se rilevata una pedina obbligata*/
                                printf("TURNO GIOCATORE %d\n", *turno);
                                printf("vuoi mangiare con x=%d y=%d? s/n\n", j, i);
                                scanf(" %c", &selection); /*chiede se vuoi mangiare con quella pedina*/
                                if (selection==si) { /*se dici si chiama mangiata*/
                                    mangiata(piano, COLONNE, i, j, turno, i + 2, j - 2);
                                } else if (selection==no) { /*se dici no prosegue*/
                                }
                            }
                        } else if (i < 5 && j < 5 && piano[(i + 1) * COLONNE + (j + 1)] != *turno &&
                                   piano[(i + 1) * COLONNE + (j + 1)] != VUOTO) {
                            /*controllo diagonale destra che sia player avversario e non vuoto*/
                            if (piano[(i + 2) * COLONNE + (j + 2)] == VUOTO) {
                                /*TODO far scegliere a giocatore quale manigata effettuare*/
                                /*passo i e j come coordinate per mangiata obbligatoria*/
                                count++;
                                printf("TURNO GIOCATORE %d\n", *turno);
                                printf("vuoi mangiare con x=%d y=%d? s/n\n", j, i);
                                scanf(" %c", &selection);
                                if (selection==si) {
                                mangiata(piano, COLONNE, i, j, turno, i + 2, j + 2);
                            } else if (selection==no) {
                                }
                            }
                        }
                    }
                    if (*turno == PLAYER2 /*|| PLAYER1 == promoted*/) {
                        if (i > 1 && j > 1 && piano[(i - 1) * COLONNE + (j - 1)] != *turno &&
                            piano[(i - 1) * COLONNE + (j - 1)] != VUOTO) {
                            /*controllo diagonale sinistra che sia player avversario e non vuoto*/
                            if (piano[(i - 2) * COLONNE + (j - 2)] == VUOTO) {
                                /*TODO far scegliere a giocatore quale manigata effettuare, con più di una*/
                                /*passo i e j come coordinate per mangiata obbligatoria*/
                                count++;
                                printf("TURNO GIOCATORE %d\n", *turno);
                                printf("vuoi mangiare con x=%d y=%d? s/n\n", j, i);
                                scanf(" %c", &selection);
                                if (selection==si) {
                                mangiata(piano, COLONNE, i, j, turno, i - 2, j - 2);
                            } else if (selection==no) {
                                    }
                            }
                        }
                        if (i > 1 && j < 5 && piano[(i - 1) * COLONNE + (j + 1)] != *turno &&
                            piano[(i - 1) * COLONNE + (j + 1)] != VUOTO) {
                            /*controllo diagonale destra che sia player avversario e non vuoto*/
                            if (piano[(i - 2) * COLONNE + (j + 2)] == VUOTO) {
                                /*TODO far scegliere a giocatore quale manigata effettuare*/
                                /*passo i e j come coordinate per mangiata obbligatoria*/
                                count++;
                                printf("TURNO GIOCATORE %d\n", *turno);
                                printf("vuoi mangiare con x=%d y=%d? s/n\n", j, i);
                                scanf(" %c", &selection);
                                if (selection==si) {
                                mangiata(piano, COLONNE, i, j, turno, i - 2, j + 2);
                                }
                            }
                        }
                    }
                }
            }
        } if (count!=0) { /*se il conteggio delle obbligate è diverso da zero riparte il controllo*/
        controllo_mangiata(piano, RIGHE, COLONNE, turno);
        } else { /*se il conteggio è uguale a zero ritorna al main*/

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
        printf("\nseleziona le coordinate della pedina x , y: \n");
        scanf("%d %d",&x,&y); /*inserimento pedina da controllare*/ /*TODO, RICHIEDERE SELEZIONE SE INSERISCI non numeri*/
        if (x>=0 && x<=6 && y>=0 && y<=6) {
            if(seleziona_pedina(piano,7,x,y,&turno)){
                printf("\nHai selezionato le coordinate x=%d y=%d\n", x, y);
                printf("seleziona le coordinate in cui muovere la pedina x , y: \n");
                scanf("%d %d",&move_x,&move_y); /*inserimento coordinate destinazione pedina*/
                if (seleziona_mossa(piano,7,x,y,&turno,move_x,move_y)){ /*se la mossa è valida stampa piano*/
                    stampa(piano,7,7);
                    controllo_mangiata(piano,7,7,&turno);
                }
            }
        }
    }
    return 0;
}

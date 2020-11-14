#include <stdio.h>
#define RIGHE 7
#define COLONNE 7
#define LIMITright 6
#define LIMITleft 0
#define LIMITdown 6
#define LIMITup 0


const int VUOTO   = 0;  /* nessun giocatore*/
const int PLAYER1 = 1;
const int PLAYER2 = 2;
const int PLAYER1_PRO = 3;
const int PLAYER2_PRO = 4;

typedef int coordinate;
typedef int bool;

typedef
struct tower{ /*struttura per le pedine, contiene il giocatore e un'array chessboard indica quanto alta è la pedina e da cosa è composta*/
    int player;
    int composition[3]; /*1 per o 2 per x 3 O e 4 X  N.B. l'elemento composition[0] è la testa della torre*/
} tower_t;

void initialization_chessboard(tower_t *chessboard){ /*funzione chessboard inizializza la scacchiera e anche le pedine N.B. sarà chiamata una volta sola */
    int r, c;
    for (r = 0; r < RIGHE; r++) {
        for (c = 0; c < COLONNE; c++) {
            if (r < 3 && ((r % 2 == 0 && c % 2 == 0) || (r % 2 != 0 && c % 2 !=0))) { /*se ci troviamo sulla parte superiore della scacchiera e si presentano le condizioni segnate*/
                chessboard[r * COLONNE + c].player = PLAYER1;                          /*assegnamo il posto al giocatore 1 e componiamo la sua torre*/
                chessboard[r * COLONNE + c].composition[0] = PLAYER1;
                chessboard[r * COLONNE + c].composition[1] = 0;
                chessboard[r * COLONNE + c].composition[2] = 0;
            }else if (r > 3 && ((r % 2 == 0 && c % 2 == 0) || (r % 2 != 0 && c % 2 !=0))) { /*se ci troviamo sulla parte della scacchiera inferiore e si presenterano le condizioni segnate*/
                chessboard[r * COLONNE + c].player = PLAYER2;                               /*assegnamo il posto al giocatore 1 e componiamo la sua torre*/
                chessboard[r * COLONNE + c].composition[0] = PLAYER2;
                chessboard[r * COLONNE + c].composition[1] = 0;
                chessboard[r * COLONNE + c].composition[2] = 0;
            } else {
                chessboard[r * COLONNE + c].player = VUOTO;
            }
        }
    }
} /*inizializzazione delle scacchiera N.B. viene chiamata solo la prima volta*/

void stampa(tower_t *chessboard) { /*funzione di stampa chessboard verrà chiamato ogni tutte le volte chessboard viene fatta una giocata*/
    int r,c;
    for (r=0; r < RIGHE; r++) {
        printf("%d|", r); /*stampa l'indice delle righe*/
        for (c=0; c < COLONNE; c++) {
            int casella = chessboard[r * COLONNE + c].player;
            if (casella==PLAYER1) {
                printf(" o ");
            } else if (casella==PLAYER2){
                printf(" x ");
            } else{
                printf("   ");
            }
        }
        printf("\n");
    }

    printf("  "); /*ciclo per stampare l'indice delle colonne*/
    for (c=0; c < COLONNE; c++)
        printf(" %d ", c);

    printf("\n");
}/*funzione di stampa chessboard verrà chiamato ogni tutte le volte chessboard viene fatta una giocata*/

void cambio_turno(int* turno){
    if(*turno == PLAYER1){
        *turno = PLAYER2;
    }else{
        *turno = PLAYER1;
    }
}

bool control_range(coordinate x, coordinate y){
    if(x <= LIMITleft || x >= LIMITright || y <= LIMITup || y >= LIMITdown) {
        printf("COORDINATE FUORI RANGE\n");
        return 0;
    }else
        return 1;
}/*controlla che x e y siano dentro la scacchiera se sono all'interno restituisce 1 altrimenti restituisce 0*/

bool diagonal_check(tower_t *chessboard, coordinate x, coordinate y, int turno){
    if(turno == PLAYER1){
        if(x == LIMITleft && chessboard[(y + 1) * COLONNE + (x + 1)].player == VUOTO){ /*se il giocatore selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMITright && chessboard[(y + 1) * COLONNE + (x - 1)].player == VUOTO){/*se il giocatore selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x>LIMITleft && x<LIMITright && (chessboard[(y + 1) * COLONNE + (x - 1)].player == VUOTO || chessboard[(y + 1) * COLONNE + (x + 1)].player == VUOTO)){/*se il giocatore selezionato non si trova alle estremità controlla tutte le diagonali disponibili*/
            return 0;
        }else
            return 1;
    }else if(turno == PLAYER2){
        if(x == LIMITleft && chessboard[(y - 1) * COLONNE + (x + 1)].player == VUOTO){ /*se il giocatore selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMITright && chessboard[(y - 1) * COLONNE + (x - 1)].player == VUOTO){/*se il giocatore selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x>LIMITleft && x<LIMITright && (chessboard[(y - 1) * COLONNE + (x - 1)].player == VUOTO || chessboard[(y - 1) * COLONNE + (x + 1)].player == VUOTO)){/*se il giocatore selezionato non si trova alle estremità controlla tutte le diagonali disponibili*/
            return 0;
        }else
            return 1;
    }else if(turno == PLAYER1_PRO){
        if(x == LIMITleft && y == LIMITup && chessboard[(y + 1) * COLONNE + (x + 1)].player == VUOTO){ /*se il giocatore1 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMITright && y == LIMITup && chessboard[(y + 1) * COLONNE + (x - 1)].player == VUOTO){/*se il giocatore1 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMITleft && y == LIMITdown && chessboard[(y - 1) * COLONNE + (x + 1)].player == VUOTO){/*se il giocatore1 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMITright && y == LIMITdown && chessboard[(y - 1) * COLONNE + (x - 1)].player == VUOTO){/*se il giocatore1 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x < LIMITright && x>LIMITleft && y == LIMITup && (chessboard[(y + 1) * COLONNE + (x - 1)].player == VUOTO) || (chessboard[(y + 1) * COLONNE + (x + 1)].player == VUOTO)){/*se il giocatore1 PRO selezionato si trova sulla prima riga della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x < LIMITright && x>LIMITleft && y == LIMITdown && (chessboard[(y - 1) * COLONNE + (x - 1)].player == VUOTO) || (chessboard[(y + 1) * COLONNE + (x + 1)].player == VUOTO)){/*se il giocatore1 PRO selezionato si trova sulla prima riga della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x == LIMITleft && y<LIMITdown && y>LIMITup && (chessboard[(y + 1) * COLONNE + (x + 1)].player == VUOTO) || (chessboard[(y - 1) * COLONNE + (x + 1)].player == VUOTO)){/*se il giocatore1 PRO selezionato si trova sulla prima colonna della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x == LIMITright && y<LIMITdown && y>LIMITup && (chessboard[(y - 1) * COLONNE + (x - 1)].player == VUOTO) || (chessboard[(y + 1) * COLONNE + (x - 1)].player == VUOTO)){/*se il giocatore1 PRO selezionato si trova sull'ultima colonna della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x>LIMITleft && x<LIMITright  && y>LIMITup && y<LIMITdown && (chessboard[(y - 1) * COLONNE + (x - 1)].player == VUOTO || chessboard[(y - 1) * COLONNE + (x + 1)].player == VUOTO || chessboard[(y + 1) * COLONNE + (x + 1)].player == VUOTO || chessboard[(y + 1) * COLONNE + (x - 1)].player == VUOTO)){/*se il giocatore1 PRO selezionato si trova dove tutte diagonali sono disponibili controlla tutte le diagonali disponibili*/
            return 0;
        }else
            return 1;
    }
    else{
        if(x == LIMITleft && y == LIMITup && chessboard[(y + 1) * COLONNE + (x + 1)].player == VUOTO){ /*se il giocatore2 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMITright && y == LIMITup && chessboard[(y + 1) * COLONNE + (x - 1)].player == VUOTO){/*se il giocatore2 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMITleft && y == LIMITdown && chessboard[(y - 1) * COLONNE + (x + 1)].player == VUOTO){/*se il giocatore2 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMITright && y == LIMITdown && chessboard[(y - 1) * COLONNE + (x - 1)].player == VUOTO){/*se il giocatore2 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x < LIMITright && x>LIMITleft && y == LIMITup && (chessboard[(y + 1) * COLONNE + (x - 1)].player == VUOTO) || (chessboard[(y + 1) * COLONNE + (x + 1)].player == VUOTO)){/*se il giocatore2 PRO selezionato si trova sulla prima riga della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x < LIMITright && x>LIMITleft && y == LIMITdown && (chessboard[(y - 1) * COLONNE + (x - 1)].player == VUOTO) || (chessboard[(y + 1) * COLONNE + (x + 1)].player == VUOTO)){/*se il giocatore2 PRO selezionato si trova sulla prima riga della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x == LIMITleft && y<LIMITdown && y>LIMITup && (chessboard[(y + 1) * COLONNE + (x + 1)].player == VUOTO) || (chessboard[(y - 1) * COLONNE + (x + 1)].player == VUOTO)){/*se il giocatore1 PRO selezionato si trova sulla prima colonna della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x == LIMITright && y<LIMITdown && y>LIMITup && (chessboard[(y - 1) * COLONNE + (x - 1)].player == VUOTO) || (chessboard[(y + 1) * COLONNE + (x - 1)].player == VUOTO)){/*se il giocatore1 PRO selezionato si trova sull'ultima colonna della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x>LIMITleft && x<LIMITright  && y>LIMITup && y<LIMITdown && (chessboard[(y - 1) * COLONNE + (x - 1)].player == VUOTO || chessboard[(y - 1) * COLONNE + (x + 1)].player == VUOTO || chessboard[(y + 1) * COLONNE + (x + 1)].player == VUOTO || chessboard[(y + 1) * COLONNE + (x - 1)].player == VUOTO)){/*se il giocatore2 PRO selezionato si trova dove tutte diagonali sono disponibili controlla tutte le diagonali disponibili*/
            return 0;
        }else
            return 1;
    }

}/*verifica le 2 diagonali nel caso di giocatore non promosso altrimenti controlla le 4 diagonali per il giocatore promosso,
 * restituisce 0 se almeno una digaonale è libera, altrimenti restituisce 1*/

bool seleziona_pedina(tower_t *chessboard, coordinate x, coordinate y, int turno){ /*controllo se pedina corrisponde al giocatore giusto*/
    if(control_range(x,y) && chessboard[y*COLONNE+x].player == turno){ /*chiama la funzione control range chessboard controlla se i range sono corretti*/
        if(!diagonal_check(chessboard, x, y, turno))/*chiama la funzione per controllare le diagonali, se almeno una diagonale è libera allora può continuare*/
            return 1;
        else{
            printf("LA PEDINA IN COORDINATE x = %d   y = %d NON PUO MUOVERSI\n", x, y);
            printf("SCELTA SBAGLIATA\n");
            return 0;
        }
    } else {
        printf("SCELTA SBAGLIATA\n");
        return 0;
    }
}/*restituisce 1 se la pedina è in una posizione in cui può essere scelta altimenti restituisce 0*/

int seleziona_mossa(tower_t *chessboard, coordinate x, coordinate y, int turno, coordinate move_x, coordinate move_y){
    if(turno==PLAYER1){
        if((move_y == y+1) && ((move_x == x+1) || (move_x == x-1)) && chessboard[move_y * COLONNE + move_x].player == VUOTO && control_range(move_x, move_y)){ /*verifica se destinazione è valida e verifica che in quella posizione non ci siano altre pedine e che il range sia corretto*/
         chessboard[move_y*COLONNE+move_x].player = turno; /*assegnamento alla casella del player*/
         chessboard[y*COLONNE+x].player = VUOTO; /*svuota casella old*/
            return 1;
        }
    } else if(turno==PLAYER2){
        if((move_y == y-1) && ((move_x == x+1)||(move_x == x-1)) && chessboard[move_y * COLONNE + move_x].player == VUOTO && control_range(move_x, move_y)){ /*verifica se destinazione è valida e verifica che in quella posizione non ci siano altre pedine e che il range sia corretto*/
            chessboard[move_y*COLONNE+move_x].player = turno;
            chessboard[y*COLONNE+x].player = VUOTO;
            return 1;
        }
        /*TODO fare per promossi*/
    }
    printf("MOSSA NON VALIDA\n");
    return 0;
}
/*TODO, array per passare più destinazioni di possibili mangiate con stessa pedina*/
void mangiata(tower_t *chessboard, coordinate y, coordinate x, int* turno, coordinate dest_y, coordinate dest_x){ /*controllo se destinazione è valida*/
    int move_x, move_y;
    printf("TURNO GIOCATORE %d\n", *turno);
    printf("Devi mangiare l'avversario con la pedina nelle coordinate x:%d y:%d\n", x, y);
    printf("Inserisci le coordinate in cui fare la mangiata\n");
    scanf("%d %d", &move_x, &move_y);
    if(chessboard[move_y * COLONNE + move_x].player == VUOTO && move_y==dest_y && move_x==dest_x){
        chessboard[y*COLONNE + x].player = VUOTO;
        chessboard[move_y*COLONNE + move_x].player = *turno;
        if(*turno == PLAYER1){
            *turno = PLAYER2;
        }else if (*turno == PLAYER2) {
            *turno = PLAYER1;
        }
        stampa(chessboard);
        /*torrefazione*/
    }
    else {
        printf("Hai sbagliato selezione\n");
        mangiata(chessboard, y, x, turno, dest_y, dest_x);
    }
    /*cambio_turno(&turno);*/
}

void controllo_mangiata(tower_t *chessboard, int *turno){
    int i, j;
    char selection; /*variabili char chessboard da scanf riceve s oppure n e fa il confronto tra selezione e i caratteri n o s*/
    char no= 'n'; /*TODO SISTEMARE LOOP INFINITO SE INSERISCI SN*/
    char si= 's';
    int count=0; /*conta le pedine obbligate a mangiare*/
        for (i = 0; i < RIGHE; i++) {
            for (j = 0; j < COLONNE; j++) {
                if (chessboard[i * COLONNE + j].player == *turno) {
                    if (*turno == PLAYER1 /*|| PLAYER2 == promoted*/) {
                        if (i < 5 && j > 1 && chessboard[(i + 1) * COLONNE + (j - 1)].player != *turno && chessboard[(i + 1) * COLONNE + (j - 1)].player != VUOTO) {
                            /*ho inserito degli and per evitare il controllo a sx se sei vicino al bordo sx e viceversa a dx*/
                            /*controllo diagonale sinistra chessboard sia player avversario e non vuoto*/
                            if (chessboard[(i + 2) * COLONNE + (j - 2)].player == VUOTO) {
                                /*TODO funzione chessboard controlla se esce dall'array del chessboard*/
                                /*passo i e j come coordinate per mangiata obbligatoria*/
                                count++; /*incrementa il conteggio se rilevata una pedina obbligata*/
                                printf("TURNO GIOCATORE %d\n", *turno);
                                printf("vuoi mangiare con x=%d y=%d? s/n\n", j, i);
                                scanf(" %c", &selection); /*chiede se vuoi mangiare con quella pedina*/
                                if (selection==si) { /*se dici si chiama mangiata*/
                                    mangiata(chessboard, i, j, turno, i + 2, j - 2);
                                } else if (selection==no) { /*se dici no prosegue*/
                                }
                            }
                        } else if (i < 5 && j < 5 && chessboard[(i + 1) * COLONNE + (j + 1)].player != *turno &&
                                   chessboard[(i + 1) * COLONNE + (j + 1)].player != VUOTO) {
                            /*controllo diagonale destra chessboard sia player avversario e non vuoto*/
                            if (chessboard[(i + 2) * COLONNE + (j + 2)].player == VUOTO) {
                                /*TODO far scegliere a giocatore quale manigata effettuare*/
                                /*passo i e j come coordinate per mangiata obbligatoria*/
                                count++;
                                printf("TURNO GIOCATORE %d\n", *turno);
                                printf("vuoi mangiare con x=%d y=%d? \n", j, i);
                                scanf(" %c", &selection);
                                if (selection==si) {
                                mangiata(chessboard, i, j, turno, i + 2, j + 2);
                            } else if (selection==no) {
                                }
                            }
                        }
                    }
                    if (*turno == PLAYER2 /*|| PLAYER1 == promoted*/) {
                        if (i > 1 && j > 1 && chessboard[(i - 1) * COLONNE + (j - 1)].player != *turno &&
                            chessboard[(i - 1) * COLONNE + (j - 1)].player != VUOTO) {
                            /*controllo diagonale sinistra chessboard sia player avversario e non vuoto*/
                            if (chessboard[(i - 2) * COLONNE + (j - 2)].player == VUOTO) {
                                /*TODO far scegliere a giocatore quale manigata effettuare, con più di una*/
                                /*passo i e j come coordinate per mangiata obbligatoria*/
                                count++;
                                printf("TURNO GIOCATORE %d\n", *turno);
                                printf("vuoi mangiare con x=%d y=%d? s/n\n", j, i);
                                scanf(" %c", &selection);
                                if (selection==si) {
                                mangiata(chessboard, i, j, turno, i - 2, j - 2);
                            } else if (selection==no) {
                                    }
                            }
                        }
                        if (i > 1 && j < 5 && chessboard[(i - 1) * COLONNE + (j + 1)].player != *turno &&
                            chessboard[(i - 1) * COLONNE + (j + 1)].player != VUOTO) {
                            /*controllo diagonale destra chessboard sia player avversario e non vuoto*/
                            if (chessboard[(i - 2) * COLONNE + (j + 2)].player == VUOTO) {
                                /*TODO far scegliere a giocatore quale manigata effettuare*/
                                /*passo i e j come coordinate per mangiata obbligatoria*/
                                count++;
                                printf("TURNO GIOCATORE %d\n", *turno);
                                printf("vuoi mangiare con x=%d y=%d? s/n\n", j, i);
                                scanf(" %c", &selection);
                                if (selection==si) {
                                mangiata(chessboard, i, j, turno, i - 2, j + 2);
                                }
                            }
                        }
                    }
                }
            }
        } if (count!=0) { /*se il conteggio delle obbligate è diverso da zero riparte il controllo*/
        controllo_mangiata(chessboard, turno);
        } else { /*se il conteggio è uguale a zero ritorna al main*/

        }
}

int main() {
    tower_t chessboard[RIGHE][COLONNE];
    coordinate x,y,move_x,move_y;
    int turno=PLAYER1;
    initialization_chessboard(&(chessboard[0][0]));
    stampa(&(chessboard[0][0]));
    while(1){
        printf("\nTURNO GIOCATORE %d \n",turno);
        printf("seleziona le coordinate della pedina x , y: \n");
        scanf("%d %d", &x, &y); /*inserimento pedina da controllare*/ /*TODO, RICHIEDERE SELEZIONE SE INSERISCI non numeri*/
            if(seleziona_pedina(&(chessboard[0][0]), x, y, turno)){
                printf("Hai selezionato le coordinate x=%d y=%d\n", x, y);
                printf("seleziona le coordinate in cui muovere la pedina x , y: \n");
                scanf("%d %d",&move_x,&move_y); /*inserimento coordinate destinazione pedina*/
                if (seleziona_mossa(&(chessboard[0][0]), x, y,turno,move_x, move_y)){ /*se la mossa è valida stampa chessboard*/
                    stampa(&(chessboard[0][0]));
                    cambio_turno(&turno);
                    controllo_mangiata(&chessboard[0][0], &turno);
                }
            }
    }
    return 0;
}

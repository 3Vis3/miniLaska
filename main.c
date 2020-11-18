#include <stdio.h>
#define RIGHE 7
#define COLONNE 7
#define LIMIT_RIGHT 6
#define LIMIT_LEFT 0
#define LIMIT_DOWN 6
#define LIMIT_UP 0

/*aggiornato al 18/11 ore 1855*/

const int VOID = 0;  /* nessun giocatore*/
const int PLAYER_1 = 1;
const int PLAYER_2 = 2;
const int PLAYER_1_PRO = 3;
const int PLAYER_2_PRO = 4;

typedef int coordinate;
typedef int bool;

typedef
struct tower { /*struttura per le pedine, contiene il giocatore e un'array chiamato chessboard indica quanto alta è la pedina e da cosa è composta*/
    int player;
    int composition[3]; /*1 per o 2 per x 3 O e 4 X  N.B. L'elemento composition[0] è la testa della torre*/
} tower_t;

void checkerboard_init(tower_t *checkerboard){ /*funzione che inizializza la scacchiera e anche le pedine N.B. sarà chiamata una volta sola */
    int r, c;
    for (r = 0; r < RIGHE; r++) {
        for (c = 0; c < COLONNE; c++) {
            if (r < 3 && ((r % 2 == 0 && c % 2 == 0) || (r % 2 != 0 && c % 2 !=0))) { /*se ci troviamo sulla parte superiore della scacchiera e si presentano le condizioni segnate*/
                checkerboard[r * COLONNE + c].player = PLAYER_1;                          /*assegnamo il posto al giocatore 1 e componiamo la sua torre*/
                checkerboard[r * COLONNE + c].composition[0] = PLAYER_1;
                checkerboard[r * COLONNE + c].composition[1] = VOID;
                checkerboard[r * COLONNE + c].composition[2] = VOID;
            }else if (r > 3 && ((r % 2 == 0 && c % 2 == 0) || (r % 2 != 0 && c % 2 !=0))) { /*se ci troviamo sulla parte della scacchiera inferiore e si presenterano le condizioni segnate*/
                checkerboard[r * COLONNE + c].player = PLAYER_2;                               /*assegnamo il posto al giocatore 1 e componiamo la sua torre*/
                checkerboard[r * COLONNE + c].composition[0] = PLAYER_2;
                checkerboard[r * COLONNE + c].composition[1] = VOID;
                checkerboard[r * COLONNE + c].composition[2] = VOID;
            } else {
                checkerboard[r * COLONNE + c].player = VOID;
            }
        }
    }
} /*inizializzazione delle scacchiera N.B. viene chiamata solo la prima volta*/

void checkerboard_print (tower_t *checkerboard) { /*funzione di stampa della chessboard che verrà chiamato tutte le volte in cui viene fatta una mossa*/
    int r , c;
    for (r = 0; r < RIGHE; r++) {
        printf("%d|", r); /*stampa l'indice delle righe*/
        for (c = 0; c < COLONNE; c++) {
            int square = checkerboard[r * COLONNE + c].player;
            if (square == PLAYER_1) {
                if (checkerboard[r * COLONNE + c].composition[0] == PLAYER_1_PRO) {
                    printf(" O ");
                } else {
                    printf(" o ");
                }
            } else if (square == PLAYER_2){
                if (checkerboard[r * COLONNE + c].composition[0] == PLAYER_2_PRO) {
                    printf(" X ");
                } else {
                    printf(" x ");
                }
            } else{
                printf("   ");
            }
        }
        printf("\n");
    }
    printf("  ");
    for (c = 0; c < COLONNE; c++) { /*ciclo per stampare l'indice delle colonne*/
        printf(" %d ", c);
    }
    printf("\n");
}/*la funzione di stampa chessboard verrà chiamata tutte le volte in cui viene fatta una mossa*/

void turn_update(int *turn){
    if(*turn == PLAYER_1){
        *turn = PLAYER_2;
    }else{
        *turn = PLAYER_1;
    }
}

void promotion_check(tower_t *checkerboard) { /*dopo ogni spostamento o mangiata controlla se a fine scacchiera c'è un player che deve essere promosso*/
    int j;
    for (j = 0; j < 7; j++) {
    if (checkerboard[LIMIT_UP * COLONNE + j].composition[0] == PLAYER_2) {
        checkerboard[LIMIT_UP * COLONNE + j].composition[0] = PLAYER_2_PRO;
    } else if (checkerboard[LIMIT_DOWN * COLONNE + j].composition[0] == PLAYER_1) {
        checkerboard[LIMIT_DOWN * COLONNE + j].composition[0] = PLAYER_1_PRO;
    }
}
}

bool control_range(coordinate x, coordinate y){
    if (x < LIMIT_LEFT || x > LIMIT_RIGHT || y < LIMIT_UP || y > LIMIT_DOWN) {
        printf("COORDINATE FUORI RANGE\n");
        return 0;
    }else
        return 1;
}/*controlla che x e y siano dentro la scacchiera se sono all'interno restituisce 1 altrimenti restituisce 0*/

bool diagonal_check (tower_t *checkerboard, coordinate x, coordinate y, int turn){
    if(turn == PLAYER_1){
        if(x == LIMIT_LEFT && checkerboard[(y + 1) * COLONNE + (x + 1)].player == VOID){ /*se il giocatore selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMIT_RIGHT && checkerboard[(y + 1) * COLONNE + (x - 1)].player == VOID){/*se il giocatore selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x>LIMIT_LEFT && x<LIMIT_RIGHT && (checkerboard[(y + 1) * COLONNE + (x - 1)].player == VOID || checkerboard[(y + 1) * COLONNE + (x + 1)].player == VOID)){/*se il giocatore selezionato non si trova alle estremità controlla tutte le diagonali disponibili*/
            return 0;
        }else
            return 1;
    }else if(turn == PLAYER_2){
        if(x == LIMIT_LEFT && checkerboard[(y - 1) * COLONNE + (x + 1)].player == VOID){ /*se il giocatore selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMIT_RIGHT && checkerboard[(y - 1) * COLONNE + (x - 1)].player == VOID){/*se il giocatore selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x>LIMIT_LEFT && x<LIMIT_RIGHT && (checkerboard[(y - 1) * COLONNE + (x - 1)].player == VOID || checkerboard[(y - 1) * COLONNE + (x + 1)].player == VOID)){/*se il giocatore selezionato non si trova alle estremità controlla tutte le diagonali disponibili*/
            return 0;
        }else
            return 1;
    }else if(turn == PLAYER_1_PRO){
        if(x == LIMIT_LEFT && y == LIMIT_UP && checkerboard[(y + 1) * COLONNE + (x + 1)].player == VOID){ /*se il giocatore1 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMIT_RIGHT && y == LIMIT_UP && checkerboard[(y + 1) * COLONNE + (x - 1)].player == VOID){/*se il giocatore1 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMIT_LEFT && y == LIMIT_DOWN && checkerboard[(y - 1) * COLONNE + (x + 1)].player == VOID){/*se il giocatore1 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMIT_RIGHT && y == LIMIT_DOWN && checkerboard[(y - 1) * COLONNE + (x - 1)].player == VOID){/*se il giocatore1 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x < LIMIT_RIGHT && x>LIMIT_LEFT && y == LIMIT_UP && (checkerboard[(y + 1) * COLONNE + (x - 1)].player == VOID) || (checkerboard[(y + 1) * COLONNE + (x + 1)].player == VOID)){/*se il giocatore1 PRO selezionato si trova sulla prima riga della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x < LIMIT_RIGHT && x>LIMIT_LEFT && y == LIMIT_DOWN && (checkerboard[(y - 1) * COLONNE + (x - 1)].player == VOID) || (checkerboard[(y + 1) * COLONNE + (x + 1)].player == VOID)){/*se il giocatore1 PRO selezionato si trova sulla prima riga della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x == LIMIT_LEFT && y<LIMIT_DOWN && y>LIMIT_UP && (checkerboard[(y + 1) * COLONNE + (x + 1)].player == VOID) || (checkerboard[(y - 1) * COLONNE + (x + 1)].player == VOID)){/*se il giocatore1 PRO selezionato si trova sulla prima colonna della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x == LIMIT_RIGHT && y<LIMIT_DOWN && y>LIMIT_UP && (checkerboard[(y - 1) * COLONNE + (x - 1)].player == VOID) || (checkerboard[(y + 1) * COLONNE + (x - 1)].player == VOID)){/*se il giocatore1 PRO selezionato si trova sull'ultima colonna della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x>LIMIT_LEFT && x<LIMIT_RIGHT  && y>LIMIT_UP && y<LIMIT_DOWN && (checkerboard[(y - 1) * COLONNE + (x - 1)].player == VOID || checkerboard[(y - 1) * COLONNE + (x + 1)].player == VOID || checkerboard[(y + 1) * COLONNE + (x + 1)].player == VOID || checkerboard[(y + 1) * COLONNE + (x - 1)].player == VOID)){/*se il giocatore1 PRO selezionato si trova dove tutte diagonali sono disponibili controlla tutte le diagonali disponibili*/
            return 0;
        }else
            return 1;
    }
    else{
        if(x == LIMIT_LEFT && y == LIMIT_UP && checkerboard[(y + 1) * COLONNE + (x + 1)].player == VOID){ /*se il giocatore2 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMIT_RIGHT && y == LIMIT_UP && checkerboard[(y + 1) * COLONNE + (x - 1)].player == VOID){/*se il giocatore2 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMIT_LEFT && y == LIMIT_DOWN && checkerboard[(y - 1) * COLONNE + (x + 1)].player == VOID){/*se il giocatore2 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x == LIMIT_RIGHT && y == LIMIT_DOWN && checkerboard[(y - 1) * COLONNE + (x - 1)].player == VOID){/*se il giocatore2 PRO selezionato si trova sul limite della scacchiera fa solo il controllo di una diagonale in quanto l'altra si trova fuori dalla matrice*/
            return 0;
        }else if(x < LIMIT_RIGHT && x>LIMIT_LEFT && y == LIMIT_UP && (checkerboard[(y + 1) * COLONNE + (x - 1)].player == VOID) || (checkerboard[(y + 1) * COLONNE + (x + 1)].player == VOID)){/*se il giocatore2 PRO selezionato si trova sulla prima riga della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x < LIMIT_RIGHT && x>LIMIT_LEFT && y == LIMIT_DOWN && (checkerboard[(y - 1) * COLONNE + (x - 1)].player == VOID) || (checkerboard[(y + 1) * COLONNE + (x + 1)].player == VOID)){/*se il giocatore2 PRO selezionato si trova sulla prima riga della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x == LIMIT_LEFT && y<LIMIT_DOWN && y>LIMIT_UP && (checkerboard[(y + 1) * COLONNE + (x + 1)].player == VOID) || (checkerboard[(y - 1) * COLONNE + (x + 1)].player == VOID)){/*se il giocatore1 PRO selezionato si trova sulla prima colonna della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x == LIMIT_RIGHT && y<LIMIT_DOWN && y>LIMIT_UP && (checkerboard[(y - 1) * COLONNE + (x - 1)].player == VOID) || (checkerboard[(y + 1) * COLONNE + (x - 1)].player == VOID)){/*se il giocatore1 PRO selezionato si trova sull'ultima colonna della scacchiera controlla solo due diagonali*/
            return 0;
        }else if(x>LIMIT_LEFT && x<LIMIT_RIGHT  && y>LIMIT_UP && y<LIMIT_DOWN && (checkerboard[(y - 1) * COLONNE + (x - 1)].player == VOID || checkerboard[(y - 1) * COLONNE + (x + 1)].player == VOID || checkerboard[(y + 1) * COLONNE + (x + 1)].player == VOID || checkerboard[(y + 1) * COLONNE + (x - 1)].player == VOID)){/*se il giocatore2 PRO selezionato si trova dove tutte diagonali sono disponibili controlla tutte le diagonali disponibili*/
            return 0;
        }else
            return 1;
    }

}/*verifica le 2 diagonali nel caso di giocatore non promosso altrimenti controlla le 4 diagonali per il giocatore promosso,
 * restituisce 0 se almeno una digaonale è libera, altrimenti restituisce 1*/

bool piece_selection (tower_t *checkerboard, coordinate x, coordinate y, int turn){ /*controllo se pedina corrisponde al giocatore giusto*/
    if (control_range(x, y) && checkerboard[y * COLONNE + x].player == turn){ /*chiama la funzione control range chessboard controlla se i range sono corretti*/
        if(!diagonal_check(checkerboard, x, y, turn))/*chiama la funzione per controllare le diagonali, se almeno una diagonale è libera allora può continuare*/
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

void composition_update(tower_t *checkerboard, coordinate y, coordinate x, coordinate move_y, coordinate move_x) {
    checkerboard[move_y * COLONNE + move_x].composition[0] = checkerboard[ y * COLONNE + x].composition[0];
    checkerboard[move_y * COLONNE + move_x].composition[1] = checkerboard[ y * COLONNE + x].composition[1];
    checkerboard[move_y * COLONNE + move_x].composition[2] = checkerboard[ y * COLONNE + x].composition[2];
}

bool move_selection(tower_t *checkerboard, coordinate x, coordinate y, int turno, coordinate move_x, coordinate move_y){
    if(turno == PLAYER_1){
        if (checkerboard[y * COLONNE+ x].composition[0] == PLAYER_1_PRO) { /*se il player è promosso ha liberta di movimento in ogni direzione*/
            if ((move_y == (y + 1) || move_y == (y - 1)) && (move_x == (x + 1) || move_x == (x - 1)) && checkerboard[move_y * COLONNE + move_x].player == VOID && control_range(move_x, move_y)) {
                checkerboard[move_y * COLONNE + move_x].player = turno; /*assegnamento alla casella del player*/
            }
            checkerboard[y * COLONNE + x].player = VOID; /*svuota casella old*/
            composition_update (checkerboard, y, x, move_y, move_x);
            return 1;
        } /*player 1 non pro*/
        else if ((move_y == y+1) && ((move_x == x+1) || (move_x == x-1)) && checkerboard[move_y * COLONNE + move_x].player == VOID && control_range(move_x, move_y)){ /*verifica se destinazione è valida e verifica che in quella posizione non ci siano altre pedine e che il range sia corretto*/
            checkerboard[move_y * COLONNE + move_x].player = turno; /*assegnamento alla casella del player*/
            checkerboard[y * COLONNE + x].player = VOID; /*svuota casella old*/
            composition_update (checkerboard, y, x, move_y, move_x);
            return 1;
        }
    }
    else if (turno == PLAYER_2){
        if (checkerboard[y * COLONNE+ x].composition[0] == PLAYER_2_PRO) {
            if ((move_y == (y + 1) || (move_y == y - 1)) && (move_x == (x + 1) || move_x == (x - 1)) && checkerboard[move_y * COLONNE + move_x].player == VOID && control_range(move_x, move_y)) {
                checkerboard[move_y * COLONNE + move_x].player = turno; /*assegnamento alla casella del player*/
            }
            checkerboard[y * COLONNE + x].player = VOID; /*svuota casella old*/
            composition_update (checkerboard, y, x, move_y, move_x);
            return 1;
        } /*player 2 non pro*/
        else if ((move_y == y-1) && ((move_x == x+1) || (move_x == x-1)) && checkerboard[move_y * COLONNE + move_x].player == VOID && control_range(move_x, move_y)){ /*verifica se destinazione è valida e verifica che in quella posizione non ci siano altre pedine e che il range sia corretto*/
            checkerboard[move_y * COLONNE + move_x].player = turno;
            checkerboard[y* COLONNE + x].player = VOID;
            composition_update (checkerboard, y, x, move_y, move_x);
            return 1;
        }
    }
    printf("MOSSA NON VALIDA\n");
    return 0;
}
/*TODO, array per passare più destinazioni di possibili mangiate con stessa pedina*/
void piece_capture(tower_t *checkerboard, coordinate y, coordinate x, int *turn, coordinate dest_y, coordinate dest_x, coordinate enemy_y, coordinate enemy_x){ /*controllo se destinazione è valida*/
    int check = 0;
    int move_x, move_y;
    int i = 0;
    printf("TURNO GIOCATORE %d\n", *turn);
    printf("Devi mangiare l'avversario con la pedina nelle coordinate x:%d y:%d\n", x, y);
    printf("Inserisci le coordinate in cui fare la mangiata\n");
    scanf("%d %d", &move_x, &move_y);
    if(checkerboard[move_y * COLONNE + move_x].player == VOID && move_y == dest_y && move_x == dest_x) { /*se hai digitato bene allora...*/
        if (checkerboard[y * COLONNE + x].composition[1] == VOID) { /*controllo se non c'è una seconda pedina nella torre*/
            /*tre if per il controllo pedine intermedie dell'attaccante ed eventuale sostituzione se vuote con pedine del nemico*/
            checkerboard[move_y * COLONNE + move_x].composition[0] = checkerboard[y * COLONNE + x].composition[0];
            checkerboard[move_y * COLONNE + move_x].composition[1] = checkerboard[enemy_y * COLONNE + enemy_x].composition[0];
            /*sostituisce al secondo o terzo elemento dell'attaccante la testa del nemico*/
        } else if (checkerboard[y * COLONNE + x].composition[2] == 0) { /*controllo se non c'è una terza pedina nella torre*/
                checkerboard[move_y * COLONNE+move_x].composition[0] = checkerboard[y * COLONNE + x].composition[0];
                checkerboard[move_y * COLONNE+move_x].composition[1] = checkerboard[y * COLONNE + x].composition[1];
                checkerboard[move_y * COLONNE + move_x].composition[2] = checkerboard[enemy_y * COLONNE + enemy_x].composition[0];
        }
        if (checkerboard[y * COLONNE + x].composition[2] != VOID) { /*controllo se torre attaccante aveva più di una pedina, se si sostituisco tutte le vecchie composizioni in quelle nuove*/
            checkerboard[move_y * COLONNE + move_x].composition[0] = checkerboard[y * COLONNE + x].composition[0];
            checkerboard[move_y * COLONNE + move_x].composition[1] = checkerboard[y * COLONNE + x].composition[1];
            checkerboard[move_y * COLONNE + move_x].composition[2] = checkerboard[y * COLONNE + x].composition[2];
        }

        /*assegnamento player a torre di destinazione*/
        checkerboard[move_y * COLONNE + move_x].player = *turn;

        /* sostituzione vecchia posizione dell'attaccante con parametri player=VUOTO e composizione 000*/
        checkerboard[y * COLONNE + x].player = VOID; /*AGGIORNAMENTO POSIZIONE PRIMA DELLA MANGIATA DELL'ATTACCANTE*/
        checkerboard[y * COLONNE + x].composition[0] = 0;
        checkerboard[y * COLONNE + x].composition[1] = 0;
        checkerboard[y * COLONNE + x].composition[2] = 0;

        /*sostituzione ultima pedina mangiata con ZERO e scalamento delle superiori */
        checkerboard[enemy_y * COLONNE + enemy_x].composition[0] = checkerboard[enemy_y * COLONNE + enemy_x].composition[1]; /*scala di una posizione gli elementi della pedina mangiata*/
        checkerboard[enemy_y * COLONNE + enemy_x].composition[1] = checkerboard[enemy_y * COLONNE + enemy_x].composition[2];
        checkerboard[enemy_y * COLONNE + enemy_x].composition[2] = 0;

        /*assegnamento nuovo player a pedina mangiata basandosi sulla testa*/
        if (checkerboard[enemy_y * COLONNE + enemy_x].composition[0] == VOID) { /*controllo testa pedina attaccata*/
            checkerboard[enemy_y * COLONNE + enemy_x].player = VOID;
        } else if (checkerboard[enemy_y * COLONNE + enemy_x].composition[0] == PLAYER_1 || checkerboard[enemy_y * COLONNE + enemy_x].composition[0] == PLAYER_1_PRO) { /*controllo se testa è player 1 o player 2 e aggiorna player*/
            checkerboard[enemy_y * COLONNE + enemy_x].player = PLAYER_1;
        } else if (checkerboard[enemy_y * COLONNE + enemy_x].composition[0] == PLAYER_2 || checkerboard[enemy_y * COLONNE + enemy_x].composition[0] == PLAYER_2_PRO) {
            checkerboard[enemy_y * COLONNE + enemy_x].player = PLAYER_2;
        }

        turn_update(turn);
        /*stampe di verifiche composizione player*/
        printf("\nl'attaccante ora vale %d %d %d", checkerboard[move_y * COLONNE + move_x].composition[0], checkerboard[move_y * COLONNE + move_x].composition[1], checkerboard[move_y * COLONNE + move_x].composition[2]);
        printf("\nil mangiato ora vale %d, %d, %d", checkerboard[enemy_y * COLONNE + enemy_x].composition[0], checkerboard[enemy_y * COLONNE + enemy_x].composition[1], checkerboard[enemy_y * COLONNE + enemy_x].composition[2]);
        printf("\n la vecchia posizione ora vale %d %d %d\n", checkerboard[y * COLONNE + x].composition[0], checkerboard[y * COLONNE + x].composition[1], checkerboard[y * COLONNE + x].composition[2]);
        printf("i player ora sono attaccante %d, attaccato %d, vecchia posizione %d\n", checkerboard[move_y * COLONNE + move_x].player, checkerboard[enemy_y * COLONNE + enemy_x].player, checkerboard[y * COLONNE + x].player);

        /*torrefazione*/
        promotion_check(checkerboard);
        checkerboard_print(checkerboard);

    } /*se c'è già una torre nella destinazione*/
    else {
        printf("Hai sbagliato selezione\n");
        piece_capture(checkerboard, y, x, turn, dest_y, dest_x, enemy_y, enemy_x); /*se sbagli a digitare coordinate devi ripetere la funzione mangiata*/
    }
}

void capture_check(tower_t *checkerboard, int *turn) {
    int i, j;
    char selection; /*variabili char chessboard da scanf riceve s oppure n e fa il confronto tra selezione e i caratteri n o s*/
    int count = 0; /*conta le pedine obbligate a mangiare*/
    for (i = 0; i < RIGHE; i++) {
        for (j = 0; j < COLONNE; j++) {
            if (checkerboard[i * COLONNE + j].player == *turn) {
                if ((*turn == PLAYER_1) ||
                    (*turn == PLAYER_2 && checkerboard[i * COLONNE + j].composition[0] == PLAYER_2_PRO)) {
                    if (i < 5 && j > 1 &&
                        ((*turn == PLAYER_1 && checkerboard[(i + 1) * COLONNE + (j - 1)].player == PLAYER_2) ||
                         (*turn == PLAYER_2 && checkerboard[(i + 1) * COLONNE + (j - 1)].player == PLAYER_1))) {
                        /*ho inserito degli and per evitare il controllo a sx se sei vicino al bordo sx e viceversa a dx*/
                        /*controllo diagonale sinistra per player 1 o player 2 pro*/
                        if (checkerboard[(i + 2) * COLONNE + (j - 2)].player == VOID) {
                            /*passo i e j come coordinate per mangiata obbligatoria*/
                            count++; /*incrementa il conteggio se rilevata una pedina obbligata*/
                            printf("TURNO GIOCATORE %d\n", *turn);
                            printf("vuoi mangiare con x=%d y=%d? s/n\n", j, i);
                            scanf_s(" %c", &selection); /*chiede se vuoi mangiare con quella pedina*/
                            if (selection == 's') { /*se dici si chiama mangiata*/
                                piece_capture(checkerboard, i, j, turn, i + 2, j - 2, i + 1, j - 1);
                            }
                        }/*controllo diagonale destra per player 1 o player 2 pro*/
                    } else if (i < 5 && j < 5 &&
                               ((*turn == PLAYER_1 && checkerboard[(i + 1) * COLONNE + (j + 1)].player == PLAYER_2) ||
                                (*turn == PLAYER_2 && checkerboard[(i + 1) * COLONNE + (j + 1)].player == PLAYER_1))) {
                        if (checkerboard[(i + 2) * COLONNE + (j + 2)].player == VOID) {
                            /*TODO far scegliere a giocatore quale manigata effettuare*/
                            /*passo i e j come coordinate per mangiata obbligatoria*/
                            count++;
                            printf("TURNO GIOCATORE %d\n", *turn);
                            printf("vuoi mangiare con x=%d y=%d? \n", j, i);
                            scanf_s(" %c", &selection);
                            if (selection == 's') {
                                piece_capture(checkerboard, i, j, turn, i + 2, j + 2, i + 1, j + 1);
                            }
                        }
                    }
                } /*controllo diagonali player 2 o player 1 pro*/
                if ((*turn == PLAYER_2) ||
                    (*turn == PLAYER_1 && checkerboard[i * COLONNE + j].composition[0] == PLAYER_1_PRO)) {
                    if (i > 1 && j > 1 &&
                        ((*turn == PLAYER_2 && checkerboard[(i - 1) * COLONNE + (j - 1)].player == PLAYER_1) ||
                         (*turn == PLAYER_1 && checkerboard[(i - 1) * COLONNE + (j - 1)].player == PLAYER_2))) {
                        /*controllo diagonale sinistra per player 2 o player 1 pro*/
                        if (checkerboard[(i - 2) * COLONNE + (j - 2)].player == VOID) {
                            /*TODO far scegliere a giocatore quale manigata effettuare, con più di una*/
                            /*passo i e j come coordinate per mangiata obbligatoria*/
                            count++;
                            printf("TURNO GIOCATORE %d\n", *turn);
                            printf("vuoi mangiare con x=%d y=%d? s/n\n", j, i);
                            scanf_s(" %c", &selection);
                            if (selection == 's') {
                                piece_capture(checkerboard, i, j, turn, i - 2, j - 2, i - 1, j - 1);
                            }
                        }/*controllo diagonale destra per player 2 o player 1 pro*/
                        if (i > 1 && j < 5 &&
                            ((*turn == PLAYER_2 && checkerboard[(i - 1) * COLONNE + (j + 1)].player == PLAYER_1) ||
                             (*turn == PLAYER_1 && checkerboard[(i - 1) * COLONNE + (j + 1)].player == PLAYER_2))) {
                            /*controllo diagonale destra chessboard sia player avversario e non vuoto*/
                            if (checkerboard[(i - 2) * COLONNE + (j + 2)].player == VOID) {
                                /*TODO far scegliere a giocatore quale manigata effettuare*/
                                /*passo i e j come coordinate per mangiata obbligatoria*/
                                count++;
                                printf("TURNO GIOCATORE %d\n", *turn);
                                printf("vuoi mangiare con x=%d y=%d? s/n\n", j, i);
                                scanf_s(" %c", &selection);
                                if (selection == 's') {
                                    piece_capture(checkerboard, i, j, turn, i - 2, j + 2, i - 1, j + 1);
                                }
                            }
                        }
                    }
                }
            }
        }
        if (count != 0) { /*se non ha ancora mangiato riparte*/
            capture_check(checkerboard, turn);
        } else { /*se il conteggio è uguale a zero ritorna al main*/
            return;
        }
    }
}

bool vittoria(tower_t *checkerboard, int turn) {
    int i, j;
    for (i = 0; i < RIGHE; i++) {
        for (j = 0; j < COLONNE; j++) {
            if (checkerboard[i * COLONNE + j].player == turn) {
                if (!diagonal_check(checkerboard, i, j, turn)) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int main() {
    tower_t checkerboard[RIGHE][COLONNE];
    coordinate x, y, move_x, move_y;
    int turn = PLAYER_1;
    checkerboard_init (&(checkerboard[0][0]));
    checkerboard_print (&(checkerboard[0][0]));
    while (vittoria(&(checkerboard[0][0]), turn)){
        printf("\nTURNO GIOCATORE %d \n",turn);
        printf("Seleziona le coordinate della pedina x , y: \n");
        scanf_s("%d %d", &x, &y); /*inserimento pedina da controllare*/ /*TODO, RICHIEDERE SELEZIONE SE INSERISCI non numeri*/
            if (piece_selection(&(checkerboard[0][0]), x, y, turn)){
                printf("Hai selezionato le coordinate x=%d y=%d\n", x, y);
                printf("seleziona le coordinate in cui muovere la pedina x , y: \n");
                scanf_s("%d %d", &move_x, &move_y); /*inserimento coordinate destinazione pedina*/
                if (move_selection(&(checkerboard[0][0]), x, y, turn, move_x, move_y)){ /*se la mossa è valida stampa chessboard*/
                    promotion_check(&(checkerboard[0][0]));
                    checkerboard_print(&(checkerboard[0][0]));
                    turn_update(&turn);
                    capture_check(&checkerboard[0][0], &turn);
                }
            }
    }

    return 0;
}

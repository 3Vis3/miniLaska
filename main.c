#include <stdio.h>
#define ROWS (7)
#define COLUMNS (7)
#define LIMIT_RIGHT (6)
#define LIMIT_LEFT (0)
#define LIMIT_DOWN (6)
#define LIMIT_UP (0)
#define FIRST_DIAGONAL (1)
#define SECOND_DIAGONAL (0)
#define ERROR (-1)

typedef int player_t;
typedef int coordinate_t;
typedef int bool_t;

const int VOID = 0;  /* nessun giocatore*/
const player_t PLAYER_1 = 1;
const player_t PLAYER_2 = 2;
const player_t PLAYER_1_PRO = 3;
const player_t PLAYER_2_PRO = 4;

typedef int coordinate;
typedef int bool;

typedef
struct tower { /*struttura per le pedine, contiene il giocatore e un'array chiamato chessboard indica quanto alta è la pedina e da cosa è composta*/
    player_t player;
    int composition[3]; /*1 per o 2 per x 3 O e 4 X  N.B. L'elemento composition[0] è la testa della torre*/
} tower_t;

void checkerboard_init(tower_t *checkerboard){ /*funzione che inizializza la scacchiera e anche le pedine N.B. sarà chiamata una volta sola */
    int r, c;
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLUMNS; c++) {
            if (r < 3 && ((r % 2 == 0 && c % 2 == 0) || (r % 2 != 0 && c % 2 !=0))) { /*se ci troviamo sulla parte superiore della scacchiera e si presentano le condizioni segnate*/
                checkerboard[r * COLUMNS + c].player = PLAYER_1;                          /*assegnamo il posto al giocatore 1 e componiamo la sua torre*/
                checkerboard[r * COLUMNS + c].composition[0] = PLAYER_1;
                checkerboard[r * COLUMNS + c].composition[1] = VOID;
                checkerboard[r * COLUMNS + c].composition[2] = VOID;
            }else if (r > 3 && ((r % 2 == 0 && c % 2 == 0) || (r % 2 != 0 && c % 2 !=0))) { /*se ci troviamo sulla parte della scacchiera inferiore e si presenterano le condizioni segnate*/
                checkerboard[r * COLUMNS + c].player = PLAYER_2;                               /*assegnamo il posto al giocatore 1 e componiamo la sua torre*/
                checkerboard[r * COLUMNS + c].composition[0] = PLAYER_2;
                checkerboard[r * COLUMNS + c].composition[1] = VOID;
                checkerboard[r * COLUMNS + c].composition[2] = VOID;
            } else {
                checkerboard[r * COLUMNS + c].player = VOID;
                checkerboard[r * COLUMNS + c].composition[0] = VOID;
                checkerboard[r * COLUMNS + c].composition[1] = VOID;
                checkerboard[r * COLUMNS + c].composition[2] = VOID;
            }
        }
    }
} /*inizializzazione delle scacchiera N.B. viene chiamata solo la prima volta*/

void checkerboard_print (tower_t *checkerboard) { /*funzione di stampa della chessboard che verrà chiamato tutte le volte in cui viene fatta una mossa*/
    int r , c;
    for (r = 0; r < ROWS; r++) {
        printf("%d|", r); /*stampa l'indice delle righe*/
        for (c = 0; c < COLUMNS; c++) {
            int square = checkerboard[r * COLUMNS + c].player;
            if (square == PLAYER_1) {
                if (checkerboard[r * COLUMNS + c].composition[0] == PLAYER_1_PRO) {
                    printf(" O ");
                } else {
                    printf(" o ");
                }
            } else if (square == PLAYER_2){
                if (checkerboard[r * COLUMNS + c].composition[0] == PLAYER_2_PRO) {
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
    for (c = 0; c < COLUMNS; c++) { /*ciclo per stampare l'indice delle colonne*/
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
        if (checkerboard[LIMIT_UP * COLUMNS + j].composition[0] == PLAYER_2) {
            checkerboard[LIMIT_UP * COLUMNS + j].composition[0] = PLAYER_2_PRO;
        } else if (checkerboard[LIMIT_DOWN * COLUMNS + j].composition[0] == PLAYER_1) {
            checkerboard[LIMIT_DOWN * COLUMNS + j].composition[0] = PLAYER_1_PRO;
        }
    }
}

bool control_range(coordinate x, coordinate y){
    if (x < LIMIT_LEFT || x > LIMIT_RIGHT || y < LIMIT_UP || y > LIMIT_DOWN) {
        return 0;
    }else
        return 1;
}/*controlla che x e y siano dentro la scacchiera se sono all'interno restituisce 1 altrimenti restituisce 0*/

/*verifica le 2 diagonali nel caso di giocatore non promosso altrimenti controlla le 4 diagonali per il giocatore promosso,
 * restituisce 0 se almeno una digaonale è libera, altrimenti restituisce 1*/

bool piece_selection (tower_t *checkerboard, coordinate x, coordinate y, int *turn){ /*controllo se pedina corrisponde al giocatore giusto*/
    if (control_range(x, y) && checkerboard[y * COLUMNS + x].player == *turn){ /*chiama la funzione control range chessboard controlla se i range sono corretti*/
        /*if(!diagonal_check(checkerboard, x, y, turn))*//*chiama la funzione per controllare le diagonali, se almeno una diagonale è libera allora può continuare*/
        return 1;
    } else {
        printf("SCELTA SBAGLIATA\n");
        return 0;
    }
}/*restituisce 1 se la pedina è in una posizione in cui può essere scelta altimenti restituisce 0*/

void composition_update(tower_t *checkerboard, coordinate y, coordinate x, coordinate move_y, coordinate move_x) {
    checkerboard[move_y * COLUMNS + move_x].composition[0] = checkerboard[y * COLUMNS + x].composition[0];
    checkerboard[move_y * COLUMNS + move_x].composition[1] = checkerboard[y * COLUMNS + x].composition[1];
    checkerboard[move_y * COLUMNS + move_x].composition[2] = checkerboard[y * COLUMNS + x].composition[2];
}

bool move_selection(tower_t *checkerboard, coordinate x, coordinate y, int *turn, coordinate move_x, coordinate move_y){
    if(*turn == PLAYER_1 && checkerboard[move_y * COLUMNS + move_x].player == VOID){
        if (checkerboard[y * COLUMNS + x].composition[0] == PLAYER_1_PRO) { /*se il player è promosso ha liberta di movimento in ogni direzione*/
            if ((move_y == y + 1 || move_y == y - 1) && (move_x == x + 1 || move_x == x - 1) && checkerboard[move_y * COLUMNS + move_x].player == VOID && control_range(move_x, move_y)) {
                checkerboard[move_y * COLUMNS + move_x].player = *turn; /*assegnamento alla casella del player*/
                checkerboard[y * COLUMNS + x].player = VOID; /*svuota casella old*/
                composition_update(checkerboard, y, x, move_y, move_x);
                return 1;
            }
        } /*player 1 non pro*/
        else if ((move_y == y+1) && ((move_x == x+1) || (move_x == x-1)) && checkerboard[move_y * COLUMNS + move_x].player == VOID && control_range(move_x, move_y)){ /*verifica se destinazione è valida e verifica che in quella posizione non ci siano altre pedine e che il range sia corretto*/
            checkerboard[move_y * COLUMNS + move_x].player = *turn; /*assegnamento alla casella del player*/
            checkerboard[y * COLUMNS + x].player = VOID; /*svuota casella old*/
            composition_update (checkerboard, y, x, move_y, move_x);
            return 1;
        }
    }
    else if (*turn == PLAYER_2 && checkerboard[move_y * COLUMNS + move_x].player == VOID){
        if (checkerboard[y * COLUMNS + x].composition[0] == PLAYER_2_PRO) {
            if ((move_y == y + 1 || move_y == y - 1) && (move_x == x + 1 || move_x == x - 1) && checkerboard[move_y * COLUMNS + move_x].player == VOID && control_range(move_x, move_y)) {
                checkerboard[move_y * COLUMNS + move_x].player = *turn; /*assegnamento alla casella del player*/
                checkerboard[y * COLUMNS + x].player = VOID; /*svuota casella old*/
                composition_update(checkerboard, y, x, move_y, move_x);
                return 1;
            }
        } /*player 2 non pro*/
        else if ((move_y == y-1) && ((move_x == x+1) || (move_x == x-1)) && checkerboard[move_y * COLUMNS + move_x].player == VOID && control_range(move_x, move_y)){ /*verifica se destinazione è valida e verifica che in quella posizione non ci siano altre pedine e che il range sia corretto*/
            checkerboard[move_y * COLUMNS + move_x].player = *turn;
            checkerboard[y * COLUMNS + x].player = VOID;
            composition_update (checkerboard, y, x, move_y, move_x);
            return 1;
        }
    }
    printf("MOSSA NON VALIDA\n");
    return 0;
}

player_t diagonal_down_left_check(tower_t *checkerboard, coordinate_t y, coordinate_t x, bool_t is_first_diagonal){
    /*is_first_diagonal serve per capire se la diaonale su cui eseguire il controllo è la prima (1) altrimenti è la seconda (0)
     * la prima diagonale corrisponde a quella adiacente, la seconda diagonale corrisponde a quella dove la pedina dovrà muoversi se potrà mangiare */
    if(is_first_diagonal && control_range(x-1, y+1)){
        return checkerboard[(y+1) * COLUMNS + x - 1].player;
    }else if(!is_first_diagonal && control_range(x-2, y+2)) {
        return checkerboard[(y + 2) * COLUMNS + x - 2].player;
    }else{
        return ERROR;
    }
}

player_t diagonal_down_right_check(tower_t *checkerboard, coordinate_t y, coordinate_t x, bool_t is_first_diagonal){
    /*is_first_diagonal serve per capire se la diaonale su cui eseguire il controllo è la prima (1) altrimenti è la seconda (0)
     * la prima diagonale corrisponde a quella adiacente, la seconda diagonale corrisponde a quella dove la pedina dovrà muoversi se potrà mangiare */
    if(is_first_diagonal && control_range(x+1, y+1)){
        return checkerboard[(y+1) * COLUMNS + x + 1].player;
    }else if(!is_first_diagonal && control_range(x+2, y+2)) {
        return checkerboard[(y + 2) * COLUMNS + x + 2].player;
    }else{
        return ERROR;
    }
}

player_t diagonal_up_left_check(tower_t *checkerboard, coordinate_t y, coordinate_t x, bool_t is_first_diagonal){
    /*is_first_diagonal serve per capire se la diaonale su cui eseguire il controllo è la prima (1) altrimenti è la seconda (0)
     * la prima diagonale corrisponde a quella adiacente, la seconda diagonale corrisponde a quella dove la pedina dovrà muoversi se potrà mangiare */
    if(is_first_diagonal && control_range(x-1, y-1)){
        return checkerboard[(y-1) * COLUMNS + x - 1].player;
    }else if(!is_first_diagonal && control_range(x-2, y-2)) {
        return checkerboard[(y - 2) * COLUMNS + x - 2].player;
    }else{
        return ERROR;
    }
}

player_t diagonal_up_right_check(tower_t *checkerboard, coordinate_t y, coordinate_t x, bool_t is_first_diagonal){
    /*is_first_diagonal serve per capire se la diaonale su cui eseguire il controllo è la prima (1) altrimenti è la seconda (0)
     * la prima diagonale corrisponde a quella adiacente, la seconda diagonale corrisponde a quella dove la pedina dovrà muoversi se potrà mangiare */
    if(is_first_diagonal && control_range(x+1, y-1)){
        return checkerboard[(y-1) * COLUMNS + x + 1].player;
    }else if(!is_first_diagonal && control_range(x+2, y-2)) {
        return checkerboard[(y - 2) * COLUMNS + x + 2].player;
    }else{
        return ERROR;
    }
}



bool win(tower_t *checkerboard, int *turn) {
    int y, x, counter_pawns=0, pawns_blocked = 0;
    printf("win\n");
    for(y = 0; y < ROWS; y++) {
        for (x = 0; x < COLUMNS; x++) {
            if (checkerboard[y * COLUMNS + x].player == *turn) {
                counter_pawns++;
                if (checkerboard[y * COLUMNS + x].composition[0] == PLAYER_1) {
                    if (diagonal_down_left_check(checkerboard, y, x, FIRST_DIAGONAL) != VOID &&
                        diagonal_down_right_check(checkerboard, y, x, FIRST_DIAGONAL) != VOID) {
                        pawns_blocked++;
                    }
                }
                if (checkerboard[y * COLUMNS + x].composition[0] == PLAYER_2) {
                    if (diagonal_up_left_check(checkerboard, y, x, FIRST_DIAGONAL) != VOID &&
                        diagonal_up_right_check(checkerboard, y, x, FIRST_DIAGONAL) != VOID) {
                        pawns_blocked++;
                    }
                }
                if(checkerboard[y * COLUMNS + x].composition[0] == PLAYER_1_PRO || checkerboard[y * COLUMNS + x].composition[0] == PLAYER_2_PRO){
                    if(diagonal_down_left_check(checkerboard, y, x, FIRST_DIAGONAL) != VOID &&
                       diagonal_down_right_check(checkerboard, y, x, FIRST_DIAGONAL) != VOID &&
                       diagonal_up_left_check(checkerboard, y, x, FIRST_DIAGONAL) != VOID &&
                       diagonal_up_right_check(checkerboard, y, x, FIRST_DIAGONAL) != VOID){
                        pawns_blocked++;
                    }
                }
            }
        }
    }

    if(pawns_blocked == counter_pawns){
        return 1;
    }
    if(counter_pawns != 0){
        return 0;
    } else {
        return 1;
    }
}/*restituisce 1 se vinci 0 contuna a giocare */

void piece_capture(tower_t *checkerboard, coordinate y, coordinate x, int *turn, coordinate dest_y, coordinate dest_x, coordinate enemy_y, coordinate enemy_x){ /*controllo se destinazione è valida*/
    if (checkerboard[y * COLUMNS + x].composition[1] == 0) { /*controllo se non c'è una seconda pedina nella torre*/
        /*tre if per il controllo pedine intermedie dell'attaccante ed eventuale sostituzione se vuote con pedine del nemico*/
        checkerboard[dest_y * COLUMNS + dest_x].composition[0] = checkerboard[y * COLUMNS + x].composition[0];
        checkerboard[dest_y * COLUMNS + dest_x].composition[1] = checkerboard[enemy_y * COLUMNS + enemy_x].composition[0];
        checkerboard[dest_y * COLUMNS + dest_x].composition[2] = checkerboard[y * COLUMNS + x].composition[2];
        /*sostituisce al secondo o terzo elemento dell'attaccante la testa del nemico*/
    } else if (checkerboard[y * COLUMNS + x].composition[2] == 0) { /*controllo se non c'è una terza pedina nella torre*/
        checkerboard[dest_y * COLUMNS + dest_x].composition[0] = checkerboard[y * COLUMNS + x].composition[0];
        checkerboard[dest_y * COLUMNS + dest_x].composition[1] = checkerboard[y * COLUMNS + x].composition[1];
        checkerboard[dest_y * COLUMNS + dest_x].composition[2] = checkerboard[enemy_y * COLUMNS + enemy_x].composition[0];
    }
    else if (checkerboard[y * COLUMNS + x].composition[2] != 0) { /*controllo se torre attaccante aveva più di una pedina, se si sostituisco tutte le vecchie composizioni in quelle nuove*/
        checkerboard[dest_y * COLUMNS + dest_x].composition[0] = checkerboard[y * COLUMNS + x].composition[0];
        checkerboard[dest_y * COLUMNS + dest_x].composition[1] = checkerboard[y * COLUMNS + x].composition[1];
        checkerboard[dest_y * COLUMNS + dest_x].composition[2] = checkerboard[y * COLUMNS + x].composition[2];
    }

    /*assegnamento player a torre di destinazione*/
    checkerboard[dest_y * COLUMNS + dest_x].player = *turn;

    /* sostituzione vecchia posizione dell'attaccante con parametri player=VUOTO e composizione 000*/
    checkerboard[y * COLUMNS + x].player = VOID; /*AGGIORNAMENTO POSIZIONE PRIMA DELLA MANGIATA DELL'ATTACCANTE*/
    checkerboard[y * COLUMNS + x].composition[0] = 0;
    checkerboard[y * COLUMNS + x].composition[1] = 0;
    checkerboard[y * COLUMNS + x].composition[2] = 0;

    /*sostituzione ultima pedina mangiata con ZERO e scalamento delle superiori */
    checkerboard[enemy_y * COLUMNS + enemy_x].composition[0] = checkerboard[enemy_y * COLUMNS + enemy_x].composition[1]; /*scala di una posizione gli elementi della pedina mangiata*/
    checkerboard[enemy_y * COLUMNS + enemy_x].composition[1] = checkerboard[enemy_y * COLUMNS + enemy_x].composition[2];
    checkerboard[enemy_y * COLUMNS + enemy_x].composition[2] = 0;

    /*assegnamento nuovo player a pedina mangiata basandosi sulla testa*/
    if (checkerboard[enemy_y * COLUMNS + enemy_x].composition[0] == 0) { /*controllo testa pedina attaccata*/
        checkerboard[enemy_y * COLUMNS + enemy_x].player = VOID;
    } else if (checkerboard[enemy_y * COLUMNS + enemy_x].composition[0] == 1 || checkerboard[enemy_y * COLUMNS + enemy_x].composition[0] == 3) { /*controllo se testa è player 1 o player 2 e aggiorna player*/
        checkerboard[enemy_y * COLUMNS + enemy_x].player = PLAYER_1;
    } else if (checkerboard[enemy_y * COLUMNS + enemy_x].composition[0] == 2 || checkerboard[enemy_y * COLUMNS + enemy_x].composition[0] == 4) {
        checkerboard[enemy_y * COLUMNS + enemy_x].player = PLAYER_2;
    }
    /*stampe di verifiche composizione player*/
    printf("\nl'attaccante ora vale %d %d %d", checkerboard[dest_y * COLUMNS + dest_x].composition[0], checkerboard[dest_y * COLUMNS + dest_x].composition[1], checkerboard[dest_y * COLUMNS + dest_x].composition[2]);
    printf("\nil mangiato ora vale %d, %d, %d", checkerboard[enemy_y * COLUMNS + enemy_x].composition[0], checkerboard[enemy_y * COLUMNS + enemy_x].composition[1], checkerboard[enemy_y * COLUMNS + enemy_x].composition[2]);
    printf("\n la vecchia posizione ora vale %d %d %d\n", checkerboard[y * COLUMNS + x].composition[0], checkerboard[y * COLUMNS + x].composition[1], checkerboard[y * COLUMNS + x].composition[2]);
    printf("i player ora sono attaccante %d, attaccato %d, vecchia posizione %d\n", checkerboard[dest_y * COLUMNS + dest_x].player, checkerboard[enemy_y * COLUMNS + enemy_x].player, checkerboard[y * COLUMNS + x].player);
    /*torrefazione*/
    /*promotion_check(checkerboard);
    checkerboard_print(checkerboard);
    turn_update(turn);*/
    /*se c'è già una torre nella destinazione*/
    /*win((checkerboard), turn);*/
}

int capture_check(tower_t *checkerboard, int *turno){
    int r, c;
    char selection; /*variabili char checkerboard da scanf riceve s oppure n e fa il confronto tra selezione e i caratteri n o s*/ /*TODO SISTEMARE LOOP INFINITO SE INSERISCI SN*/
    char si= 's';
    int count=0; /*conta le pedine obbligate a mangiare*/
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLUMNS; c++) {
            if (checkerboard[r * COLUMNS + c].player == *turno) {
                if (*turno == PLAYER_1 /*|| PLAYER2 == promoted*/) {
                    if (diagonal_down_left_check(checkerboard, r, c, FIRST_DIAGONAL) == PLAYER_2) {
                        /*ho inserito degli and per evitare il controllo a sx se sei vicino al bordo sx e viceversa a dx*/
                        /*controllo diagonale sinistra checkerboard sia player avversario e non vuoto*/
                        if (diagonal_down_left_check(checkerboard, r, c, SECOND_DIAGONAL) == VOID) {
                            /*TODO funzione checkerboard controlla se esce dall'array del checkerboard*/
                            /*passo i e c come coordinate per mangiata obbligatoria*/
                            count++; /*incrementa il conteggio se rilevata una pedina obbligata*/
                            printf("TURNO GIOCATORE %d\n", *turno);
                            printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c - 1, r + 1);
                            scanf(" %c", &selection); /*chiede se vuoi mangiare con quella pedina*/
                            if (selection==si) { /*se dici si chiama mangiata*/
                                piece_capture(checkerboard, r, c, turno, r + 2, c - 2, r + 1, c - 1);
                                return 1;
                            }
                        }
                    } if (diagonal_down_right_check(checkerboard, r, c, FIRST_DIAGONAL) == PLAYER_2) {
                        /*controllo diagonale destra checkerboard sia player avversario e non vuoto*/
                        if (diagonal_down_right_check(checkerboard, r, c, SECOND_DIAGONAL) == VOID) {
                            /*passo i e c come coordinate per mangiata obbligatoria*/
                            count++;
                            printf("TURNO GIOCATORE %d\n", *turno);
                            printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c + 1, r + 1);
                            scanf(" %c", &selection);
                            if (selection==si) {
                                piece_capture(checkerboard, r, c, turno, r + 2, c + 2, r + 1, c + 1);
                                return 1;
                            }
                        }
                    }
                    if (checkerboard[r * COLUMNS + c].composition[0] == PLAYER_1_PRO) {
                        if (diagonal_up_left_check(checkerboard, r, c, FIRST_DIAGONAL) == PLAYER_2) {
                            /*controllo diagonale sinistra checkerboard sia player avversario e non vuoto*/
                            if (diagonal_up_left_check(checkerboard, r, c, SECOND_DIAGONAL) == VOID) {
                                /*passo i e c come coordinate per mangiata obbligatoria*/
                                count++;
                                printf("TURNO GIOCATORE %d\n", *turno);
                                printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c - 1, r - 1);
                                scanf(" %c", &selection);
                                if (selection==si) {
                                    piece_capture(checkerboard, r, c, turno, r - 2, c - 2, r - 1, c - 1);
                                    return 1;
                                }
                            }
                        }
                        if (diagonal_up_right_check(checkerboard, r, c, FIRST_DIAGONAL) == PLAYER_2) {
                            /*controllo diagonale destra checkerboard sia player avversario e non vuoto*/
                            if (diagonal_up_right_check(checkerboard, r, c, SECOND_DIAGONAL) == VOID) {
                                /*passo i e c come coordinate per mangiata obbligatoria*/
                                count++;
                                printf("TURNO GIOCATORE %d\n", *turno);
                                printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c + 1, r - 1);
                                scanf(" %c", &selection);
                                if (selection==si) {
                                    piece_capture(checkerboard, r, c, turno, r - 2, c + 2, r - 1, c + 1);
                                    return 1;
                                }
                            }
                        }
                    }
                }
                if (*turno == PLAYER_2 /*|| PLAYER1 == promoted*/) {
                    if (diagonal_up_left_check(checkerboard, r, c, FIRST_DIAGONAL) == PLAYER_1) {
                        /*controllo diagonale sinistra checkerboard sia player avversario e non vuoto*/
                        if (diagonal_up_left_check(checkerboard, r, c, SECOND_DIAGONAL) == VOID) {
                            /*passo i e c come coordinate per mangiata obbligatoria*/
                            count++;
                            printf("TURNO GIOCATORE %d\n", *turno);
                            printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c - 1, r - 1);
                            scanf(" %c", &selection);
                            if (selection==si) {
                                piece_capture(checkerboard, r, c, turno, r - 2, c - 2, r - 1, c - 1);
                                return 1;
                            }
                        }
                    }
                    if (diagonal_up_right_check(checkerboard, r, c, FIRST_DIAGONAL) == PLAYER_1) {
                        /*controllo diagonale destra checkerboard sia player avversario e non vuoto*/
                        if (diagonal_up_right_check(checkerboard, r, c, SECOND_DIAGONAL) == VOID) {
                            /*passo i e c come coordinate per mangiata obbligatoria*/
                            count++;
                            printf("TURNO GIOCATORE %d\n", *turno);
                            printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c + 1, r - 1);
                            scanf(" %c", &selection);
                            if (selection==si) {
                                piece_capture(checkerboard, r, c, turno, r - 2, c + 2, r - 1, c + 1);
                                return 1;
                            }
                        }
                    }
                    if (checkerboard[r * COLUMNS + c].composition[0] == PLAYER_2_PRO) {
                        if (diagonal_down_left_check(checkerboard, r, c, FIRST_DIAGONAL) == PLAYER_1) {
                            /*ho inserito degli and per evitare il controllo a sx se sei vicino al bordo sx e viceversa a dx*/
                            /*controllo diagonale sinistra checkerboard sia player avversario e non vuoto*/
                            if (diagonal_down_left_check(checkerboard, r, c, SECOND_DIAGONAL) == VOID) {
                                /*passo i e c come coordinate per mangiata obbligatoria*/
                                count++; /*incrementa il conteggio se rilevata una pedina obbligata*/
                                printf("TURNO GIOCATORE %d\n", *turno);
                                printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c - 1, r + 1);
                                scanf(" %c", &selection); /*chiede se vuoi mangiare con quella pedina*/
                                if (selection==si) { /*se dici si chiama mangiata*/
                                    piece_capture(checkerboard, r, c, turno, r + 2, c - 2, r + 1, c - 1);
                                    return 1;
                                }
                            }
                        } if (diagonal_down_right_check(checkerboard, r, c, FIRST_DIAGONAL) == PLAYER_1) {
                            /*controllo diagonale destra checkerboard sia player avversario e non vuoto*/
                            if (diagonal_down_right_check(checkerboard, r, c, SECOND_DIAGONAL) == VOID) {
                                /*passo i e c come coordinate per mangiata obbligatoria*/
                                count++;
                                printf("TURNO GIOCATORE %d\n", *turno);
                                printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c + 1, r + 1);
                                scanf(" %c", &selection);
                                if (selection==si) {
                                    piece_capture(checkerboard, r, c, turno, r + 2, c + 2, r + 1, c + 1);
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    } if (count!=0) { /*se il conteggio delle obbligate è diverso da zero riparte il controllo*/
        return 2;
    } return 0; /*se il conteggio è uguale a zero ritorna al main*/
}

int main() {
    tower_t checkerboard[ROWS][COLUMNS];
    coordinate x, y, move_x, move_y;
    int turn = PLAYER_1;
    int win_count = 0;
    checkerboard_init(&(checkerboard[0][0]));
    checkerboard_print(&(checkerboard[0][0]));
    /*setvbuf(stdout, NULL, _IONBF, 0);*/ /* permette di debuggare con scanf */
    while (win_count == 0) {
        switch (capture_check(&checkerboard[0][0], &turn)) {
            case 0:
                printf("case 0\n");
                if (win(&(checkerboard[0][0]), &turn)) {
                    win_count++;
                    continue;
                }
                printf("\nTURNO GIOCATORE %d \n", turn);
                printf("Seleziona le coordinate della pedina x , y: \n");
                scanf("%d %d", &x, &y); /*inserimento pedina da controllare*/ /*TODO, RICHIEDERE SELEZIONE SE INSERISCI non numeri*/
                if (!piece_selection(&(checkerboard[0][0]), x, y, &turn)) {
                    continue;
                } else {
                    printf("Hai selezionato le coordinate x=%d y=%d\n", x, y);
                    printf("seleziona le coordinate in cui muovere la pedina x , y: \n");
                    scanf("%d %d", &move_x, &move_y); /*inserimento coordinate destinazione pedina*/
                    if (!move_selection(&(checkerboard[0][0]), x, y, &turn, move_x, move_y)) {
                        continue;
                    } /*se la mossa è valida stampa chessboard*/
                }
                break;
            case 1:
                printf("case 1\n");
                break;
            case 2:
                printf("case 2\n");
                continue;
        }
        printf("updating\n");
        promotion_check(&(checkerboard[0][0]));
        checkerboard_print(&(checkerboard[0][0]));
        turn_update(&turn);
    }
    turn_update(&turn);
    printf("WIN Player %d ", turn);
    return 0;
}

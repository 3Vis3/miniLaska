#include "minilaska.h"
#include <stdio.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <windows.h>
    #define OS_Windows 1
#endif

#define ROWS (7)
#define COLUMNS (7)
#define LIMIT_RIGHT (6)
#define LIMIT_LEFT (0)
#define LIMIT_DOWN (6)
#define LIMIT_UP (0)
#define FIRST_DIAGONAL (1)
#define SECOND_DIAGONAL (0)
#define PLAYER_TOWER checkerboard[r * COLUMNS + c].player
#define HEAD_TOWER checkerboard[r * COLUMNS + c].composition[0]
#define SRC_PLAYER_TOWER checkerboard[src.r * COLUMNS + src.c].player
#define SRC_HEAD_TOWER checkerboard[src.r * COLUMNS + src.c].composition[0]
#define MOVE_HEAD_TOWER checkerboard[move.src.r * COLUMNS + move.src.c].composition[0]
#define PLAYER_DESTINATION checkerboard[move.dst.r * COLUMNS + move.dst.c].player /*player della casella in cui effettuare il movimento nella funzione piece_move*/
#define PLAYER_ENEMY checkerboard[enemy.r * COLUMNS + enemy.c].player /*player nemico che sta per essere mangiato*/
#define HEAD_ENEMY checkerboard[enemy.r * COLUMNS + enemy.c].composition[0]
#define TAIL_ENEMY checkerboard[enemy.r * COLUMNS + enemy.c].composition[COMPOSITION_SIZE-1]
#define _ERROR (-1)





const size_t COMPOSITION_SIZE = 3; /*TODO SISTEMA TAIL ENEMY*/
int s_chance[3] = {0, 0, 0};


/*funzione per la parte grafica, passando il tipo di os (w, windows, oppure l, linux) permette di cambiare il colore per la stampa*/
void print_player_color (char color, char player) {
    #ifdef OS_Windows
        HANDLE hConsole;
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        if(color == 'r'){
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            printf(" %c ", player);
            SetConsoleTextAttribute(hConsole, 15); /*colore bianco*/
        }
        else if (color == 'y') {
            SetConsoleTextAttribute(hConsole, 14);
            printf(" %c ", player);
            SetConsoleTextAttribute(hConsole, 15);
        }
        else if (color == 'b') {
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
            printf(" %c ", player);
            SetConsoleTextAttribute(hConsole, 15);
        }
    #else
        if(color == 'r'){
            printf("%s %c %s","\x1b[31m", player, "\x1b[0m");
        }
        else if (color == 'y') {
            printf("%s %c %s","\x1b[33m", player, "\x1b[0m");
        }else if (color == 'b') {
            printf("%s %c %s","\x1b[34m", player, "\x1b[0m");
        }

    #endif

}


/*ogni casella è formata da una struttura tower_t, composta da player (un intero) e da un array di tre elementi (composition),
il primo elemento corrisponde alla testa (la quale determinerà il campo player), il secondo alla parte centrale e il terzo alla coda.
Nel caso la pedina fosse formata da meno di tre elementi, l'elemento dell'array sarà uguale a 0*/
void checkerboard_init(tower_t *checkerboard){ /*funzione che inizializza la scacchiera e anche le pedine N.B. sarà chiamata una volta sola */
    int r, c, i;
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLUMNS; c++) {
            if (r < 3 && ((r % 2 == 0 && c % 2 == 0) || (r % 2 != 0 && c % 2 !=0))) { /*se ci troviamo sulla parte superiore della scacchiera e l'indice delle righe e colonne è entrambe pari o entrambe dispari*/
                PLAYER_TOWER = PLAYER_1;                              /*assegnamo il posto al giocatore 1 e componiamo la sua torre*/
                HEAD_TOWER = PLAYER_1;
                for (i = 1; i < COMPOSITION_SIZE; i++) {
                    checkerboard[r * COLUMNS + c].composition[i] = 0;
                }
            }else if (r > 3 && ((r % 2 == 0 && c % 2 == 0) || (r % 2 != 0 && c % 2 !=0))) { /*se ci troviamo sulla parte della scacchiera inferiore e l'indice delle righe e colonne è o entrambe pari o entrambe dispari*/
                PLAYER_TOWER = PLAYER_2;                               /*assegnamo il posto al giocatore 2 e componiamo la sua torre*/
                HEAD_TOWER = PLAYER_2;
                for (i = 1; i < COMPOSITION_SIZE; i++) {
                    checkerboard[r * COLUMNS + c].composition[i] = 0;
                }
            } else {
                PLAYER_TOWER = VOID_0;                                  /*la casella avrà una struttura tower_t inizializzata a 0 (vuota), sia nel player che nella composition*/
                HEAD_TOWER = VOID_0;
                for (i = 1; i < COMPOSITION_SIZE; i++) {
                    checkerboard[r * COLUMNS + c].composition[i] = 0;
                }
            } /*TODO generalizzare controlli +1 e -1 e aumentsre grandezza scacchiera*/
        }
    }
}

int string_to_coordinate(const char* s, coordinate_t* src) {
    if(s){
        src->c = s[0] - 'a';
        src->r = s[1] - '0';
        return 1;
    }
    return 0;
}

void checkerboard_print (tower_t *checkerboard, coordinate_t last_move) { /*funzione di stampa della checkerboard che verrà chiamata tutte le volte in cui viene fatta una mossa*/
    int r , c, square;
    char color;
    char* map = "abcdefg";

    for (r = 0; r < ROWS; r++) {
        printf("%d|", r); /*stampa l'indice delle righe*/
        for (c = 0; c < COLUMNS; c++) {
            color = 'a'; /*valore iniziale non possibile*/
            if(last_move.r == r && last_move.c == c) { /*cambia colore ultima mossa in blu evidenziandola*/
                color = 'b';
            }

            square = PLAYER_TOWER;
            if (square == PLAYER_1) {
                if(color == 'a'){
                    color = 'r';
                }
                if (HEAD_TOWER == PLAYER_1_PRO) {
                    print_player_color(color, 'O');
                } else {
                    print_player_color(color, 'o');
                }
            } else if (square == PLAYER_2){
                if(color == 'a'){
                    color = 'y';
                }
                if (HEAD_TOWER == PLAYER_2_PRO) {
                    print_player_color (color, 'X');
                } else {
                    print_player_color (color, 'x');
                }
            } else{
                printf("   ");
            }
        }
        printf("\n");
    }
    printf("  ");
    for (c = 0; c < COLUMNS; c++) { /*ciclo per stampare l'indice delle colonne*/
        printf(" %c ", map[c]);
    }
    printf("\n");
}

void turn_update(int *turn){ /*funzione che cambia turno al termine di ogni mossa avvenuta con successo*/
    if(*turn == PLAYER_1){
        *turn = PLAYER_2;
    }else{
        *turn = PLAYER_1;
    }
}

void promotion_check(tower_t *checkerboard) { /*dopo ogni mossa controlla se a fine scacchiera c'è un player che deve essere promosso*/
    int c, r;
    for (c = 0; c < 7; c++) {
        r = LIMIT_UP;
        if (HEAD_TOWER == PLAYER_2) {
            HEAD_TOWER = PLAYER_2_PRO;
        }
        r = LIMIT_DOWN;
        if (HEAD_TOWER == PLAYER_1) {
            HEAD_TOWER = PLAYER_1_PRO;
        }
    }
}

bool control_range(const int c, const int r){ /*controlla che le coordinate x e y siano dentro la scacchiera (comprese tra 0 e 6), se sono all'interno restituisce 1,
 *                                                              altrimenti restituisce 0*/
    if (c >= LIMIT_LEFT && c <= LIMIT_RIGHT && r >= LIMIT_UP && r <= LIMIT_DOWN) {
        return 1;
    }else {
        return 0;
    }
}

bool piece_selection (tower_t *checkerboard, coordinate_t src, int turn){ /*controlla se la pedina, date le coordinate in input, corrisponde al proprio player.
                                                                                                       Viene chiamata dopo che l'utente ha inserito le coordinate per verificarne la validità*/
    if (control_range(src.c, src.r) && SRC_PLAYER_TOWER == turn){ /*controlla se le coordinate sono valide*/
        return 1;
    } else {
        printf("SCELTA SBAGLIATA\n");
        return 0;
    }
}/*restituisce 1 se la pedina è in una posizione in cui può essere scelta altimenti restituisce 0*/

void composition_update(tower_t *checkerboard, move_t move) {
    int i;
    for (i = 0; i < COMPOSITION_SIZE; i ++) {
        checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[i] = checkerboard[move.src.r * COLUMNS + move.src.c].composition[i];
    }
} /*TODO da generalizzare dopo mangiata*/

void clear_square(tower_t *checkerboard, coordinate_t src) {
    int i;
    SRC_PLAYER_TOWER = VOID_0; /*svuota la casella che ha effettuato la mangiata*/
    for (i = 0; i < COMPOSITION_SIZE; i ++) {
        checkerboard[src.r * COLUMNS + src.c].composition[i] = VOID_0;
    }
}

bool move_selection(tower_t *checkerboard, move_t move, int turn){
    /* controlla se le coordinate della destinazione inserite dall'utente corrispondono ad una casella vuota e valida*/
    if (control_range (move.dst.c, move.dst.r)) {
        if (turn == PLAYER_1 && PLAYER_DESTINATION == VOID_0) {
            if (MOVE_HEAD_TOWER == PLAYER_1_PRO) { /*se il player è promosso ha liberta di movimento in ogni direzione*/
                if ((move.dst.r == move.src.r + 1 || move.dst.r == move.src.r - 1) && (move.dst.c == move.src.c + 1 || move.dst.c == move.src.c - 1) &&
                    PLAYER_DESTINATION == VOID_0 && control_range(move.dst.c, move.dst.r)) {
                    PLAYER_DESTINATION = turn; /*assegnamento alla casella del player*/
                    composition_update(checkerboard, move);
                    clear_square(checkerboard, move.src);
                    /*printf("player vecchio e %d, player nuovo e %d, composition vecchia e %d %d %d, composition nuova e %d %d %d", checkerboard[move.src.r * COLUMNS + move.src.c].player, checkerboard[move.dst.r * COLUMNS + move.dst.c].player, checkerboard[move.src.r * COLUMNS + move.src.c].composition[0], checkerboard[move.src.r * COLUMNS + move.src.c].composition[1], checkerboard[move.src.r * COLUMNS + move.src.c].composition[2], checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[0], checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[1], checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[2]);
                    */
                    return 1;
                }
            } /*player 1 non pro*/
            else if ((move.dst.r == move.src.r + 1) && ((move.dst.c == move.src.c + 1) || (move.dst.c == move.src.c - 1)) && PLAYER_DESTINATION == VOID_0 &&
                     control_range(move.dst.c, move.dst.r)) { /*verifica se destinazione è valida e verifica che in quella posizione non ci siano altre pedine e che il range sia corretto*/
                PLAYER_DESTINATION = turn; /*assegnamento alla casella del player*/
                composition_update(checkerboard, move);
                clear_square(checkerboard, move.src);
                /*printf("player vecchio e %d, player nuovo e %d, composition vecchia e %d %d %d, composition nuova e %d %d %d", checkerboard[move.src.r * COLUMNS + move.src.c].player, checkerboard[move.dst.r * COLUMNS + move.dst.c].player, checkerboard[move.src.r * COLUMNS + move.src.c].composition[0], checkerboard[move.src.r * COLUMNS + move.src.c].composition[1], checkerboard[move.src.r * COLUMNS + move.src.c].composition[2], checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[0], checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[1], checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[2]);
                */
                return 1;
            }
        } else if (turn == PLAYER_2 && PLAYER_DESTINATION == VOID_0) {
            if (MOVE_HEAD_TOWER == PLAYER_2_PRO) {
                if ((move.dst.r == move.src.r + 1 || move.dst.r == move.src.r - 1) && (move.dst.c == move.src.c + 1 || move.dst.c == move.src.c - 1) &&
                    PLAYER_DESTINATION == VOID_0 && control_range(move.dst.c, move.dst.r)) {
                    PLAYER_DESTINATION = turn; /*assegnamento alla casella del player*/
                    composition_update(checkerboard, move);
                    clear_square(checkerboard, move.src);
                    /*printf("player vecchio e %d, player nuovo e %d, composition vecchia e %d %d %d, composition nuova e %d %d %d", checkerboard[move.src.r * COLUMNS + move.src.c].player, checkerboard[move.dst.r * COLUMNS + move.dst.c].player, checkerboard[move.src.r * COLUMNS + move.src.c].composition[0], checkerboard[move.src.r * COLUMNS + move.src.c].composition[1], checkerboard[move.src.r * COLUMNS + move.src.c].composition[2], checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[0], checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[1], checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[2]);
                    */
                    return 1;
                }
            } /*player 2 non pro*/
            else if ((move.dst.r == move.src.r - 1) && ((move.dst.c == move.src.c + 1) || (move.dst.c == move.src.c - 1)) && PLAYER_DESTINATION == VOID_0 &&
                     control_range(move.dst.c, move.dst.r)) { /*verifica se destinazione è valida e verifica che in quella posizione non ci siano altre pedine e che il range sia corretto*/
                PLAYER_DESTINATION = turn;
                composition_update(checkerboard, move);
                clear_square(checkerboard, move.src);
                /*printf("player vecchio e %d, player nuovo e %d, composition vecchia e %d %d %d, composition nuova e %d %d %d", checkerboard[move.src.r * COLUMNS + move.src.c].player, checkerboard[move.dst.r * COLUMNS + move.dst.c].player, checkerboard[move.src.r * COLUMNS + move.src.c].composition[0], checkerboard[move.src.r * COLUMNS + move.src.c].composition[1], checkerboard[move.src.r * COLUMNS + move.src.c].composition[2], checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[0], checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[1], checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[2]);
                */
                return 1;
            }
        }
    }
    /*printf("MOSSA NON VALIDA\n");*/
    return 0;
}

player_t diagonal_down_left_check(tower_t *checkerboard, coordinate_t src, bool is_first_diagonal){
    /*il "is_first_diagonal" serve per capire se la diagonale su cui eseguire il controllo è la prima (1) altrimenti è la seconda (0)
     * la prima diagonale corrisponde a quella adiacente, la seconda diagonale corrisponde a quella dove la pedina dovrà muoversi se potrà mangiare */
    if(is_first_diagonal && control_range(src.c - 1, src.r + 1)){
        return checkerboard[(src.r + 1) * COLUMNS + src.c - 1].player;
    }else if(!is_first_diagonal && control_range(src.c - 2, src.r + 2)) {
        return checkerboard[(src.r + 2) * COLUMNS + src.c - 2].player;
    }else{
        return _ERROR;
    }
}

player_t diagonal_down_right_check(tower_t *checkerboard, coordinate_t src, bool is_first_diagonal){
    /*is_first_diagonal serve per capire se la diaonale su cui eseguire il controllo è la prima (1) altrimenti è la seconda (0)
     * la prima diagonale corrisponde a quella adiacente, la seconda diagonale corrisponde a quella dove la pedina dovrà muoversi se potrà mangiare */
    if(is_first_diagonal && control_range(src.c + 1, src.r + 1)){
        return checkerboard[(src.r + 1) * COLUMNS + src.c + 1].player;
    }else if(!is_first_diagonal && control_range(src.c + 2, src.r + 2)) {
        return checkerboard[(src.r + 2) * COLUMNS + src.c + 2].player;
    }else{
        return _ERROR;
    }
}

player_t diagonal_up_left_check(tower_t *checkerboard, coordinate_t src, bool is_first_diagonal){
    /*is_first_diagonal serve per capire se la diaonale su cui eseguire il controllo è la prima (1) altrimenti è la seconda (0)
     * la prima diagonale corrisponde a quella adiacente, la seconda diagonale corrisponde a quella dove la pedina dovrà muoversi se potrà mangiare */
    if(is_first_diagonal && control_range(src.c - 1, src.r - 1)){
        return checkerboard[(src.r - 1) * COLUMNS + src.c - 1].player;
    }else if(!is_first_diagonal && control_range(src.c - 2, src.r - 2)) {
        return checkerboard[(src.r - 2) * COLUMNS + src.c - 2].player;
    }else{
        return _ERROR;
    }
}

player_t diagonal_up_right_check(tower_t *checkerboard, coordinate_t src, bool is_first_diagonal){
    /*is_first_diagonal serve per capire se la diaonale su cui eseguire il controllo è la prima (1) altrimenti è la seconda (0)
     * la prima diagonale corrisponde a quella adiacente, la seconda diagonale corrisponde a quella dove la pedina dovrà muoversi se potrà mangiare */
    if(is_first_diagonal && control_range(src.c + 1, src.r - 1)){
        return checkerboard[(src.r - 1) * COLUMNS + src.c + 1].player;
    }else if(!is_first_diagonal && control_range(src.c + 2, src.r - 2)) {
        return checkerboard[(src.r - 2) * COLUMNS + src.c + 2].player;
    }else{
        return _ERROR;
    }
}

int pieces_left (tower_t *checkerboard, int turn){
    int r, c, pieces = 0;
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLUMNS; c++) {
            if (PLAYER_TOWER == turn) { /*se ci troviamo sulla parte superiore della scacchiera e l'indice delle righe e colonne è entrambe pari o entrambe dispari*/
                pieces++;
            }
        }
    }
    return pieces;
}

bool win (tower_t *checkerboard, int turn) { /*controlla se la partita arriva al termine, verificando se non sono ancora presenti pedine del player corrente,
*                                                    oppure se ci sono ma sono bloccate. Al verificarsi di uno di questi casi ritorna 1 e interrompe il gioco*/
    int r, c, pieces_blocked = 0;
    coordinate_t src;
    int counter_pieces = pieces_left(checkerboard, turn);
    for(r = 0; r < ROWS; r++) {
        for (c = 0; c < COLUMNS; c++) {
            if (PLAYER_TOWER == turn) {
                if (HEAD_TOWER == PLAYER_1) {
                    src.r = r, src.c = c;
                    if (diagonal_down_left_check(checkerboard, src, FIRST_DIAGONAL) != VOID_0 &&
                        diagonal_down_right_check(checkerboard, src, FIRST_DIAGONAL) != VOID_0) {
                        pieces_blocked++;
                    }
                }
                if (HEAD_TOWER == PLAYER_2) {
                    src.r = r, src.c = c;
                    if (diagonal_up_left_check(checkerboard, src, FIRST_DIAGONAL) != VOID_0 &&
                        diagonal_up_right_check(checkerboard, src, FIRST_DIAGONAL) != VOID_0) {
                        pieces_blocked++;
                    }
                }
                if(HEAD_TOWER == PLAYER_1_PRO || HEAD_TOWER == PLAYER_2_PRO){
                    src.r = r, src.c = c;
                    if(diagonal_down_left_check(checkerboard, src, FIRST_DIAGONAL) != VOID_0 &&
                       diagonal_down_right_check(checkerboard, src, FIRST_DIAGONAL) != VOID_0 &&
                       diagonal_up_left_check(checkerboard, src, FIRST_DIAGONAL) != VOID_0 &&
                       diagonal_up_right_check(checkerboard, src, FIRST_DIAGONAL) != VOID_0){
                        pieces_blocked++;
                    }
                }
            }
        }
    }

    if(pieces_blocked == counter_pieces){
        return 1;
    }
    if(counter_pieces != 0){
        return 0;
    } else {
        return 1;
    }
}

void piece_capture(tower_t *checkerboard, move_t move, int turn, coordinate_t enemy){
    /*effettua la mangiata e il conseguente aggiornamento della composzione delle torri
    3 if per il controllo pedine intermedie dell'attaccante ed eventuale sostituzione se vuote con pedine del nemico*/
    int i;
    for (i = 1; i < COMPOSITION_SIZE; i++) {
        if (checkerboard[move.src.r * COLUMNS + move.src.c].composition[i] == 0) {
            checkerboard[move.src.r * COLUMNS + move.src.c].composition[i] = HEAD_ENEMY;
            i = COMPOSITION_SIZE;
        }
    }
    composition_update(checkerboard, move);
    /*assegnamento player a torre di destinazione*/
    PLAYER_DESTINATION = turn;

    /* sostituzione vecchia posizione dell'attaccante con parametri player=VUOTO e composizione 000*/
    clear_square(checkerboard, move.src);

    /*sostituzione ultima pedina mangiata con ZERO e scalamento delle superiori */
    /*scala di una posizione gli elementi della pedina mangiata*/
    for (i = 0; i < COMPOSITION_SIZE - 1; i ++) {
        checkerboard[enemy.r * COLUMNS + enemy.c].composition[i] = checkerboard[enemy.r * COLUMNS + enemy.c].composition[i + 1]; /*scala di una posizione gli elementi della pedina mangiata*/
    }
    TAIL_ENEMY = VOID_0; /*TODO DEVI FARE FOR PER METTERE VOID_0 AL PRIMO ELEMENTO GIUSTO*/

    /*assegnamento nuovo player a pedina mangiata basandosi sulla testa*/
    if (HEAD_ENEMY == 0) { /*controllo testa pedina attaccata*/
        PLAYER_ENEMY = VOID_0;
    } else if (HEAD_ENEMY == 1 || HEAD_ENEMY == 3) { /*controllo se testa è player 1 o player 2 e aggiorna player*/
        PLAYER_ENEMY = PLAYER_1;
    } else if (HEAD_ENEMY == 2 || HEAD_ENEMY == 4) {
        PLAYER_ENEMY = PLAYER_2;
    }
    /*stampe di verifiche composizione player*/
    /*printf("\nl'attaccante ora vale %d %d %d", checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[0], checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[1], checkerboard[move.dst.r * COLUMNS + move.dst.c].composition[2]);
    printf("\nil mangiato ora vale %d, %d, %d", checkerboard[enemy.r * COLUMNS + enemy.c].composition[0], checkerboard[enemy.r * COLUMNS + enemy.c].composition[1], checkerboard[enemy.r * COLUMNS + enemy.c].composition[2]);
    printf("\n la vecchia posizione ora vale %d %d %d\n", checkerboard[move.src.r * COLUMNS + move.src.c].composition[0], checkerboard[move.src.r * COLUMNS + move.src.c].composition[1], checkerboard[move.src.r * COLUMNS + move.src.c].composition[2]);
    printf("i player ora sono attaccante %d, attaccato %d, vecchia posizione %d\n", checkerboard[move.dst.r * COLUMNS + move.dst.c].player, checkerboard[enemy.r * COLUMNS + enemy.c].player, checkerboard[move.src.r * COLUMNS + move.src.c].player);
    */
}

/*ritorna il numero delle mosse possibili date le coordinate della pedina src (ex get_nr_all_moves), modifica inoltre l'array move con tutte le possibili coordinate move*/
int can_piece_be_moved(tower_t* checkerboard, coordinate_t src, move_t* move, int nr_moves, int turn){

    /*puntatore indirizzo move avrà al massimo 4 elementi, ovvero le 4 coordinate in cui una pedina pro può mangiare*/

    if (turn == PLAYER_1 && SRC_PLAYER_TOWER == PLAYER_1) { /*mi interessa sapere se, date le coordinate r e c della pedina, ci sono movimenti possibili*/

        if (SRC_HEAD_TOWER == PLAYER_1_PRO) { /*se il player è promosso ha liberta di movimento in ogni direzione*/

            if (control_range(src.c + 1, src.r - 1) && checkerboard[(src.r - 1) * COLUMNS + (src.c + 1)].player == VOID_0){
                move[nr_moves].src.r = src.r;
                move[nr_moves].src.c = src.c;
                move[nr_moves].dst.r = src.r - 1;
                move[nr_moves].dst.c = src.c + 1;
                nr_moves++;
            }
            if(control_range(src.c - 1, src.r - 1) && checkerboard[(src.r - 1) * COLUMNS + (src.c - 1)].player == VOID_0 ) {
                move[nr_moves].src.r = src.r;
                move[nr_moves].src.c = src.c;
                move[nr_moves].dst.r = src.r - 1;
                move[nr_moves].dst.c = src.c - 1;
                nr_moves++;
            }
        } /*player 1 non pro*/

        if (control_range(src.c + 1, src.r + 1) && checkerboard[(src.r + 1) * COLUMNS + (src.c + 1)].player == VOID_0){
            move[nr_moves].src.r = src.r;
            move[nr_moves].src.c = src.c;
            move[nr_moves].dst.r = src.r + 1;
            move[nr_moves].dst.c = src.c + 1;
            nr_moves++;
        }
        if(control_range(src.c - 1, src.r + 1) && checkerboard[(src.r + 1) * COLUMNS + (src.c - 1)].player == VOID_0) { /*verifica se destinazione è valida e verifica che in quella posizione non ci siano altre pedine e che il range sia corretto*/
            move[nr_moves].src.r = src.r;
            move[nr_moves].src.c = src.c;
            move[nr_moves].dst.r = src.r + 1;
            move[nr_moves].dst.c = src.c - 1;
            nr_moves++;
        }

    } else if (turn == PLAYER_2 && SRC_PLAYER_TOWER == PLAYER_2) {
        if (SRC_HEAD_TOWER == PLAYER_2_PRO) {
            if (control_range(src.c + 1, src.r + 1) && checkerboard[(src.r + 1) * COLUMNS + (src.c + 1)].player == VOID_0) {
                move[nr_moves].src.r = src.r;
                move[nr_moves].src.c = src.c;
                move[nr_moves].dst.r = src.r + 1;
                move[nr_moves].dst.c = src.c + 1;
                nr_moves++;
            }
            if (control_range(src.c - 1, src.r + 1) && checkerboard[(src.r + 1) * COLUMNS + (src.c - 1)].player == VOID_0 ) {
                move[nr_moves].src.r = src.r;
                move[nr_moves].src.c = src.c;
                move[nr_moves].dst.r = src.r + 1;
                move[nr_moves].dst.c = src.c - 1;
                nr_moves++;
            }
        } /*player 2 non pro*/
        if (control_range(src.c + 1, src.r - 1) && checkerboard[(src.r - 1) * COLUMNS + (src.c + 1)].player == VOID_0) {
            move[nr_moves].src.r = src.r;
            move[nr_moves].src.c = src.c;
            move[nr_moves].dst.r = src.r - 1;
            move[nr_moves].dst.c = src.c + 1;
            nr_moves++;
        }
        if (control_range(src.c - 1, src.r - 1) && checkerboard[(src.r - 1) * COLUMNS + (src.c - 1)].player == VOID_0) { /*verifica se destinazione è valida e verifica che in quella posizione non ci siano altre pedine e che il range sia corretto*/
            move[nr_moves].src.r = src.r;
            move[nr_moves].src.c = src.c;
            move[nr_moves].dst.r = src.r - 1;
            move[nr_moves].dst.c = src.c - 1;
            nr_moves++;
        }
    }

    /*TODO mossa mangiata in tutte e 4 le direzioni*/

    return nr_moves;
}

/*verifica se la pedina di coordinate src ha manigate possibili e mette nelgli array move e enemy
 * le coordinate della mangiata e del nemico, restituendo se ci sono mangiate o no
 * nr_captures corrisponde all'indice nel quale salvare le mosse */
int can_piece_capture (tower_t* checkerboard, coordinate_t src, move_t* move, coordinate_t* enemy, int nr_captures, int turn){

    if (SRC_PLAYER_TOWER == turn) {
        if (turn == PLAYER_1 /*|| PLAYER2 == promoted*/) {
            if (diagonal_down_left_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_2) {
                if (diagonal_down_left_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                    move[nr_captures].src = src;
                    move[nr_captures].dst.r = src.r + 2;
                    move[nr_captures].dst.c = src.c - 2;
                    enemy[nr_captures].r = src.r + 1;
                    enemy[nr_captures].c = src.c - 1;
                    nr_captures++;
                }
            }
            if (diagonal_down_right_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_2) {
                if (diagonal_down_right_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                    move[nr_captures].src = src;
                    move[nr_captures].dst.r = src.r + 2;
                    move[nr_captures].dst.c = src.c + 2;
                    enemy[nr_captures].r = src.r + 1;
                    enemy[nr_captures].c = src.c + 1;
                    nr_captures++;
                }
            }
            if (SRC_HEAD_TOWER == PLAYER_1_PRO) {
                if (diagonal_up_left_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_2) {
                    if (diagonal_up_left_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                        move[nr_captures].src = src;
                        move[nr_captures].dst.r = src.r - 2;
                        move[nr_captures].dst.c = src.c - 2;
                        enemy[nr_captures].r = src.r - 1;
                        enemy[nr_captures].c = src.c - 1;
                        nr_captures++;
                    }
                }
                if (diagonal_up_right_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_2) {
                    if (diagonal_up_right_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                        move[nr_captures].src = src;
                        move[nr_captures].dst.r = src.r - 2;
                        move[nr_captures].dst.c = src.c + 2;
                        enemy[nr_captures].r = src.r - 1;
                        enemy[nr_captures].c = src.c + 1;
                        nr_captures++;
                    }
                }
            }
        }
        if (turn == PLAYER_2 /*|| PLAYER1 == promoted*/) {
            if (diagonal_up_left_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_1) {
                if (diagonal_up_left_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                    move[nr_captures].src = src;
                    move[nr_captures].dst.r = src.r - 2;
                    move[nr_captures].dst.c = src.c - 2;
                    enemy[nr_captures].r = src.r - 1;
                    enemy[nr_captures].c = src.c - 1;
                    nr_captures++;
                }
            }
            if (diagonal_up_right_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_1) {
                if (diagonal_up_right_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                    move[nr_captures].src = src;
                    move[nr_captures].dst.r = src.r - 2;
                    move[nr_captures].dst.c = src.c + 2;
                    enemy[nr_captures].r = src.r - 1;
                    enemy[nr_captures].c = src.c + 1;
                    nr_captures++;
                }
            }
            if (SRC_HEAD_TOWER == PLAYER_2_PRO) {
                if (diagonal_down_left_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_1) {
                    if (diagonal_down_left_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                        move[nr_captures].src = src;
                        move[nr_captures].dst.r = src.r + 2;
                        move[nr_captures].dst.c = src.c - 2;
                        enemy[nr_captures].r = src.r + 1;
                        enemy[nr_captures].c = src.c - 1;
                        nr_captures++;
                    }
                } if (diagonal_down_right_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_1) {
                    if (diagonal_down_right_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                        move[nr_captures].src = src;
                        move[nr_captures].dst.r = src.r + 2;
                        move[nr_captures].dst.c = src.c + 2;
                        enemy[nr_captures].r = src.r + 1;
                        enemy[nr_captures].c = src.c + 1;
                        nr_captures++;
                    }
                }
            }
        }
    }

    return nr_captures;
    /*ritorna valore effettivo di mangiate salvate nell'array*/
}


int capture_check(tower_t *checkerboard, int turn){
    int r, c;
    char selection; /*variabili char checkerboard da scanf riceve s oppure n e fa il confronto tra selezione e i caratteri n o s*/ /*TODO SISTEMARE LOOP INFINITO SE INSERISCI SN*/
    char si= 's';
    int count=0; /*conta le pedine obbligate a mangiare*/
    coordinate_t src;
    move_t move;
    coordinate_t enemy /*, check */;
    for (r = s_chance[0]; r < ROWS; r++) {
        for (c = s_chance[1]; c < COLUMNS; c++) {
            if (PLAYER_TOWER == turn) {
                src.c = c, src.r = r;
                if (turn == PLAYER_1 /*|| PLAYER2 == promoted*/) {
                    if (diagonal_down_left_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_2) {
                        /*ho inserito degli and per evitare il controllo a sx se sei vicino al bordo sx e viceversa a dx*/
                        /*controllo diagonale sinistra checkerboard sia player avversario e non vuoto*/
                        if (diagonal_down_left_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                            /*TODO funzione checkerboard controlla se esce dall'array del checkerboard*/
                            /*passo i e c come coordinate per mangiata obbligatoria*/
                            count++; /*incrementa il conteggio se rilevata una pedina obbligata*/
                            printf("TURNO GIOCATORE %d\n\n", turn);
                            printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c - 1, r + 1);
                            scanf(" %c", &selection); /*chiede se vuoi mangiare con quella pedina*/
                            if (selection==si) { /*se dici si chiama mangiata*/
                                s_chance[0] = r + 2;
                                s_chance[1] = c - 2;
                                s_chance[2] = 1;
                                move.src.c = c;
                                move.src.r = r;
                                move.dst.c = c - 2;
                                move.dst.r = r + 2;
                                enemy.c = c - 1;
                                enemy.r = r + 1;
                                piece_capture(checkerboard, move, turn, enemy);
                                return 1;
                            }
                        }
                    } if (diagonal_down_right_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_2) {
                        /*controllo diagonale destra checkerboard sia player avversario e non vuoto*/
                        if (diagonal_down_right_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                            /*passo i e c come coordinate per mangiata obbligatoria*/
                            count++;
                            printf("TURNO GIOCATORE %d\n\n", turn);
                            printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c + 1, r + 1);
                            scanf(" %c", &selection);
                            if (selection==si) {
                                s_chance[0] = r + 2;
                                s_chance[1] = c + 2;
                                s_chance[2] = 1;
                                move.src.c = c;
                                move.src.r = r;
                                move.dst.c = c + 2;
                                move.dst.r = r + 2;
                                enemy.c = c + 1;
                                enemy.r = r + 1;
                                piece_capture(checkerboard, move, turn, enemy);
                                return 1;
                            }
                        }
                    }
                    if (HEAD_TOWER == PLAYER_1_PRO) {
                        if (diagonal_up_left_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_2) {
                            /*controllo diagonale sinistra checkerboard sia player avversario e non vuoto*/
                            if (diagonal_up_left_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                                /*passo i e c come coordinate per mangiata obbligatoria*/
                                count++;
                                printf("TURNO GIOCATORE %d\n\n", turn);
                                printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c - 1, r - 1);
                                scanf(" %c", &selection);
                                if (selection==si) {
                                    s_chance[0] = r - 2;
                                    s_chance[1] = c - 2;
                                    s_chance[2] = 1;
                                    move.src.c = c;
                                    move.src.r = r;
                                    move.dst.c = c - 2;
                                    move.dst.r = r - 2;
                                    enemy.c = c - 1;
                                    enemy.r = r - 1;
                                    piece_capture(checkerboard, move, turn, enemy);
                                    return 1;
                                }
                            }
                        }
                        if (diagonal_up_right_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_2) {
                            /*controllo diagonale destra checkerboard sia player avversario e non vuoto*/
                            if (diagonal_up_right_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                                /*passo i e c come coordinate per mangiata obbligatoria*/
                                count++;
                                printf("TURNO GIOCATORE %d\n\n", turn);
                                printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c + 1, r - 1);
                                scanf(" %c", &selection);
                                if (selection==si) {
                                    s_chance[0] = r - 2;
                                    s_chance[1] = c + 2;
                                    s_chance[2] = 1;
                                    move.src.c = c;
                                    move.src.r = r;
                                    move.dst.c = c + 2;
                                    move.dst.r = r - 2;
                                    enemy.c = c + 1;
                                    enemy.r = r - 1;
                                    piece_capture(checkerboard, move, turn, enemy);
                                    return 1;
                                }
                            }
                        }
                    }
                }
                if (turn == PLAYER_2 /*|| PLAYER1 == promoted*/) {
                    if (diagonal_up_left_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_1) {
                        /*controllo diagonale sinistra checkerboard sia player avversario e non vuoto*/
                        if (diagonal_up_left_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                            /*passo i e c come coordinate per mangiata obbligatoria*/
                            count++;
                            printf("TURNO GIOCATORE %d\n\n", turn);
                            printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c - 1, r - 1);
                            scanf(" %c", &selection);
                            if (selection==si) {
                                s_chance[0] = r - 2;
                                s_chance[1] = c - 2;
                                s_chance[2] = 1;
                                move.src.c = c;
                                move.src.r = r;
                                move.dst.c = c - 2;
                                move.dst.r = r - 2;
                                enemy.c = c - 1;
                                enemy.r = r - 1;
                                piece_capture(checkerboard, move, turn, enemy);
                                return 1;
                            }
                        }
                    }
                    if (diagonal_up_right_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_1) {
                        /*controllo diagonale destra checkerboard sia player avversario e non vuoto*/
                        if (diagonal_up_right_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                            /*passo i e c come coordinate per mangiata obbligatoria*/
                            count++;
                            printf("TURNO GIOCATORE %d\n\n", turn);
                            printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c + 1, r - 1);
                            scanf(" %c", &selection);
                            if (selection==si) {
                                s_chance[0] = r - 2;
                                s_chance[1] = c + 2;
                                s_chance[2] = 1;
                                move.src.c = c;
                                move.src.r = r;
                                move.dst.c = c + 2;
                                move.dst.r = r - 2;
                                enemy.c = c + 1;
                                enemy.r = r - 1;
                                piece_capture(checkerboard, move, turn, enemy);
                                return 1;
                            }
                        }
                    }
                    if (HEAD_TOWER == PLAYER_2_PRO) {
                        if (diagonal_down_left_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_1) {
                            /*ho inserito degli and per evitare il controllo a sx se sei vicino al bordo sx e viceversa a dx*/
                            /*controllo diagonale sinistra checkerboard sia player avversario e non vuoto*/
                            if (diagonal_down_left_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                                /*passo i e c come coordinate per mangiata obbligatoria*/
                                count++; /*incrementa il conteggio se rilevata una pedina obbligata*/
                                printf("TURNO GIOCATORE %d\n\n", turn);
                                printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c - 1, r + 1);
                                scanf(" %c", &selection); /*chiede se vuoi mangiare con quella pedina*/
                                if (selection==si) { /*se dici si chiama mangiata*/
                                    s_chance[0] = r + 2;
                                    s_chance[1] = c - 2;
                                    s_chance[2] = 1;
                                    move.src.c = c;
                                    move.src.r = r;
                                    move.dst.c = c - 2;
                                    move.dst.r = r + 2;
                                    enemy.c = c - 1;
                                    enemy.r = r + 1;
                                    piece_capture(checkerboard, move, turn, enemy);
                                    return 1;
                                }
                            }
                        } if (diagonal_down_right_check(checkerboard, src, FIRST_DIAGONAL) == PLAYER_1) {
                            /*controllo diagonale destra checkerboard sia player avversario e non vuoto*/
                            if (diagonal_down_right_check(checkerboard, src, SECOND_DIAGONAL) == VOID_0) {
                                /*passo i e c come coordinate per mangiata obbligatoria*/
                                count++;
                                printf("TURNO GIOCATORE %d\n\n", turn);
                                printf("vuoi mangiare con x=%d y=%d il nemico in x=%d y=%d s/n?\n", c, r, c + 1, r + 1);
                                scanf(" %c", &selection);
                                if (selection==si) {
                                    s_chance[0] = r + 2;
                                    s_chance[1] = c + 2;
                                    s_chance[2] = 1;
                                    move.src.c = c;
                                    move.src.r = r;
                                    move.dst.c = c + 2;
                                    move.dst.r = r + 2;
                                    enemy.c = c + 1;
                                    enemy.r = r + 1;
                                    piece_capture(checkerboard, move, turn, enemy);
                                    return 1;
                                }
                            }
                        }
                    }
                }
            } if (s_chance[2] != 0 && count == 0) {
                s_chance[0] = 0;
                s_chance[1] = 0;
                s_chance[2] = 0;
                return 3;
            }
        }
    } if (count != 0) { /*se il conteggio delle obbligate è diverso da zero riparte il controllo*/
        return 2; /*nel caso l'utente non vuole fare la mangiata*/
    }
    return 0; /*se il conteggio è uguale a zero ritorna al main*/
}

/**
 * suggerisce all'utente le mosse valide che può fare una volta selezionate le pedine
 *//*
void draw_valid_moves (){

}*/



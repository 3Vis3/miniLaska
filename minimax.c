/**
* @file minimax.c
* @author Gruppo 15
* @brief sorgente contenente l'algoritmo dell'intelligienza artificiale per la CPU in modalità 1 vs CPU
* @date 16/01/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include "minilaska.h"


#define ROWS (7)
#define COLUMNS (7)
#define LIMIT_RIGHT (6)
#define LIMIT_LEFT (0)
#define LIMIT_DOWN (6)
#define LIMIT_UP (0)
#define FIRST_DIAGONAL (1)
#define SECOND_DIAGONAL (0)
#define ERROR_ (-1)
#define PLAYER_TOWER checkerboard[r * COLUMNS + c].player
#define HEAD_TOWER checkerboard[r * COLUMNS + c].composition[0]
#define SRC_PLAYER_TOWER checkerboard[src.r * COLUMNS + src.c].player
#define SRC_HEAD_TOWER checkerboard[src.r * COLUMNS + src.c].composition[0]
#define MOVE_HEAD_TOWER checkerboard[move.src.r * COLUMNS + move.src.c].composition[0]
#define PLAYER_DESTINATION checkerboard[move.dst.r * COLUMNS + move.dst.c].player /*player della casella in cui effettuare il movimento nella funzione piece_move*/
#define PLAYER_ENEMY checkerboard[enemy.r * COLUMNS + enemy.c].player /*player nemico che sta per essere mangiato*/
#define HEAD_ENEMY checkerboard[enemy.r * COLUMNS + enemy.c].composition[0]
#define TAIL_ENEMY checkerboard[enemy.r * COLUMNS + enemy.c].composition[COMPOSITION_SIZE-1]
#define COMPOSITION_SIZE (3)

const int MAX_DEPTH = 7;
const double INF = 1000.;

/**
 * Restituisce il punteggio della composizione di una singola torre in base a quanto è alta e a quanti player promossi
 * ha, propri e avversari
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale flattened di tipo struct tower)
 * @param src - coordinate sorgenti della pedina dove effettuare il controllo
 * @param turn
 * @param score - punteggio da modificare
 * @return score, dopo averlo incrementato/ decrementato
 */
double get_composition_score (const tower_t* checkerboard, coordinate_t src, int turn, double score) {
    int i;
    for (i = 1; i < COMPOSITION_SIZE && checkerboard[src.r * COLUMNS + src.c].composition[i] != VOID_0; i++) {

        if (checkerboard[src.r * COLUMNS + src.c].composition[i] == turn) {
            /*se la pedina è del player attuale, ovvero corrisponde al turno, incremento lo score in base se è pro o normale*/
            if (checkerboard[src.r * COLUMNS + src.c].composition[i] == PLAYER_1_PRO ||
                checkerboard[src.r * COLUMNS + src.c].composition[i] == PLAYER_2_PRO) {
                score *= 1.50;
            } else {
                score *= 1.25;
            }
        } else { /*torre nemica*/
            if (checkerboard[src.r * COLUMNS + src.c].composition[i] == PLAYER_1_PRO ||
                checkerboard[src.r * COLUMNS + src.c].composition[i] == PLAYER_2_PRO) {
                score *= 0.60;
            } else {
                score *= 0.88;
            }
        }
    }
    return score; /*sottraggo 1 in quanto l'ho inizializzato a 1 e non 0 perchè moltiplico*/
}

/**
 * Restituisce il punteggio di una singola torre, basandosi sulla testa promossa, aggiungendo quella della sua composizione
 * @param checkerboard
 * @param src
 * @param turn
 * @param score
 * @return score, dopo averlo incrementato/ decrementato
 */
double get_square_score (tower_t* checkerboard, coordinate_t src, int turn, double score) {

    if(SRC_PLAYER_TOWER == turn){
        if(SRC_HEAD_TOWER == PLAYER_1_PRO || SRC_HEAD_TOWER == PLAYER_2_PRO){
            score += 1.5;
        }
        score = get_composition_score(checkerboard, src, turn, score);
    }else if(SRC_PLAYER_TOWER != VOID_0){ /*pedina nemica ma non vuota*/
        if(SRC_HEAD_TOWER == PLAYER_1_PRO || SRC_HEAD_TOWER == PLAYER_2_PRO){
            score -= 1.5;
        }
        score = get_composition_score(checkerboard, src, turn, score);
    }
    return score;
}

/**
 * Restituisce il punteggio effettivo della scacchiera, calcolando il  punteggio di ogni torre e della sua composizione,
 * e aggiungendoci la sottrazione tra le pedine rimaste del player della CPU sotratte a quelle del player avversario
 * @param checkerboard
 * @param turn
 * @return score, il punteggio della versione della scacchiera passata
 */
double get_total_score (tower_t *checkerboard, int turn) {
    double score;
    int r, c;
    /*sottraggo nr di pedine*/
    score = pieces_left(checkerboard, PLAYER_2) - pieces_left(checkerboard, PLAYER_1);

    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLUMNS; c++) {
            coordinate_t src;
            src.r = r, src.c = c;
            score = get_square_score(checkerboard, src, turn, score);
        }
    }
    return score;
}

/**
 * Effettua la mossa simulata nella scacchiera passata come riferimento, sia la mangiata sia il movimento
 * @param checkerboard - simulazione della scacchiera, creata con una deep copy
 * @param move
 * @param enemy
 * @param turn
 * @param eat - variabile booleana, se affermata effettua una manigata, altrimenti un movimento
 * @return 1 se è andata a buon fine, 0 altrimenti
 */
int cpu_play (tower_t* checkerboard, move_t move, coordinate_t enemy, int turn, int eat) {

    if(piece_selection(checkerboard, move.src, turn)){
        /*mangiata*/
        if(eat){
            piece_capture(checkerboard, move, turn, enemy);
            return 1;
        }
        /*mossa*/
        if(move_selection(checkerboard, move, turn)){
           return 1;
        }
    }
    return 0;
}



/**
 * Effettua una deep copy della scacchiera passata come parametro, creando nuovi puntatori e quindi non andando a modificare
 * la scacchiera base, per le simulazioni. N.B. Il chiamante si occuperà di effettuare la free della nuova scacchiera
 * @param checkerboard
 * @return
 */
tower_t* deep_tower_copy (const tower_t* checkerboard){
    int r, c, i;
    tower_t* temp_checkerboard = (tower_t*) calloc (ROWS * COLUMNS, sizeof(tower_t));
    for (r = 0; r < ROWS; r++){
        for (c = 0; c < COLUMNS; c++){
            temp_checkerboard[r * COLUMNS + c].player = PLAYER_TOWER;
            for (i = 0; i < COMPOSITION_SIZE; i++) {
                temp_checkerboard[r * COLUMNS + c].composition[i] = checkerboard[r * COLUMNS + c].composition[i];
            }
        }
    }
    return temp_checkerboard;
}

/**
 * Algoritmo ricorsivo che calcola il punteggio migliore per il proprio giocatore, in base alla versione della simulazione della scacchiera,
 * richiamando sè stessa passando una nuova simulazione della scacchiera, eseguendo tutte le mosse possibili,
 * cambiando turno e diminuendo la profondità. Si fermerà quando un giocatore raggiunge la vittoria o finisce la profondità
 * @param checkerboard
 * @param depth
 * @param turn
 * @return
 */
double minimax (tower_t *checkerboard, int depth, int turn){

    int i, i_moves, i_captures; /*indici mosse e mangiate*/
    double curr_score, best_score;
    /*array memorizzati nello stack contenenti tutte le possibili combinazioni di mosse, mangiate e nemici*/
    move_t possible_move[44], possible_capture[44];
    coordinate_t possible_enemy[44];

    /*casi base*/
    if (win(checkerboard, turn)){
        if(turn == PLAYER_2){
            return INF;
        }else{
            return  -INF;
        }
    }else if(depth == 0) {
        return get_total_score(checkerboard, turn);
    }

    /*passo ricorsivo*/
    i_captures = possible_captures(checkerboard, turn, possible_capture, possible_enemy);

    if(!i_captures){ /*calcolo mosse solo se non ci sono mangiate possibili*/
        i_moves = possible_moves(checkerboard, possible_move, turn);
    }

    if (turn == PLAYER_2) { /*assegno peggior punteggio come partenza in base al turno*/
        best_score = -INF;
    }else{
        best_score = INF;
    }

    if(i_captures) {

        for (i = 0; i < i_captures; i++) {
            tower_t* temp_checkerboard = deep_tower_copy(checkerboard);
            /*eseguo mangiata delle coordinate dell'array possible_captures[i]*/
            cpu_play(temp_checkerboard, possible_capture[i], possible_enemy[i], turn, 1);

            turn_update(&turn); /*cambio turno per chiamare minimax con turno cambiato*/

            curr_score = minimax(temp_checkerboard, depth - 1, turn);

            turn_update(&turn); /*finita la chiamata torno al turno precedente*/

            if (turn == PLAYER_2) {
                if(best_score <= curr_score) {
                    best_score = curr_score;
                }
            }else{
                if(best_score >= curr_score) {
                    best_score = curr_score;
                }
            }
            /*free della checkerboard simulata, creata con una deep copy*/
            free(temp_checkerboard);
        }

    }
    else if(i_moves) { /*nel caso non ci fossero mangiate possibili eseguo la mossa*/

        for (i = 0; i < i_moves; i++) {
            tower_t* temp_checkerboard = deep_tower_copy(checkerboard);
            /*eseguo mossa delle coordinate dell'array possible_moves[i]*/
            cpu_play(temp_checkerboard, possible_move[i], possible_enemy[43], turn, 0);
            /*passo possible_enemy[43] in quanto è poco possibile che sia occupato e non viene valutato dalla funzione*/

            turn_update(&turn); /*cambio turno per chiamare minimax con turno cambiato*/

            curr_score = minimax(temp_checkerboard, depth - 1, turn);

            turn_update(&turn); /*finita la chiamata torno al turno precedente*/

            if (turn == PLAYER_2) {
                if(best_score <= curr_score) {
                    best_score = curr_score;
                }
            }else{
                if(best_score >= curr_score) {
                    best_score = curr_score;
                }
            }
            free(temp_checkerboard);
        }
    }
    return best_score;
}


/**
 * Verifica se le coordinate della destinazione dell'ultima mangiata sono uguali a quelle di sorgente di una mangiata del turno successivo,
 * in modo da capire se la stessa pedina può effettuare una concatenazione di mangiate
 * @param old_capture - coordinate di destinazione dell'ultima mangiata
 * @param new_capture - coordinate di sorgente di una possibile mangiata del turno successivo
 * @return 1 se le coordinate coincidono e quindi può fare una concatenazione di manigate, 0 altrimenti
 */
int check_capture_concatenation_cpu(coordinate_t old_capture, coordinate_t new_capture){

    if(new_capture.r == old_capture.r && new_capture.c == old_capture.c){
        return 1;
    }
    return 0;
}

/**
 * funzione che esegue la miglior mossa per la CPU, chiamando minimax per ogni sua mossa valida delle sue pedine, viene chiamata dopo il movimento del
 * player 1 (umano)
 * */
int cpu_minimax (tower_t *checkerboard, int graphic) {

    int i, i_best_move = 0, i_moves, i_captures, check = 1;
    double curr_score, best_score = -INF;
    move_t possible_move[44], possible_capture[44];
    coordinate_t possible_enemy[44], temp_possible_capture;

    i_captures = possible_captures(checkerboard, PLAYER_2, possible_capture, possible_enemy);

    if(!i_captures){ /*calcolo mosse solo se non ci sono mangiate possibili*/
        i_moves = possible_moves(checkerboard, possible_move, PLAYER_2);
    }

    if(i_captures) {

        if(i_captures != 1){ /*se c'è solo una mangiata la eseguo dopo l'if senza controllare, perchè unica mossa possibile*/

            for (i = 0; i < i_captures; i++) {
                tower_t* temp_checkerboard = deep_tower_copy(checkerboard);
                cpu_play(temp_checkerboard, possible_capture[i], possible_enemy[i], PLAYER_2, 1);
                curr_score = minimax(temp_checkerboard, MAX_DEPTH - 1, PLAYER_1);

                if(best_score <= curr_score) {
                    best_score = curr_score;
                    i_best_move = i;
                }
                free(temp_checkerboard);
            }

        }

        /*nel caso di singola mangiata eseguo l'unica mosssa in quanto i_best_move è uguale a 0*/
        cpu_play(checkerboard, possible_capture[i_best_move], possible_enemy[i_best_move], PLAYER_2, 1);

        promotion_check(checkerboard);
        printf("\n");
        checkerboard_print(checkerboard, possible_capture[i_best_move].dst, graphic);

        /*se ha fatto la mangiata controllo se può fare la concatenazione di mangiate*/

        temp_possible_capture.r = possible_capture[i_best_move].dst.r;
        temp_possible_capture.c = possible_capture[i_best_move].dst.c;

        /*controllo concatenazione finchè esegue una mangiata*/
        while (check != 0){
            check = 0;
            i_captures = possible_captures(checkerboard, PLAYER_2, possible_capture, possible_enemy);

            for (i = 0; i < i_captures; i++) { /*verifico se tra tutte le nuove mangiate possibili c'è la pedina che ha appena mangiato*/
                if(check_capture_concatenation_cpu(temp_possible_capture, possible_capture[i].src)){

                    cpu_play(checkerboard, possible_capture[i], possible_enemy[i], PLAYER_2, 1);

                    promotion_check(checkerboard);
                    printf("\n");
                    checkerboard_print(checkerboard, possible_capture[i].dst, graphic);

                    temp_possible_capture.r = possible_capture[i].dst.r; /*nel caso di una ennesima concatenazione salvo la nuova destinazione della mangiata nella variabile temp*/
                    temp_possible_capture.c = possible_capture[i].dst.c;

                    check++;
                }
            }
        }

    }
    else if(i_moves) {

        for (i = 0; i < i_moves; i++) {
            tower_t* temp_checkerboard = deep_tower_copy(checkerboard);
            cpu_play(temp_checkerboard, possible_move[i], possible_enemy[43], PLAYER_2, 0);
            /*passo possible_enemy[43] in quanto è poco possibile che sia occupato e non va modificato dalla mossa*/
            curr_score = minimax(temp_checkerboard, MAX_DEPTH - 1, PLAYER_1);

            if(best_score <= curr_score) {
                best_score = curr_score;
                i_best_move = i;
            }
            free(temp_checkerboard);
        }
        /*decidere se fare già mossa qua o nel main */
        cpu_play(checkerboard, possible_move[i_best_move], possible_enemy[43], PLAYER_2, 0);

        promotion_check(checkerboard);
        printf("\n");

        checkerboard_print(checkerboard, possible_move[i_best_move].dst, graphic);

    }
    else { /*non ci sono nè mangiate nè mosse possibli per il player 2, quindi ha perso, non dovrebbe entrare in quanto c'è la funzione win*/
        return 0;
    }
    return 1;
}



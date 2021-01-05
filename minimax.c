//
// Created by franc on 04/12/2020.
//
#include "minilaska.h"
#include "stdio.h"
#include <stdlib.h>

#define ROWS (7)
#define COLUMNS (7)
#define LIMIT_RIGHT (6)
#define LIMIT_LEFT (0)
#define LIMIT_DOWN (6)
#define LIMIT_UP (0)
#define FIRST_DIAGONAL (1)
#define SECOND_DIAGONAL (0)
#define _ERROR (-1)
#define PLAYER_TOWER checkerboard[r * COLUMNS + c].player
#define HEAD_TOWER checkerboard[r * COLUMNS + c].composition[0]
#define SRC_PLAYER_TOWER checkerboard[src.r * COLUMNS + src.c].player
#define SRC_HEAD_TOWER checkerboard[src.r * COLUMNS + src.c].composition[0]
#define MOVE_HEAD_TOWER checkerboard[move.src.r * COLUMNS + move.src.c].composition[0]

#define PLAYER_DESTINATION checkerboard[move.dst.r * COLUMNS + move.dst.c].player /*player della casella in cui effettuare il movimento nella funzione piece_move*/

#define PLAYER_ENEMY checkerboard[enemy.r * COLUMNS + enemy.c].player /*player nemico che sta per essere mangiato*/
#define HEAD_ENEMY checkerboard[enemy.r * COLUMNS + enemy.c].composition[0]
#define TAIL_ENEMY checkerboard[enemy.r * COLUMNS + enemy.c].composition[COMPOSITION_SIZE-1]

#define COMPOSITION_SIZE (3) /*TODO SISTEMA TAIL ENEMY*/



/*TODO IMPORTANT
 * L'algoritmo minimax ha bisogno di una funzione che gli passi tutte le possibili combinazioni di mosse date le coordinate di una pedina (get_piece_moves),
 * esse serviranno per chiamare ricorsivamente la funzione minimax e valutare il punteggio (get_total_score) di ogni possibile mossa,
 * scegliendo il max o il min valore tra tutte le possibili mosse di tutte le pedine
 * */


const int MAX_DEPTH = 7;
const double INF = 100000.;


/*sottofunzione che restituisce il punteggio della composizione di una singola torre in base a quanto è alta e a quanti player promosso propri e avversari*/
double get_composition_score (tower_t* checkerboard, coordinate_t src, int turn) {
    int i;
    double score = 1.;
    for (i = 1; i < COMPOSITION_SIZE && checkerboard[src.r * COLUMNS + src.c].composition[i] != VOID_0; i++){

        if(checkerboard[src.r * COLUMNS + src.c].composition[i] == turn){
            /*se la pedina è del player attuale, ovvero corrisponde al turno, incremento lo score in base se è pro o normale*/
            if(checkerboard[src.r * COLUMNS + src.c].composition[i] == PLAYER_1_PRO || checkerboard[src.r * COLUMNS + src.c].composition[i]  == PLAYER_2_PRO) {
                score *= 1.30;
            }
            else{
                score *= 1.15;
            }
        }else{ /*torre nemica*/
            if(checkerboard[src.r * COLUMNS + src.c].composition[i] == PLAYER_1_PRO || checkerboard[src.r * COLUMNS + src.c].composition[i]  == PLAYER_2_PRO) {
                score *= 0.70;
            }
            else{
                score *= 0.85;
            }
        }
    }
    return score - 1; /*sottraggo 1 in quanto l'ho inizializzato a 1 e non 0 perchè moltiplico*/
}

/*funzione che restituisce il punteggio di una singola torre, aggiungendo quella della sua composizione*/
double get_square_score (tower_t* checkerboard, coordinate_t src, int turn) {
    double score = 0.;
    if(SRC_PLAYER_TOWER == turn){
        if(SRC_HEAD_TOWER == PLAYER_1_PRO || SRC_HEAD_TOWER == PLAYER_2_PRO){
            score += 0.5;
        }
        score += get_composition_score(checkerboard, src, turn);
    }else if(SRC_PLAYER_TOWER != VOID_0){ /*pedina nemica ma non vuota*/
        if(SRC_HEAD_TOWER == PLAYER_1_PRO || SRC_HEAD_TOWER == PLAYER_2_PRO){
            score -= 0.5;
        }
        score -= get_composition_score(checkerboard, src, turn);
    }

    return score;
}

/**
 * restituisce la sottrazione tra le pedine rimaste del player della CPU - quelle del player avversario e ci aggiunge la somma di tutte le torri in gioco
 * Per tornare un punteggio più significativo sommo lo score della sottrazione con la differenza tra il nr di pedine promosse della CPU (x 0.5) e quelle del player (x 0.5)
 * @return
 */
double get_total_score (tower_t *checkerboard, int turn) {
    double score = 0.;
    int r, c;
    if(win(checkerboard, turn)){
        if(turn == PLAYER_1){
            return -INF; /*caso migliore del turno giocatore umano, che deve minimizzare*/
        }else{
            return  INF;
        }
    }

    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLUMNS; c++) {
            coordinate_t src;
            src.r = r, src.c = c;
            score += get_square_score(checkerboard, src, turn);
        }
    }

    score += pieces_left(checkerboard, PLAYER_2) - pieces_left(checkerboard, PLAYER_1);

    return score;
}

/*restituisce numero di mangiate possibili e mette in move un array contenente le struct mossa con coordinate sorgente e destinazione delle
 * pedine che possono mangiare, da controllare prima di quelle che possono muoversi, e le coordinate del nemico, necessarie per la funzione mangiata*/
int possible_captures (tower_t* checkerboard, int turn, move_t* move, coordinate_t* enemy) {
    int r, c, nr_captures = 0;
    /*move e enemy saranno array di grandezza max 44, ovvero 11 pedine x 4 possibili mangiate ciascuna*/
    coordinate_t src;
    for (r = 0; r < ROWS; r++) {
        for(c = 0; c < COLUMNS; c++) {
            src.r = r, src.c = c;
            nr_captures = can_piece_capture(checkerboard, src, move, enemy, nr_captures, turn);

            /*incremento i in base a quante mangiate possibili ci sono state*/

        }
    }

    return nr_captures;
}

/*restituisce il numero delle mosse possibili chiamando per ogni cella se esiste una pedina che può spostarsi*/
int possible_moves (tower_t* checkerboard, move_t* move, int turn) {
    int r, c, nr_moves = 0;
    /*move e enemy saranno array di grandezza max 44, ovvero 11 pedine x 4 possibili mangiate ciascuna*/
    coordinate_t src;
    for (r = 0; r < ROWS; r++) {
        for(c = 0; c < COLUMNS; c++) {
            src.r = r, src.c = c;
            nr_moves = can_piece_be_moved(checkerboard, src, move, nr_moves, turn);

            /*incremento i in base a quanti movimenti possibili ci sono stati*/

        }
    }

    return nr_moves;
}

/**
 * effettua la mossa simulata, se eat è positivo effettua la mangiata, altrimenti effettua la mossa
 */
bool cpu_play (tower_t* checkerboard, move_t move, coordinate_t enemy, int turn, bool eat) {

    if(piece_selection(checkerboard, move.src, turn)){
        /*mangiata*/
        if(eat){
            piece_capture(checkerboard, move, turn, enemy);
            return true;
        }

        /*mossa*/
        if(move_selection(checkerboard, move, turn)){
           return true;
        }
    }
    return false;
}
/* TODO Riccardo ha cambiato deeptower con cicli for*/


/*copia la scacchiera creando nuovi puntatori e quindi non andando a modificare la scacchiera base, per le simulazioni, bisogna fare la FREE dopo averla chiamata*/
tower_t* deep_tower_copy (const tower_t* checkerboard){
    int r, c, i;
    tower_t* temp_checkerboard = (tower_t*) calloc (ROWS * COLUMNS, sizeof(tower_t));
    for (r = 0; r < ROWS; r++){
        for (c = 0; c < COLUMNS; c++){
            temp_checkerboard[r * COLUMNS + c].player = checkerboard[r * COLUMNS + c].player;
            for (i = 0; i < COMPOSITION_SIZE; i++) {
                temp_checkerboard[r * COLUMNS + c].composition[i] = checkerboard[r * COLUMNS + c].composition[i];
            }

        }
    }
    return temp_checkerboard;
}

/*PLAYER 2 è il CPU*/
/**
 * restituisce un punteggio in base alle simulazioni effettuate e al giocatore
 */
double minimax (tower_t *checkerboard, int depth, int turn){

    int i, i_moves = 0, i_captures = 0;
    double curr_score = 0., best_score;
    move_t possible_move[44], possible_capture[44];
    coordinate_t possible_enemy[44];

    if (win(checkerboard, turn) || depth == 0) {
        return get_total_score(checkerboard, turn);
    }

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
            cpu_play(temp_checkerboard, possible_capture[i], possible_enemy[i], turn, 1);

            turn_update(&turn); /*cambio turno per chiamare minimax con turno cambiato*/

            curr_score = minimax(temp_checkerboard, depth - 1, turn);

            turn_update(&turn); /*finita la chiamata torno al turno precedente*/

            if (turn == PLAYER_2) {
                if(best_score < curr_score) {
                    best_score = curr_score;
                }
            }else{
                if(best_score > curr_score) {
                    best_score = curr_score;
                }
            }

            free(temp_checkerboard);
        }

    }
    else if(i_moves) { /*nel caso non ci fossero mangiate possibili eseguo la mossa*/

        for (i = 0; i < i_moves; i++) {
            tower_t* temp_checkerboard = deep_tower_copy(checkerboard);
            cpu_play(temp_checkerboard, possible_move[i], possible_enemy[43], turn, 0);
            /*passo possible_enemy[43] in quanto è poco possibile che sia occupato e non va modificato dalla mossa*/

            turn_update(&turn); /*cambio turno per chiamare minimax con turno cambiato*/

            curr_score = minimax(temp_checkerboard, depth - 1, turn);

            turn_update(&turn); /*finita la chiamata torno al turno precedente*/

            if (turn == PLAYER_2) {
                if(best_score < curr_score) {
                    best_score = curr_score;
                }
            }else{
                if(best_score > curr_score) {
                    best_score = curr_score;
                }
            }

            free(temp_checkerboard);
        }
    }
    else{
        /*se non ha mangiate o movimenti possibili win dovrebbe dare il punteggio, non entrando mai qui*/
        printf("ERRORE");
    }
    return best_score;
}

bool check_capture_concatenation_cpu(tower_t* checkerboard, coordinate_t old_capture, coordinate_t new_capture){

    if(new_capture.r == old_capture.r && new_capture.c == old_capture.c){
        return true;
    }
    return false;
}

/*funzione che esegue la miglior mossa per la CPU, chiamando minimax per ogni sua mossa valida delle sue pedine*/
int cpu_minimax (tower_t *checkerboard) {

    /*per ogni pedina che può fare una mossa nella scacchiera originale chiamo la funzione minimax, salvando alla fine le coordinate della mossa con punteggio migliore*/

    int i, i_best_move = 0, i_moves = 0, i_captures = 0, check = 1;
    double curr_score = 0., best_score = -1000.;
    move_t possible_move[44], possible_capture[44];
    coordinate_t possible_enemy[44], temp_possible_capture;

    i_captures = possible_captures(checkerboard, PLAYER_2, possible_capture, possible_enemy);

    if(!i_captures){ /*calcolo mosse solo se non ci sono mangiate possibili*/
        i_moves = possible_moves(checkerboard, possible_move, PLAYER_2);
    }

    if(i_captures) {

        /*if(i_captures != 1){ *//*se c'è solo una mangiata la eseguo dopo l'if senza controllare, perchè unica mossa possibile*/

            for (i = 0; i < i_captures; i++) {
                tower_t* temp_checkerboard = deep_tower_copy(checkerboard);
                cpu_play(temp_checkerboard, possible_capture[i], possible_enemy[i], PLAYER_2, 1);
                curr_score = minimax(temp_checkerboard, MAX_DEPTH - 1, PLAYER_1);

                if(best_score < curr_score) {
                    best_score = curr_score;
                    i_best_move = i;
                }
                free(temp_checkerboard);
            }

        /*} */

        /*nel caso di singola mangiata eseguo l'unica mosssa in quanto i_best_move è uguale a 0*/
        cpu_play(checkerboard, possible_capture[i_best_move], possible_enemy[i_best_move], PLAYER_2, 1);

        promotion_check(checkerboard);
        printf("\n");
        checkerboard_print(checkerboard, possible_capture[i_best_move].dst);

        /*se ha fatto la mangiata controllo se può fare la concatenazione di mangiate*/

        temp_possible_capture.r = possible_capture[i_best_move].dst.r;
        temp_possible_capture.c = possible_capture[i_best_move].dst.c;

        while (check != 0){
            check = 0;
            i_captures = possible_captures(checkerboard, PLAYER_2, possible_capture, possible_enemy);

            for (i = 0; i < i_captures; i++) { /*verifico se tra tutte le nuove mangiate possibili c'è la pedina che ha appena mangiato*/
                if(check_capture_concatenation_cpu(checkerboard, temp_possible_capture, possible_capture[i].src)){

                    cpu_play(checkerboard, possible_capture[i], possible_enemy[i], PLAYER_2, 1);

                    promotion_check(checkerboard);
                    printf("\n");
                    checkerboard_print(checkerboard, possible_capture[i].dst);

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

            if(best_score < curr_score) {
                best_score = curr_score;
                i_best_move = i;
            }
            free(temp_checkerboard);
        }
        /*decidere se fare già mossa qua o nel main */
        cpu_play(checkerboard, possible_move[i_best_move], possible_enemy[43], PLAYER_2, 0);

        promotion_check(checkerboard);
        printf("\n");
        checkerboard_print(checkerboard, possible_move[i_best_move].dst);

    }
    else { /*non ci sono nè mangiate nè mosse possibli per il player 2, quindi ha perso, non dovrebbe entrare in quanto c'è la funzione win*/
        return 0;
    }

    return 1;
}



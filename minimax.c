//
// Created by franc on 04/12/2020.
//
#include "minilaska.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#define ROWS (7)
#define COLUMNS (7)
#define LIMIT_RIGHT (6)
#define LIMIT_LEFT (0)
#define LIMIT_DOWN (6)
#define LIMIT_UP (0)
#define FIRST_DIAGONAL (1)
#define SECOND_DIAGONAL (0)
#define ERROR (-1)
#define PLAYER_TOWER checkerboard[r * COLUMNS + c].player
#define HEAD_TOWER checkerboard[r * COLUMNS + c].composition[0]
#define MID_TOWER checkerboard[r * COLUMNS + c].composition[1]
#define TAIL_TOWER checkerboard[r * COLUMNS + c].composition[2]

#define PLAYER_DESTINATION checkerboard[move_r * COLUMNS + move_c].player /*player della casella in cui effettuare il movimento nella funzione piece_move*/
#define HEAD_DESTINATION checkerboard[move_r * COLUMNS + move_c].composition[0]
#define MID_DESTINATION checkerboard[move_r * COLUMNS + move_c].composition[1]
#define TAIL_DESTINATION checkerboard[move_r * COLUMNS + move_c].composition[2]

#define PLAYER_ENEMY checkerboard[enemy_r * COLUMNS + enemy_c].player /*player nemico che sta per essere mangiato*/
#define HEAD_ENEMY checkerboard[enemy_r * COLUMNS + enemy_c].composition[0]
#define MID_ENEMY checkerboard[enemy_r * COLUMNS + enemy_c].composition[1]
#define TAIL_ENEMY checkerboard[enemy_r * COLUMNS + enemy_c].composition[2]



/*TODO IMPORTANT
 * L'algoritmo minimax ha bisogno di una funzione che gli passi tutte le possibili combinazioni di mosse date le coordinate di una pedina (get_piece_moves), esse serviranno per chiamare ricorsivamente
 * la funzione minimax e valutare il punteggio (get_score) di ogni possibile mossa, scegliendo il max o il min valore tra tutte le possibili mosse di tutte le pedine
 * */


const int MAX_DEPTH = 7;

/*PLAYER 2 è il CPU*/

/**
 * restituisce la sottrazione tra le pedine rimaste del player della CPU - quelle del player avversario
 * Per tornare un punteggio più significato sommo lo score della sottrazione con la differenza tra il nr di pedine promosse della CPU (x 0.5) e quelle del player (x 0.5)
 * @return
 */
int get_score (tower_t *checkerboard, int turn) {
    if(win(checkerboard, turn)){
        if(turn == PLAYER_1){
            return -1000;
        }else{
            return  1000;
        }
    }
    return pieces_left(checkerboard, PLAYER_2) - pieces_left(checkerboard, PLAYER_1);
}

/**
 * ritorna la copia della simulazione della checkerboard e fa l'update del turno
 * @return
 */

int* get_valid_moves (tower_t* temp_checkerboard, int turn) {

}

/*ritorna il numero delle mosse possibili date le coordinate della pedina src (ex get_nr_all_moves), modifica inoltre l'array dst con tutte le possibili coordinate dst*/
int can_piece_be_moved(tower_t* checkerboard, coordinate_t src, int turn, coordinate_t* dst){
    int nr_moves = 0;
    if (turn == PLAYER_1) { /*mi interessa sapere se, date le coordinate r e c della pedina, ci sono movimenti possibili*/

        if (HEAD_TOWER == PLAYER_1_PRO) { /*se il player è promosso ha liberta di movimento in ogni direzione*/
            /*(move_r == r + 1 || move_r == r - 1) && (move_c == c + 1 || move_c == c - 1)*/
            if (checkerboard[(r - 1) * COLUMNS + (c + 1)].player == VOID({
                    nr_moves++;
                    dst[nr_moves]->r = r - 1;
                    dst[nr_moves]->c = c + 1;
            }
            if(checkerboard[(r - 1) * COLUMNS + (c - 1)].player == VOID ) {
                nr_moves++;
                dst[nr_moves]->r = r - 1;
                dst[nr_moves]->c = c - 1;
            }
        } /*player 1 non pro*/
        /*(move_r == r + 1) && ((move_c == c + 1) || (move_c == c - 1))*/
        if (checkerboard[(r + 1) * COLUMNS + (c + 1)].player == VOID){
            nr_moves++;
            dst[nr_moves]->r = r + 1;
            dst[nr_moves]->c = c + 1;
        }
        if(checkerboard[(r + 1) * COLUMNS + (c - 1)].player == VOID) { /*verifica se destinazione è valida e verifica che in quella posizione non ci siano altre pedine e che il range sia corretto*/
            nr_moves++;
            dst[nr_moves]->r = r + 1;
            dst[nr_moves]->c = c - 1;
        }

    } else if (turn == PLAYER_2) {
        if (HEAD_TOWER == PLAYER_2_PRO) {
            if (checkerboard[(r + 1) * COLUMNS + (c+1)].player == VOID || checkerboard[(r + 1) * COLUMNS + (c - 1)].player == VOID || checkerboard[(r - 1) * COLUMNS + (c + 1)].player == VOID || checkerboard[(r - 1) * COLUMNS + (c - 1)].player == VOID ) {
                nr_moves++;
            }
        } /*player 2 non pro*/
        else if (checkerboard[(r + 1) * COLUMNS + (c + 1)].player == VOID || checkerboard[(r + 1) * COLUMNS + (c - 1)].player == VOID) { /*verifica se destinazione è valida e verifica che in quella posizione non ci siano altre pedine e che il range sia corretto*/
            nr_moves++;
        }
    }
    return nr_moves;
}

/**
 * data la funzione get_all_pieces di un giocatore, mi torna tutte le mosse possibili data una simulazione della checkerboard e il turno, in modo da valutare i vari score possibili
 * @return
 */
/*int get_nr_all_moves (tower_t* checkerboard, coordinate_t src, int turn) {
    int move_r, move_c, nr_movements = 0;

    nr_movements += can_piece_be_moved(checkerboard, src, turn, NULL);
    *//*chiama la funzione get_valid_moves per ogni piece restituito da get_all_pieces e la inserisco in un array composto dalla checkerboard e dalla pedina*//*
    return nr_movements;
}*/

/**
 * effettua la mossa e ritorna la simulazione della temp_checkerboard
 */
tower_t* cpu_play (tower_t* temp_checkerboard, move_t move, int turn) {
    if(move_selection(temp_checkerboard,)){
        return  temp_checkerboard;
    }
}
/* TODO Riccardo ha cambiato deeptower con cicli for*/
/*copia la scacchiera creando nuovi puntatori e quindi non andando a modificare la scacchiera base, per le simulazioni*/
tower_t* deep_tower_copy (const tower_t* checkerboard){
    int r, c, i;
    tower_t* temp_checkerboard = (tower_t*) malloc (ROWS * COLUMNS * sizeof(tower_t));
    for (r = 0; r < ROWS; r++){
        for (c = 0; c < COLUMNS; c++){
            temp_checkerboard[r * COLUMNS + c].player = checkerboard[r * COLUMNS + c].player;
            for (i = 0; i < COMPOSIZION_SIZE; i++) {
                temp_checkerboard[r * COLUMNS + c].composition[i] = checkerboard[r * COLUMNS + c].composition[i];
            }
        }
    }
    return temp_checkerboard;
}



/*PLAYER 2 è il CPU*/
/**
 * torna un array di due elementi corrispondenti alle coordinate x, y su cui effettura la mossa
 */
int minimax (tower_t *checkerboard, coordinate_t src, int depth, int turn, coordinate_t *best_move){ /*l'array best move conterrà le coordinate x e y della migliore mossa*/
    int best_score, curr_score, i;

    tower_t* temp_checkerboard = deep_tower_copy(checkerboard); /*TODO free*/

    /*int* curr_move = (int*) calloc(4, sizeof(int));
    free(checkerboard);*/

    if(win(temp_checkerboard, turn) || depth == 0){
        return get_score(temp_checkerboard, turn);
    }

    if(turn == PLAYER_1) { /*player NON CPU vuole -1000*/
        best_score = +1000; /*peggiore dei casi*/
        for (i = 0, i < can_piece_be_moved(temp_checkerboard, src, turn, dst); i++){ /*itera per tutte le mosse possibili di quella pedina, / 2 perchè l'array contiene coppie di coordinate*/
            move_t curr_move;
            curr_move.src = src;
            curr_move.dst = dst[i]; /*parametro dato da can piece be moved*/
            cpu_play (temp_checkerboard, curr_move, turn);
            /*TODO CPU_PLAY esegui mossa nella temp_checkerboard*/

            /*TODO nuovo src?*/
            curr_score = min(best_score, minimax(temp_checkerboard, dst[i], depth - 1, PLAYER_2)); /*min tra curr_score e il punteggio restituito dalla simulazione della scacchiera*/
            if (curr_score < best_score) {
                best_score = curr_score;
                *best_move = *curr_move; /* TODO assegno al puntatore best_move il valore del puntatore della mossa corrente*/
                free(curr_move);
            }
        }
    }else{
        best_score = -1000; /*PLAYER CPU*/
        for (i = 0, i < get_nr_all_moves(temp_checkerboard[r * COLUMNS + c], turn); i++) {
            curr_score = max (best_score, minimax(temp_checkerboard, depth - 1, PLAYER_1));
            if (curr_score > best_score) {
                best_score = curr_score;
                /*best_move[0] = curr_move[0];
                best_move[1] = curr_move[1]; */
                *best_move = *curr_move;
                free(curr_move);
            }
        }
    }
    free(temp_checkerboard);
    return best_score;
}

/*funzione che restituisce le coordinate della mossa migliore per la CPU, chiamando minimax per ogni sua mossa valida delle sue pedine*/
move_t best_move (tower_t *checkerboard, move_t move, int turn) {
    int r, c, curr_score = 0, best_score = -1000;
    move_t best_move, curr_move[4];
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLUMNS; c++) {
            if(can_piece_be_moved(checkerboard, r, c, turn, &curr_move.dst)){
                int i;
                curr_move.src.r = r;
                curr_move.src.c = c;
                /*i al massimo arriverà a 4 perchè è il nr max che una pedina può muoversi*/
                for(i = 0; i < can_piece_be_moved(checkerboard, curr_move.src, turn, &curr_move.dst); i++) {

                    curr_score = minimax(checkerboard, curr_move.src, MAX_DEPTH, turn, &curr_move[i].dst);

                    if(best_score < curr_score) {

                        best_score = curr_score;
                        best_move.src.r = curr_move.src.r;
                        best_move.src.c = curr_move.src.c;
                        best_move.dst.r = curr_move.dst.r;
                        best_move.dst.c = curr_move.dst.c;
                    }
                }
            }
        }
    }
    return best_move;
}



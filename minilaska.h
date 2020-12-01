//
// Created by franc on 28/11/2020.
//
#ifndef MINILASKA_MINILASKA_H
#define MINILASKA_MINILASKA_H




#include <stdio.h>
#include <stdbool.h>

typedef int player_t;
typedef int coordinate_t;


typedef
struct tower { /*struttura per le pedine, contiene il giocatore e un'array chiamato chessboard indica quanto alta è la pedina e da cosa è composta*/
    player_t player;
    int composition[3]; /*1 per o 2 per x 3 O e 4 X  N.B. L'elemento composition[0] è la testa della torre*/
} tower_t;

void checkerboard_init(tower_t *checkerboard);

void checkerboard_print (tower_t *checkerboard);

void turn_update(int *turn);

void promotion_check(tower_t *checkerboard);

bool control_range(coordinate_t x, coordinate_t y);

bool piece_selection (tower_t *checkerboard, coordinate_t c, coordinate_t r, const int *turn);

void composition_update(tower_t *checkerboard, coordinate_t r, coordinate_t x, coordinate_t move_y, coordinate_t move_x);

bool move_selection(tower_t *checkerboard, coordinate_t c, coordinate_t r, const int *turn, coordinate_t move_c, coordinate_t move_r);

player_t diagonal_down_left_check(tower_t *checkerboard, coordinate_t r, coordinate_t c, bool is_first_diagonal);

player_t diagonal_down_right_check(tower_t *checkerboard, coordinate_t r, coordinate_t c, bool is_first_diagonal);

player_t diagonal_up_left_check(tower_t *checkerboard, coordinate_t r, coordinate_t c, bool is_first_diagonal);

player_t diagonal_up_right_check(tower_t *checkerboard, coordinate_t r, coordinate_t c, bool is_first_diagonal);

bool win(tower_t *checkerboard, const int *turn);

void piece_capture(tower_t *checkerboard, coordinate_t r, coordinate_t c, const int *turn, coordinate_t move_r, coordinate_t move_c, coordinate_t enemy_r, coordinate_t enemy_c);

int capture_check(tower_t *checkerboard, int *turn);

#endif //MINILASKA_MINILASKA_H
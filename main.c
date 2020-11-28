#include <stdio.h>
#include <stdlib.h>
#define ROWS (7)
#define COLUMNS (7)

#include "minilaska.h"

typedef int player_t;
typedef int coordinate_t;


int main() {
    tower_t checkerboard[ROWS][COLUMNS];
    coordinate_t x, y, move_x, move_y;
    int turn = 1;
    int win_count = 0;

    setvbuf(stdout, NULL, _IONBF, 0); /* permette di debuggare con scanf */

    checkerboard_init(&(checkerboard[0][0]));
    checkerboard_print(&(checkerboard[0][0]));
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

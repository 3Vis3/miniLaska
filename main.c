#include <stdio.h>
#include <string.h>
#define ROWS (7)
#define COLUMNS (7)

#include "minilaska.h"

coordinate_t best_move[4] = {-1, -1, -1, -1}; /*r c base e r c destinazione*/

int main() {
    tower_t checkerboard[ROWS][COLUMNS];
    coordinate_t src, dst;
    move_t move;
    char xx; /*per poi convertirlo in integer e chiamare le coordinate*/
    int turn = 1;
    int win_count = 0;
    int mode = 2;

    setvbuf(stdout, NULL, _IONBF, 0); /* permette di debuggare con scanf */

    printf("Inserire modalita' 1 (1 giocatore) o modalita' 2 (2 giocatori)\n");
    scanf("%d", &mode);

    checkerboard_init(&(checkerboard[0][0])); /*funzione checkerboard create a cui non passi nulla e ti crea la checkerboard inizializzandola*/
    checkerboard_print(&(checkerboard[0][0]));

    if(mode == 2){
        while (!win_count) {
            switch (capture_check(&checkerboard[0][0], turn)) {
                case 0:
                    printf("case 0\n");
                    if (win(&(checkerboard[0][0]), turn)) {
                        win_count++;
                        continue;
                    }
                    printf("\nTURNO GIOCATORE %d \n", turn);
                    printf("Seleziona le coordinate della pedina x , y: \n");
                    scanf("%d %d", &src.c, &src.r); /*inserimento pedina da controllare*/ /*TODO, RICHIEDERE SELEZIONE SE INSERISCI non numeri*/
                    if (!piece_selection(&(checkerboard[0][0]), src, turn)) {
                        continue;
                    } else {
                        printf("Hai selezionato le coordinate x=%d y=%d\n", src.c, src.r);
                        printf("seleziona le coordinate in cui muovere la pedina x , y: \n");
                        scanf("%d %d", &dst.c, &dst.r); /*inserimento coordinate destinazione pedina*/
                        move.src = src, move.dst = dst;
                        if (!move_selection(&(checkerboard[0][0]), move, turn)) {
                            continue;
                        } /*se la mossa è valida stampa chessboard*/
                    }
                    turn_update(&turn);
                    promotion_check(&(checkerboard[0][0]));
                    printf("\n");
                    checkerboard_print(&(checkerboard[0][0]));
                    break;
                case 1:
                    printf("case 1\n");
                    printf("updating\n");
                    promotion_check(&(checkerboard[0][0]));
                    checkerboard_print(&(checkerboard[0][0]));
                    /*DOPO IL case 1 andrà sempre di nuovo o a case 1 o case 3*/
                    continue;
                case 2:
                    /*quando spammi no continuamente ti obbliga a scegliere ripetendo il ciclo*/
                    printf("case 2\n");
                    continue;
                case 3:
                    /*ci entra sempre dopo case 1 (quando mangi) e se quella pedina non puo fare catene di mangiate, case 3 fa un cambio turno e basta*/
                    printf("case 3\n");
                    turn_update(&turn);
                    break;
            }
        }
        turn_update(&turn);
        printf("WIN Player %d ", turn);
    }
    else { /*modalità 1: 1 giocatore contro CPU*/
        while (!win_count) {
            switch (capture_check(&checkerboard[0][0], PLAYER_1)) {
                case 0:
                    printf("case 0\n");
                    if (win(&(checkerboard[0][0]), turn)) {
                        win_count++;
                        continue;
                    }
                    printf("\nTURNO GIOCATORE %d \n", turn);

                    if (turn == PLAYER_1){

                        printf("Seleziona le coordinate della pedina x , y: \n");
                        scanf("%d %d", &src.c, &src.r); /*inserimento pedina da controllare*/ /*TODO, RICHIEDERE SELEZIONE SE INSERISCI non numeri*/
                        if (!piece_selection(&(checkerboard[0][0]), src, turn)) {
                            continue;
                        } else {
                            printf("Hai selezionato le coordinate x=%d y=%d\n", src.c, src.r);
                            printf("seleziona le coordinate in cui muovere la pedina x , y: \n");
                            scanf("%d %d", &dst.c, &dst.r); /*inserimento coordinate destinazione pedina*/
                            move.src = src, move.dst = dst;
                            /*se la mossa è valida stampa chessboard*/
                        }

                        if (!move_selection(&(checkerboard[0][0]), move, turn)) {
                            continue;
                        }

                        promotion_check(&(checkerboard[0][0]));
                        printf("\n");
                        checkerboard_print(&(checkerboard[0][0]));

                    }/*else{ *//*turno CPU, mossa eseguita da minimax*//*
                        cpu_minimax(&(checkerboard[0][0]));
                    }*/
                    printf("qui\n");
/*
                    turn_update(&turn);
*/
                    cpu_minimax(&(checkerboard[0][0]));
                    /*turn_update(&turn);*/
                    break;
                case 1:
                    printf("case 1\n");
                    printf("updating\n");
                    promotion_check(&(checkerboard[0][0]));
                    checkerboard_print(&(checkerboard[0][0]));
                    /*DOPO IL case 1 andrà sempre di nuovo o a case 1 o case 3*/
                    continue;
                case 2:
                    /*quando spammi no continuamente ti obbliga a scegliere ripetendo il ciclo*/
                    printf("case 2\n");
                    continue;
                case 3:
                    /*ci entra sempre dopo case 1 (quando mangi) e se quella pedina non puo fare catene di mangiate, case 3 fa un cambio turno e basta*/
                    printf("case 3\n");
                    /*turn_update(&turn);*/
                    cpu_minimax(&(checkerboard[0][0]));
                    /*turn_update(&turn);*/
                    break;
            }
        }
        turn_update(&turn);
        printf("WIN Player %d ", turn);
    }

    return 0;
}

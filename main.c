/**
* @file main.c
* @author Gruppo 15
* @mainpage main miniLaska
* @section intro_sec Introduzione
*
* Questa è una piccola introduzione, tipo la consegna e le specifiche del
* programma.
*
* @section install_sec Installazione
* Qui abbiamo creato una nuova sezione dove possiamo scrivere
* le istruzioni per installare il programma
* @subsection step1 Step 1: Installa gcc
*/

#include <stdio.h>
#include <malloc.h>

#define ROWS (7)
#define COLUMNS (7)

#include "minilaska.h"


int main() {
    tower_t checkerboard[ROWS][COLUMNS];
    coordinate_t src, dst, last_move;
    move_t move;
    int turn = 1, win_count = 0, mode = 2;
    char s_move[3];
    last_move.c = -1, last_move.r = -1;

    /*char xx; */ /*per poi convertirlo in integer e chiamare le coordinate*/

    s_move[2] = '\0';

    setvbuf(stdout, NULL, _IONBF, 0); /* permette di debuggare con scanf */

    printf("Inserire modalita' 1 (1 giocatore) o modalita' 2 (2 giocatori)\n");
    scanf("%d", &mode);

    checkerboard_init(&(checkerboard[0][0])); /*funzione checkerboard create a cui non passi nulla e ti crea la checkerboard inizializzandola*/
    checkerboard_print(&(checkerboard[0][0]), last_move);

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
                    printf("Seleziona le coordinate della pedina: (Es. e4) \n");
                    scanf("%s", s_move ); /*inserimento pedina da controllare*/ /*TODO, RICHIEDERE SELEZIONE SE INSERISCI non numeri*/
                    string_to_coordinate(s_move, &src);
                    getchar(); /*evita che se si digitano dei caratteri vada in endless loop*/
                    if (!piece_selection(&(checkerboard[0][0]), src, turn)) {
                        continue;
                    } else {
                        printf("Hai selezionato le coordinate della pedina in %d%d\n", src.c, src.r);
                        printf("seleziona le coordinate in cui muovere la pedina: \n");
                        scanf("%s", s_move ); /*inserimento pedina da controllare*/ /*TODO, RICHIEDERE SELEZIONE SE INSERISCI non numeri*/
                        string_to_coordinate(s_move, &dst);
                        move.src = src, move.dst = dst;
                        if (!move_selection(&(checkerboard[0][0]), move, turn)) {
                            continue;
                        } /*se la mossa è valida stampa chessboard*/
                    }
                    turn_update(&turn);
                    promotion_check(&(checkerboard[0][0]));
                    printf("\n");
                    checkerboard_print(&(checkerboard[0][0]), move.dst);
                    break;
                case 1:
                    printf("case 1\n");
                    printf("updating\n");
                    promotion_check(&(checkerboard[0][0]));
                    checkerboard_print(&(checkerboard[0][0]), move.dst); /*TODO cambia move.dst nelle coordinate della mangiata*/
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
            switch (capture_check(&checkerboard[0][0], PLAYER_1)) { /*TODO risolvere bug, controlla se ci sono mangiate anche dopo che il player 1 ha già fatto la mossa*/
                case 0: /*non ci sono manigate disponibili*/
                    printf("case 0\n");
                    if (win(&(checkerboard[0][0]), PLAYER_1) || win(&(checkerboard[0][0]), PLAYER_2)) {
                        win_count++;
                        continue;
                    }
                    /*GIOCATORE UMANO*/
                    printf("\nTURNO GIOCATORE 1 \n\n");

                    printf("Seleziona le coordinate della pedina (Es. e4)): \n");

                    scanf("%s", s_move ); /*inserimento pedina da controllare*/ /*TODO, RICHIEDERE SELEZIONE SE INSERISCI non numeri*/
                    string_to_coordinate(s_move, &src);
                    if (!piece_selection(&(checkerboard[0][0]), src, turn)) {
                        continue;
                    } else {
                        printf("Hai selezionato le coordinate della pedina in %d%d\n", src.c, src.r);
                        printf("seleziona le coordinate in cui muovere la pedina: \n");
                        scanf("%s", s_move); /*inserimento coordinate destinazione pedina*/
                        string_to_coordinate(s_move, &dst);
                        move.src = src, move.dst = dst;
                        /*se la mossa è valida stampa chessboard*/
                    }

                    if (!move_selection(&(checkerboard[0][0]), move, turn)) {
                        continue;
                    }

                    promotion_check(&(checkerboard[0][0]));
                    printf("\n");



                    checkerboard_print(&(checkerboard[0][0]), move.dst);

                    /*GIOCATORE CPU*/
                    printf("\nTURNO GIOCATORE CPU \n");
                    cpu_minimax(&(checkerboard[0][0]));
                    break;
                case 1: /*quando c'è la prima mangiata disponibile*/
                    printf("case 1\n");
                    printf("updating\n");
                    promotion_check(&(checkerboard[0][0]));
                    checkerboard_print(&(checkerboard[0][0]), move.dst);
                    /*DOPO IL case 1 andrà sempre di nuovo o a case 1 o case 3*/
                    continue;
                case 2:
                    /*quando spammi no continuamente ti obbliga a scegliere ripetendo il ciclo*/
                    printf("case 2\n");
                    continue;
                case 3:
                    /*ci entra sempre dopo case 1 (quando mangi) e se quella pedina non puo fare catene di mangiate, case 3 fa un cambio turno e basta*/
                    printf("case 3\n");
                    printf("\nTURNO GIOCATORE CPU \n");
                    cpu_minimax(&(checkerboard[0][0]));
                    break;
            }
        }

        if(win(&(checkerboard[0][0]), PLAYER_2)){
            printf("WIN Player 2");
        }else{
            printf("WIN Player 1");
        }
    }

    return 0;
}

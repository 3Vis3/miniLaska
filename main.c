/**
* @file main.c
* @author Gruppo 15
* @mainpage main miniLaska
* @section intro_sec Introduzione
*
* Progetto Minilaska, creato dal gruppo 015 per il corso di Introduzione alla Programmazione
*
* @section install_sec Installazione
* Istruzioni per la compilazione tramite Linux:
* @subsection step1 Step 1: Installa gcc
* @subsection step2 Step 2: Compila con "gcc -ansi -pedantic main.c minilaska.c minimax.c"
* @subsection step3 Step 3: Esegui il programma con "./a.out"
* @subsection step4 Step 4: Divertiti
*/

#include <stdio.h>
#include "minilaska.h"
#define ROWS (7)
#define COLUMNS (7)

int main() {
    char s_move[3];
    tower_t checkerboard[ROWS][COLUMNS];
    coordinate_t src, dst, last_move;
    move_t move;
    int turn = 1, win_count = 0, mode = 2, graphic = 1;


    last_move.c = -1, last_move.r = -1;
    s_move[2] = '\0';

    /*setvbuf(stdout, NULL, _IONBF, 0); *//* permette di debuggare con scanf */
    printf("VISUALIZZAZIONE:\n");
    printf("0 - standard\n");
    printf("1 - minimal\n");
    scanf("%d", &graphic);
    printf("Inserire modalita' 1 (1 giocatore) o modalita' 2 (2 giocatori)\n");
    scanf("%d", &mode);
    /*inizializzazione checkerboard (scacchiera)*/
    checkerboard_init(&(checkerboard[0][0]));
    /*prima stampa della scacchiera*/
    checkerboard_print(&(checkerboard[0][0]), last_move, graphic);

    /*modalità di gioco 2 player*/
    if(mode == 2){
        while (!win_count) {
            switch (capture_check(&checkerboard[0][0], turn)) {
                case 0:
                    if (win(&(checkerboard[0][0]), turn)) {
                        win_count++;
                        continue;
                    }
                    printf("TURNO GIOCATORE %d \n", turn);
                    printf("Seleziona le coordinate della pedina: (Es. e4) \n");
                    scanf("%s", s_move ); /*inserimento pedina da controllare*/
                    string_to_coordinate(s_move, &src);
                    getchar(); /*evita che se si digitano dei caratteri vada in endless loop*/
                    if (!piece_selection(&(checkerboard[0][0]), src, turn)) {

                        continue;
                    } else {
                        printf("Hai selezionato le coordinate della pedina in %c%d\n", (char) src.c + 'a', src.r);
                        printf("seleziona le coordinate in cui muovere la pedina: \n");
                        scanf("%s", s_move ); /*inserimento pedina da controllare*/
                        string_to_coordinate(s_move, &dst);
                        move.src = src, move.dst = dst;
                        if (!move_selection(&(checkerboard[0][0]), move, turn)) {
                            continue;
                        } /*se la mossa è valida stampa chessboard*/
                    }
                    turn_update(&turn);
                    promotion_check(&(checkerboard[0][0]));
                    printf("\n");
                    checkerboard_print(&(checkerboard[0][0]), move.dst, graphic);
                    break;
                case 1:
                    promotion_check(&(checkerboard[0][0]));
                    checkerboard_print(&(checkerboard[0][0]), move.dst, graphic);
                    /*DOPO IL case 1 andrà sempre di nuovo o a case 1 o case 3*/
                    continue;
                case 2:
                    /*quando spammi no continuamente ti obbliga a scegliere ripetendo il ciclo*/
                    continue;
                case 3:
                    /*ci entra sempre dopo case 1 (quando mangi) e se quella pedina non puo fare catene di mangiate, case 3 fa un cambio turno e basta*/

                    turn_update(&turn);
                    break;
            }
        }
        turn_update(&turn);
        printf("WIN Player %d ", turn);
    }
    else { /*modalità 1 giocatore vs CPU*/
        while (!win_count) {
            switch (capture_check(&checkerboard[0][0], PLAYER_1)) {
                case 0: /*non ci sono manigate disponibili*/
                    /*printf("case 0\n");*/
                    if (win(&(checkerboard[0][0]), PLAYER_1) || win(&(checkerboard[0][0]), PLAYER_2)) {
                        win_count++;
                        continue;
                    }
                    /*GIOCATORE UMANO*/

                    printf("TURNO GIOCATORE 1 \n");
                    printf("Seleziona le coordinate della pedina (Es. e4)): \n");

                    scanf("%s", s_move ); /*inserimento pedina da controllare*/
                    string_to_coordinate(s_move, &src);
                    if (!piece_selection(&(checkerboard[0][0]), src, turn)) {
                        continue;
                    } else {
                        printf("Hai selezionato le coordinate della pedina in %c%d\n", (char) src.c + 'a', src.r);
                        printf("seleziona le coordinate in cui muovere la pedina: \n");
                        scanf("%s", s_move); /*inserimento coordinate destinazione pedina*/
                        string_to_coordinate(s_move, &dst);
                        move.src = src, move.dst = dst;
                        /*se la mossa è valida stampa chessboard*/
                    }

                    if (!move_selection(&(checkerboard[0][0]), move, turn)) {
                        printf("La pedina selezionata non può muoversi in questa posizione\n");
                        continue;
                    }

                    promotion_check(&(checkerboard[0][0]));
                    printf("\n");

                    checkerboard_print(&(checkerboard[0][0]), move.dst, graphic);

                    /*GIOCATORE CPU*/
                    printf("TURNO GIOCATORE CPU \n");
                    cpu_minimax(&(checkerboard[0][0]), graphic);
                    break;
                case 1: /*quando c'è la prima mangiata disponibile*/
                    promotion_check(&(checkerboard[0][0]));
                    checkerboard_print(&(checkerboard[0][0]), move.dst, graphic);
                    /*DOPO IL case 1 andrà sempre di nuovo o a case 1 o case 3*/
                    continue;
                case 2:
                    /*quando spammi no continuamente ti obbliga a scegliere ripetendo il ciclo*/
                    printf("' Cio' che non e' assolutamente possibile e' non scegliere. ' Jean-Paul Sartre (^v^)\n\n ");
                    continue;
                case 3:
                    /*ci entra sempre dopo case 1 (quando mangi) e se quella pedina non puo fare catene di mangiate, case 3 fa un cambio turno e basta*/
                    printf("TURNO GIOCATORE CPU \n");
                    cpu_minimax(&(checkerboard[0][0]), graphic);
                    break;
            }
        }

        if(win(&(checkerboard[0][0]), PLAYER_1)){
            printf("WIN Player 1");
        }else if(win(&(checkerboard[0][0]), PLAYER_2)){
            printf("WIN Player 2");
        }
    }

    return 0;
}

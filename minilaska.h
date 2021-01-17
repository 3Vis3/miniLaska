/**
* @file minilaska.h
* @author Gruppo 15
* @brief libreria con le definizioni delle funzioni utilizzate nel minilaska.c
* @date 16/01/2020
*/

#ifndef MINILASKA_MINILASKA_H
#define MINILASKA_MINILASKA_H

#include <stdio.h>
#include <stdbool.h>
#define COMPOSITION_SIZE (3)

typedef int player_t;

/**
 * Definizione del tipo composition_t, un enum che può assumere solo dei valori da noi assegnati, corrispondenti al player 1,
 * al player 2, alle corrispettive versioni Pro e al player VOID
 */
typedef enum player_t {VOID_0 = 0, PLAYER_1 = 1, PLAYER_2 = 2, PLAYER_1_PRO = 3, PLAYER_2_PRO = 4} composition_t;

/**
 * Struttura dati di tipo struct tower: contiene due campi: il campo player, corrispondende al giocatore, e un array statico di
 * COMPOSITION_SIZE elementi,ww corrispondente alla composizione della torre, ovvero alla sua altezza.
 * All'inizio del gioco la composizione della torre avrà solo una pedina (la testa), quindi il secondo e il terzo elemento
 * dell'array composition saranno vuoti (VOID_0). Essa prenderà come valori VOID_0, PLAYER_1, PLAYER_2, PLAYER_1_PRO o PLAYER_2_PRO
 */
typedef
struct tower {
    player_t player;
    composition_t composition[COMPOSITION_SIZE];
} tower_t;


/**
 * Struttura dati di tipo struct coordinate: contiene due campi, corrispondenti alle coordinate (colonna, riga) della casella
 */
typedef
struct coordinate {
    int c;
    int r;
} coordinate_t;

/**
 * Struttura dati di tipo struct move: contiene due campi di tipo coordinate_t, contententi le coordinate di partenza (src) e destinazione (dst)
 * della mossa
 */
typedef
struct move {
    coordinate_t src;
    coordinate_t dst;
} move_t;



/**
 * Crea la scacchiera inizializzando ogni casella con i corrispettivi player e la composizione delle loro pedine
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale flattened di tipo struct tower)
 */
void checkerboard_init(tower_t *checkerboard);

/**
 * Converte una stringa composta da due caratteri e passata in input in una coppia di interi, memorizzati nel parametro src
 * @param s - stringa in input
 * @param src - indirizzo di memoria che punta alla coppia di coordinate che verranno modificate in base ai valori della stringa s
 * @return 1 se la conversione va a buon fine, 0 altrimenti
 */
int string_to_coordinate(const char* s, coordinate_t* src);

/**
 * Stampa nella console la scacchiera aggiornata, evidenziando l'ultima mossa
 * @param checkerboard
 * @param last_move, coordinate dell'ultima mossa effettuata da evidenziare
 * @param graphic, valore per la decisione della grafica
 */
void checkerboard_print (const tower_t *checkerboard, coordinate_t last_move, int graphic);

/**
 * Cambia il turno del giocatore al termine di ogni mossa avvenuta con successo
 * @param turn - puntatore all'indirizzo di memoria del turno, che verrà modificato
 */
void turn_update(int *turn);

/**
 * Controlla se c'è un player che deve essere promosso all'interno della scacchiera controllando se una torre ha raggiunto la prima o
 * l'ultima riga della scacchiera
 * @param checkerboard
 */
void promotion_check(tower_t *checkerboard);

/**
 * Controlla se le coordinate passate come parametro siano all'interno della scacchiera
 * @param c - copia della coordinata della colonna
 * @param r - copia della coordinata della riga
 * @return 1 se sono all'interno, 0 altrimenti
 */
int control_range(int c, int r);

/**
 * Controlla se le coordinate fornite in input dal giocatore si riferiscono ad una pedina del proprio player che può effettuare una mossa
 * @param checkerboard
 * @param src - coordinate sorgenti della pedina dove effettuare il controllo del movimento
 * @param turn
 * @return 1 se la selezione è corretta, 0 altrimenti
 */
int piece_selection (const tower_t *checkerboard, coordinate_t src, int turn);


/**
 * Controlla che nelle coordinate di destinazione della mossa passata come parametro non ci siano pedine e, se sono valide,
 * richiama composition_update per spostare la pedina e la sua composizione
 * @param checkerboard
 * @param move
 * @param turn
 * @return 1 se lo spostamento è andato a buon fine, 0 altrimenti
 */
int move_selection(tower_t *checkerboard, move_t move, int turn);

/**
 * Controlla quante pedine rimaste ha il giocatore del turno passato
 * @param checkerboard
 * @param turn
 * @return il numero di pedine rimanenti del giocatore
 */
int pieces_left (const tower_t *checkerboard, int turn);

/**
 * Controlla se la partita arriva al termine, verificando se non sono ancora presenti pedine del player opposto, oppure se ci sono ma sono bloccate
 * @param checkerboard
 * @param turn
 * @return 1 nel caso di vittoria del player passato come parametro, 0 altrimenti
 */
int win(const tower_t *checkerboard, int turn);

/**
 * Effettua la mangiata e il conseguente aggiornamento della composizione delle torri chiamando composition_update
 * @param checkerboard
 * @param move
 * @param turn
 * @param enemy - coordinate del nemico, di cui eliminare la composizione
 */
void piece_capture(tower_t *checkerboard, move_t move, int turn, coordinate_t enemy);

/**
 * Restituisce se una certa pedina può effetturare mangiate oppure no, controllando le varie diagonali
 * @param checkerboard
 * @param src
 * @param move - puntatore all'indirizzo di memoria dell'array di mosse su cui salvare le coordinate src e dst della pedina mangiante
 * @param enemy - puntatore all'indirizzo di memoria dell'array di coordinate su cui salvare la posizione del nemico, collegato al mangiante
 * @param nr_captures - numero dell'indice dell'array su cui salvare le nuove coordinate, della mossa e del nemico
 * @param turn
 * @return valore dell'indice nr_captures, ovvero di quante possibili mangiate ci sono
 */
int can_piece_capture (const tower_t* checkerboard, coordinate_t src, move_t* move, coordinate_t* enemy, int nr_captures, int turn);

/**
 * Verifica quali pedine possono effettuare un movimento, salvandole nell'array di mosse passate come parametro
 * @param checkerboard
 * @param src
 * @param move
 * @param nr_moves - numero dell'indice dell'array su cui salvare la nuova mossa
 * @param turn
 * @return valore dell'indice nr_move, ovvero di quanti possibili movimenti ci sono data l'attuale versione della scacchiera
 */
int can_piece_be_moved(const tower_t* checkerboard, coordinate_t src, move_t* move, int nr_moves, int turn);

/**
 * Controlla attraverso le funzioni di diagonal_check se ci sono mangiate possibili e chiede all'utente se vuole mangiare
 * con quella determinata pedina oppure no
 * @param checkerboard
 * @param turn
 * @return 0 se non ci sono mangiate, 1 se il player ha mangiato, 2 se non ha voluto mangiare
 */
int capture_check(tower_t *checkerboard, int turn);

/**
 * Aggiorna l'array di mosse move e di coordinate enemy passati come puntatore con tutte le coordinate delle possibili mangiate e
 * dei possibili nemici, data l'attuale scacchiera
 * @param checkerboard
 * @param turn
 * @param move
 * @param enemy
 * @return il numero delle mangiate disponibili data l'attuale configurazione della scacchiera
 */
int possible_captures (const tower_t* checkerboard, int turn, move_t* move, coordinate_t* enemy);

/**
 * Aggiorna l'array di mosse move passsato come puntatore con tutte le coordinate delle possibili mosse, data l'attuale scacchiera
 * @param checkerboard
 * @param move
 * @param turn
 * @return il numero dellemosse disponibili data l'attuale configurazione della scacchiera
 */
int possible_moves (const tower_t* checkerboard, move_t* move, int turn);


/**
 * Algoritmo di intelligenza artificiale che esegue la mossa migliore per la CPU, simulando come risposta le mosse migliori
 * per l'avversario fino ad una profondità massima data
 * @param checkerboard
 * @param graphic scelta grafica scacchiera
 * @return 1 se ha eseguito la mossa, 0 se non ci sono più mosse disponibili
 */
int cpu_minimax (tower_t *checkerboard, int graphic);

#endif /*MINILASKA_MINILASKA_H */

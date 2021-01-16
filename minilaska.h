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
 * definizione tipo composition_t, un enum che può assumere solo 1 tra i 5 valori assegnati
 */
typedef enum player_t {VOID_0 = 0, PLAYER_1 = 1, PLAYER_2 = 2, PLAYER_1_PRO = 3, PLAYER_2_PRO = 4} composition_t;

/**
 * Struttura dati di tipo struct tower: contiene il campo player, corrispondende al giocatore, e un array statico di
 * COMPOSITION_SIZE elementi corrispondente alla composizione della torre, ovvero alla sua altezza.
 * All'inizio del gioco la composizione della torre avrà solo una pedina, quindi il secondo e il terzo elemento
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
 * Struttura dati di tipo struct move: contiene due campi, di tipo coordinate_t, contententi le coordinate di partenza (src) e destinazione(dst)
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
 * converte una stringa passata in input in una coppia di interi, memorizzati nel parametro src
 * @param s, stringa in input
 * @param src, puntatore all'indirizzo di memoria del parametro src, di tipo coordinate_t
 * @return 1 se s è un'indirizzo valido e quindi la conversione va a buon fine, 0 altrimenti
 */
int string_to_coordinate(const char* s, coordinate_t* src);

/**
 * Stampa a video la scacchiera aggiornata all'ultima modifica, evidenziando di blu l'ultima mossa
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale di tipo struct tower)
 * @param last_move, coordinate dell'ultima mossa effettuata
 */
void checkerboard_print (tower_t *checkerboard, coordinate_t last_move);

/**
 * Cambia il turno del giocatore al termine di ogni mossa avvenuta con successo
 * @param turn - Puntatore all'indirizzo di memoria del turno
 */
void turn_update(int *turn);

/**
 * Controlla se c'è un player che deve essere promosso all'interno della scacchiera controllando solo prima e ultima riga
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale di tipo struct tower)
 */
void promotion_check(tower_t *checkerboard);

/**
 * Controlla che le coordinate passate siano all'interno della scacchiera
 * @param c - copia della coordinata della colonna
 * @param r - copia della coordinata della riga
 * @return 1 se sono all'interno, 0 altrimenti
 */
bool control_range(int c, int r);

/**
 * Controlla se il giocatore seleziona una pedina corretta, basandosi sul turno e sulla validità delle coordinate
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale di tipo struct tower)
 * @param c - copia della coordinata della colonna
 * @param r - copia della coordinata della riga
 * @param turn - Puntatore all'indirizzo di memoria del turno
 * @return 1 se la selezione è corretta, 0 altrimenti
 */
bool piece_selection (tower_t *checkerboard, coordinate_t src, int turn);

/**
 * Aggiorna la composizione della pedina dopo lo spostamento.
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale di tipo struct tower)
 * @param r - copia della coordinata della riga
 * @param c - copia della coordinata della colonna
 * @param move_r - copia della coordinata della riga di destinazione
 * @param move_c - copia della coordinata della colonna di destinazione
 */
void composition_update(tower_t *checkerboard, move_t move);

/**
 * Dopo la mossa, questa funzione elimina la composizione e svuota la casella da dove è iniziato lo spostamento
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale di tipo struct tower)
 * @param r - copia della coordinata della riga
 * @param c - copia della coordinata della colonna
 */
void clear_square(tower_t *checkerboard, coordinate_t src);

/**
 * controlla che nelle coordinate di destinazione non ci siano pedine e, se sono valide, richiama composition_update per spostare la pedina e la sua composizione, anche per i promossi
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale di tipo struct tower)
 * @param c - copia della coordinata della colonna
 * @param r - copia della coordinata della riga
 * @param turn - Puntatore all'indirizzo di memoria del turno
 * @param move_c - copia della coordinata della colonna di destinazione
 * @param move_r - copia della coordinata della riga di destinazione
 * @return 1 se lo spostamento è andato a buon fine, 0 altrimenti
 */
bool move_selection(tower_t *checkerboard, move_t move, int turn);

/**
 * Controlla il player nelle diagonali in basso a sinistra dato il parametro "is_first_diagonal". Oltre al player della casella destinataria può tornare ERROR_, nel caso le coordinate siano fuori dal range valido
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale di tipo struct tower)
 * @param r - copia della coordinata della riga
 * @param c - copia della coordinata della colonna
 * @param is_first_diagonal - stabilisce la diagonale su cui eseguire il controllo, 1 nel caso sia la prima diagonale, che corrisponde a quella adiacente, 0 nel caso sia la seconda, ovvero
 * la casella dove la pedina dovrà muoversi se potrà mangiare
 * @return PLAYER_1 / PLAYER_2 / VOID_0 / ERROR_ (-1)
 */
player_t diagonal_down_left_check(tower_t *checkerboard, coordinate_t src, bool is_first_diagonal);

/**
* Controlla il player nelle diagonali in basso a destra dato il parametro "is_first_diagonal". Oltre al player della casella destinataria può tornare ERROR_, nel caso le coordinate siano fuori dal range valido
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale di tipo struct tower)
 * @param r - copia della coordinata della riga
 * @param c - copia della coordinata della colonna
 * @param is_first_diagonal - stabilisce la diagonale su cui eseguire il controllo, 1 nel caso sia la prima diagonale, che corrisponde a quella adiacente, 0 nel caso sia la seconda, ovvero
 * la casella dove la pedina dovrà muoversi se potrà mangiare
 * @return PLAYER_1 / PLAYER_2 / VOID_0 / ERROR_ (-1)
 */
player_t diagonal_down_right_check(tower_t *checkerboard, coordinate_t src, bool is_first_diagonal);

/**
 * * Controlla il player nelle diagonali in alto a sinistra dato il parametro "is_first_diagonal". Oltre al player della casella destinataria può tornare ERROR_, nel caso le coordinate siano fuori dal range valido
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale di tipo struct tower)
 * @param r - copia della coordinata della riga
 * @param c - copia della coordinata della colonna
 * @param is_first_diagonal - stabilisce la diagonale su cui eseguire il controllo, 1 nel caso sia la prima diagonale, che corrisponde a quella adiacente, 0 nel caso sia la seconda, ovvero
 * la casella dove la pedina dovrà muoversi se potrà mangiare
 * @return PLAYER_1 / PLAYER_2 / VOID_0 / ERROR_ (-1)
 */
player_t diagonal_up_left_check(tower_t *checkerboard, coordinate_t src, bool is_first_diagonal);

/**
 * * Controlla il player nelle diagonali in alto a destra dato il parametro "is_first_diagonal". Oltre al player della casella destinataria può tornare ERROR_, nel caso le coordinate siano fuori dal range valido
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale di tipo struct tower)
 * @param r - copia della coordinata della riga
 * @param c - copia della coordinata della colonna
 * @param is_first_diagonal - stabilisce la diagonale su cui eseguire il controllo, 1 nel caso sia la prima diagonale, che corrisponde a quella adiacente, 0 nel caso sia la seconda, ovvero
 * la casella dove la pedina dovrà muoversi se potrà mangiare
 * @return PLAYER_1 / PLAYER_2 / VOID_0 / ERROR_ (-1)
 */
player_t diagonal_up_right_check(tower_t *checkerboard, coordinate_t src, bool is_first_diagonal);

/**
 * controlla quante pedine rimaste ha il giocatore passato come turno
 * @param checkerboard  - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale di tipo struct tower)
 * @param turn - Puntatore all'indirizzo di memoria del turno
 * @return il numero di pedine rimanenti del giocatore
 */
int pieces_left (tower_t *checkerboard, int turn);

/**
 * Controlla se la partita arriva al termine, verificando se non sono ancora presenti pedine del player corrente, oppure se ci sono ma sono bloccate
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale di tipo struct tower)
 * @param turn - Puntatore all'indirizzo di memoria del turno
 * @return 1 nel caso di vittoria, 0 altrimetni
 */
bool win(tower_t *checkerboard, int turn);

/**
 * Effettua la mangiata e il conseguente aggiornamento della composizione delle torri chiamando composition_update
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale di tipo struct tower)
 * @param r - copia della coordinata della riga
 * @param c - copia della coordinata della colonna
 * @param turn - Puntatore all'indirizzo di memoria del turno
 * @param move_r - copia della coordinata della riga di destinazione
 * @param move_c move_c - copia della coordinata della colonna di destinazione
 * @param enemy_r - copia della coordinata della riga della pedina avversaria
 * @param enemy_c copia della coordinata della colonna della pedina avversaria
 * @return le coordinate dell'ultimo spostamento, ovvero di move.dst
 */
void piece_capture(tower_t *checkerboard, move_t move, int turn, coordinate_t enemy);

/**
 * funzione che dice se una certa pedina può mangiare o no
 * @param checkerboard
 * @param r
 * @param c
 * @param turn
 * @return valore dell'indice nr_captures, ovvero di quante possibili mangiate ci sono
 */
int can_piece_capture (tower_t* checkerboard, coordinate_t src, move_t* move, coordinate_t* enemy, int nr_captures, int turn);

/**
 *
 * @param checkerboard
 * @param src
 * @param turn
 * @param dst
 * @return
 */
int can_piece_be_moved(tower_t* checkerboard, coordinate_t src, move_t* move, int nr_moves, int turn);

/**
 * controlla attraverso le funzioni di diagonal_check se ci sono mangiate possibili e chiede all'utente se vuole mangiare o no
 * @param checkerboard - Puntatore all'indirizzo di memoria della scacchiera (una matrice bidimensionale di tipo struct tower)
 * @param turn - Puntatore all'indirizzo di memoria del turno
 * @return 0 se non ci sono mangiate, 1 se il player ha mangiato, 2 se non ha voluto mangiare
 */
int capture_check(tower_t *checkerboard, int turn);

/**
 * esegue la mossa della CPU, per l'AI
 * @param checkerboard
 * @return 0 se ha eseguito la mossa, 1 se non ci sono più mosse disponibili
 */
int cpu_minimax (tower_t *checkerboard);

#endif /*MINILASKA_MINILASKA_H */

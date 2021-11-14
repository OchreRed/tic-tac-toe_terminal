#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TURN_P1 0
#define TURN_P2 1

typedef enum TileState { NONE = 0, P1 = 'X', P2 = 'O' } TileState;

typedef struct Tiles {
    char prints[9];         // What gets printed to the terminal
    TileState states[9];    // Records if tile is empty, or marked by P1 or P2
} Tiles;

void tiles_init(struct Tiles* BoardTiles);
void print_board(struct Tiles* BoardTiles, int turn_no, bool player_turn);
char get_choice(struct Tiles* BoardTiles);
void update_tiles(struct Tiles* BoardTiles, int choice, bool player_turn);
int check_winners(struct Tiles* BoardTiles);

int main (void) {
    static int turn_no = 1;
    static bool player_turn = TURN_P1;
    static int choice;
    static int win_check;

    static struct Tiles BoardTiles;
    tiles_init(&BoardTiles);

    while (turn_no < 10) {
        print_board(&BoardTiles, turn_no, player_turn);
        choice = get_choice(&BoardTiles);
        update_tiles(&BoardTiles, choice, player_turn);
        player_turn = !player_turn;
        ++turn_no;
        win_check = check_winners(&BoardTiles);
        if (win_check != 0) {
            print_board(&BoardTiles, turn_no, player_turn);
            printf("Player %d wins!\n\n", win_check);
            goto gameover;
        }
    }
    puts("\nAll tiles have been marked, and with no clear winner determined, the game is a draw.");
    gameover:
    return EXIT_SUCCESS;
}

void tiles_init(struct Tiles* BoardTiles) {
    for (int i = 0 ; i < 9 ; ++ i) {
        BoardTiles->prints[i] = 'a' + i;
    }
    for (int i = 0 ; i < 9 ; ++ i) {
        BoardTiles->states[i] = NONE;
    }
    return;
}

void print_board(struct Tiles* BoardTiles, int turn_no, bool player_turn) {
    putchar('\n');
    printf("Turn %d - player %d's turn.\n", turn_no, (int)player_turn + 1);
    printf(" %c | %c | %c\n", BoardTiles->prints[0], BoardTiles->prints[1], BoardTiles->prints[2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", BoardTiles->prints[3], BoardTiles->prints[4], BoardTiles->prints[5]);
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", BoardTiles->prints[6], BoardTiles->prints[7], BoardTiles->prints[8]); 
    putchar('\n');
    return;
}

char get_choice(struct Tiles* BoardTiles) {
    puts("Input the letter of the spot you want to mark.");
    char buf[3]; 
    int len;
    int ch;

    // You can only enter one letter, otherwise it's invalid.
    // That letter must be between 'a' and 'i'.
    input:
    len = 0;
    while ( (ch = getchar()) != '\n') {
        buf[len] = ch;
        if (len > 1)
            len = 1;
        ++len;
    }
    if (len > 1) {
        puts("Invalid input. Please try again: ");
        goto input;
    }
    if ( !(buf[0] >= 'a' && buf[0] <= 'i') ) { // if not a letter between a and i 
        puts("Invalid input. Please try again: ");
        goto input;
    }
    
    int choice = buf[0] - 'a';
    if (BoardTiles->states[choice] != NONE) {
        puts("That spot is taken. Please try again: ");
        goto input;
    }

    return choice;
}

void update_tiles(struct Tiles* BoardTiles, int choice, bool player_turn) {
    if (BoardTiles->states[choice] == NONE) {
        BoardTiles->states[choice] = (player_turn == TURN_P1)? P1 : P2;
        BoardTiles->prints[choice] = (player_turn == TURN_P1)? 'X' : 'O';
    } else {
        fprintf(stderr, "Invalid tile choice in update_tiles.\n");
        exit(EXIT_FAILURE);
    }
    return;
}

int check_winners(struct Tiles* BoardTiles) {
    int p1_count, p2_count;
    /* The board:
     *  0 1 2
     *  3 4 5
     *  6 7 8
     */
    // Check top, middle, and bottom horizontal rows for three in a row.
    for (int n = 0 ; n < 9 ; n += 3) {
        p1_count = 0;
        p2_count = 0;
        for (int j = 0+n ; j < 3+n ; ++j) {
            if (BoardTiles->states[j] == P1) 
                ++p1_count;
            if (BoardTiles->states[j] == P2)
                ++p2_count;
        }
        if (p1_count > 2) // (yes, this is copy-pasted throughout the function)
            return 1; // P1 wins 
        if (p2_count > 2)
            return 2; // P2 wins
    }
    // Check left, middle, and right vertical rows.
    for (int n = 0 ; n < 3 ; ++n) {
        p1_count = 0;
        p2_count = 0;
        for (int i = 0+n ; i < 7+n ; i += 3) {
            if (BoardTiles->states[i] == P1) 
                ++p1_count;
            if (BoardTiles->states[i] == P2)
                ++p2_count;
        }
        if (p1_count > 2)
            return 1; // P1 wins
        if (p2_count > 2)
            return 2; // P2 wins
    }
    // Check top-left to bottom-right diagonal row.
    p1_count = 0;
    p2_count = 0;
    for (int i = 0 ; i < 9 ; i += 4) {
        if (BoardTiles->states[i] == P1)
            ++p1_count;
        if (BoardTiles->states[i] == P2)
            ++p2_count;
    }
    if (p1_count > 2)
        return 1; // P1 wins
    if (p2_count > 2)
        return 2; // P2 wins

    // Check top-right to bottom-left diagonal row.
    p1_count = 0;
    p2_count = 0;
    for (int i = 2 ; i < 7 ; i += 2) {
        if (BoardTiles->states[i] == P1)
            ++p1_count;
        if (BoardTiles->states[i] == P2)
            ++p2_count;
    }
    if (p1_count > 2)
        return 1; // P1 wins
    if (p2_count > 2)
        return 2; // P2 wins
    
    return 0; // No winners yet
}
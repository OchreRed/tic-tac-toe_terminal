#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void print_board(char* board_tiles, int turn_no, bool player_turn);
char get_choice(char* board_tiles);
bool check_row(char a, char b, char c);
bool check_board(char* board_tiles);

#define TURN_P1 0
#define TURN_P2 1

int main(void) {
    static int turn_no = 1;
    static bool player_turn, game_won;
    static int choice;
    char board_tiles[9] = {'a','b','c','d','e','f','g','h','i'};
    while (turn_no < 10) {
        print_board(board_tiles, turn_no, player_turn);
        choice = get_choice(board_tiles);
        board_tiles[choice] = (player_turn == TURN_P1)? 'X' : 'O';
        player_turn = !player_turn;
        ++turn_no;
        game_won = check_board(board_tiles);
        if (game_won) {
            print_board(board_tiles, turn_no, player_turn);
            printf("Player %d wins!\n\n", (int)!player_turn + 1);
            return EXIT_SUCCESS;
        }
    }
    puts("All tiles have been marked, and with no clear winner determined, the game is a draw.\n");
    return EXIT_SUCCESS;
}

void print_board(char* board_tiles, int turn_no, bool player_turn) {
    printf("\nTurn %d - player %d's turn (you are %c):\n", turn_no, (int)player_turn + 1, (player_turn)? 'O' : 'X');
    printf(" %c | %c | %c\n", board_tiles[0], board_tiles[1], board_tiles[2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", board_tiles[3], board_tiles[4], board_tiles[5]);
    printf("---+---+---\n");
    printf(" %c | %c | %c\n\n", board_tiles[6], board_tiles[7], board_tiles[8]); 
    return;
}

char get_choice(char* board_tiles) {
    puts("Input the letter of the spot you want to mark.");
    char buf[3]; 
    int len, c;
    input:
    len = 0;
    while ( (c = getchar()) != '\n') {
        buf[len] = c;
        if (len > 1) 
            len = 1;
        ++len;
    }
    if (len > 1 || !(buf[0] >= 'a' && buf[0] <= 'i') || board_tiles[buf[0]-'a'] != buf[0]) {
        puts("Invalid input. Please try again: ");
        goto input;
    }
    return buf[0]-'a';
}

bool check_row(char a, char b, char c) {
    return (a == b) && (b == c);
}

bool check_board(char* board_tiles){
    bool h_win, v_win, d_win;
    for (int i = 0; i < 7; i+=3) {
        h_win = check_row(board_tiles[i], board_tiles[i+1], board_tiles[i+2]);
        if (h_win) return true;
    }
    for (int i = 0; i < 3; ++i) {
        v_win = check_row(board_tiles[i], board_tiles[i+3], board_tiles[i+6]);
        if (v_win) return true;
    }
    for (int i = 0; i < 3; i+=2) {
        d_win = check_row(board_tiles[i], board_tiles[4], board_tiles[8-i]);
        if (d_win) return true;
    }
    return false;
}

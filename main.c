#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TURN_P1 0
#define TURN_P2 1

void print_board(char* board, int turn_no, bool player_turn);
int get_choice(char* board);
int check_row_win (char a, char b, char c, int ch, char* board);
int check_board_win(int ch, char* board);

int main (void) {
    static int turn_no = 1;
    static bool player_turn = TURN_P1;
    static int ch;
    static int win_check;
    char board[9] = {'a','b','c','d','e','f','g','h','i'};

    while (turn_no < 10) {
        print_board(board, turn_no, player_turn);
        ch = get_choice(board);
        board[ch] = (player_turn == TURN_P1)? 'X' : 'O';
        player_turn = !player_turn;
        ++turn_no;
        win_check = check_board_win(ch, board);
        if (win_check != 0) {
            print_board(board, turn_no, player_turn);
            printf("Player %d wins!\n\n", win_check);
            return EXIT_SUCCESS;
        }
    }
    puts("\nAll tiles have been marked, and with no clear winner determined, the game is a draw.\n");
    return EXIT_SUCCESS;
}

void print_board(char* board, int turn_no, bool player_turn) {
    printf("\nTurn %d - player %d's turn (you are %c):\n", turn_no, (int)player_turn + 1, (player_turn)? 'O' : 'X');
    printf(" %c | %c | %c\n", board[0], board[1], board[2]);
      puts("---+---+---");
    printf(" %c | %c | %c\n", board[3], board[4], board[5]);
      puts("---+---+---");
    printf(" %c | %c | %c\n\n", board[6], board[7], board[8]); 
    return;
}

int get_choice(char* board) {
    puts("Input the letter of the spot you want to mark.");
    char buf[3]; 
    int c;

    input:
    int len = 0;
    while ( (c = getchar()) != '\n') {
        buf[len] = c;
        if (len > 1)
            len = 1;
        ++len;
    }
    if (len > 1 || !(buf[0] >= 'a' && buf[0] <= 'i')) {
        puts("Invalid input. Please try again: ");
        goto input;
    }    
    int ch = buf[0] - 'a';
    if (board[ch] == 'X' || board[ch] == 'O') {
        puts("That spot is taken. Please try again: ");
        goto input;
    }
    return ch;
}

int check_row_win (char a, char b, char c, int ch, char* board) {
    if (a==board[ch] && b==board[ch] && c==board[ch]) { 
        if (board[ch]=='X') return 1;
        if (board[ch]=='O') return 2;
    }
    return 0;
}

int check_board_win(int ch, char* board) {
    for (int i = 0; i < 7; i += 3) {
        int h_win = check_row_win(board[i], board[i+1], board[i+2], ch, board);
        if (h_win==1) return 1;
        if (h_win==2) return 2;
    }
    for (int i = 0; i < 3; ++i) {
        int v_win = check_row_win(board[i], board[i+3], board[i+6], ch, board);
        if (v_win==1) return 1;
        if (v_win==2) return 2;
    }
    for (int i = 0; i < 3 ; i+= 2) {
        int d_win = check_row_win(board[i], board[4], board[8-i], ch, board);
        if (d_win==1) return 1;
        if (d_win==2) return 2;
    }
    return 0;
}

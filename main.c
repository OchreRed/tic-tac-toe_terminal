#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define IS_ODD(x) (((x) % (2)) ? (1) : (0))

#define TURN_O 0
#define TURN_X 1

struct Tiles {
    char states[9];      // What's on the game board, and what gets printed.
    int priority_lvls[9];  // The CPU will use this for determining its next move (not implemented).
};

enum question { WHICH_MODE, WHICH_SPOT };

const char* INVALID = "Invalid input. Please try again: ";

void init(struct Tiles* Board);
void print_board(struct Tiles* Board, int turn_no, bool mode_cpu);
int get_choice(struct Tiles* Board, enum question which_q, bool* mode_cpu);
char input(void);
int check_row(char a, char b, char c, int ch, struct Tiles* Board);
int check_won(int ch, struct Tiles* Board);
int cpu_choice_rand(struct Tiles* Board);

int main(void) {
    srand(time(0));
    static struct Tiles Board;
    static int turn_no = 1;
    static bool mode_cpu;
    static int ch;
    static int winner;

    init(&Board);
    ch = get_choice(&Board, WHICH_MODE, &mode_cpu);
    puts("\nGAME START!");
    while (turn_no < 10) {
        if (mode_cpu) {
            if (IS_ODD(turn_no)) {
                print_board(&Board, turn_no, mode_cpu);
                ch = get_choice(&Board, WHICH_SPOT, &mode_cpu);
            } else {
                ch = cpu_choice_rand(&Board);
                printf("CPU choice: spot %c\n", ch+'a');
            }
        } else {
            print_board(&Board, turn_no, mode_cpu);
            ch = get_choice(&Board, WHICH_SPOT, &mode_cpu);
        }
    
        Board.states[ch] = IS_ODD(turn_no)?'X':'O';
        ++turn_no;
        winner = check_won(ch, &Board);
        if (winner) {
            print_board(&Board, turn_no, mode_cpu);
            printf("Player %d %s wins!\n\n", winner, (mode_cpu && IS_ODD(turn_no))? "(CPU)" : "");
            return EXIT_SUCCESS;
        }
    }
    puts("\nAll tiles have been marked, and with no clear winner determined, the game is a draw.\n");
    return EXIT_SUCCESS;
}

void init(struct Tiles* Board) {
    for (int i = 0; i < 9; ++i)
        Board->states[i] = 'a' + i;
    for (int i = 0; i < 9; ++i)
        Board->priority_lvls[i] = 0;
    return;
}

void print_board(struct Tiles* Board, int turn_no, bool mode_cpu) {
    if (mode_cpu)
        printf("\nTurn %d - your turn:\n", turn_no);
    else
        printf("\nTurn %d - player %d's turn (you are %c):\n", turn_no, IS_ODD(turn_no)?1:2, IS_ODD(turn_no)?'X':'O'); 
    printf(" %c | %c | %c\n", Board->states[0], Board->states[1], Board->states[2]);
      puts("---+---+---");
    printf(" %c | %c | %c\n", Board->states[3], Board->states[4], Board->states[5]);
      puts("---+---+---");
    printf(" %c | %c | %c\n\n", Board->states[6], Board->states[7], Board->states[8]);
    return;
}

int get_choice(struct Tiles* Board, enum question which_q, bool* mode_cpu) {
    int ans = -1;
    if (which_q == WHICH_MODE) {
        puts("Do you want to play against the CPU (y/n)?");
        do {
            ans = input();
            ans = tolower(ans);
            if (ans != 'y' && ans != 'n')
                puts(INVALID);
        } while ((ans != 'y' && ans != 'n') || ans == '\n');
        if (ans == 'y') *mode_cpu = true; // false by default

    } else if (which_q == WHICH_SPOT) {
        puts("Input the letter of the spot you want to mark: ");
        do {
            ans = input();
            ans = tolower(ans);
            if (!(ans>='a' && ans <= 'i') || !(ans == Board->states[ans-'a']))
                puts(INVALID);
        } while (!(ans>='a' && ans<='i') || !(ans == Board->states[ans-'a']) || ans == '\n');
        ans -= 'a'; // conversion to integer between 0 and 8
    }
    return ans;
}

// Function that takes input of one character only. Safe from buffer overflow.
char input(void) {
    char buf[3];
    int len, c;
    do {
        len = 0;
        while ( (c = getchar()) != '\n') {
            buf[len] = c;
            if (len > 1)
                len = 1;
            len++;
            if (c == EOF)
                exit(EXIT_SUCCESS);
        }
        if (len > 1)
            puts(INVALID);
    } while (len > 1);
    return buf[0];
}

/* To-do: See if you can combine this with the enemy CPU so you don't have this big thing.
 * Also, idea: this could check if two contiguous tiles match, and if so, check if three match, 
 * otherwise return that only two match. */
int check_row (char a, char b, char c, int ch, struct Tiles* Board) {
    if (a==Board->states[ch] && b==Board->states[ch] && c==Board->states[ch]) { 
        if (Board->states[ch]=='X') return 1;
        if (Board->states[ch]=='O') return 2;
    }
    return 0;
}

int check_won(int ch, struct Tiles* Board) {
    for (int i = 0; i < 7; i += 3) {
        int h_win = check_row(Board->states[i], Board->states[i+1], Board->states[i+2], ch, Board);
        if (h_win==1) return 1; // P1 wins
        if (h_win==2) return 2; // P2 wins
    }
    for (int i = 0; i < 3; ++i) {
        int v_win = check_row(Board->states[i], Board->states[i+3], Board->states[i+6], ch, Board);
        if (v_win==1) return 1;
        if (v_win==2) return 2;
    }
    for (int i = 0; i < 3 ; i += 2) {
        int d_win = check_row(Board->states[i], Board->states[4], Board->states[8-i], ch, Board);
        if (d_win==1) return 1;
        if (d_win==2) return 2;
    }
    return 0; // No winners yet
}

// Makes an array of the available tiles remaining and randomly chooses one. 
int cpu_choice_rand(struct Tiles* Board) {
    char choices[9];
    int i = 0, j = 0;
    while (i < 9) {
        if (Board->states[i] != 'X' && Board->states[i] != 'O') {
            choices[j++] = Board->states[i];
        }
        ++i;
    }
    int n = rand() % (j);
    char ch = choices[n];
    ch -= 'a';
    return ch;
}

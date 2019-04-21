#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

// to access (column, row) do (row * COLUMNS) + column - 1;
// these representing grid rows/columns, NOT in-game columns/rows
const int COLUMNS = 8;
const int ROWS = 7;
const int SECOND_ROW_LETTER = 15;
const int BOTTOMROW = 47;
const int TOPROW = 7;

// newfield() returns a pointer to a 7x8 char array, representing the connect 4 board
// caller must free memory
char *newfield() {
    char *field = malloc(COLUMNS * ROWS * sizeof(char));
    for (int k=0; k<COLUMNS*ROWS-1; ++k) { // background
         *(field+k) = ' ';
    }
    for (int i = 0; i < COLUMNS - 1; ++i) { // numbering the columns
        *(field + i) = i + '1';
    }
    for (int j = 0; j < ROWS - 1; ++j) { // lettering the rows
        *(field + (SECOND_ROW_LETTER + (COLUMNS * j))) = j + '1';
    }
    *(field+7) = '~';
    return field;
}

//print(field) prints the state of the field
// effects: produces output
void print(const char * const field) {
    assert(field);
    for (int i=0; i<ROWS; ++i) { // printing rows e.g (0, Y)
        for (int j = 0; j < COLUMNS; ++j) { // prints columns of each row e.g. (X, 0)
            printf("%3c", *(field + i * COLUMNS + j));
        }
        printf("\n");
    }
}

// winner(field, column) checks to see if the last move causes a win
// a win is either
// 1. 4 horizontal pieces
// 2. 4 vertical pieces
// 3. 4 pieces diagonally left or right.
bool winner(char *field, int column) {
    int row = 0;
    char piece;
    for (; row < ROWS; ++row) { // finds first occur. of occupied in column
        if (*(field + (TOPROW + (COLUMNS * row + column))) != ' ') {
            piece = *(field + (TOPROW + (COLUMNS * row + column)));
            break;
        }
    }
    row++; // to go along with ingame rows
    int arraylocation = (row * COLUMNS) + column - 1;
    int verticalcount = 1, horizontalcount = 1, rdigcount = 1, ldigcount = 1;
    // examining the horizontal
    for (int p = 1; *(field + arraylocation + p) == piece; ++p) {
        ++horizontalcount;
    }
    for (int p = 1; *(field + arraylocation - p) == piece; ++p) {
        ++horizontalcount;
    }
    if (horizontalcount == 4) return true;
    // examining the vertical

    for (int p = 1; *(field + arraylocation + (p * 8)) == piece; ++p) {
        ++verticalcount;
    }
    for (int p = 1; *(field + arraylocation - (p * 8)) == piece; ++p) {
        ++verticalcount;
    }
    if (verticalcount == 4) return true;
    // examining the right diagonal
    for (int p = 1; *(field + arraylocation + (p * (COLUMNS - 1))) == piece; ++p) {
        ++rdigcount;
    }
    for (int p = 1; *(field + arraylocation - (p * (COLUMNS - 1))) == piece; ++p) {
        ++rdigcount;
    }
    if (rdigcount == 4) return true;
    // examining the left diagonal
    for (int p = 1; *(field + arraylocation + (p * COLUMNS + 1)) == piece; ++p) {
        ++ldigcount;
    }
    for (int p = 1; *(field + arraylocation - (p * COLUMNS + 1)) == piece; ++p) {
        ++ldigcount;
    }
    if (ldigcount == 4) return true;
    return false;
}

// validmove (field, column) checks if the move is valid
bool validmove(const char * const field, int column) {
    if (column>COLUMNS-1 || column<1) {
        printf("Invalid column! Try again!\n");
        return false;
    }
    else if (*(field+ column+ROWS) != ' ') {
        printf("This column has been filled! Try again!\n");
        return false;
    }
    return true;
}

// playmove (field, column, player, piece) plays the requested move
// requires: validmove must be called prior
// effect: produces output, modifies field
void playmove(char * const field, int column, char piece) {
    for (int n = 0; n < ROWS; ++n) {
        if (*(field + (BOTTOMROW - (COLUMNS * n) + column)) == ' ') {  // each row within the column
            *(field + (BOTTOMROW - (COLUMNS * n) + column)) = piece;
            break;
        }
    }
}

// quitgame() quits the game
void quitgame() {
    printf("I hope you had fun!!! Game is now quitting...");
    exit(EXIT_SUCCESS);
}

//goesfirst(name1, name2) determines who goes first. 0 = p1, 1 = p2
bool goesfirst(const char * const name1, const char * const name2) {
    int p1guess;
    int p2guess;
    printf ("I will randomly generate a number between [0,1000].\n"
            "Whoever guesses closer will go first!\n");
    srand(time(NULL));
    int guess = rand() % 1001;
    printf("%s, give me your guess!\n", name1);
    scanf("%d", &p1guess);
    printf("%s, give me your guess!\n", name2);
    scanf("%d", &p2guess);
    if (abs(guess - p1guess) <= abs(guess - p2guess)) {
        printf("My number was %d, so %s goes first!\n", guess, name1);
        return 0;
    } else {
        printf("My number was %d, so %s goes first!\n", guess, name2);
        return 1;
    }
}

// playgame() initializes the game
void playgame() {
    char name1[10] = {0};
    char name2[10] = {0};
    char piece1;
    char piece2;
    char discard;
    int whowon; // 1 is p1, 2 is p2
    printf("What is your first name Player 1? \n");
    for (int i = 0;; ++i) {
        if ((scanf("%c", name1 + i) != 1) || (*(name1 + i) == 10)) {
            *(name1 + i) = 0;
            break;
        } else if (i == 9) {
            printf("Wow how do you remember your name!! It is so long! \nI will call you Bob instead.\n");
            strcpy(name1, "Bob");
            while (1) {
                scanf("%c", &discard);
                if (discard == 10) break;
            }
            break;
        }
    }
    printf("Hello %s!\n", name1);
    printf("What is your first name Player 2? \n");
    for (int i = 0;; ++i) {
        if ((scanf("%c", name2 + i) != 1) || (*(name2 + i) == 10)) {
            *(name2 + i) = 0;
            break;
        } else if (i == 9) {
            printf("Wow how do you remember your name!! It is so long! \nI will call you Kate instead.\n");
            strcpy(name2, "Kate");
            while (1) {
                scanf("%c", &discard);
                if (discard == 10) break;
            }
        }
    }
    int p1of = 0; //overflow msg
    int p2of = 0; // overflow msg
    printf("Hello %s!\n", name2);
    printf("%s, what piece do you want?\n", name1);
    scanf(" %c", &piece1);
    while (1) {
        scanf("%c", &discard);
        if (discard == 10) break;
        if (!p1of) {
            printf("Don't try to buffer overflow me!!! :( Using first character in string as piece! \n");
            ++p1of;
        }
    }
    printf("%s, what piece do you want?\n", name2);
    scanf(" %c", &piece2); //whitespace!
    while (1) {
        scanf("%c", &discard);
        if (discard == 10) break;
        if (!p2of) {
            printf("Don't try to buffer overflow me!!! :( Using first character in string as piece! \n");
            ++p2of;
        };
    }
    printf("%s has the %c pieces, and %s has the %c pieces.\n", name1, piece1, name2, piece2);
    bool gamewinner = goesfirst(name1, name2);
    printf("------------------------------------------------------------------------------\n");
    // game start
    char *field = newfield();
    int move;
    printf("Type the column where you want to drop a piece!\n");
    if (!gamewinner) { // player 1 won
        while (1) {
            printf("-------------------------------------------------------------------\n");
            print(field);
            printf("%s's turn\n", name1);
            scanf("%d", &move);
            while (!(validmove(field, move))) { // validates move
                scanf("%d", &move);
            }
            playmove(field, move, piece1);
            if (winner(field, move)) {
                whowon = 1;
                break;
            }
            printf("----------------------------------------------------------\n");
            print(field);
            printf("%s's turn\n", name2);
            scanf("%d", &move);
            while (!(validmove(field, move))) { // validates move
                scanf("%d", &move);
            }
            playmove(field, move, piece2);
            if (winner(field, move)) {
                whowon = 2;
                break;
            }
        }
    } else { // player 2 won
        while (1) {
            print(field);
            printf("%s's turn\n", name2);
            scanf("%d", &move);
            while (!(validmove(field, move))) { // validates move
                scanf("%d", &move);
            }
            playmove(field, move, piece2);
            if (winner(field, move)) {
                whowon = 2;
                break;
            }
            print(field);
            printf("%s's turn\n", name1);
            scanf("%d", &move);
            while (!(validmove(field, move))) { // validates move
                scanf("%d", &move);
            }
            playmove(field, move, piece1);
            if (winner(field, move)) {
                whowon = 1;
                break;
            }
        }
    } //after winner has been found
    print(field);
    if (whowon == 1) printf("Congrats %s! You have won! \n", name1);
    else printf("Congrats %s! You have won! \n", name2);
    quitgame();
}

int main() {
    playgame();
}
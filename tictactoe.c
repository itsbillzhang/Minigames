#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "stdbool.h"

/* The board will be a 25 ints, looking as follows:
 * 0,1,2,3,4,
 * 5,6,7,8,9,
 * 10,11,12,13,14,
 * 15,16,17,18,19,
 * 20,21,22,23,24
 */

// enum {a, b, c} == (a=0, b=1, c=2)
enum {NOUGHTS, CROSSES, BORDER, EMPTY};
const int intconverter[] = {6, 7, 8,
                            11,12,13,
                            16,17,18}; // the squares that are the ones in play

const int reverseintconverter[] = {0,0,0,0,0,
                                   0,1,2,3,0,
                                   0,4,5,6,0,
                                   0,7,8,9,0};

// 1 looks for horizontal win, 5 is vertical, 4 is left diag win, 6 is right diag win
const int directions[4] = {1, 5, 4, 6};
const int middle = 12;
const int corners[4] = {0, 2, 6, 8}; // in terms of in game array
const int NODATA = -2;

// createboard(board) creates a blank board
// effects: mutates board through pointer parameter
void createboard(int * const board) {
    for (int i = 0; i < 25; ++i) { // setting borders
        *(board + i) = BORDER;
    }
    for (int j = 0; j < 9; ++j) {
        *(board + (*(intconverter + j))) = EMPTY; //setting empties
    }
}

// boardprinter(board) prints the board it is given
void boardprinter(const int * const board) {
    printf("\n");
    char pieces[] = {'O','X','|','_'};
    printf("\nBoard:\n");
    for (int i = 0; i<9; ++i){
        printf("%3c", pieces[*(board + (*(intconverter+i)))]);
        if (((i+1) % 3) == 0) printf ("\n");
    }
    printf("\n");
}

// squaremover return how many pieces in a particular direction is connected
int squaremover(const int * const board, const int direction, int currsquare, const int piece) {
    int found = 0;
    while(*(board+currsquare+direction) != BORDER) {
        if (*(board+currsquare+direction) != piece) break;
        ++found;
        currsquare += direction;
    }
    return found;
}

// victory(board, moveplayed, piece) determines if someone has won
// moveplayed in terms of 'actual' in game coordinates
int victory(const int * const board, int moveplayed, int piece){
    int currsquare = intconverter[moveplayed-1];
    for (int i = 0; i < 4; ++i) {
        int mostconnections = 1;
        mostconnections += squaremover(board, directions[i], currsquare, piece);
        mostconnections += squaremover(board, (directions[i])* -1, currsquare, piece);
        if (mostconnections == 3) return 3;
    }
    return 1;
}

int ply = 0; // numbers of turns passed since blank game
int maxply = 0; // how many turns the AI will simulate within the tree
int positions = 0; // how many positions AI has searched


// Used for level 3 AI
// movelessvictory (board, piece) looks at the the entire board and determines if there is a win. Unlike
// victory, we do not care about the last moveplayed.
bool movelessvictory(const int * const board, int piece){
    bool foundthree = false;
    for (int i=0; i<9; ++i) {
        if (*(board+intconverter[i]) == piece) {
            if (victory(board, i+1, piece) == 3) {
                foundthree = true;
                break;
            }
        }
    }
    return foundthree;
}


// Used for level 3 AI
// evaluation(board, side) evaluates if any player has won the game
// 1 for win, -1 for loss, 0 for neither.
int evaluation(const int * const board, int piece) {
    if (movelessvictory(board, piece)) return 1;
    if (movelessvictory(board, piece^1)) return -1;
    else return 0;
}
    // check is a win
    // gen all moves for side
    // loop moves, make move, mimax() on move to get score
    // assess bestscore
    // end moves return bestscore

// Used for level 3 AI
int minmax(int *board, const int side) {
    int bestscore = NODATA;
    int currentscore = NODATA;
    int bestmove = -1;
    int movelist[9]; // there can only be 9 moves to consider max.
    int movecount = 0;
    int move;

    if (ply>maxply) maxply = ply;
    ++positions;

    if (ply) { // if we are not at the top of the tree
        currentscore = evaluation(board, side);
        if (currentscore) return currentscore; // if a won position is found, don't search anymore
    }

    for (int i=0; i<9; ++i) { // finding all possible moves
        // movecount++ == do movelist[movecount], then increment
        if (*(board+intconverter[i]) == EMPTY) movelist[movecount++] = intconverter[i];
    }

    for (int i=0; i<movecount; ++i) { // goes through all moves found
        move = movelist[i];
        *(board + move) = side;
        ++ply;

        // simulates remainder of game with each possible move in movelist
        // the side^1 represents from the opposing players perspective
        // example:
        // Pretend crosses' turn, crosses just made a move and gave them 3 in a row (with *(board+move) = side)
        // we would then call -minmax for noughts. It would give a -1 to show noughts are losing (in all scenarios with
        // optimal play). We get -1 back, and because it's negative, currentscore = 1 = winning for crosses.
        currentscore = -(minmax(board, side^1)); // consider opposition score
        if (currentscore > bestscore) {
            bestscore = currentscore;
            bestmove = move;
        }
        *(board+move) = EMPTY; //resets changes
        --ply;
    }
    if (!movecount) bestscore = 0; // board is full, no win can be found
    if (ply) return bestscore;
    else return bestmove; // occurs iff we are at top of tree
}

// playermoves(board) gives the player the move, and returns the last player move.
int playermoves(int *board, int side) {
    boardprinter(board);
    printf("It's your turn, make a move.\n");
    int lastmove = getchar() - '0';
    fflush(stdin); //removes excess inputs
    while (!(((lastmove > 0) && (lastmove < 10))
             && (*(board + (intconverter[lastmove - 1])) == EMPTY))) {
        printf("This is an invalid input! Try again!\n");
        printf("%d", lastmove);
        lastmove = getchar() - '0';
        fflush(stdin);
    }
    *(board + (intconverter[lastmove - 1])) = side;
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    return lastmove;
}

// dumbcomputermoves(board) randomly generates a move and returns the computer's last move.
int dumbcomputermoves(int *board, int turn) {
    printf("The computer has moved:\n");
    int randomindex = rand() % 9; // 0 to 8
    while (!(*(board + (intconverter[randomindex])) == EMPTY)) {
        randomindex = rand() % 9; // we need a valid char
    }
    *(board + (intconverter[randomindex])) = turn;
    return randomindex+1;
}


// winningmove(board, side) attempts to find the winning move. It will either return the winning move,
// or return 0 if none has been found.
int winningmove(int *board, const int side) {
    int winfound = 0;
    for(int i = 0; i < 9; ++i) {
        int actualboardarray = *(intconverter+i);
        if (*(board+actualboardarray) == EMPTY) { // only simulates on empty
            *(board+actualboardarray) = side; // sees if putting it there will generate a win
            if (victory(board, i+1, side) == 3) winfound = 1; // if there is a win..
            *(board+actualboardarray) = EMPTY; // undo this winning move, as we don't actually want to make the move
            if (winfound) {
                return (i); // found the winning move, in terms of *ingame* board
                printf("I am returning %d\n", i);
            }
        }
    }
    return winfound; // returns 0
}

// nextbest(board, side) plays in the middle, then corners if they are available.
// returns the in game array equivalence
int nextbest(int *board, const int side) {
    int cornerfound = 0;
    if (*(board + middle) == EMPTY) {
        *(board + middle) = side; // snatches middle
        return 5;
    }
    for (int i = 0; i < 4; ++i) { // plays in corners
        int actualarray = intconverter[*(corners + i)];
        if (*(board + actualarray) == EMPTY) {
            printf("Array %c is empty\n", *(board + actualarray));
            *(board + actualarray) = side;
            printf("Returning %d\n", *(corners + i) + 1);
            return *(corners + i) + 1;
        }
    }
    return 0; // nothing was available.
}


// smartcomputermoves(board, side) plays either a winning move or a randomly generated move
int smartcomputermoves(int *board, int side) {
    printf("The computer has moved:\n");
    if (winningmove(board, side)) { // a win has been found
        int ingamewin = winningmove(board, side);
        int boardlocation = intconverter[ingamewin];
        *(board + boardlocation) = side;
        return ingamewin + 1; // to 'actual' in game coordinates
    }

    // prevents the human from winning
    int possiblewin = winningmove(board, side ^ 1); // either 1 or 0
    if (possiblewin) {
        printf("I want to block you by playing %d\n", possiblewin + 1);
        *(board + intconverter[possiblewin]) = side;
        return (possiblewin+1);
    }

    // play in corners and middle
    int nextbestresult = (nextbest(board, side));
    if (nextbestresult) return nextbestresult;

    else { // if absoulutely nothing can be found (unsure if this is possible)
        int randomindex = rand() % 9; // 0 to 8
        while (!(*(board + (intconverter[randomindex])) == EMPTY)) {
            randomindex = rand() % 9; // we need a valid char
        }
        *(board + (intconverter[randomindex])) = side;
        return randomindex + 1; // to 'actual' in game coordinates
    }
}

int flawlesscomputermoves( int * const board,int side) {
    if (*(board + middle) == EMPTY) {
        *(board + middle) = side; // snatches middle
        printf("This first move is from my opening book, no depth searched.\n");
        return 5;
    }
    int bestmove = minmax(board, side);
    printf("After calculating %d positions, with a maxdepth of %d, I believe this is the optimal move:\n",
           positions, maxply);
    *(board + bestmove) = side;
    return reverseintconverter[bestmove];
}

// run commences the game
void run() {
    printf("Welcome to Tic-Tac-Toe. Pick your difficulty:\n");
    printf("1 is easy, 2 is challenging, 3 is unbeatable\n");
    int difficulty = getchar() - '0';
    fflush(stdin);
    printf("You have chosen difficulty %d\n", difficulty);
    printf("To make moves, type in the number associated with where you want to drop the piece, based on this mapping:\n");
    printf("1, 2, 3\n4, 5, 6\n7, 8, 9\n");
    int computerhasinrow;
    printf("Would you like to go first or second? 1 to go first, 2 to go second.\n");
    //if (difficulty == 3) printf("If you're up for a challange, try going second! \n");
    int turnorder = getchar() - '0';
    fflush(stdin);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    int turn;
    if (turnorder == 1) turn = NOUGHTS;
    else turn = CROSSES;
    int board[25];
    createboard(board);
    int totalturns = 0;
    while (1) {
        if (totalturns == 9) {
            boardprinter(board);
            printf("The game is a tie!\n");
            break;
        }
        if (turn == NOUGHTS) {
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            if ((victory(board, playermoves(board, turn), turn)) == 3) {
                boardprinter(board);
                printf("You have won! \n");
                break;
            }
            ++totalturns;
            turn = CROSSES;
        }
        else {
            if (difficulty == 1) computerhasinrow = (victory(board, dumbcomputermoves(board, turn), turn));
            if (difficulty == 2) computerhasinrow = (victory(board, smartcomputermoves(board, turn), turn));
            if (difficulty == 3) computerhasinrow = (victory(board, flawlesscomputermoves(board,turn), turn));
            if (computerhasinrow == 3) {
                printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                boardprinter(board);
                printf("I won! \n");
                break;
            }
            ++totalturns;
            turn = NOUGHTS;
        }
    }
}


int main() {
    srand(time(0));
    run();
}
#include "string.h"
#include "time.h"
#include "stdio.h"
#include "stdlib.h"

int main() {
    srand(time(0));
    int choice = 0;
    printf("Welcome to hangman!\n");
    printf("If you wish to quit, anytime while guessing, enter \"quit\"\n");
    char animals[][13] = { //words that random generation will choose from
            "starfish",
            "hippopotamus",
            "giraffe",
            "rhinoceros",
            "crocodile",
            "ferret"
    };

    int randomindex = rand() % 6; // random word generator
    int lives = 5, correctguesses = 0;
    int wordlength = strlen(animals[randomindex]);


    int lettersguessed[16] = {}; // all are 0s
    int quit = 0; // if program wants to be terminated
    char guess[16];
    char enteredletter;


    printf("Your category is: animals!\n");
    printf("Your word has %d letters.\n", wordlength);
    printf("You have %d lives in total!\n", lives);

    while (correctguesses < wordlength) {
        int oldcorrect = correctguesses; // oldcorrect is used to see if guess succeeded in char discovery
        printf("You have guessed correctly %d letters. \n", correctguesses);
        printf("Enter a guess:\n");
        fgets(guess, 16, stdin);
        int correctmsg = 0; // we only want congrats msg showing up once a guess

        if (!(strncmp(guess, "quit", 4))) { // exiting program
            quit = 1;
            break;
        }

        enteredletter = *(guess); // always takes in the first letter inputted
        printf("You guessed: %c \n", enteredletter);

        for (int loopindex = 0; loopindex < wordlength; ++loopindex) {

            if (lettersguessed[loopindex]) continue; // as char has already prev been counted as correct

            if (enteredletter == animals[randomindex][loopindex]) {

                lettersguessed[loopindex] = 1; // where 1 is guessed
                ++correctguesses;
                if (!correctmsg) printf("You have correctly guessed a character!\n");
                ++correctmsg;
            }

        }
        printf("The current state of your word is: \n");
        for (int charstatus = 0; charstatus<wordlength; ++charstatus) {
            if (lettersguessed[charstatus]) printf("%c", animals[randomindex][charstatus]);
            else printf("_");
        }
        printf("\n");
        if (oldcorrect == correctguesses) {
            --lives;
            printf("Unfortunately, that was not a valid character. \nYour life count is now only: %d\n", lives);
        }
        if (!lives) break;
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    }

    if (quit == 1) {
        printf("Wow what a loser, I can't believe you quit!");
    }

    if(correctguesses == wordlength) printf("Congrats, you won the game!");

    else printf("Sad! The correct word was %s. Better luck next time!", animals[randomindex]);
}
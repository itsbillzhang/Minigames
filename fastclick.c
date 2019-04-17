#include <stdio.h>
#include <time.h>
#include <stdbool.h>

int main() {
    printf("How many times can you press ENTER in 5 seconds?\n");
    double d, e;
    bool threetime = 1, twotime = 1, onetime = 1;
    clock_t t;
    printf("Starting in...\n");
    clock_t countdown = clock();
    while (1) {
        if ((e = clock() - countdown) / CLOCKS_PER_SEC > 1 && threetime) {
            printf("3..\n");
            threetime--;
        }
        if ((e = clock() - countdown) / CLOCKS_PER_SEC > 2 && twotime) {
            printf("2..\n");
            twotime--;
        }
        if ((e = clock() - countdown) / CLOCKS_PER_SEC > 3 && onetime) {
            printf("1..\n");
            onetime--;
        }
        if ((e = clock() - countdown) / CLOCKS_PER_SEC > 4) {
            printf("GO!!\n");
            break;
        }
    }
    t = clock();
    int clicks = 0;
    while (1) {
        if (getchar()) ++clicks;
        if ((d = clock() - t) / CLOCKS_PER_SEC > 5) {
            printf("TIME IS UP!\n");
            break;
        }
    }
    printf("Nice, you got %d clicks!\n", clicks);
}

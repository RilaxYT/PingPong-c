#include <stdio.h>
#include <stdbool.h>
#include "rl-txtgui.h"


bool hide_cursor = false;

// header fn
void rtg_hide_cursor() {

}
// header fn


int main(void) {
    printf("Hello world!!!\n");
    printf("\033[?25l"); // Ukryj kursor

    printf("\033[0;5H");
    printf("HUJHUJ");

    printf("\033[?25h"); // Pokaż kursor
    return 0;
}



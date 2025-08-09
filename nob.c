#define NOB_IMPLEMENTATION
#include <stdio.h>
#include "nob.h"

#define LIBS_PATH  "libs/termbox/"
#define BUILD_PATH "build/"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    printf("Build system!\n");

    nob_mkdir_if_not_exists(BUILD_PATH);


    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "cc", LIBS_PATH"termbox.c", LIBS_PATH"utf8.c", "src/main.c", "-o", BUILD_PATH"main");

    if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;


    nob_cmd_append(&cmd, BUILD_PATH"main");

    if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;

    return 0;
}

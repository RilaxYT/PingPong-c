#include <stdio.h>
#include "../libs/termbox/termbox.h"
#include <unistd.h>
#include <stdbool.h>
#include <string.h>


typedef struct {
    int x, y;
    int Xm, Ym;
} Ball;

typedef struct {
    bool win;
    int pk;
    int x;
    int y, size;
} Player;

Ball ball      = {0};
Player player1 = {0};
Player player2 = {0};

int width  = 0;
int height = 0;

bool w_p    = false;
bool s_p    = false;
bool up_p   = false;
bool down_p = false;

int step = 1;

char player1_pk_str[16];
char player2_pk_str[16];

void window_update() {
    width = tb_width();
    height = tb_height()-1;
}

void draw_text(int x, int y, const char* text, uint16_t fg, uint16_t bg) {
    for (int i = 0; i < strlen(text); ++i) {
        tb_change_cell(x + i, y, text[i], fg, bg);
    }
}

void draw_mirror_text(int x, int y, const char* text, uint16_t fg, uint16_t bg) {
    for (int i = 0; i < strlen(text); ++i) {
        tb_change_cell(x - i, y, text[strlen(text)-i-1], fg, bg);
    }
}

void draw_fgui() {
    snprintf(player1_pk_str, sizeof(player1_pk_str), "%dLVL", player1.pk);
    snprintf(player2_pk_str, sizeof(player2_pk_str), "%dLVL", player2.pk);

    draw_text((float)width*0.4f-(float)strlen(player1_pk_str)*0.5f, 0, player1_pk_str, TB_YELLOW, TB_BLUE);
    draw_text((float)width*0.6f-(float)strlen(player1_pk_str)*0.5f, 0, player2_pk_str, TB_YELLOW, TB_RED);

    draw_text(0, height, "PingPong v1", TB_YELLOW, TB_BLACK);
    draw_mirror_text(width, height, "Press 'q' to exit.", TB_YELLOW, TB_BLACK);
}

void draw_bgui() {
    for(int i=0; i < height; i++) {
         tb_change_cell((float)width*0.5f, i, '_', TB_BLACK, TB_WHITE);
    }

    for(int i=0; i < height; i++) {
         tb_change_cell((float)width*0.3f, i, '|', TB_WHITE, TB_BLACK);
         tb_change_cell((float)width*0.7f, i, '|', TB_WHITE, TB_BLACK);
    }
}

void draw_ball(Ball ball) {
    tb_change_cell(ball.x, ball.y, '#', TB_CYAN, TB_WHITE);
}

void draw_player(Player player) {
    for (int i=0; i < player.size; i++) {
        tb_change_cell(player.x, player.y+i-(float)player.size/2.0f, ' ', TB_WHITE, TB_WHITE);
    }
}

int main() {
    printf("PingPong-c! v1\n");

    if (tb_init() != 0) {
        fprintf(stderr, "Failed to initialize termbox\n");
        return 1;
    }

    struct tb_event event;
    tb_set_cursor(-1, -1);
    window_update();

    //
    ball.x  = (float)width*0.5f;
    ball.y  = 2;
    ball.Xm = -1;
    ball.Ym = 1;

    player1.y    = (float)height*0.5f;
    player2.y    = (float)height*0.5f;
    //

    bool main_loop = true;
    while(main_loop) { // main loop
        window_update();

        draw_bgui();

        for (int i=0; i < player1.size+2; i++) {
            if (ball.x == player1.x+1 && ball.y == player1.y-1+i-(int)((float)player1.size/2.0f)) {
                ball.Xm = 1;
            }
        }

        for (int i=0; i < player2.size+2; i++) {
            if (ball.x == player2.x-1 && ball.y == player2.y-1+i-(int)((float)player2.size/2.0f)) {
                ball.Xm = -1;
            }
        }

        if (ball.x <= 0) {
            player2.pk += 1;
            ball.x     = (float)width*0.5f;
            ball.y     = 2;
            ball.Xm    = 1;
            ball.Ym    = 1;
        }
        else if (ball.x >= width) {
            player1.pk += 1;
            ball.x     = (float)width*0.5f;
            ball.y     = 2;
            ball.Xm    = -1;
            ball.Ym    = 1;
        }

        if (ball.y <= 0)           ball.Ym = 1;
        else if (ball.y >= height) ball.Ym = -1;
    
        draw_ball(ball);
            
        ball.x += ball.Xm;
        ball.y += ball.Ym;

        w_p    = false;
        s_p    = false;
        up_p   = false;
        down_p = false;

        tb_peek_event(&event, 0);
        if (event.type == TB_EVENT_KEY) {
            switch (event.key) {
                case TB_KEY_ESC:        main_loop = false; break;
                case TB_KEY_CTRL_C:     main_loop = false; break;
                case TB_KEY_ARROW_UP:   up_p      = true;  break;
                case TB_KEY_ARROW_DOWN: down_p    = true;  break;
                default:                                   break;
            }

            switch (event.ch) {
                case 'q': main_loop = false; break;
                case 'w': w_p       = true;  break;
                case 's': s_p       = true;  break;
                default:                     break;
            }
        }

        step = (float)height*0.05f; // player speed scale
        if (step < 1) step = 1;

        if (w_p)    player1.y -= step;
        if (s_p)    player1.y += step;
        if (up_p)   player2.y -= step;
        if (down_p) player2.y += step;

        player1.size = (float)height* 0.1f;
        player1.x    = (float)width* 0.05f;
        player2.size = (float)height* 0.1f;
        player2.x    = (float)width* 0.95f;

        if (player1.y+(int)((float)player1.size/2.0f) > height) player1.y = (float)height-(float)player1.size/2.0f;
        if (player1.y < (int)((float)player1.size/2.0f))        player1.y = (float)player1.size/2.0f;
           
        if (player2.y+(int)((float)player2.size/2.0f) > height) player2.y = (float)height-(float)player2.size/2.0f;
        if (player2.y < (int)((float)player2.size/2.0f))        player2.y = (float)player2.size/2.0f;

        draw_player(player1);
        draw_player(player2);

        draw_fgui();

        tb_present();
        tb_clear();
        usleep(33 * 1000); //ms
        
        if (player1.pk == 10) {
            main_loop   = false;
            player1.win = true;
            printf("Player1 WIN!!! (10pk/%dpk)", player2.pk);
        }
        else if (player2.pk == 10) {
            main_loop   = false;
            player2.win = true;
            printf("Player2 WIN!!! (10pk/%dpk)", player1.pk);
        }
    }

    tb_shutdown();
    printf("\n");
    return 0;
}

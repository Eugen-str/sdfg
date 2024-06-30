#include <raylib.h>
#include <string.h>
#include "player.h"
#include "map.h"

const int scrw = 48 * 16, scrh = 48 * 16;

Color color_from_int(int n){
    return (Color){
        .a = 255,
        .r = n >> 8 * 2,
        .g = n >> 8 * 1,
        .b = n >> 8 * 0,
    };
}

void debug_info(char *logs){
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        int x = GetMouseX() / 48;
        int y = GetMouseY() / 48;
        strcpy(logs, TextFormat("[%d, %d]", x, y));
    }
}

int main(){
    //SetTraceLogLevel(LOG_NONE);
    InitWindow(scrw, scrh, "sdfg");
    SetTargetFPS(60);

    Color bg_color = color_from_int(0x3c3836);
    Font iosevka = LoadFont("assets/iosevka.ttf");
    Level current_level = init_lvl_0(scrw, scrh);
    Player player = make_player(current_level);

    int frames = 0;
    bool debug = false;
    char logs[128];
    while(!WindowShouldClose()){
        if(IsKeyPressed(KEY_ESCAPE)){
            break;
        }
        float delta = GetFrameTime();
        BeginDrawing();
            ClearBackground(bg_color);
            if(IsKeyPressed(KEY_F1)){
                debug = !debug;
            }

            if(debug){
                debug_info(logs);
                DrawText(logs, 50, 50, 25, RED);
            }
            draw_player(player);
            draw_level(current_level, iosevka, frames * delta, debug);
            update_player(&player, current_level, frames, delta);
        EndDrawing();
        frames++;
    }
}

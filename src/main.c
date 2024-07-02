#include <raylib.h>
#include <string.h>
#include "load_level.h"
#include "player.h"
#include "map.h"

const int scrw = 48 * 16, scrh = 48 * 16;

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

    Font iosevka = LoadFont("assets/iosevka.ttf");

    //Level current_level = init_lvl_0(scrw, scrh);
    Level current_level = load_level(0);
    Player player = make_player(current_level);
    Camera2D camera = {0};
    camera.target = (Vector2){player.rect.x, player.rect.y};

    int frames = 0;
    bool debug = false;
    char logs[128];
    while(!WindowShouldClose()){
        if(IsKeyPressed(KEY_ESCAPE)){
            break;
        }
        float delta = GetFrameTime();
        BeginMode2D(camera);
        BeginDrawing();
            ClearBackground(current_level.bg_color);
            if(IsKeyPressed(KEY_F1)){
                debug = !debug;
            }

            if(debug){
                debug_info(logs);
                DrawText(logs, 50, 50, 25, RED);
            }
            draw_level_bg(current_level, iosevka, frames * delta, debug);
            draw_player(player);
            draw_level_fg(current_level, iosevka, frames * delta, debug);
            update_player(&player, current_level, frames, delta);
        EndDrawing();
        EndMode2D();
        frames++;
    }
}

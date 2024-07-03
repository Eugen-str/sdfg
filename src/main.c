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

void update_camera_center(Camera2D *camera, Player *player, Level level){
    camera->offset = (Vector2){ scrw/2.0f, scrh/2.0f };
    camera->target = (Vector2){ player->rect.x, player->rect.y};
    float minX = 0, minY = 0, maxX = level.width * 48, maxY = level.height * 48;

    Vector2 max = GetWorldToScreen2D((Vector2){ maxX, maxY }, *camera);
    Vector2 min = GetWorldToScreen2D((Vector2){ minX, minY }, *camera);

    if (max.x < scrw) camera->offset.x = scrw - (max.x - scrw/2.0f);
    if (max.y < scrh) camera->offset.y = scrh - (max.y - scrh/2.0f);
    if (min.x > 0) camera->offset.x = scrw/2.0f - min.x;
    if (min.y > 0) camera->offset.y = scrh/2.0f - min.y;
}

int main(){
    //SetTraceLogLevel(LOG_NONE);
    InitWindow(scrw, scrh, "sdfg");
    SetTargetFPS(60);

    Font iosevka = LoadFont("assets/iosevka.ttf");

    Level current_level = load_level(0);
    Player player = make_player(current_level);
    Camera2D camera = {0};
    camera.target = (Vector2){player.rect.x, player.rect.y};
    camera.offset = (Vector2){ scrw/2.0f, scrh/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    int frames = 0;
    bool debug = false;
    char logs[128];
    while(!WindowShouldClose()){
        float delta = GetFrameTime();
        update_camera_center(&camera, &player, current_level);
        BeginDrawing();
            ClearBackground(current_level.bg_color);
            BeginMode2D(camera);
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
            EndMode2D();
        EndDrawing();
        frames++;
    }
}

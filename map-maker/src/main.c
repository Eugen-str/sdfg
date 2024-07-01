#include <raylib.h>

Color color_from_int(int n){
    return (Color){
        .a = 255,
        .r = n >> 8 * 2,
        .g = n >> 8 * 1,
        .b = n >> 8 * 0,
    };
}

typedef struct{
    Rectangle rect;
}Panel;

int main(){
    InitWindow(1000, 1000, "sdfg map maker!");

    Color bg_color = color_from_int(0x3c3836);
    //Panel panel = {.x = };
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(bg_color);
        EndDrawing();
    }
    CloseWindow();
}

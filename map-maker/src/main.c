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
    Color bg_color;
    Color border_color;
}Panel;

void draw_panels(Panel *p, int n){
    for(int i = 0; i < n; i++){
        DrawRectangleRec(p[i].rect, p[i].bg_color);
    }
}

void draw_button_up_down(Panel button, Texture2D tex, bool up, int tex_size){
    Rectangle source = (Rectangle){
        .x = !up * tex_size,
        .y = 0,
        .width = tex_size,
        .height = tex_size,
    };
    DrawTexturePro(tex, source, button.rect, (Vector2){0, 0}, 0, WHITE);
}

void draw_texture_panel(Panel tex_panel, Texture2D tex, int tex_size, int n){
    for(int i = 0; i < 6; i++){
        if(i % 2 == 0){
            DrawRectangle(tex_panel.rect.x + i*96, tex_panel.rect.y, 96, 96, LIGHTGRAY);
        } else {
            DrawRectangle(tex_panel.rect.x + i*96, tex_panel.rect.y, 96, 96, GRAY);
        }
    }
    Rectangle source = (Rectangle){
        .x = 0,
        .y = n * tex_size,
        .width = tex_size * 6,
        .height = tex_size
    };
    DrawTexturePro(tex, source, tex_panel.rect, (Vector2){0, 0}, 0, WHITE);
}

bool clicked_within(Panel p, Vector2 mouse_pos){
    return (p.rect.x <= mouse_pos.x &&
            p.rect.x + p.rect.width >= mouse_pos.x &&
            p.rect.y <= mouse_pos.y &&
            p.rect.y + p.rect.height >= mouse_pos.y);
}

void update_buttons_up_down(Panel btn_up, Panel btn_down, Vector2 mouse_pos, int *n, Color highlight_color){
    if(clicked_within(btn_up, mouse_pos)){
        DrawRectangleLinesEx(btn_up.rect, 3, highlight_color);
        if(*n > 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            *n -= 1;
        }
        return;
    }
    if(clicked_within(btn_down, mouse_pos)){
        DrawRectangleLinesEx(btn_down.rect, 3, highlight_color);
        if(*n < 7 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            *n += 1;
        }
        return;
    }
}

void draw_panel_outlines(Panel *panels, int panel_count){
    for(int i = 0; i < panel_count; i++){
        DrawRectangleLinesEx(panels[i].rect, 1, panels[i].border_color);
    }
}

void draw_panel_text(Panel panel, Font font, char *text, int font_size, Color color){
    int spacing = 2;

    Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, spacing);

    DrawTextEx(font, text, (Vector2){.x = panel.rect.x + 10,
            .y = panel.rect.y + (panel.rect.height - text_size.y) / 2}, font_size, spacing, color);
}

int main(){
    InitWindow(1100, 1024, "sdfg map maker!");
    Texture2D atlas = LoadTexture("../assets/tiles.png");
    Texture2D button_tex = LoadTexture("assets/button.png");
    Font iosevka = LoadFont("../assets/iosevka.ttf");
    int a = 48;

    Color lblue = color_from_int(0x83a598);
    Color dblue = color_from_int(0x458588);
    Color white = color_from_int(0xffffff);

    Color bg_color = color_from_int(0x181818);
    Color panel_border_color = lblue;
    Color game_bg_color = color_from_int(0x3c3836);
    Color selected_button = color_from_int(0x666666);

    Panel game_panel = (Panel){.bg_color = game_bg_color, .border_color = lblue,
        .rect = (Rectangle){.x = a, .y = a, .width = 768, .height = 768}};

    Panel tex_panel = (Panel){.bg_color = game_bg_color, .border_color = lblue,
        .rect = (Rectangle){.x = a, .y = 2*a + 768, .width = 6 * a * 2, .height = a * 2}};

    int d = 5;
    Panel button_up = (Panel){.bg_color = game_bg_color, .border_color = lblue,
        .rect = (Rectangle){.x = a + tex_panel.rect.x + tex_panel.rect.width + d,
            .y = 2*a + 768, .width = a - d, .height = a - d}};
    Panel button_down = (Panel){.bg_color = game_bg_color, .border_color = lblue,
        .rect = (Rectangle){.x = button_up.rect.x, .y = 3*a + 768 + d,
            .width = a - d, .height = a - d}};

    int right_btn_x = game_panel.rect.x + game_panel.rect.width + a;
    int right_btn_w = 200;
    int right_btn_h = a;
    Panel button_select_bg = (Panel){.bg_color = selected_button, .border_color = lblue,
        .rect = (Rectangle){.x = right_btn_x, .y = a, .width = right_btn_w, .height = right_btn_h}};
    Panel button_select_fg = (Panel){.bg_color = game_bg_color, .border_color = lblue,
        .rect = (Rectangle){.x = right_btn_x, .y = 2*a + right_btn_h, .width = right_btn_w, .height = right_btn_h}};
    Panel button_select_objects = (Panel){.bg_color = game_bg_color, .border_color = lblue,
        .rect = (Rectangle){.x = right_btn_x, .y = 3*a + 2*right_btn_h, .width = right_btn_w, .height = right_btn_h}};

    Panel panels[] = {
        game_panel,
        tex_panel,
        button_up,
        button_down,
        button_select_bg,
        button_select_fg,
        button_select_objects,
    };

    int panel_count = sizeof(panels) / sizeof(panels[0]);

    int tex_panel_loc = 0;
    int selected_layer = 0;

    SetTargetFPS(60);
    while(!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(bg_color);
            draw_panels(panels, panel_count);

            draw_texture_panel(tex_panel, atlas, 16, tex_panel_loc);
            draw_button_up_down(button_up, button_tex, true, 16);
            draw_button_up_down(button_down, button_tex, false, 16);

            draw_panel_outlines(panels, panel_count);

            draw_panel_text(button_select_fg, iosevka, "FG LAYER", 32, white);
            draw_panel_text(button_select_bg, iosevka, "BG LAYER", 32, white);
            draw_panel_text(button_select_objects, iosevka, "OBJECT LAYER", 32, white);

            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                Vector2 mouse_pos = GetMousePosition();
                update_buttons_up_down(button_up, button_down, mouse_pos, &tex_panel_loc, dblue);
            }
        EndDrawing();
    }
    CloseWindow();
}

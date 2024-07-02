#include <raylib.h>
#include <stdio.h>

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

void draw_panels(Panel **p, int n){
    for(int i = 0; i < n; i++){
        DrawRectangleRec(p[i]->rect, p[i]->bg_color);
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

void update_buttons_select(Panel *button_select_bg, Panel *button_select_fg, Panel *button_select_objects,
                        Vector2 mouse_pos, int *selected_layer, Color highlight_color, Color bg_color_unselected, Color bg_color_selected){
    if(clicked_within(*button_select_bg, mouse_pos)){
        DrawRectangleLinesEx(button_select_bg->rect, 3, highlight_color);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            *selected_layer = 0;
            button_select_bg->bg_color = bg_color_selected;
            button_select_fg->bg_color = bg_color_unselected;
            button_select_objects->bg_color = bg_color_unselected;
        }
        return;
    }
    if(clicked_within(*button_select_fg, mouse_pos)){
        DrawRectangleLinesEx(button_select_fg->rect, 3, highlight_color);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            *selected_layer = 1;
            button_select_fg->bg_color = bg_color_selected;
            button_select_bg->bg_color = bg_color_unselected;
            button_select_objects->bg_color = bg_color_unselected;
        }
        return;
    }
    if(clicked_within(*button_select_objects, mouse_pos)){
        DrawRectangleLinesEx(button_select_objects->rect, 3, highlight_color);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            *selected_layer = 2;
            button_select_objects->bg_color = bg_color_selected;
            button_select_fg->bg_color = bg_color_unselected;
            button_select_bg->bg_color = bg_color_unselected;
        }
        return;
    }
}

void draw_panel_outlines(Panel **panels, int panel_count){
    for(int i = 0; i < panel_count; i++){
        DrawRectangleLinesEx(panels[i]->rect, 1, panels[i]->border_color);
    }
}

void draw_panel_text(Panel panel, Font font, char *text, int font_size, Color color){
    int spacing = 2;

    Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, spacing);

    DrawTextEx(font, text, (Vector2){.x = panel.rect.x + 10,
            .y = panel.rect.y + (panel.rect.height - text_size.y) / 2}, font_size, spacing, color);
}

void update_texture_select(Panel tex_panel, int tex_panel_loc, int *selected_tex, Vector2 mouse_pos){
    if(clicked_within(tex_panel, mouse_pos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        *selected_tex = (int)(mouse_pos.x - tex_panel.rect.x) / (int)(tex_panel.rect.width / 6) + tex_panel_loc * 6;
    }
}

void draw_selected_tex(Panel selected_tex_view, Font font, Texture2D atlas, int selected_tex, int tex_size){
    if(selected_tex == 0){
        draw_panel_text(selected_tex_view, font, "no texture\nselected", 15, WHITE);
        return;
    }

    Rectangle source = (Rectangle){.x = selected_tex % 6 * tex_size, .y = (int)(selected_tex / 6) * tex_size,
        .width = tex_size, .height = tex_size};
    DrawTexturePro(atlas, source, selected_tex_view.rect, (Vector2){0, 0}, 0, WHITE);
}

void draw_game_panel(Panel game_panel, Texture2D atlas, int bg_buffer[16][16], int fg_buffer[16][16], Color line_color){
    int sq_size = game_panel.rect.width / 16;
    for(int i = 1; i < 16; i++){
        DrawLine(game_panel.rect.x, i * sq_size + game_panel.rect.y, sq_size + game_panel.rect.width, i * sq_size + game_panel.rect.y, line_color);
        DrawLine(i * sq_size + game_panel.rect.x, game_panel.rect.y, i * sq_size + game_panel.rect.y, game_panel.rect.y + game_panel.rect.height, line_color);
    }

    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            int bg_tex = bg_buffer[j][i];
            int fg_tex = fg_buffer[j][i];

            Rectangle bg_source = (Rectangle){.x = bg_tex % 6 * 16, .y = (int)(bg_tex / 6) * 16,
                .width = 16, .height = 16};
            Rectangle fg_source = (Rectangle){.x = fg_tex % 6 * 16, .y = (int)(fg_tex / 6) * 16,
                .width = 16, .height = 16};
            Rectangle dest = (Rectangle){.x = game_panel.rect.x + j*sq_size, .y = game_panel.rect.y + i*sq_size, .width = sq_size, .height = sq_size};
            DrawTexturePro(atlas, bg_source, dest, (Vector2){0, 0}, 0, WHITE);
            DrawTexturePro(atlas, fg_source, dest, (Vector2){0, 0}, 0, WHITE);
        }
    }
}

void update_place_texture(Panel game_panel, Vector2 mouse_pos, int bg_buffer[16][16], int fg_buffer[16][16], int selected_tex, int current_layer){
    int sq_size = game_panel.rect.width / 16;
    if(clicked_within(game_panel, mouse_pos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        int x = (mouse_pos.x - game_panel.rect.x) / sq_size, y = (mouse_pos.y - game_panel.rect.y) / sq_size;
        printf("%d %d\n", x, y);

        if(current_layer == 0){ // background
            bg_buffer[x][y] = selected_tex;
        } else if(current_layer == 1){ // foreground
            fg_buffer[x][y] = selected_tex;
        }
    }
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
    Color dusty_gray = color_from_int(0x665c54);

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
        .rect = (Rectangle){.x = (int)(a / 4) + tex_panel.rect.x + tex_panel.rect.width,
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


    Panel selected_tex_view = (Panel){ .bg_color = game_bg_color, .border_color = lblue,
        .rect = (Rectangle){.x = game_panel.rect.x + game_panel.rect.width - a*2, .y = button_up.rect.y, .width = a*2, .height = a*2}};

    Panel *panels[] = {
        &game_panel,
        &tex_panel,
        &button_up,
        &button_down,
        &button_select_bg,
        &button_select_fg,
        &button_select_objects,
        &selected_tex_view,
    };

    int panel_count = sizeof(panels) / sizeof(&panels[0]);

    int tex_panel_loc = 0;
    int selected_tex = 0;
    int selected_layer = 0;

    int bg_buffer[16][16];
    int fg_buffer[16][16];
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            bg_buffer[j][i] = 0;
            fg_buffer[j][i] = 0;
        }
    }

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

            draw_selected_tex(selected_tex_view, iosevka, atlas, selected_tex, 16);

            draw_game_panel(game_panel, atlas, bg_buffer, fg_buffer, dusty_gray);

            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                Vector2 mouse_pos = GetMousePosition();
                update_buttons_up_down(button_up, button_down, mouse_pos, &tex_panel_loc, dblue);
                update_buttons_select(&button_select_bg, &button_select_fg, &button_select_objects, mouse_pos,
                        &selected_layer, dblue, game_bg_color, selected_button);
                update_texture_select(tex_panel, tex_panel_loc, &selected_tex, mouse_pos);
                update_place_texture(game_panel, mouse_pos, bg_buffer, fg_buffer, selected_tex, selected_layer);
            }

        EndDrawing();
    }
    CloseWindow();
}

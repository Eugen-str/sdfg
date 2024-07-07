#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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

typedef struct{
    char logs[128];
    int started_time;
    int length;
}Logs;

void logs_add(Logs *logs, const char *str, int length){
    strcpy(logs->logs, str);
    logs->started_time = GetTime();
    logs->length = length;
}

void logs_update_draw(Logs *logs, Font font, Color color){
    DrawLine(0, 980, 1100, 980, color);
    DrawTextEx(font, "LOGS:", (Vector2){.x = 0, .y = 960}, 20, 1, WHITE);

    if(GetTime() >= logs->started_time + logs->length){
        return;
    }

    DrawTextEx(font, logs->logs, (Vector2){.x = 0, .y = 982}, 30, 1, WHITE);
}

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

bool clicked_within_rect(Rectangle rect, Vector2 mouse_pos){
    return (rect.x <= mouse_pos.x &&
            rect.x + rect.width >= mouse_pos.x &&
            rect.y <= mouse_pos.y &&
            rect.y + rect.height >= mouse_pos.y);
}

void update_buttons_up_down(Panel btn_up, Panel btn_down, Vector2 mouse_pos, int *n, int max_y, Color highlight_color){
    if(clicked_within(btn_up, mouse_pos)){
        DrawRectangleLinesEx(btn_up.rect, 3, highlight_color);
        if(*n > 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            *n -= 1;
        }
        return;
    }
    if(clicked_within(btn_down, mouse_pos)){
        DrawRectangleLinesEx(btn_down.rect, 3, highlight_color);
        if(*n < max_y && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
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

void draw_game_panel(Panel game_panel, RenderTexture screen, Texture2D atlas, Camera2D camera,
        int **bg_buffer, bool show_bg, int **fg_buffer, bool show_fg, int map_width, int map_height, Color line_color){
    int sq_size = game_panel.rect.width / 16;

    BeginTextureMode(screen);
    ClearBackground(game_panel.bg_color);
    BeginMode2D(camera);
    for(int i = 0; i < map_height; i++){
        for(int j = 0; j < map_width; j++){
            DrawRectangleLines(j * sq_size, i * sq_size,
                    sq_size, sq_size, line_color);
        }
    }

    for(int i = 0; i < map_height; i++){
        for(int j = 0; j < map_width; j++){
            int bg_tex = bg_buffer[j][i];
            int fg_tex = fg_buffer[j][i];

            Rectangle bg_source = (Rectangle){.x = bg_tex % 6 * 16, .y = (int)(bg_tex / 6) * 16,
                .width = 16, .height = 16};
            Rectangle fg_source = (Rectangle){.x = fg_tex % 6 * 16, .y = (int)(fg_tex / 6) * 16,
                .width = 16, .height = 16};
            Rectangle dest = (Rectangle){.x = game_panel.rect.x + j*sq_size, .y = game_panel.rect.y + i*sq_size, .width = sq_size, .height = sq_size};
            if(show_bg)
                DrawTexturePro(atlas, bg_source, dest, (Vector2){0, 0}, 0, WHITE);
            if(show_fg)
                DrawTexturePro(atlas, fg_source, dest, (Vector2){0, 0}, 0, WHITE);
        }
    }
    EndMode2D();
    EndTextureMode();
}

void update_place_texture(Panel game_panel, Camera2D camera, Vector2 mouse_pos, int **bg_buffer, int **fg_buffer, int selected_tex, int current_layer){
    int sq_size = game_panel.rect.width / 16;
    if(clicked_within(game_panel, mouse_pos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        int x = floor(mouse_pos.x - 2* game_panel.rect.x + camera.target.x) / sq_size,
            y = floor(mouse_pos.y - 2* game_panel.rect.y + camera.target.y) / sq_size;

        if(current_layer == 0){ // background
            bg_buffer[x][y] = selected_tex;
        } else if(current_layer == 1){ // foreground
            fg_buffer[x][y] = selected_tex;
        }
    }
    else if (clicked_within(game_panel, mouse_pos) && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        int x = (mouse_pos.x - game_panel.rect.x) / sq_size, y = (mouse_pos.y - game_panel.rect.y) / sq_size;
        if(current_layer == 0){ // background
            bg_buffer[x][y] = 0;
        } else if(current_layer == 1){ // foreground
            fg_buffer[x][y] = 0;
        }
    }
}

void save_to_file(int map_width, int map_height, Vector2 spawn_point, int **bg_buffer, int **fg_buffer, int *objects, int object_count){
    FILE *map_file;
    map_file = fopen("saved_maps/map.txt", "w");

    // map width and height
    fprintf(map_file, "%d %d\n", map_width, map_height);
    // map background color
    fprintf(map_file, "0x3c3836\n");
    // spawn point
    fprintf(map_file, "%d %d\n", (int)spawn_point.x, (int)spawn_point.y);
    fprintf(map_file, "\n");
    // background tiles
    for(int i = 0; i < map_height; i++){
        for(int j = 0; j < map_width; j++){
            fprintf(map_file, "%d ", bg_buffer[j][i]);
        }
        fprintf(map_file, "\n");
    }
    // foreground tiles
    fprintf(map_file, "\n");
    for(int i = 0; i < map_height; i++){
        for(int j = 0; j < map_width; j++){
            fprintf(map_file, "%d ", fg_buffer[j][i]);
        }
        fprintf(map_file, "\n");
    }
    // object_count
    fprintf(map_file, "%d\n", object_count);
    // objects
    fprintf(map_file, "\n");
    for(int i = 0; i < object_count; i++){
        fprintf(map_file, "%d %d %d %d %d\n",
                objects[i*5 + 0],
                objects[i*5 + 1],
                objects[i*5 + 2],
                objects[i*5 + 3],
                objects[i*5 + 4]);
    }

    fclose(map_file);
}


void update_button_save(Panel button_save, Vector2 mouse_pos, Logs *logs, Color highlight_color,
        int map_width, int map_height, int **bg_buffer, int **fg_buffer, int *objects, int object_count){
    if(clicked_within(button_save, mouse_pos)){
        DrawRectangleLinesEx(button_save.rect, 3, highlight_color);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            DrawRectangleLinesEx(button_save.rect, 3, highlight_color);
            save_to_file(map_width, map_height, (Vector2){15, 0}, bg_buffer, fg_buffer, objects, object_count);

            logs_add(logs, "Saved map.txt to saved_maps directory! Don't forget to copy it to game folder", 5);
        }
    }
}

void update_place_object(Panel game_panel, Vector2 mouse_pos, bool *object_start, Vector2 *object_start_loc, int *objects, int *object_count){
    int sq_size = game_panel.rect.width / 16;
    if(clicked_within(game_panel, mouse_pos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if(*object_start == false){
            *object_start = true;

            int x = (mouse_pos.x - game_panel.rect.x) / sq_size, y = (mouse_pos.y - game_panel.rect.y) / sq_size;
            *object_start_loc = (Vector2){.x = x, .y = y};
        } else {
            *object_start = false;
            int x = floor((mouse_pos.x - game_panel.rect.x) / sq_size)+1, y = floor((mouse_pos.y - game_panel.rect.y) / sq_size)+1;
            // objects are int this format
            // x y w h type
            Rectangle object = (Rectangle){.x = object_start_loc->x, .y = object_start_loc->y,
                .width = x - object_start_loc->x, .height = y - object_start_loc->y};
            objects[*object_count * 5 + 0] = object.x;
            objects[*object_count * 5 + 1] = object.y;
            objects[*object_count * 5 + 2] = object.width;
            objects[*object_count * 5 + 3] = object.height;
            objects[*object_count * 5 + 4] = 0;
            *object_count += 1;
        }
    }
    else if(clicked_within(game_panel, mouse_pos) && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        int x = round((mouse_pos.x - game_panel.rect.x) / sq_size), y = round((mouse_pos.y - game_panel.rect.y) / sq_size);
        for(int i = 0; i < *object_count; i++){
            Rectangle object = (Rectangle){
                .x = objects[i*5 + 0] * sq_size + game_panel.rect.x,
                .y = objects[i*5 + 1] * sq_size + game_panel.rect.y ,
                .width = objects[i*5 + 2] * sq_size,
                .height = objects[i*5 + 3] * sq_size,
            };
            if(clicked_within_rect(object, mouse_pos)){
                for(int j = i; j < *object_count; j++){
                    objects[j*5 + 0] = objects[(j+1)*5 + 0];
                    objects[j*5 + 1] = objects[(j+1)*5 + 1];
                    objects[j*5 + 2] = objects[(j+1)*5 + 2];
                    objects[j*5 + 3] = objects[(j+1)*5 + 3];
                    objects[j*5 + 4] = objects[(j+1)*5 + 4];
                }
                *object_count -= 1;
                return;
            }
        }
    }
}

void draw_objects(Panel game_panel, int *objects, int object_count, Color color){
    int sq_size = game_panel.rect.width / 16;
    for(int i = 0; i < object_count; i++){
        Rectangle object = (Rectangle){
            .x = objects[i*5 + 0] * sq_size + game_panel.rect.x,
            .y = objects[i*5 + 1] * sq_size + game_panel.rect.y ,
            .width = objects[i*5 + 2] * sq_size,
            .height = objects[i*5 + 3] * sq_size,
        };
        DrawRectangleLinesEx(object, 16, color);
    }
}
void update_checkbox(Panel *checkbox_object, Vector2 mouse_pos, Color highlight_color, Color checked, Color unchecked, bool *check){
    if(clicked_within(*checkbox_object, mouse_pos)){
        DrawRectangleLinesEx(checkbox_object->rect, 3, highlight_color);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(*check){
                checkbox_object->bg_color = unchecked;
            } else {
                checkbox_object->bg_color = checked;
            }
            *check = !(*check);
        }
    }
}

void load_file(Logs *logs, int **bg_buffer, int **fg_buffer, int *objects, int *object_count, char *path){
    FILE *map_file;
    map_file = fopen(path, "r");
    if(map_file == NULL){
        logs_add(logs, TextFormat("Could not load map file: %s", path), 5);
        return;
    }
    int map_width, map_height;
    int bg_color;
    int spawn_x, spawn_y;

    fscanf(map_file, "%d%d%x%d%d", &map_width, &map_height, &bg_color, &spawn_x, &spawn_y);

    int bg_tile;
    for(int i = 0; i < map_height; i++){
        for(int j = 0; j < map_width; j++){
            fscanf(map_file, "%d", &bg_tile);
            if(bg_tile != 0){
                bg_buffer[j][i] = bg_tile;
            }
        }
    }

    int fg_tile;
    for(int i = 0; i < map_height; i++){
        for(int j = 0; j < map_width; j++){
            fscanf(map_file, "%d", &fg_tile);
            if(fg_tile != 0){
                fg_buffer[j][i] = fg_tile;
            }
        }
    }

    int num_objects;
    fscanf(map_file, "%d", &num_objects);

    for(int i = 0; i < num_objects; i++){
        int x, y, w, h, type;
        fscanf(map_file, "%d%d%d%d%d", &x, &y, &w, &h, &type);
        objects[*object_count * 5 + 0] = x;
        objects[*object_count * 5 + 1] = y;
        objects[*object_count * 5 + 2] = w;
        objects[*object_count * 5 + 3] = h;
        objects[*object_count * 5 + 4] = 0;
        *object_count += 1;
    }

    fclose(map_file);
    logs_add(logs, TextFormat("Sucessfully loaded map file: %s", path), 5);
}

const int scrw = 1100, scrh = 1024;

int main(int argc, char* argv[]){
    InitWindow(scrw, scrh, "sdfg map maker!");

    int map_width = 16;
    int map_height = 16;

    Texture2D atlas = LoadTexture("../assets/tiles.png");
    Texture2D button_tex = LoadTexture("assets/button.png");
    Font iosevka = LoadFont("../assets/iosevka.ttf");
    int a = 48;

    Color lblue = color_from_int(0x83a598);
    Color dblue = color_from_int(0x458588);
    Color white = color_from_int(0xffffff);
    Color dusty_gray = color_from_int(0x665c54);
    Color green = color_from_int(0xb8bb26);
    Color red = color_from_int(0xcc241d);

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


    Panel selected_tex_view = (Panel){.bg_color = game_bg_color, .border_color = lblue,
        .rect = (Rectangle){.x = game_panel.rect.x + game_panel.rect.width - a*2, .y = button_up.rect.y, .width = a*2, .height = a*2}};

    Panel button_save = (Panel){.bg_color = game_bg_color, .border_color = lblue,
        .rect = (Rectangle){.x = selected_tex_view.rect.x + selected_tex_view.rect.width + a, 
            .y = selected_tex_view.rect.y, .width = 200, .height = a*2}};

    int checkbox_side = 25;
    bool show_objects = false;
    bool show_backround = true;
    bool show_foreground = true;
    Panel checkbox_object = (Panel){.bg_color = game_bg_color, .border_color = lblue,
        .rect = (Rectangle){.x = button_select_objects.rect.x - checkbox_side - d,
            .y = button_select_objects.rect.y + (int)(checkbox_side / 2), .width = checkbox_side, .height = checkbox_side}};

    Panel checkbox_bg = (Panel){.bg_color = green, .border_color = lblue,
        .rect = (Rectangle){.x = button_select_bg.rect.x - checkbox_side - d,
            .y = button_select_bg.rect.y + (int)(checkbox_side / 2), .width = checkbox_side, .height = checkbox_side}};

    Panel checkbox_fg = (Panel){.bg_color = green, .border_color = lblue,
        .rect = (Rectangle){.x = button_select_fg.rect.x - checkbox_side - d,
            .y = button_select_fg.rect.y + (int)(checkbox_side / 2), .width = checkbox_side, .height = checkbox_side}};


    int input_box_width = 200;
    Panel filename_input_box = (Panel){.bg_color = game_bg_color, .border_color = lblue,
        .rect = (Rectangle){.x = button_save.rect.x,
            .y = button_select_fg.rect.y + (int)(checkbox_side / 2), .width = checkbox_side, .height = checkbox_side}};

    Panel *panels[] = {
        &game_panel,
        &tex_panel,

        &button_up,
        &button_down,

        &button_select_bg,
        &button_select_fg,
        &button_select_objects,

        &selected_tex_view,

        &button_save,

        &checkbox_object,
        &checkbox_bg,
        &checkbox_fg,
    };

    Logs logs = {0};

    int panel_count = sizeof(panels) / sizeof(&panels[0]);

    int tex_panel_loc = 0;
    int selected_tex = 0;
    int selected_layer = 0;

    bool object_start = false;
    Vector2 object_start_loc;

    int *objects = malloc(sizeof(int) * 128);
    int object_count = 0;

    int **bg_buffer = (int**)malloc(map_width* sizeof(int*));
    int **fg_buffer = (int**)malloc(map_width* sizeof(int*));
    for(int i = 0; i < map_height; i++){
        bg_buffer[i] = (int*)malloc(map_height * sizeof(int));
        fg_buffer[i] = (int*)malloc(map_height * sizeof(int));
    }
    for(int i = 0; i < map_width; i++){
        for(int j = 0; j < map_height; j++){
            bg_buffer[j][i] = 0;
            fg_buffer[j][i] = 0;
        }
    }

    if(argc > 1){
        load_file(&logs, bg_buffer, fg_buffer, objects, &object_count, argv[1]);
    }

    Camera2D camera = { 0 };
    //camera.target = (Vector2){game_panel.rect.x + game_panel.rect.width/2, game_panel.rect.y + game_panel.rect.height/2};
    //camera.offset = (Vector2){game_panel.rect.x + game_panel.rect.width/2, game_panel.rect.y + game_panel.rect.height/2};
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    RenderTexture game_screen = LoadRenderTexture(game_panel.rect.width, game_panel.rect.height);
    Rectangle screen_rect = { 0.0f, 0.0f, (float)game_screen.texture.width, (float)-game_screen.texture.height };
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        draw_game_panel(game_panel, game_screen, atlas, camera, bg_buffer, show_backround, fg_buffer, show_foreground, map_width, map_height, dusty_gray);
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

            DrawTextureRec(game_screen.texture, screen_rect, (Vector2){.x = game_panel.rect.x, .y = game_panel.rect.y}, WHITE);

            draw_panel_text(button_save, iosevka, "SAVE", 48, WHITE);

            //TODO: draw_object does not account for camera
            if(show_objects)
                draw_objects(game_panel, objects, object_count, red);

            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
                Vector2 mouse_pos = GetMousePosition();

                update_buttons_up_down(button_up, button_down, mouse_pos, &tex_panel_loc, atlas.height/16 -1, dblue);
                update_buttons_select(&button_select_bg, &button_select_fg, &button_select_objects, mouse_pos,
                        &selected_layer, dblue, game_bg_color, selected_button);
                update_button_save(button_save, mouse_pos, &logs, dblue, map_width, map_height, bg_buffer, fg_buffer, objects, object_count);

                update_texture_select(tex_panel, tex_panel_loc, &selected_tex, mouse_pos);

                if(selected_layer == 2)
                    update_place_object(game_panel, mouse_pos, &object_start, &object_start_loc, objects, &object_count);
                else
                    update_place_texture(game_panel, camera, mouse_pos, bg_buffer, fg_buffer, selected_tex, selected_layer);

                update_checkbox(&checkbox_object, mouse_pos, dblue, green, game_bg_color, &show_objects);
                update_checkbox(&checkbox_fg, mouse_pos, dblue, green, game_bg_color, &show_foreground);
                update_checkbox(&checkbox_bg, mouse_pos, dblue, green, game_bg_color, &show_backround);
            }

            if(IsKeyDown(KEY_LEFT) && camera.target.x > -5.0f * 48){
                camera.target.x -= 5.0f;
            }
            if(IsKeyDown(KEY_RIGHT) && camera.target.x < map_width * 48 - 48 * 11){
                camera.target.x += 5.0f;
            }
            if(IsKeyDown(KEY_UP) && camera.target.y > -5.0f * 48){
                camera.target.y -= 5.0f;
            }
            if(IsKeyDown(KEY_DOWN) && camera.target.y < map_height * 48 - 48 * 11){
                camera.target.y += 5.0f;
            }

            logs_update_draw(&logs, iosevka, dblue);
        EndDrawing();
    }

    for(int i = 0; i < map_height; i++){
        free(bg_buffer[i]);
        free(fg_buffer[i]);
    }
    free(bg_buffer);
    free(fg_buffer);

    CloseWindow();
    return 0;
}

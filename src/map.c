#include "map.h"
#include <stdlib.h>
#include <raylib.h>

#define count(x) sizeof(x) / sizeof(x[0])

void place_tile(Tile *tiles, int *tile_count, TileType type, int x, int y){
    tiles[*tile_count].pos = (Vector2){.x = x * 48.0f, .y = y * 48.0f};
    tiles[*tile_count].type = type;
    *tile_count += 1;
}

void place_object(Object *objects, int *object_count, ObjectType type, Rectangle rect){
    objects[*object_count].rect = rect;
    objects[*object_count].type= type;
    *object_count += 1;

}

#define place_tileq_bg(type, x, y) place_tile(bg_tiles, &bg_tile_count, type, x, y)
#define place_tileq_fg(type, x, y) place_tile(fg_tiles, &fg_tile_count, type, x, y)
#define place_objectq(type, x, y, w, h) place_object(objects, &object_count, type, (Rectangle){(x)*48, (y)*48, (w)*48, (h)*48})

Level init_lvl_0(int scrw, int scrh){
    int max_right = scrw / 48;
    int max_down = scrh / 48;

    Level lvl0 = {0};
    lvl0.tilemap = LoadTexture("assets/tiles.png");
    //lvl0.spawn_point = (Vector2){.x = 48 * 3, .y = max_down * 48 - 3 * 48};
    lvl0.bg_color = (Color){100, 100, 100, 255};

    Object objects[256];
    int object_count = 0;

    //walls
    place_objectq(OBJ_WP, 0, 0, 1, max_down-2);
    place_objectq(OBJ_WP, max_right-1, 0, 1, max_down-1);
    place_objectq(OBJ_WP, 0, 0, max_right, 1);
    //floor
    place_objectq(OBJ_WP, 0, max_down - 1, max_right, 1);
    //small pillar
    place_objectq(OBJ_WP, 7, max_down - 3, 1, 2);
    //big pillar
    place_objectq(OBJ_WP, 10, max_down - 5, 1, 4);
    //island
    place_objectq(OBJ_WP, max_right - 3, 9, 2, 1);

    Tile fg_tiles[256] = {0};
    int fg_tile_count = 0;

    /// ---- FOREGROUND
    for(int i = 0; i < max_right - 1; i++){
        place_tileq_fg(GRASS, i, max_down - 1);
    }
    place_tileq_fg(WALL_RIGHT, max_right - 1, max_down - 1);

    place_tileq_fg(PILLAR_BOTTOM, 10, max_down - 2);
    place_tileq_fg(PILLAR_MID, 10, max_down - 3);
    place_tileq_fg(PILLAR_MID, 10, max_down - 4);
    place_tileq_fg(PILLAR_TOP, 10, max_down - 5);

    place_tileq_fg(PILLAR_BOTTOM, 7, max_down - 2);
    place_tileq_fg(PILLAR_TOP, 7, max_down - 3);

    for(int i = 1; i < max_right - 1; i++){
        place_tileq_fg(DEFAULT_CEIL, i, 0);
    }
    for(int j = 1; j < max_down - 3; j++){
        place_tileq_fg(WALL_LEFT, 0, j);
        place_tileq_fg(WALL_RIGHT, max_right - 1, j);
    }

    place_tileq_fg(WALL_LEFT_DOWN, 0, max_down - 3);
    place_tileq_fg(WALL_RIGHT, max_right - 1, max_down - 2);
    place_tileq_fg(WALL_RIGHT, max_right - 1, max_down - 3);

    place_tileq_fg(RED_FLOWER, max_right - 2, max_down - 2);
    place_tileq_fg(BLUE_FLOWER, 3, max_down - 2);
    place_tileq_fg(BLUE_FLOWER, 8, max_down - 2);
    place_tileq_fg(BUSH, 6, max_down - 2);
    place_tileq_fg(VINES, 10, max_down - 5);

    for(int x = max_right - 3; x < max_right - 1; x++){
        place_tileq_fg(GROUND_TOP, x, 9);
    }

    place_tileq_fg(CONNECTOR_RIGHT_DOWN, 0, 0);
    place_tileq_fg(CONNECTOR_LEFT_DOWN, max_right-1, 0);

    /// ----
    ///
    
    Tile bg_tiles[256] = {0};
    int bg_tile_count = 0;

    /// ---- BACKGROUND

    place_tileq_bg(RED_FLOWER, 5, max_down - 2);

    /// ----


    // objects
    lvl0.objects = malloc(sizeof(objects));
    for (int i = 0; i < object_count; i++) {
            lvl0.objects[i] = objects[i];
    }
    lvl0.object_count = object_count;
    // foreground
    lvl0.fg_tiles = malloc(sizeof(fg_tiles));
    for (int i = 0; i < fg_tile_count; i++) {
            lvl0.fg_tiles[i] = fg_tiles[i];
    }
    lvl0.fg_tile_count = fg_tile_count;
    // background
    lvl0.bg_tiles = malloc(sizeof(bg_tiles));
    for (int i = 0; i < bg_tile_count; i++) {
            lvl0.bg_tiles[i] = bg_tiles[i];
    }
    lvl0.bg_tile_count = bg_tile_count;
    return lvl0;
}

void draw_level_bg(Level level, Font font, float sec, bool debug){
    for(int i = 0; i < level.bg_tile_count; i++){
        int tm_x = level.bg_tiles[i].type % 6;
        int tm_y = level.bg_tiles[i].type / 6;
        Rectangle source = (Rectangle){.x = 16 * tm_x, .y = 16 * tm_y, .width = 16, .height = 16};
        Rectangle dest = (Rectangle){.x = level.bg_tiles[i].pos.x, .y = level.bg_tiles[i].pos.y, .width = 16 * 3, .height = 16 * 3};
        DrawTexturePro(level.tilemap, source, dest, (Vector2){.x = 0, .y = 0}, 0, WHITE);
    }
}
void draw_level_fg(Level level, Font font, float sec, bool debug){
    for(int i = 0; i < level.fg_tile_count; i++){
        int tm_x = level.fg_tiles[i].type % 6;
        int tm_y = level.fg_tiles[i].type / 6;
        Rectangle source = (Rectangle){.x = 16 * tm_x, .y = 16 * tm_y, .width = 16, .height = 16};
        Rectangle dest = (Rectangle){.x = level.fg_tiles[i].pos.x, .y = level.fg_tiles[i].pos.y, .width = 16 * 3, .height = 16 * 3};
        DrawTexturePro(level.tilemap, source, dest, (Vector2){.x = 0, .y = 0}, 0, WHITE);
    }

    if(debug){
        Color cl[] = {RED, BLUE, YELLOW, GREEN};
        for(int i = 0; i < level.object_count; i++){
            DrawRectangleRec(level.objects[i].rect, cl[level.objects[i].type]);
        }
    }
}

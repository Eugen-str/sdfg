#include "load_level.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>

Level load_level(int n){
    Level level = {0};
    level.tilemap = LoadTexture("assets/tiles.png");
    FILE *fg_file, *bg_file;

    // ---- FOREGROUND & BACKGROUND
    Tile fg_tiles[256] = {0};
    int fg_tile_count = 0;
    Tile bg_tiles[256] = {0};
    int bg_tile_count = 0;

    fg_file = fopen(TextFormat("maps/%d/fg.txt", n), "r");
    bg_file = fopen(TextFormat("maps/%d/bg.txt", n), "r");

    int fg_tile, bg_tile;
    for(int i = 0; i < 16 * 16; i++){
        fscanf(fg_file, "%d", &fg_tile);
        fscanf(bg_file, "%d", &bg_tile);
        if(fg_tile != 0){
            place_tile(fg_tiles, &fg_tile_count, fg_tile, i % 16, i / 16);
        }

        if(bg_tile != 0){
            place_tile(bg_tiles, &bg_tile_count, bg_tile, i % 16, i / 16);
        }
    }

    fclose(fg_file);
    fclose(bg_file);

    // ----

    // ---- OBJECTS

    FILE *object_file;
    object_file = fopen(TextFormat("maps/%d/objects.txt", n), "r");
    Object objects[256];
    int object_count = 0;

    int line_count = 0;

    char buf[32];
    while(fgets(buf, 32, object_file) != NULL){
        line_count++;
    }
    fclose(object_file);
    object_file = fopen(TextFormat("maps/%d/objects.txt", n), "r");

    int x, y, w, h, type;
    for(int i = 0; i < line_count; i++){
        fscanf(object_file, "%d%d%d%d%d", &x, &y, &w, &h, &type);
        place_object(objects, &object_count, type, (Rectangle){.x = x, .y = y, .width = w, .height = h});
    }

    fclose(object_file);

    // ----

    // COPYING EVERYTHING INTO THE STRUCT
    // foreground
    level.fg_tiles = malloc(sizeof(fg_tiles));
    for(int i = 0; i < fg_tile_count; i++) {
            level.fg_tiles[i] = fg_tiles[i];
    }
    level.fg_tile_count = fg_tile_count;
    // background
    level.bg_tiles = malloc(sizeof(bg_tiles));
    for(int i = 0; i < bg_tile_count; i++) {
            level.bg_tiles[i] = bg_tiles[i];
    }
    level.bg_tile_count = bg_tile_count;
    // objects
    level.objects = malloc(sizeof(objects));
    for(int i = 0; i < object_count; i++) {
            level.objects[i] = objects[i];
    }
    level.object_count = object_count;

    return level;
}

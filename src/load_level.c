#include "load_level.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>

Level load_level(int n){
    Level level = {0};
    level.tilemap = LoadTexture("assets/tiles.png");
    FILE *map_file;
    map_file = fopen(TextFormat("maps/%d/map.txt", n), "r");

    int map_width, map_height;
    int bg_color;

    fscanf(map_file, "%d%d%x", &map_width, &map_height, &bg_color);

    // BACKGROUND

    Tile bg_tiles[256] = {0};
    int bg_tile_count = 0;

    int bg_tile;
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            fscanf(map_file, "%d", &bg_tile);
            if(bg_tile != 0){
                place_tile(bg_tiles, &bg_tile_count, bg_tile, j, i);
            }
        }
    }

    // FOREGROUND

    Tile fg_tiles[256] = {0};
    int fg_tile_count = 0;
    
    int fg_tile;
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            fscanf(map_file, "%d", &fg_tile);
            if(fg_tile != 0){
                place_tile(fg_tiles, &fg_tile_count, fg_tile, j, i);
            }
        }
    }

    // OBJECTS

    Object objects[256];
    int object_count = 0;

    int num_objects;
    fscanf(map_file, "%d", &num_objects);

    for(int i = 0; i < num_objects; i++){
        int x, y, w, h, type;
        fscanf(map_file, "%d%d%d%d%d", &x, &y, &w, &h, &type);
        place_object(objects, &object_count, type, (Rectangle){.x = x*48, .y = y*48, .width = w*48, .height = h*48});
    }

    fclose(map_file);

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

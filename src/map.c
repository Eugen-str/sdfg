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

#define place_tileq(type, x, y) place_tile(tiles, &tile_count, type, x, y)
#define place_objectq(type, x, y, w, h) place_object(objects, &object_count, type, (Rectangle){(x)*48, (y)*48, (w)*48, (h)*48})

Level init_lvl_0(int scrw, int scrh){
    int max_right = scrw / 48;
    int max_down = scrh / 48;

    Level lvl0 = {0};
    lvl0.level_count = 0;
    lvl0.name = "Tutorial";
    lvl0.tilemap = LoadTexture("assets/tiles.png");
    lvl0.spawn_point = (Vector2){.x = 48 * 3, .y = max_down * 48 - 3 * 48};

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

    Tile tiles[256] = {0};
    int tile_count = 0;

    /// ---- PLACEMENTS
    for(int i = 0; i < max_right - 1; i++){
        place_tileq(GRASS, i, max_down - 1);
    }
    place_tileq(WALL_RIGHT, max_right - 1, max_down - 1);

    place_tileq(PILLAR_BOTTOM, 10, max_down - 2);
    place_tileq(PILLAR_MID, 10, max_down - 3);
    place_tileq(PILLAR_MID, 10, max_down - 4);
    place_tileq(PILLAR_TOP, 10, max_down - 5);

    place_tileq(PILLAR_BOTTOM, 7, max_down - 2);
    place_tileq(PILLAR_TOP, 7, max_down - 3);

    for(int i = 1; i < max_right - 1; i++){
        place_tileq(DEFAULT_CEIL, i, 0);
    }
    for(int j = 1; j < max_down - 3; j++){
        place_tileq(WALL_LEFT, 0, j);
        place_tileq(WALL_RIGHT, max_right - 1, j);
    }
    //TODO: Make these connectors work!
    //place_tileq(CONNECTOR_RIGHT, 0, 0);
    //place_tileq(CONNECTOR_LEFT, max_right-1, 0);

    place_tileq(WALL_LEFT_DOWN, 0, max_down - 3);
    place_tileq(WALL_RIGHT, max_right - 1, max_down - 2);
    place_tileq(WALL_RIGHT, max_right - 1, max_down - 3);

    place_tileq(RED_FLOWER, 5, max_down - 2);
    place_tileq(RED_FLOWER, max_right - 2, max_down - 2);
    place_tileq(BLUE_FLOWER, 3, max_down - 2);
    place_tileq(BLUE_FLOWER, 8, max_down - 2);
    place_tileq(BUSH, 6, max_down - 2);
    place_tileq(VINES, 10, max_down - 5);

    for(int x = max_right - 3; x < max_right - 1; x++){
        place_tileq(GROUND_TOP, x, 9);
    }

    //place_tileq(LADDER, );

    /// ----


    lvl0.objects = malloc(sizeof(objects));
    for (int i = 0; i < object_count; i++) {
            lvl0.objects[i] = objects[i];
    }
    lvl0.object_count = object_count;

    lvl0.tiles = malloc(sizeof(tiles));
    for (int i = 0; i < tile_count; i++) {
            lvl0.tiles[i] = tiles[i];
    }
    lvl0.tile_count = tile_count;

    return lvl0;
}

void draw_level(Level level, Font font, float sec, bool debug){
    for(int i = 0; i < level.tile_count; i++){
        int tm_x = level.tiles[i].type % 6;
        int tm_y = level.tiles[i].type / 6;
        Rectangle source = (Rectangle){.x = 16 * tm_x, .y = 16 * tm_y, .width = 16, .height = 16};
        Rectangle dest = (Rectangle){.x = level.tiles[i].pos.x, .y = level.tiles[i].pos.y, .width = 16 * 3, .height = 16 * 3};
        DrawTexturePro(level.tilemap, source, dest, (Vector2){.x = 0, .y = 0}, 0, WHITE);
    }
    if(sec < 6){
        DrawTextEx(font,
                TextFormat("Level %d: %s", level.level_count, level.name),
                (Vector2){.x = 20, .y = 20}, 32, 1, RAYWHITE);
    } else if(sec <= 10){
        DrawTextEx(font,
                TextFormat("Level %d: %s", level.level_count, level.name),
                (Vector2){.x = 20, .y = 20}, 32, 1, (Color){245, 245, 245, 255 - (sec - 6) * 60});
    }

    if(debug){
        Color cl[] = {RED, BLUE, YELLOW, GREEN};
        for(int i = 0; i < level.object_count; i++){
            DrawRectangleRec(level.objects[i].rect, cl[level.objects[i].type]);
        }
    }
}

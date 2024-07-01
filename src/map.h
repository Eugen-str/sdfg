#ifndef MAP_H_
#define MAP_H_

#include <raylib.h>

typedef enum{
    WATER = 1,
    WATER_TOP1,
    WATER_TOP2,

    GROUND_FILL = 6,
    GROUND_TOP,

    GRASS = 12,
    GRASS_RIGHT,
    GRASS_LEFT,
    GRASS_SLOPE_LEFT,
    GRASS_SLOPE_RIGHT,

    VINES = 18,
    RED_FLOWER,
    BUSH,
    BLUE_FLOWER,
    LONG_VINE1,
    LONG_VINE2,

    PILLAR_TOP = 24,
    PILLAR_BOTTOM,
    PILLAR_MID,
    PILLAR_BROKEN,

    WALL_LEFT = 30,
    WALL_LEFT_DOWN,
    WALL_LEFT_UP,
    WALL_RIGHT,
    WALL_RIGHT_DOWN,
    WALL_RIGHT_UP,

    DEFAULT_FLOOR = 36,
    DEFAULT_CEIL,
    CONNECTOR_RIGHT_UP,
    CONNECTOR_LEFT_UP,
    CONNECTOR_RIGHT_DOWN,
    CONNECTOR_LEFT_DOWN,

    LADDER = 42,
}TileType;

typedef struct{
    TileType type;
    Vector2 pos;
}Tile;

typedef enum{
    OBJ_WP,
    OBJ_SLOPE,
}ObjectType;

typedef struct{
    Rectangle rect;
    ObjectType type;
}Object;

typedef struct{
    int level_count;
    const char* name;

    Vector2 spawn_point;
    Texture2D tilemap;

    Object *objects;
    int object_count;

    Tile *fg_tiles;
    int fg_tile_count;
    
    Tile *bg_tiles;
    int bg_tile_count;
}Level;

Level init_lvl_0(int scrw, int scrh);
void draw_level_fg(Level level, Font font, float sec, bool debug);
void draw_level_bg(Level level, Font font, float sec, bool debug);

void place_tile(Tile *tiles, int *tile_count, TileType type, int x, int y);
void place_object(Object *objects, int *object_count, ObjectType type, Rectangle rect);

#endif //MAP_H_

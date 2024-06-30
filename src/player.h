#ifndef PLAYER_H_
#define PLAYER_H_

#include <raylib.h>
#include "map.h"

typedef enum{
    NORMAL = 0,
    WALKING,
    JUMPING,
    FALLING
}PlayerAnimState;

typedef struct{
    Rectangle rect;

    int state;
    int speed;
    bool can_jump;

    Texture2D sprite_map;
    int size;
    int tex_size;
}Player;

Player make_player(Level current_level);
void draw_player(Player player);
void update_player(Player *player, Level level, int frames, float delta);

#endif //PLAYER_H_

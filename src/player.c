#include <raylib.h>
#include "player.h"
#include "map.h"
#include "collision.h"

#define G 2000
#define JUMP_SPD 700
#define MOV_SPD 5

Player make_player(Level current_level){
    Player player = {.tex_size = 16, .size = 48, .speed = 20, .state = 0};
    player.rect = (Rectangle){.x = current_level.spawn_point.x, .y = current_level.spawn_point.y,
        .width = player.size, .height = player.size};
    player.sprite_map = LoadTexture("assets/guy_all.png");
    return player;
}

void draw_player(Player player){
    Rectangle source = (Rectangle){.x = player.tex_size * player.state, .y = 0, .width = player.tex_size, .height = player.tex_size};
    Rectangle dest = (Rectangle){.x = player.rect.x, .y = player.rect.y, .width = player.size, .height = player.size};
    DrawTexturePro(player.sprite_map, source, dest, (Vector2){.x = 0, .y = 0}, 0, WHITE);
}

void update_player(Player *player, Level level, int frames, float delta){
    int mov_x = 0;
    if(IsKeyPressed(KEY_R)){
        player->rect.x = level.spawn_point.x;
        player->rect.y = level.spawn_point.y;
    }
    if(IsKeyDown(KEY_A)){
        mov_x -= MOV_SPD;
        if(frames % 16 == 0){
            if(player->state == WALKING) player->state = NORMAL;
            else player->state = WALKING;
        }
    }
    if(IsKeyDown(KEY_D)){
        mov_x += MOV_SPD;
        if(frames % 16 == 0){
            if(player->state == WALKING) player->state = NORMAL;
            else player->state = WALKING;
        }
    }
    if(IsKeyDown(KEY_W) && player->can_jump){
        player->rect.y--;
        player->speed = -JUMP_SPD;
        player->can_jump = false;
    }
    if(IsKeyReleased(KEY_A) || IsKeyReleased(KEY_D)){
        player->state = NORMAL;
    }

    bool hit_obstacle = false;
    for(int i = 0; i < level.object_count; i++){
        collision(player, level.objects[i]);
    }

    if(!hit_obstacle){
        player->speed += G * delta;
    }
    player->rect.y += player->speed * delta;
    player->rect.x += mov_x;

    if(hit_obstacle){
    } else if(player->speed > G * delta){
        player->state = FALLING;
    } else if(player->speed < 0){
        player->state = JUMPING;
    }

    if(player->rect.y >= 16*70){
        player->rect.x = level.spawn_point.x;
        player->rect.y = level.spawn_point.y;
    }
}

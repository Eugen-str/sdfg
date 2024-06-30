#include "collision.h"
#include "map.h"
#include <stdio.h>

float absf(float x){
    if(x < 0) return -x;
    return x;
}

bool collision(Player *player, Object object){
    Rectangle pr = player->rect;
    Rectangle or = object.rect;
    switch(object.type){
        case OBJ_SLOPE:
            printf("OBJ_SLOPE NOT IMPLEMENTED!");
            break;
        case OBJ_WP: {
            if(pr.x + pr.width >= or.x &&
                    pr.x <= or.x + object.rect.width &&
                    pr.y + pr.height >= or.y &&
                    pr.y <= or.y + or.height){
                Vector2 pc = (Vector2){.x = pr.x + pr.width / 2, .y = pr.y + pr.height / 2};
                Vector2 oc = (Vector2){.x = or.x + or.width / 2, .y = or.y + or.height / 2};
                float x = or.width/48.0f, y = or.height/48.0f;
                float dx = pc.x * y - oc.x * y;
                float dy = pc.y * x - oc.y * x;
                if(absf(dy) < absf(dx)){
                    if(pr.x < or.x){
                        player->rect.x = object.rect.x - player->rect.width;
                    } else {
                        player->rect.x = object.rect.x + object.rect.width;
                    }
                } else {
                    if(pr.y < or.y){
                        player->rect.y = object.rect.y - player->size;
                        player->can_jump = true;
                    } else {
                        player->rect.y = object.rect.y + object.rect.height;
                    }
                    player->speed = 0.0f;
                }
                return true;
            }
        } break;
    }
    return false;
}

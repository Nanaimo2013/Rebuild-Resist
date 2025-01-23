#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "data/game_settings.h"

typedef struct Player {
    float x, y;
    float health;
    float max_health;
    float resources;
    float movement_speed;
    bool is_building_mode;
    bool is_moving;
    bool is_attacking;
    bool is_gathering;
    int money;
} Player;

void init_player(struct Player* player);
void update_player(struct Player* player);

#endif
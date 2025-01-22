#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
    float x, y;
    int health;
    int max_health;
    int resources;
    float movement_speed;
    bool is_building_mode;
    bool is_moving;
    bool is_attacking;
    bool is_gathering;
} Player;

void init_player(Player* player);
void update_player(Player* player);
void render_player(SDL_Renderer* renderer, const Player* player);

#endif
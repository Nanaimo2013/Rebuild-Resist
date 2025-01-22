#ifndef ENEMY_H
#define ENEMY_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "game.h"

// Forward declaration
struct GameState;

typedef enum {
    ENEMY_WALKING,
    ENEMY_ATTACKING,
    ENEMY_DYING
} EnemyState;

typedef struct {
    float x, y;
    float velocity_x, velocity_y;
    int health;
    int damage;
    float speed;
    EnemyState state;
    bool is_active;
    float attack_cooldown;
} Enemy;

void init_enemy_manager(void);
bool spawn_enemy(float x, float y);
void spawn_wave(int wave_number);
void update_enemies(GameState* state);
void render_enemies(SDL_Renderer* renderer);

#endif 
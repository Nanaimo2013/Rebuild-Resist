#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "game_types.h"
#include "data/enemy_settings.h"
#include "data/wave_settings.h"

struct EnemyManager {
    Enemy* enemies;
    int count;
    int capacity;
    float spawn_timer;
    int current_wave;
    bool wave_in_progress;
    int enemies_spawned;
    int enemies_remaining;
};

// Function declarations
void init_enemy_manager(struct EnemyManager* manager);
void update_enemies(struct GameState* state, struct EnemyManager* manager);
void cleanup_enemy_manager(struct EnemyManager* manager);

// Enemy spawning functions
void spawn_enemy(struct EnemyManager* manager, float x, float y, int type, int level);
void start_wave(struct GameState* state, struct EnemyManager* manager);
bool is_wave_complete(const struct EnemyManager* manager);

// Enemy state functions
void damage_enemy(Enemy* enemy, int damage);
bool is_enemy_alive(const Enemy* enemy);
float get_enemy_distance_to_target(const Enemy* enemy);

// Wave management functions
void set_wave_data(struct EnemyManager* manager, int wave_number);
int get_wave_enemy_count(int wave_number);
float get_wave_spawn_interval(int wave_number);

#endif 
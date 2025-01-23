#include "enemy_manager.h"
#include "game.h"
#include "data/game_settings.h"
#include <stdlib.h>
#include <math.h>

#define INITIAL_ENEMY_CAPACITY 100
#define MAX_ENEMIES 1000
#define BASE_SPAWN_INTERVAL 2.0f
#define SPAWN_DISTANCE 800.0f

void init_enemy_manager(struct EnemyManager* manager) {
    manager->enemies = (Enemy*)malloc(INITIAL_ENEMY_CAPACITY * sizeof(Enemy));
    manager->count = 0;
    manager->capacity = INITIAL_ENEMY_CAPACITY;
    manager->spawn_timer = 0;
    manager->current_wave = 0;
    manager->wave_in_progress = false;
    manager->enemies_spawned = 0;
    manager->enemies_remaining = 0;
}

void cleanup_enemy_manager(struct EnemyManager* manager) {
    free(manager->enemies);
    manager->enemies = NULL;
    manager->count = 0;
    manager->capacity = 0;
}

static void ensure_capacity(struct EnemyManager* manager) {
    if (manager->count >= manager->capacity) {
        int new_capacity = manager->capacity * 2;
        if (new_capacity > MAX_ENEMIES) new_capacity = MAX_ENEMIES;
        if (new_capacity == manager->capacity) return;

        Enemy* new_enemies = (Enemy*)realloc(manager->enemies, new_capacity * sizeof(Enemy));
        if (new_enemies) {
            manager->enemies = new_enemies;
            manager->capacity = new_capacity;
        }
    }
}

void spawn_enemy(struct EnemyManager* manager, float x, float y, int type, int level) {
    if (manager->count >= MAX_ENEMIES) return;
    
    ensure_capacity(manager);
    Enemy* enemy = &manager->enemies[manager->count++];
    
    // Initialize enemy properties
    enemy->x = x;
    enemy->y = y;
    enemy->target_x = x;
    enemy->target_y = y;
    enemy->velocity_x = 0;
    enemy->velocity_y = 0;
    enemy->type = type;
    enemy->level = level;
    enemy->is_active = true;
    enemy->state = ENEMY_WALKING;
    enemy->attack_cooldown = 0;
    
    // Scale stats based on type and level
    enemy->health = (50 + type * 25) * (1 + level * 0.5f);
    enemy->max_health = enemy->health;
    enemy->speed = (2.0f + type * 0.5f) * (1 + level * 0.2f);
    enemy->damage = (10 + type * 5) * (1 + level * 0.3f);
}

void update_enemies(struct GameState* state, struct EnemyManager* manager) {
    if (!state || !manager->wave_in_progress) return;

    // Update spawn timer and spawn new enemies
    manager->spawn_timer -= state->delta_time;
    if (manager->spawn_timer <= 0 && manager->enemies_spawned < manager->enemies_remaining) {
        // Spawn at random position around the map edges
        float angle = (float)(rand() % 360) * M_PI / 180.0f;
        float x = state->player->x + cosf(angle) * SPAWN_DISTANCE;
        float y = state->player->y + sinf(angle) * SPAWN_DISTANCE;
        
        spawn_enemy(manager, x, y, manager->current_wave % 3, manager->current_wave / 3);
        manager->enemies_spawned++;
        manager->spawn_timer = get_wave_spawn_interval(manager->current_wave);
    }

    // Update each enemy
    for (int i = 0; i < manager->count; i++) {
        Enemy* enemy = &manager->enemies[i];
        if (!enemy->is_active) continue;

        // Update attack cooldown
        if (enemy->attack_cooldown > 0) {
            enemy->attack_cooldown -= state->delta_time;
        }

        // State machine for enemy behavior
        switch (enemy->state) {
            case ENEMY_WALKING: {
                // Move towards player
                float dx = state->player->x - enemy->x;
                float dy = state->player->y - enemy->y;
                float dist = sqrtf(dx * dx + dy * dy);
                
                if (dist > 0) {
                    enemy->velocity_x = (dx / dist) * enemy->speed;
                    enemy->velocity_y = (dy / dist) * enemy->speed;
                }

                // Update position
                enemy->x += enemy->velocity_x * state->delta_time;
                enemy->y += enemy->velocity_y * state->delta_time;

                // Check if in attack range
                if (dist < GRID_SIZE) {
                    enemy->state = ENEMY_ATTACKING;
                }
                break;
            }
            case ENEMY_ATTACKING: {
                // Attack player if in range and cooldown is ready
                float dist = get_enemy_distance_to_target(enemy);
                if (dist < GRID_SIZE && enemy->attack_cooldown <= 0) {
                    state->player->health -= enemy->damage;
                    enemy->attack_cooldown = 1.0f; // Attack once per second
                } else if (dist >= GRID_SIZE) {
                    enemy->state = ENEMY_WALKING;
                }
                break;
            }
            case ENEMY_DYING: {
                enemy->is_active = false;
                manager->enemies_remaining--;
                break;
            }
        }
    }

    // Check if wave is complete
    if (is_wave_complete(manager)) {
        manager->wave_in_progress = false;
        state->mode = GAME_STATE_BUILDING;
    }
}

void damage_enemy(Enemy* enemy, int damage) {
    if (!enemy || !enemy->is_active) return;
    
    enemy->health -= damage;
    if (enemy->health <= 0) {
        enemy->state = ENEMY_DYING;
    }
}

bool is_enemy_alive(const Enemy* enemy) {
    return enemy && enemy->is_active && enemy->health > 0;
}

float get_enemy_distance_to_target(const Enemy* enemy) {
    if (!enemy) return INFINITY;
    
    float dx = enemy->target_x - enemy->x;
    float dy = enemy->target_y - enemy->y;
    return sqrtf(dx * dx + dy * dy);
}

void start_wave(struct GameState* state, struct EnemyManager* manager) {
    if (!state) return;
    
    manager->current_wave++;
    manager->wave_in_progress = true;
    manager->enemies_spawned = 0;
    manager->enemies_remaining = get_wave_enemy_count(manager->current_wave);
    manager->spawn_timer = 0;
    
    state->mode = GAME_STATE_WAVE;
    state->wave_start_time = state->elapsed_time;
}

bool is_wave_complete(const struct EnemyManager* manager) {
    if (!manager) return false;
    return manager->enemies_spawned >= manager->enemies_remaining && 
           manager->enemies_remaining <= 0;
}

void set_wave_data(struct EnemyManager* manager, int wave_number) {
    if (!manager) return;
    manager->current_wave = wave_number;
    manager->enemies_remaining = get_wave_enemy_count(wave_number);
}

int get_wave_enemy_count(int wave_number) {
    // Base enemies plus additional enemies per wave
    return 5 + wave_number * 3;
}

float get_wave_spawn_interval(int wave_number) {
    // Spawn interval decreases with wave number but has a minimum
    float interval = BASE_SPAWN_INTERVAL - (wave_number * 0.1f);
    return interval < 0.5f ? 0.5f : interval;
} 
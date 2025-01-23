#include <SDL3/SDL.h>
#include "game.h"     // Include game.h first to get full GameState definition
#include "enemy.h"
#include "math_utils.h"  // Add this include
#include "data/game_settings.h"
#include "data/enemy_settings.h"
#include "data/wave_settings.h"

#define ENEMY_SIZE 24
#define PI 3.14159265358979323846f

static struct {
    Enemy enemies[MAX_ENEMIES];
    int count;
    float spawn_timer;
    int wave_size;
} enemy_manager = {
    .count = 0,
    .spawn_timer = 0,
    .wave_size = 10
};

static WaveData current_wave = {0};

void init_enemy_manager(void) {
    enemy_manager.count = 0;
    enemy_manager.spawn_timer = 0;
    enemy_manager.wave_size = 10;
}

bool spawn_enemy(float x, float y) {
    if (enemy_manager.count >= MAX_ENEMIES) {
        return false;
    }

    Enemy* enemy = &enemy_manager.enemies[enemy_manager.count++];
    enemy->x = x;
    enemy->y = y;
    enemy->velocity_x = 0;
    enemy->velocity_y = 0;
    enemy->health = 100;
    enemy->damage = 10;
    enemy->speed = 1.5f;
    enemy->state = ENEMY_WALKING;
    enemy->is_active = true;
    enemy->attack_cooldown = 0;

    return true;
}

void spawn_wave(int wave_number, const WaveData* wave_data) {
    for (int i = 0; i < wave_data->spawn_count; i++) {
        WaveSpawn spawn = wave_data->spawns[i];
        
        // Calculate enemy stats based on level
        EnemyTypeData base_data = ENEMY_DATA[spawn.type];
        float level_multiplier = 1.0f + (spawn.level - 1) * 0.2f;
        
        for (int j = 0; j < spawn.count; j++) {
            // Calculate spawn position on grid using math_utils
            float angle = (float)(j * (2 * PI / spawn.count));
            int grid_x = (GRID_COLS / 2) + (int)(my_cosf(angle) * (GRID_COLS / 3));
            int grid_y = (GRID_ROWS / 2) + (int)(my_sinf(angle) * (GRID_ROWS / 3));
            
            Enemy* enemy = &enemy_manager.enemies[enemy_manager.count++];
            enemy->x = grid_x * GRID_SIZE;
            enemy->y = grid_y * GRID_SIZE;
            enemy->health = (int)(base_data.base_health * level_multiplier);
            enemy->damage = (int)(base_data.base_damage * level_multiplier);
            enemy->speed = base_data.base_speed * level_multiplier;
            enemy->type = spawn.type;
            enemy->level = spawn.level;
            enemy->state = ENEMY_WALKING;
            enemy->is_active = true;
        }
    }
}

static void update_enemy(Enemy* enemy, GameState* state) {
    if (!enemy->is_active) return;

    switch (enemy->state) {
        case ENEMY_WALKING: {
            // Move towards center/player
            float dx = (WINDOW_WIDTH / 2) - enemy->x;
            float dy = (WINDOW_HEIGHT / 2) - enemy->y;
            float distance = my_sqrtf(dx * dx + dy * dy);
            
            if (distance > 0) {
                enemy->velocity_x = (dx / distance) * enemy->speed;
                enemy->velocity_y = (dy / distance) * enemy->speed;
            }
            
            enemy->x += enemy->velocity_x;
            enemy->y += enemy->velocity_y;
            
            // Check for collision with buildings or player
            // TODO: Implement collision detection
            break;
        }
        
        case ENEMY_ATTACKING: {
            enemy->attack_cooldown -= state->delta_time;
            if (enemy->attack_cooldown <= 0) {
                // TODO: Deal damage to target
                enemy->attack_cooldown = 1.0f; // Attack once per second
            }
            break;
        }
        
        case ENEMY_DYING: {
            enemy->is_active = false;
            break;
        }
    }

    // Check health
    if (enemy->health <= 0) {
        enemy->state = ENEMY_DYING;
    }
}

void update_enemies(GameState* state) {
    // Spawn new wave at night
    if (state->time_of_day >= 18.0f || state->time_of_day <= 6.0f) {
        enemy_manager.spawn_timer += state->delta_time;
        if (enemy_manager.spawn_timer >= 5.0f) {
            const WaveData* wave = get_wave_data(state->wave_number - 1);
            if (wave != NULL) {
                spawn_wave(state->wave_number, wave);
            }
            enemy_manager.spawn_timer = 0;
        }
    }

    // Update all active enemies
    for (int i = 0; i < enemy_manager.count; i++) {
        update_enemy(&enemy_manager.enemies[i], state);
    }
}

void render_enemies(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    
    for (int i = 0; i < enemy_manager.count; i++) {
        Enemy* enemy = &enemy_manager.enemies[i];
        if (enemy->is_active) {
            SDL_FRect enemy_rect = {
                .x = enemy->x - ENEMY_SIZE/2,
                .y = enemy->y - ENEMY_SIZE/2,
                .w = ENEMY_SIZE,
                .h = ENEMY_SIZE
            };
            SDL_RenderFillRect(renderer, &enemy_rect);
        }
    }
}

#include <SDL3/SDL.h>
#include "game.h"     // Include game.h first to get full GameState definition
#include "enemy.h"
#include "math_utils.h"  // Add this include

#define MAX_ENEMIES 100
#define ENEMY_SIZE 24
#define ENEMY_SPAWN_DISTANCE 400
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

void spawn_wave(int wave_number) {
    int enemies_to_spawn = enemy_manager.wave_size + (wave_number * 2);
    
    for (int i = 0; i < enemies_to_spawn; i++) {
        // Spawn enemies around the map edges
        float angle = (float)(SDL_GetTicks() % 360) * (PI / 180.0f);
        float x = WINDOW_WIDTH / 2 + my_cosf(angle) * ENEMY_SPAWN_DISTANCE;
        float y = WINDOW_HEIGHT / 2 + my_sinf(angle) * ENEMY_SPAWN_DISTANCE;
        
        spawn_enemy(x, y);
    }
}

static void update_enemy(Enemy* enemy, GameState* state) {
    if (!enemy->is_active) return;

    switch (enemy->state) {
        case ENEMY_WALKING: {
            // Move towards center/player
            float dx = (WINDOW_WIDTH / 2) - enemy->x;
            float dy = (WINDOW_HEIGHT / 2) - enemy->y;
            float distance = SDL_sqrtf(dx * dx + dy * dy);
            
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
            spawn_wave(state->wave_number);
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

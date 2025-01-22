#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include "game.h"

#define MAX_SURVIVORS 50

typedef enum {
    SURVIVOR_IDLE,
    SURVIVOR_GATHERING,
    SURVIVOR_BUILDING,
    SURVIVOR_FIGHTING
} SurvivorState;

typedef struct {
    float x, y;
    int health;
    int max_health;
    float speed;
    SurvivorState state;
    float target_x, target_y;
    bool is_active;
} Survivor;

typedef struct {
    Survivor survivors[MAX_SURVIVORS];
    int count;
} SurvivorManager;

static SurvivorManager survivor_manager;

void init_survivor_manager(void) {
    survivor_manager.count = 0;
}

bool add_survivor(float x, float y) {
    if (survivor_manager.count >= MAX_SURVIVORS) {
        return false;
    }

    Survivor* survivor = &survivor_manager.survivors[survivor_manager.count++];
    survivor->x = x;
    survivor->y = y;
    survivor->health = 100;
    survivor->max_health = 100;
    survivor->speed = 2.0f;
    survivor->state = SURVIVOR_IDLE;
    survivor->is_active = true;
    return true;
}

static void update_survivor_ai(Survivor* survivor, const GameState* state) {
    float dx, dy, distance;  // Declare variables at start of function
    
    switch (survivor->state) {
        case SURVIVOR_IDLE:
            // Randomly choose new task
            if ((SDL_GetTicks() % 100) < 5) { // 5% chance to start gathering
                survivor->state = SURVIVOR_GATHERING;
                survivor->target_x = (float)(SDL_GetTicks() % WINDOW_WIDTH);
                survivor->target_y = (float)(SDL_GetTicks() % WINDOW_HEIGHT);
            }
            break;
            
        case SURVIVOR_GATHERING:
            // Move towards target
            dx = survivor->target_x - survivor->x;
            dy = survivor->target_y - survivor->y;
            distance = SDL_sqrtf(dx * dx + dy * dy);
            
            if (distance < 1.0f) {
                survivor->state = SURVIVOR_IDLE;
            } else {
                survivor->x += (dx / distance) * survivor->speed;
                survivor->y += (dy / distance) * survivor->speed;
            }
            break;
            
        default:
            break;
    }
}

void update_survivors(const GameState* state) {
    for (int i = 0; i < survivor_manager.count; i++) {
        Survivor* survivor = &survivor_manager.survivors[i];
        if (survivor->is_active) {
            update_survivor_ai(survivor, state);
        }
    }
}

void render_survivors(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    
    for (int i = 0; i < survivor_manager.count; i++) {
        Survivor* survivor = &survivor_manager.survivors[i];
        if (survivor->is_active) {
            SDL_FRect survivor_rect = {
                .x = survivor->x - TILE_SIZE/4,
                .y = survivor->y - TILE_SIZE/4,
                .w = TILE_SIZE/2,
                .h = TILE_SIZE/2
            };
            SDL_RenderFillRect(renderer, &survivor_rect);
        }
    }
}

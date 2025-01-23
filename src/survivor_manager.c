#include "survivor_manager.h"
#include "game.h"
#include "data/game_settings.h"
#include <stdlib.h>
#include <math.h>

#define INITIAL_SURVIVOR_CAPACITY 10
#define MAX_SURVIVORS 50
#define BASE_SURVIVOR_SPEED 3.0f
#define BASE_RESOURCE_GATHER_RATE 1.0f
#define SURVIVOR_HEALTH 100

// Change these lines at the top:
static struct SurvivorManager manager;

void init_survivor_manager(struct SurvivorManager* manager) {
    manager->survivors = (struct Survivor*)malloc(INITIAL_SURVIVOR_CAPACITY * sizeof(struct Survivor));
    manager->count = 0;
    manager->capacity = INITIAL_SURVIVOR_CAPACITY;
    manager->spawn_timer = 0;
    manager->max_survivors = MAX_SURVIVORS;
    manager->active_survivors = 0;
    manager->resource_gather_rate = BASE_RESOURCE_GATHER_RATE;
}

void cleanup_survivor_manager(void) {
    free(manager.survivors);
    manager.survivors = NULL;
    manager.count = 0;
    manager.capacity = 0;
}

static void ensure_capacity(void) {
    if (manager.count >= manager.capacity) {
        int new_capacity = manager.capacity * 2;
        if (new_capacity > MAX_SURVIVORS) new_capacity = MAX_SURVIVORS;
        if (new_capacity == manager.capacity) return;

        struct Survivor* new_survivors = (struct Survivor*)realloc(manager.survivors, new_capacity * sizeof(struct Survivor));
        if (new_survivors) {
            manager.survivors = new_survivors;
            manager.capacity = new_capacity;
        }
    }
}

bool add_survivor(float x, float y) {
    if (manager.count >= MAX_SURVIVORS) return false;
    
    ensure_capacity();
    struct Survivor* survivor = &manager.survivors[manager.count++];
    
    // Initialize survivor properties
    survivor->x = x;
    survivor->y = y;
    survivor->target_x = x;
    survivor->target_y = y;
    survivor->velocity_x = 0;
    survivor->velocity_y = 0;
    survivor->health = SURVIVOR_HEALTH;
    survivor->max_health = SURVIVOR_HEALTH;
    survivor->speed = BASE_SURVIVOR_SPEED;
    survivor->is_active = true;
    survivor->state = SURVIVOR_IDLE;
    survivor->action_timer = 0;
    survivor->assigned_building = -1;
    
    manager.active_survivors++;
    return true;
}

void remove_survivor(int index) {
    if (index < 0 || index >= manager.count) return;
    
    if (manager.survivors[index].is_active) {
        manager.active_survivors--;
    }
    
    // Move last survivor to this position if it's not the last one
    if (index < manager.count - 1) {
        manager.survivors[index] = manager.survivors[manager.count - 1];
    }
    manager.count--;
}

struct Survivor* get_survivor(int index) {
    if (index < 0 || index >= manager.count) return NULL;
    return &manager.survivors[index];
}

int get_survivor_count(void) {
    return manager.count;
}

void update_survivors(struct GameState* state) {
    if (!state) return;

    for (int i = 0; i < manager.count; i++) {
        struct Survivor* survivor = &manager.survivors[i];
        if (!survivor->is_active) continue;

        // Update action timer
        if (survivor->action_timer > 0) {
            survivor->action_timer -= state->delta_time;
        }

        // State machine for survivor behavior
        switch (survivor->state) {
            case SURVIVOR_IDLE: {
                // Maybe find something to do
                break;
            }
            case SURVIVOR_GATHERING: {
                // Gather resources if at target
                float dx = survivor->target_x - survivor->x;
                float dy = survivor->target_y - survivor->y;
                float dist = sqrtf(dx * dx + dy * dy);
                
                if (dist < GRID_SIZE) {
                    // Gather resources
                    update_resource_gathering(state);
                } else {
                    // Move towards target
                    survivor->velocity_x = (dx / dist) * survivor->speed;
                    survivor->velocity_y = (dy / dist) * survivor->speed;
                    survivor->x += survivor->velocity_x * state->delta_time;
                    survivor->y += survivor->velocity_y * state->delta_time;
                }
                break;
            }
            case SURVIVOR_BUILDING: {
                // Help with construction if at target
                break;
            }
            case SURVIVOR_FIGHTING: {
                // Attack nearby enemies
                break;
            }
            case SURVIVOR_FLEEING: {
                // Run from danger
                break;
            }
        }
    }
}

void assign_survivor_to_building(int survivor_index, int building_index) {
    struct Survivor* survivor = get_survivor(survivor_index);
    if (!survivor) return;
    
    survivor->assigned_building = building_index;
    survivor->state = SURVIVOR_BUILDING;
}

void set_survivor_target(int survivor_index, float x, float y) {
    struct Survivor* survivor = get_survivor(survivor_index);
    if (!survivor) return;
    
    survivor->target_x = x;
    survivor->target_y = y;
}

void set_survivor_state(int survivor_index, SurvivorState state) {
    struct Survivor* survivor = get_survivor(survivor_index);
    if (!survivor) return;
    
    survivor->state = state;
    survivor->action_timer = 0;  // Reset action timer on state change
}

float get_resource_generation_rate(void) {
    return manager.resource_gather_rate * manager.active_survivors;
}

void update_resource_gathering(struct GameState* state) {
    if (!state) return;
    
    // Add resources based on number of gathering survivors
    float resources = manager.resource_gather_rate * state->delta_time;
    // TODO: Add resources to state->resources based on what's being gathered
} 
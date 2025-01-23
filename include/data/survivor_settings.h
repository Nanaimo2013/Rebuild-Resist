#ifndef SURVIVOR_SETTINGS_H
#define SURVIVOR_SETTINGS_H

#include <SDL3/SDL.h>
#include <stdbool.h>

// Survivor constants
#define SURVIVOR_MAX_HEALTH 100
#define SURVIVOR_SPEED 3.0f
#define SURVIVOR_INITIAL_COUNT 5
#define SURVIVOR_MAX_COUNT 20
#define SURVIVOR_SPAWN_INTERVAL 30.0f
#define SURVIVOR_ACTION_INTERVAL 1.0f
#define SURVIVOR_RESOURCE_RATE 1.0f

// Forward declarations
struct Building;

typedef enum {
    SURVIVOR_IDLE,
    SURVIVOR_GATHERING,
    SURVIVOR_BUILDING,
    SURVIVOR_FIGHTING,
    SURVIVOR_FLEEING
} SurvivorState;

typedef struct Survivor {
    float x, y;
    float target_x, target_y;
    float velocity_x, velocity_y;
    int health;
    int max_health;
    float speed;
    bool is_active;
    SurvivorState state;
    float action_timer;
    int assigned_building;
} Survivor;

typedef struct SurvivorManager {
    Survivor* survivors;
    int count;
    int capacity;
    float spawn_timer;
    int max_survivors;
    int active_survivors;
    float resource_gather_rate;
} SurvivorManager;

#endif 
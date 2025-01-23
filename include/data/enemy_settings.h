#ifndef ENEMY_SETTINGS_H
#define ENEMY_SETTINGS_H

#include <SDL3/SDL.h>
#include <stdbool.h>

// Enemy constants
#define MAX_ENEMIES 1000
#define ENEMY_BASE_SIZE (GRID_SIZE - 8)
#define ENEMY_SPAWN_DISTANCE (GRID_SIZE * 15)

// Enemy States
typedef enum {
    ENEMY_WALKING,
    ENEMY_ATTACKING,
    ENEMY_DYING
} EnemyState;

// Enemy types
typedef enum {
    ENEMY_ZOMBIE,
    ENEMY_RUNNER,
    ENEMY_TANK,
    ENEMY_BOSS,
    ENEMY_TYPE_COUNT
} EnemyType;

// Enemy instance data
typedef struct {
    float x, y;
    float target_x, target_y;
    float velocity_x, velocity_y;
    int health;
    int max_health;
    float speed;
    int damage;
    bool is_active;
    int type;
    int level;
    EnemyState state;
    float attack_cooldown;
} Enemy;

// Enemy manager data
typedef struct {
    Enemy* enemies;
    int count;
    int capacity;
    float spawn_timer;
    int current_wave;
    bool wave_in_progress;
    int enemies_spawned;
    int enemies_remaining;
} EnemyManager;

// Enemy type data
typedef struct {
    const char* name;
    const char* description;
    int base_health;
    int base_damage;
    float base_speed;
    int resource_reward;
    float size_multiplier;
    const char* sprite_name;
    int spawn_sound_id;
    int attack_sound_id;
    int death_sound_id;
    float attack_range;
    float detection_range;
} EnemyTypeData;

// Make enemy data accessible
extern const EnemyTypeData ENEMY_DATA[ENEMY_TYPE_COUNT];

// Helper functions
const char* get_enemy_name(EnemyType type);
const char* get_enemy_description(EnemyType type);
const EnemyTypeData* get_enemy_data(EnemyType type);

#endif 
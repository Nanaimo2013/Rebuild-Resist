#include "data/enemy_settings.h"

const EnemyTypeData ENEMY_DATA[ENEMY_TYPE_COUNT] = {
    [ENEMY_ZOMBIE] = {
        .name = "Zombie",
        .base_health = 100,
        .base_damage = 10,
        .base_speed = 1.0f,
        .resource_reward = 10,
        .size_multiplier = 1.0f,
        .description = "Basic enemy type"
    },
    [ENEMY_RUNNER] = {
        .name = "Runner",
        .base_health = 50,
        .base_damage = 5,
        .base_speed = 2.0f,
        .resource_reward = 5,
        .size_multiplier = 0.8f,
        .description = "Fast but weak enemy"
    },
    [ENEMY_TANK] = {
        .name = "Tank",
        .base_health = 200,
        .base_damage = 15,
        .base_speed = 0.5f,
        .resource_reward = 20,
        .size_multiplier = 1.5f,
        .description = "Slow but tough enemy"
    },
    [ENEMY_BOSS] = {
        .name = "Boss",
        .base_health = 500,
        .base_damage = 30,
        .base_speed = 0.7f,
        .resource_reward = 100,
        .size_multiplier = 2.0f,
        .description = "Powerful boss enemy"
    }
};

const char* get_enemy_name(EnemyType type) {
    if (type >= 0 && type < ENEMY_TYPE_COUNT) {
        return ENEMY_DATA[type].name;
    }
    return "Unknown Enemy";
}

const char* get_enemy_description(EnemyType type) {
    if (type >= 0 && type < ENEMY_TYPE_COUNT) {
        return ENEMY_DATA[type].description;
    }
    return "No description available";
}

const EnemyTypeData* get_enemy_data(EnemyType type) {
    if (type >= 0 && type < ENEMY_TYPE_COUNT) {
        return &ENEMY_DATA[type];
    }
    return NULL;
} 
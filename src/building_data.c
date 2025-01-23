#include "data/building_settings.h"
#include "data/game_settings.h"

// Define the building data array
const BuildingData BUILDING_DATA[BUILDING_TYPE_COUNT] = {
    [WALL] = {
        .name = "Wall",
        .description = "Basic defense structure",
        .wood_cost = 10,
        .stone_cost = 5,
        .metal_cost = 0,
        .build_time = 1,
        .max_health = 100,
        .health = 100,
        .defense = 10,
        .attack_speed = 0.0f,
        .attack_damage = 0,
        .attack_range = 0,
        .sprite_name = "sprites/buildings/wall.png",
        .build_sound_id = 0,
        .damage_sound_id = 1,
        .destroy_sound_id = 2,
        .cost = 100,
        .resource_generation = 0
    },
    [TURRET] = {
        .name = "Turret",
        .description = "Automated defense tower",
        .wood_cost = 20,
        .stone_cost = 10,
        .metal_cost = 15,
        .build_time = 3,
        .max_health = 150,
        .health = 150,
        .defense = 5,
        .attack_speed = 1.0f,
        .attack_damage = 25,
        .attack_range = 128,
        .sprite_name = "sprites/buildings/turret.png",
        .build_sound_id = 3,
        .damage_sound_id = 4,
        .destroy_sound_id = 5,
        .cost = 250,
        .resource_generation = 0
    },
    [HOUSE] = {
        .name = "House",
        .description = "Provides shelter for survivors",
        .wood_cost = 30,
        .stone_cost = 15,
        .metal_cost = 5,
        .build_time = 5,
        .max_health = 200,
        .health = 200,
        .defense = 3,
        .attack_speed = 0.0f,
        .attack_damage = 0,
        .attack_range = 0,
        .sprite_name = "sprites/buildings/house.png",
        .build_sound_id = 6,
        .damage_sound_id = 7,
        .destroy_sound_id = 8,
        .cost = 300,
        .resource_generation = 1
    },
    [FARM] = {
        .name = "Farm",
        .description = "Produces food over time",
        .wood_cost = 25,
        .stone_cost = 5,
        .metal_cost = 0,
        .build_time = 4,
        .max_health = 100,
        .health = 100,
        .defense = 1,
        .attack_speed = 0.0f,
        .attack_damage = 0,
        .attack_range = 0,
        .sprite_name = "sprites/buildings/farm.png",
        .build_sound_id = 9,
        .damage_sound_id = 10,
        .destroy_sound_id = 11,
        .cost = 200,
        .resource_generation = 2
    },
    [STORAGE] = {
        .name = "Storage",
        .description = "Increases resource storage capacity",
        .wood_cost = 40,
        .stone_cost = 20,
        .metal_cost = 10,
        .build_time = 6,
        .max_health = 300,
        .health = 300,
        .defense = 5,
        .attack_speed = 0.0f,
        .attack_damage = 0,
        .attack_range = 0,
        .sprite_name = "sprites/buildings/storage.png",
        .build_sound_id = 12,
        .damage_sound_id = 13,
        .destroy_sound_id = 14,
        .cost = 150,
        .resource_generation = 0
    }
};

// Helper function implementations
const char* get_building_name(BuildingType type) {
    if (type >= 0 && type < BUILDING_TYPE_COUNT) {
        return BUILDING_DATA[type].name;
    }
    return "Unknown Building";
}

const char* get_building_description(BuildingType type) {
    if (type >= 0 && type < BUILDING_TYPE_COUNT) {
        return BUILDING_DATA[type].description;
    }
    return "No description available";
}

int get_building_cost(BuildingType type, int resource_type) {
    if (type >= 0 && type < BUILDING_TYPE_COUNT) {
        switch (resource_type) {
            case 0: return BUILDING_DATA[type].wood_cost;
            case 1: return BUILDING_DATA[type].stone_cost;
            case 2: return BUILDING_DATA[type].metal_cost;
            default: return 0;
        }
    }
    return 0;
}

const BuildingData* get_building_data(BuildingType type) {
    if (type >= 0 && type < BUILDING_TYPE_COUNT) {
        return &BUILDING_DATA[type];
    }
    return NULL;
} 
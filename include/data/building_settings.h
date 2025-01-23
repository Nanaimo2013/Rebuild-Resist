#ifndef BUILDING_SETTINGS_H
#define BUILDING_SETTINGS_H

#include <SDL3/SDL.h>
#include <stdbool.h>

// Building Types
typedef enum {
    WALL,
    TURRET,
    HOUSE,
    FARM,
    STORAGE,
    BUILDING_TYPE_COUNT
} BuildingType;

// Building Constants
#define BUILDING_MAX_HEALTH 100

// Building instance data
typedef struct {
    int x, y;
    int z;
    int grid_x, grid_y;
    int health;
    int max_health;
    BuildingType type;
    bool is_operational;
} Building;

// Building manager data
typedef struct {
    Building* buildings;
    int building_count;
    int capacity;
} BuildingManager;

// Building Data Structure
typedef struct BuildingData {
    const char* name;
    const char* description;
    int wood_cost;
    int stone_cost;
    int metal_cost;
    int build_time;
    int max_health;
    int health;
    int defense;
    float attack_speed;
    int attack_damage;
    int attack_range;
    const char* sprite_name;
    int build_sound_id;
    int damage_sound_id;
    int destroy_sound_id;
    int cost;
    int resource_generation;
} BuildingData;

// Building Costs
static const int BUILDING_COSTS[BUILDING_TYPE_COUNT] = {
    [WALL] = 100,      // Wall cost
    [TURRET] = 250,    // Turret cost
    [HOUSE] = 300,     // House cost
    [FARM] = 200,      // Farm cost
    [STORAGE] = 150    // Storage cost
};

// Building Data Array
extern const BuildingData BUILDING_DATA[BUILDING_TYPE_COUNT];

// Helper functions
const char* get_building_name(BuildingType type);
const char* get_building_description(BuildingType type);
int get_building_cost(BuildingType type, int resource_type);
const BuildingData* get_building_data(BuildingType type);

#endif 
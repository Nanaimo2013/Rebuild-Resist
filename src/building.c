#include "building.h"
#include "resource_manager.h"
#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game.h"
#include "data/game_settings.h"

#define INITIAL_BUILDING_CAPACITY 100

void init_building_manager(BuildingManager* manager) {
    manager->capacity = INITIAL_BUILDING_CAPACITY;
    manager->building_count = 0;
    manager->buildings = malloc(sizeof(Building) * manager->capacity);
}

bool add_building(BuildingManager* manager, int grid_x, int grid_y, BuildingType type) {
    // Check if we have enough resources
    const BuildingData* building_data = &BUILDING_DATA[type];
    if (!can_afford(building_data->wood_cost, building_data->stone_cost, building_data->metal_cost, 0)) {
        return false;
    }
    
    // Check if grid position is valid and empty
    if (grid_x < 0 || grid_x >= GRID_COLS || grid_y < 0 || grid_y >= GRID_ROWS) {
        return false;
    }
    
    // Check if space is occupied
    for (int i = 0; i < manager->building_count; i++) {
        Building* b = &manager->buildings[i];
        if (b->grid_x == grid_x && b->grid_y == grid_y) {
            return false;
        }
    }
    
    // Add the building
    Building* building = &manager->buildings[manager->building_count++];
    building->grid_x = grid_x;
    building->grid_y = grid_y;
    building->x = grid_x * GRID_SIZE;
    building->y = grid_y * GRID_SIZE;
    building->type = type;
    building->health = building_data->health;
    building->is_operational = true;
    
    // Spend resources
    spend_resources(building_data->wood_cost, building_data->stone_cost, building_data->metal_cost, 0);
    
    return true;
}

void update_buildings(BuildingManager* manager) {
    for (int i = 0; i < manager->building_count; i++) {
        Building* building = &manager->buildings[i];
        if (building->health <= 0) {
            building->is_operational = false;
        }
    }
}

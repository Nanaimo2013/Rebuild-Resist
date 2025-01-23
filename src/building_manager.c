#include "building_manager.h"
#include <stdlib.h>

#define INITIAL_CAPACITY 10

void init_building_manager(BuildingManager* manager) {
    manager->buildings = malloc(INITIAL_CAPACITY * sizeof(Building));
    manager->building_count = 0;
    manager->capacity = INITIAL_CAPACITY;
}

bool add_building(BuildingManager* manager, int grid_x, int grid_y, BuildingType type) {
    if (manager->building_count >= manager->capacity) {
        int new_capacity = manager->capacity * 2;
        Building* new_buildings = realloc(manager->buildings, new_capacity * sizeof(Building));
        if (!new_buildings) return false;
        manager->buildings = new_buildings;
        manager->capacity = new_capacity;
    }

    Building* building = &manager->buildings[manager->building_count++];
    building->grid_x = grid_x;
    building->grid_y = grid_y;
    building->x = grid_x * GRID_SIZE;
    building->y = grid_y * GRID_SIZE;
    building->z = 0;
    building->type = type;
    building->health = BUILDING_MAX_HEALTH;
    building->max_health = BUILDING_MAX_HEALTH;
    building->is_operational = true;

    return true;
}

void update_buildings(BuildingManager* manager) {
    for (int i = 0; i < manager->building_count; i++) {
        Building* building = &manager->buildings[i];
        // Update building logic here
    }
}

void cleanup_building_manager(BuildingManager* manager) {
    free(manager->buildings);
    manager->buildings = NULL;
    manager->building_count = 0;
    manager->capacity = 0;
} 
#ifndef BUILDING_MANAGER_H
#define BUILDING_MANAGER_H

#include <stdbool.h>
#include "building.h"
#include "data/game_settings.h"    // Add for GRID_SIZE
#include "data/building_settings.h" // Add for BUILDING_MAX_HEALTH

struct BuildingManager {
    Building* buildings;
    int building_count;
    int capacity;
};

// Function declarations
void init_building_manager(struct BuildingManager* manager);
bool add_building(struct BuildingManager* manager, int grid_x, int grid_y, BuildingType type);
void update_buildings(struct BuildingManager* manager);
void cleanup_building_manager(struct BuildingManager* manager);

#endif 
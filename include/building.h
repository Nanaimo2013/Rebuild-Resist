#ifndef BUILDING_H
#define BUILDING_H

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef enum {
    WALL,
    TURRET,
    HOUSE,
    FARM,
    STORAGE
} BuildingType;

typedef struct {
    int x, y;
    int health;
    BuildingType type;
    bool is_operational;
} Building;

typedef struct {
    Building* buildings;
    int count;
    int capacity;
} BuildingManager;

void init_building_manager(BuildingManager* manager);
bool add_building(BuildingManager* manager, int x, int y, BuildingType type);
void update_buildings(BuildingManager* manager);
void render_buildings(SDL_Renderer* renderer, BuildingManager* manager);

#endif 
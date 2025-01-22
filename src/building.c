#include "building.h"
#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game.h"

#define INITIAL_BUILDING_CAPACITY 100

void init_building_manager(BuildingManager* manager) {
    manager->capacity = INITIAL_BUILDING_CAPACITY;
    manager->count = 0;
    manager->buildings = malloc(sizeof(Building) * manager->capacity);
}

bool add_building(BuildingManager* manager, int x, int y, BuildingType type) {
    if (manager->count >= manager->capacity) {
        int new_capacity = manager->capacity * 2;
        Building* new_buildings = realloc(manager->buildings, 
            sizeof(Building) * new_capacity);
        
        if (!new_buildings) return false;
        
        manager->buildings = new_buildings;
        manager->capacity = new_capacity;
    }

    Building* building = &manager->buildings[manager->count++];
    building->x = x;
    building->y = y;
    building->type = type;
    building->health = 100;
    building->is_operational = true;

    return true;
}

void update_buildings(BuildingManager* manager) {
    for (int i = 0; i < manager->count; i++) {
        Building* building = &manager->buildings[i];
        if (building->health <= 0) {
            building->is_operational = false;
        }
    }
}

void render_buildings(SDL_Renderer* renderer, BuildingManager* manager) {
    for (int i = 0; i < manager->count; i++) {
        Building* building = &manager->buildings[i];
        SDL_FRect building_rect = {
            .x = (float)building->x,
            .y = (float)building->y,
            .w = (float)TILE_SIZE,
            .h = (float)TILE_SIZE
        };

        // Different colors for different building types
        switch (building->type) {
            case WALL:
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
                break;
            case TURRET:
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                break;
            case HOUSE:
                SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
                break;
            case FARM:
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                break;
            case STORAGE:
                SDL_SetRenderDrawColor(renderer, 218, 165, 32, 255);
                break;
        }

        SDL_RenderFillRect(renderer, &building_rect);
    }
}

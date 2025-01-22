#include "resource_manager.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
    Resources resources;
    int max_storage;
} ResourceManagerState;

static ResourceManagerState resource_manager = {
    .resources = {0, 0, 0, 0},
    .max_storage = 1000
};

void init_resource_manager(void) {
    resource_manager.resources.wood = 100;
    resource_manager.resources.stone = 50;
    resource_manager.resources.food = 200;
    resource_manager.resources.metal = 25;
}

bool can_afford(int wood, int stone, int food, int metal) {
    return resource_manager.resources.wood >= wood &&
           resource_manager.resources.stone >= stone &&
           resource_manager.resources.food >= food &&
           resource_manager.resources.metal >= metal;
}

bool spend_resources(int wood, int stone, int food, int metal) {
    if (!can_afford(wood, stone, food, metal)) {
        return false;
    }
    
    resource_manager.resources.wood -= wood;
    resource_manager.resources.stone -= stone;
    resource_manager.resources.food -= food;
    resource_manager.resources.metal -= metal;
    return true;
}

void add_resources(int wood, int stone, int food, int metal) {
    resource_manager.resources.wood = SDL_min(
        resource_manager.resources.wood + wood, 
        resource_manager.max_storage
    );
    resource_manager.resources.stone = SDL_min(
        resource_manager.resources.stone + stone, 
        resource_manager.max_storage
    );
    resource_manager.resources.food = SDL_min(
        resource_manager.resources.food + food, 
        resource_manager.max_storage
    );
    resource_manager.resources.metal = SDL_min(
        resource_manager.resources.metal + metal, 
        resource_manager.max_storage
    );
}

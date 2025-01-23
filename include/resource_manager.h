#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <stdbool.h>

typedef struct {
    int wood;
    int stone;
    int food;
    int metal;
} Resources;

typedef struct {
    Resources resources;
    int max_storage;
} ResourceManager;

void init_resource_manager(void);
bool can_afford(int wood, int stone, int metal, int food);
bool spend_resources(int wood, int stone, int metal, int food);
void add_resources(int wood, int stone, int metal, int food);

#endif 
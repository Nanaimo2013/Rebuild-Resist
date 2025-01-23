#ifndef RESOURCE_SETTINGS_H
#define RESOURCE_SETTINGS_H

// Resource Types
typedef enum {
    RESOURCE_WOOD,
    RESOURCE_STONE,
    RESOURCE_METAL,
    RESOURCE_FOOD,
    RESOURCE_TYPE_COUNT
} ResourceType;

// Resource Generation Rates (per tick)
#define WOOD_GENERATION_RATE 1
#define STONE_GENERATION_RATE 1
#define METAL_GENERATION_RATE 1
#define FOOD_GENERATION_RATE 1

// Storage Limits
#define BASE_STORAGE_CAPACITY 1000
#define STORAGE_UPGRADE_MULTIPLIER 1.5f

// Resource Collection
#define COLLECTION_RADIUS (GRID_SIZE * 2)
#define COLLECTION_RATE 1.0f  // Resources per second

#endif 
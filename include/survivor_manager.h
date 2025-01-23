#ifndef SURVIVOR_MANAGER_H
#define SURVIVOR_MANAGER_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "data/survivor_settings.h"  // This should be the only place defining Survivor

// Forward declarations
struct GameState;

// Core survivor manager functions
void init_survivor_manager(SurvivorManager* manager);
void cleanup_survivor_manager(SurvivorManager* manager);
void update_survivors(SurvivorManager* manager, struct GameState* state);

// Survivor spawning and management
bool add_survivor(SurvivorManager* manager, float x, float y);
void remove_survivor(SurvivorManager* manager, int index);
Survivor* get_survivor(SurvivorManager* manager, int index);
int get_survivor_count(SurvivorManager* manager);

// Survivor actions
void assign_survivor_to_building(SurvivorManager* manager, int survivor_index, int building_index);
void set_survivor_target(SurvivorManager* manager, int survivor_index, float x, float y);
void set_survivor_state(SurvivorManager* manager, int survivor_index, SurvivorState state);

// Resource gathering
float get_resource_generation_rate(SurvivorManager* manager);
void update_resource_gathering(SurvivorManager* manager, struct GameState* state);

#endif 
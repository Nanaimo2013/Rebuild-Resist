#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>
#include "game.h"
#include "building_manager.h"
#include "player.h"
#include "enemy.h"
#include "survivor.h"
#include "enemy_manager.h"
#include "survivor_manager.h"

// Forward declare GameState to avoid circular dependency
struct GameState;

typedef struct {
    float x, y;
    float zoom;
    float rotation;
} Camera;

// Core rendering functions
void init_renderer(void);
void render_game_world(SDL_Renderer* renderer, struct GameState* state, const Camera* camera);

// UI rendering functions
void render_ui(SDL_Renderer* renderer, struct GameState* state);
void render_resource_bar(SDL_Renderer* renderer, const struct GameState* state);
void render_building_menu(SDL_Renderer* renderer, const struct GameState* state);
void render_start_wave_button(SDL_Renderer* renderer, const struct GameState* state);
void render_wave_info(SDL_Renderer* renderer, const struct GameState* state);
void render_debug_info(SDL_Renderer* renderer, const struct GameState* state);

// World rendering functions
void render_world_grid(SDL_Renderer* renderer);
void render_buildings(SDL_Renderer* renderer, const struct BuildingManager* manager);
void render_enemies(SDL_Renderer* renderer, const struct EnemyManager* manager);
void render_player(SDL_Renderer* renderer, const struct Player* player);
void render_survivors(SDL_Renderer* renderer, const struct SurvivorManager* manager);

// Helper functions
SDL_FPoint world_to_screen(float world_x, float world_y, float world_z);

#endif
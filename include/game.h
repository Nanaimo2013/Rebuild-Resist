#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "building.h"
#include "player.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TILE_SIZE 32

#ifndef FRAME_TIME
#define FRAME_TIME (1.0f / 60.0f)  // Target 60 FPS
#endif

typedef struct GameState {
    float delta_time;
    bool is_running;
    int wave_number;
    float time_of_day;
    SDL_Window* window;
    SDL_Renderer* renderer;
    BuildingManager building_manager;
    Player player;
} GameState;

bool init_game(GameState* state);
void handle_input(GameState* state);
void update_game(GameState* state);
void render_game(GameState* state);
void cleanup_game(GameState* state);

#endif

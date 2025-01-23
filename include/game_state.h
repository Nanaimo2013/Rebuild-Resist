#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "camera.h"
#include "building_manager.h"
#include "enemy_manager.h"
#include "survivor_manager.h"
#include "player.h"

typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_BUILDING,
    GAME_STATE_WAVE,
    GAME_STATE_PAUSED
} GameMode;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    BuildingManager* building_manager;
    EnemyManager* enemy_manager;
    SurvivorManager* survivor_manager;
    Player* player;
    Camera* camera;
    bool is_running;
    bool is_paused;
    bool is_building_mode;
    bool is_wave_in_progress;
    float elapsed_time;
    float delta_time;
    float time_of_day;
    int wave_number;
    int selected_building_type;
    GameMode mode;
} GameState;

#endif 
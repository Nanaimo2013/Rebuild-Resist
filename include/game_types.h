#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_BUILDING,
    GAME_STATE_WAVE,
    GAME_STATE_PAUSED
} GameMode;

struct GameState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    struct BuildingManager* building_manager;
    struct EnemyManager* enemy_manager;
    struct SurvivorManager* survivor_manager;
    struct Player* player;
    struct Camera* camera;
    bool is_running;
    bool is_paused;
    bool is_building_mode;
    bool is_wave_in_progress;
    float elapsed_time;
    float delta_time;
    float time_of_day;
    float wave_start_time;
    int wave_number;
    int selected_building_type;
    GameMode mode;
};

#endif 
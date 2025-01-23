#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#include <SDL3/SDL.h>
#include <math.h>

// Window and Display Settings
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_FLAGS (SDL_WINDOW_RESIZABLE)

// Grid and Tile Settings
#define TILE_SIZE 32
#define GRID_SIZE TILE_SIZE
#define GRID_COLS 150
#define GRID_ROWS 150

// Math Constants
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// Game Time Settings
#define DAY_LENGTH 300.0f          // 5 minutes per day
#define NIGHT_START 18.0f          // 6 PM
#define NIGHT_END 6.0f             // 6 AM
#define WAVE_PREPARATION_TIME 60.0f // 1 minute between waves
#define RESOURCE_TICK_TIME 1.0f    // Resource generation interval

// Player Settings
#define PLAYER_START_MONEY 1000
#define PLAYER_START_HEALTH 100
#define PLAYER_MAX_HEALTH 100
#define PLAYER_MOVEMENT_SPEED 5.0f
#define PLAYER_SIZE (GRID_SIZE - 4)
#define PLAYER_ATTACK_RANGE (GRID_SIZE * 2)
#define PLAYER_ATTACK_DAMAGE 20
#define PLAYER_ATTACK_SPEED 0.5f   // Attacks per second

// Include other setting files
#include "building_settings.h"
#include "enemy_settings.h"
#include "resource_settings.h"
#include "wave_settings.h"

#endif
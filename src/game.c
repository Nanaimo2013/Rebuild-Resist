#include "game.h"
#include "player.h"
#include "building.h"
#include "resource_manager.h"
#include "enemy.h"
#include "renderer.h"  // Add this for render functions
#include "survivor.h"  // Add this for survivor functions
#include <stdio.h>
#include <SDL3/SDL.h>
#include "camera.h"
#include "building_manager.h"
#include "enemy_manager.h"
#include "survivor_manager.h"
#include "data/game_settings.h"

bool init_game(GameState* state) {
    if (!state) return false;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    state->window = SDL_CreateWindow("Rebuild & Resist",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE);

    if (!state->window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer
    state->renderer = SDL_CreateRenderer(state->window, NULL, 0);
    if (!state->renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(state->window);
        return false;
    }

    SDL_SetRenderDrawBlendMode(state->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderScale(state->renderer, 1.0f, 1.0f);

    // Allocate memory for managers
    state->building_manager = malloc(sizeof(BuildingManager));
    state->enemy_manager = malloc(sizeof(EnemyManager));
    state->survivor_manager = malloc(sizeof(SurvivorManager));
    state->player = malloc(sizeof(Player));
    state->camera = malloc(sizeof(Camera));

    if (!state->building_manager || !state->enemy_manager || !state->survivor_manager || 
        !state->player || !state->camera) {
        printf("Failed to allocate memory for managers\n");
        return false;
    }

    // Initialize subsystems
    init_resource_manager();
    init_building_manager(state->building_manager);
    init_player(state->player);
    init_enemy_manager(state->enemy_manager);
    init_survivor_manager(state->survivor_manager);
    init_camera(state->camera);

    // Initialize game state
    state->is_running = true;
    state->is_paused = false;
    state->is_building_mode = false;
    state->is_wave_in_progress = false;
    state->elapsed_time = 0.0f;
    state->delta_time = 1.0f / 60.0f;  // Target 60 FPS
    state->time_of_day = 0.0f;
    state->wave_number = 1;
    state->selected_building_type = -1;
    state->mode = GAME_STATE_BUILDING;

    return true;
}

void handle_input(GameState* state) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                state->is_running = false;
                break;
                
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                handle_mouse_click(state, event.button.x, event.button.y);
                break;
                
            case SDL_EVENT_KEY_DOWN:
                handle_keyboard(state, event.key.scancode);
                break;
                
            case SDL_EVENT_MOUSE_MOTION:
                // Handle mouse movement for UI hover effects, etc.
                break;
                
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
                // Handle window resize
                int w, h;
                SDL_GetWindowSize(state->window, &w, &h);
                break;
            }
        }
    }
}

void handle_mouse_click(GameState* state, int x, int y) {
    // Check if click is in building menu
    if (x > WINDOW_WIDTH - 200 && y > 40) {
        handle_menu_click(state, x, y);
        return;
    }
    
    // Check if click is on start button
    if (state->mode == GAME_STATE_BUILDING &&
        x > WINDOW_WIDTH - 190 && 
        x < WINDOW_WIDTH - 10 &&
        y > WINDOW_HEIGHT - 70 &&
        y < WINDOW_HEIGHT - 10) {
        start_wave(state);
        return;
    }
    
    // Handle building placement
    if (state->mode == GAME_STATE_BUILDING && state->selected_building_type != -1) {
        int grid_x = (x / GRID_SIZE) * GRID_SIZE;
        int grid_y = (y / GRID_SIZE) * GRID_SIZE;
        add_building(&state->building_manager, grid_x, grid_y, state->selected_building_type);
    }
}

void update_game(GameState* state) {
    // Update time of day (24-hour cycle)
    state->time_of_day += state->delta_time * 0.1f;  // Adjust speed as needed
    if (state->time_of_day >= 24.0f) {
        state->time_of_day = 0.0f;
        state->wave_number++;
    }

    // Update game systems
    update_player(state->player);  // This now handles all player input
    update_buildings(state->building_manager);
    update_enemies(state, state->enemy_manager);
    
    // Resource generation (example)
    if (state->time_of_day >= 6.0f && state->time_of_day <= 18.0f) {
        // Generate resources during day time
        if ((SDL_GetTicks() % 1000) < 16) {  // About once per second
            add_resources(1, 0, 1, 0);  // Add some wood and food
        }
    }

    state->elapsed_time += state->delta_time;  // Increment elapsed time
}

void render_game(GameState* state) {
    // Clear screen with background color
    SDL_SetRenderDrawColor(state->renderer, 20, 20, 20, 255);
    SDL_RenderClear(state->renderer);
    
    // Render world grid first
    render_world_grid(state->renderer);
    
    // Render game objects in proper order
    render_buildings(state->renderer, state->building_manager);
    render_enemies(state->renderer, state->enemy_manager);
    render_survivors(state->renderer, state->survivor_manager);
    render_player(state->renderer, state->player);
    
    // Render UI on top
    render_ui(state->renderer, state);
    
    // Present the rendered frame
    SDL_RenderPresent(state->renderer);
}

void cleanup_game(GameState* state) {
    if (!state) return;

    // Cleanup subsystems
    cleanup_building_manager(state->building_manager);
    cleanup_enemy_manager(state->enemy_manager);
    cleanup_survivor_manager(state->survivor_manager);

    // Free allocated memory
    free(state->building_manager);
    free(state->enemy_manager);
    free(state->survivor_manager);
    free(state->player);
    free(state->camera);

    // Cleanup SDL
    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
    SDL_Quit();
}

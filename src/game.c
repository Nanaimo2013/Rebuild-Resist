#include "game.h"
#include "player.h"
#include "building.h"
#include "resource_manager.h"
#include "enemy.h"
#include "renderer.h"  // Add this for render functions
#include "survivor.h"  // Add this for survivor functions
#include <stdio.h>
#include <SDL3/SDL.h>

bool init_game(GameState* state) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return false;
    }

    // Create window with proper flags
    state->window = SDL_CreateWindow(
        "Rebuild & Resist",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    
    if (!state->window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer with proper flags for SDL3
    state->renderer = SDL_CreateRenderer(
        state->window,
        NULL  // Use first available driver
    );
    
    if (!state->renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(state->window);
        return false;
    }

    // Set render scale quality
    SDL_SetRenderScale(state->renderer, 1.0f, 1.0f);

    // Initialize subsystems
    init_resource_manager();
    init_building_manager(&state->building_manager);
    init_player(&state->player);
    init_enemy_manager();
    init_renderer();  // Make sure this is called after renderer creation

    state->is_running = true;
    state->wave_number = 1;
    state->time_of_day = 0.0f;
    state->delta_time = 0.016f; // ~60 FPS

    return true;
}

void handle_input(GameState* state) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                state->is_running = false;
                break;
                
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        state->is_running = false;
                        break;
                        
                    case SDL_SCANCODE_F:  // Toggle fullscreen
                        {
                            uint32_t flags = SDL_GetWindowFlags(state->window);
                            SDL_SetWindowFullscreen(state->window, 
                                (flags & SDL_WINDOW_FULLSCREEN) ? 0 : SDL_WINDOW_FULLSCREEN);
                        }
                        break;
                }
                break;
                
            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                // Handle mouse clicks for building placement, etc.
                if (state->player.is_building_mode) {
                    int mouse_x, mouse_y;
                    uint32_t buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
                    (void)buttons; // Suppress unused variable warning
                    // Snap to grid
                    int grid_x = (mouse_x / TILE_SIZE) * TILE_SIZE;
                    int grid_y = (mouse_y / TILE_SIZE) * TILE_SIZE;
                    add_building(&state->building_manager, grid_x, grid_y, WALL);
                }
                break;
            }
                
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

void update_game(GameState* state) {
    // Update time of day (24-hour cycle)
    state->time_of_day += state->delta_time * 0.1f;  // Adjust speed as needed
    if (state->time_of_day >= 24.0f) {
        state->time_of_day = 0.0f;
        state->wave_number++;
    }

    // Update game systems
    update_player(&state->player);  // This now handles all player input
    update_buildings(&state->building_manager);
    update_enemies(state);
    
    // Resource generation (example)
    if (state->time_of_day >= 6.0f && state->time_of_day <= 18.0f) {
        // Generate resources during day time
        if ((SDL_GetTicks() % 1000) < 16) {  // About once per second
            add_resources(1, 0, 1, 0);  // Add some wood and food
        }
    }
}

void render_game(GameState* state) {
    // Clear screen with background color
    SDL_SetRenderDrawColor(state->renderer, 20, 20, 20, 255);
    SDL_RenderClear(state->renderer);
    
    // Render world grid first
    render_world_grid(state->renderer);
    
    // Render game objects in proper order
    render_buildings(state->renderer, &state->building_manager);
    render_enemies(state->renderer);
    render_survivors(state->renderer);
    render_player(state->renderer, &state->player);
    
    // Render UI on top
    render_ui(state->renderer, state);
    
    // Present the rendered frame
    SDL_RenderPresent(state->renderer);
}

void cleanup_game(GameState* state) {
    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
    SDL_Quit();
}

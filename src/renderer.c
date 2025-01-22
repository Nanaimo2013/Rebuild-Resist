#include <SDL3/SDL.h>
#include "game.h"
#include "building.h"
#include "player.h"

typedef struct {
    SDL_Color background_color;
    SDL_Color ui_color;
    float camera_x;
    float camera_y;
    float zoom;
} RenderState;

static RenderState render_state = {
    .background_color = {20, 20, 20, 255},
    .ui_color = {200, 200, 200, 255},
    .camera_x = 0,
    .camera_y = 0,
    .zoom = 1.0f
};

void init_renderer(void) {
    render_state.camera_x = 0;
    render_state.camera_y = 0;
    render_state.zoom = 1.0f;
}

SDL_FPoint world_to_screen(float world_x, float world_y) {
    SDL_FPoint screen_point = {
        (world_x - render_state.camera_x) * render_state.zoom,
        (world_y - render_state.camera_y) * render_state.zoom
    };
    return screen_point;
}

void render_ui(SDL_Renderer* renderer, const GameState* state) {
    // Render resource counts
    char buffer[128];
    SDL_FRect text_rect = {10, 10, 200, 20};
    
    // TODO: Add proper text rendering
    // For now, we'll just draw rectangles as placeholders
    SDL_SetRenderDrawColor(renderer, 
        render_state.ui_color.r, 
        render_state.ui_color.g,
        render_state.ui_color.b,
        render_state.ui_color.a);
    
    SDL_RenderFillRect(renderer, &text_rect);
}

void render_world_grid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    
    // Draw vertical lines
    for (int x = 0; x < WINDOW_WIDTH; x += TILE_SIZE) {
        SDL_RenderLine(renderer, 
            x, 0,
            x, WINDOW_HEIGHT);
    }
    
    // Draw horizontal lines
    for (int y = 0; y < WINDOW_HEIGHT; y += TILE_SIZE) {
        SDL_RenderLine(renderer,
            0, y,
            WINDOW_WIDTH, y);
    }
}

void render_game_world(SDL_Renderer* renderer, const GameState* state) {
    // Clear screen with background color
    SDL_SetRenderDrawColor(renderer, 
        render_state.background_color.r,
        render_state.background_color.g,
        render_state.background_color.b,
        render_state.background_color.a);
    SDL_RenderClear(renderer);
    
    // Render grid
    render_world_grid(renderer);
    
    // Render game objects
    render_buildings(renderer, (BuildingManager*)&state->building_manager);
    render_player(renderer, (Player*)&state->player);
    
    // Render UI elements on top
    render_ui(renderer, state);
    
    SDL_RenderPresent(renderer);
}

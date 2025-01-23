#include <SDL3/SDL.h>
#include "data/game_settings.h"
#include "building.h"
#include "player.h"
#include "enemy.h"
#include "survivor.h"
#include "survivor_manager.h"
#include "renderer.h"
#include "game.h"
#include "building_manager.h"

// Constants for UI colors
static const SDL_Color COLOR_BACKGROUND = {20, 20, 20, 255};
static const SDL_Color COLOR_UI_BG = {30, 30, 30, 255};
static const SDL_Color COLOR_UI_HOVER = {60, 60, 60, 255};
static const SDL_Color COLOR_UI_NORMAL = {45, 45, 45, 255};
static const SDL_Color COLOR_TEXT = {255, 255, 255, 255};
static const SDL_Color COLOR_GRID = {40, 40, 40, 255};
static const SDL_Color COLOR_BUTTON_GREEN = {0, 180, 0, 255};
static const SDL_Color COLOR_BUTTON_RED = {180, 0, 0, 255};

typedef struct {
    SDL_Color background_color;
    SDL_Color ui_color;
    Camera camera;
} RenderState;

// Initialize render state with default values
static RenderState render_state;

void init_renderer(void) {
    render_state.background_color = COLOR_BACKGROUND;
    render_state.ui_color = COLOR_UI_BG;
    render_state.camera.x = 0;
    render_state.camera.y = 0;
    render_state.camera.zoom = 1.0f;
    render_state.camera.rotation = 0;
}

SDL_FPoint world_to_screen(float world_x, float world_y, float world_z) {
    SDL_FPoint screen_point = {
        (world_x - render_state.camera.x) * render_state.camera.zoom,
        (world_y - render_state.camera.y) * render_state.camera.zoom - world_z
    };
    return screen_point;
}

void render_world_grid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, COLOR_GRID.r, COLOR_GRID.g, COLOR_GRID.b, COLOR_GRID.a);
    
    // Calculate visible grid range based on camera position
    int start_x = (int)render_state.camera.x / GRID_SIZE;
    int start_y = (int)render_state.camera.y / GRID_SIZE;
    int end_x = start_x + (WINDOW_WIDTH / GRID_SIZE) + 2;
    int end_y = start_y + (WINDOW_HEIGHT / GRID_SIZE) + 2;
    
    // Clamp to grid boundaries
    start_x = (start_x < 0) ? 0 : start_x;
    start_y = (start_y < 0) ? 0 : start_y;
    end_x = (end_x > GRID_COLS) ? GRID_COLS : end_x;
    end_y = (end_y > GRID_ROWS) ? GRID_ROWS : end_y;
    
    // Draw vertical lines
    for (int x = start_x; x <= end_x; x++) {
        SDL_FPoint start = world_to_screen(x * GRID_SIZE, start_y * GRID_SIZE, 0);
        SDL_FPoint end = world_to_screen(x * GRID_SIZE, end_y * GRID_SIZE, 0);
        SDL_RenderLine(renderer, start.x, start.y, end.x, end.y);
    }
    
    // Draw horizontal lines
    for (int y = start_y; y <= end_y; y++) {
        SDL_FPoint start = world_to_screen(start_x * GRID_SIZE, y * GRID_SIZE, 0);
        SDL_FPoint end = world_to_screen(end_x * GRID_SIZE, y * GRID_SIZE, 0);
        SDL_RenderLine(renderer, start.x, start.y, end.x, end.y);
    }
}

void render_resource_bar(SDL_Renderer* renderer, const struct GameState* state) {
    // Top bar background
    SDL_FRect bar_rect = {0, 0, WINDOW_WIDTH, 40};
    SDL_SetRenderDrawColor(renderer, COLOR_UI_BG.r, COLOR_UI_BG.g, COLOR_UI_BG.b, COLOR_UI_BG.a);
    SDL_RenderFillRect(renderer, &bar_rect);
    
    // Resource text
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "Wood: %d  Stone: %d  Metal: %d  Food: %d  Wave: %d  Time: %02d:%02d",
        state->resources.wood,
        state->resources.stone,
        state->resources.metal,
        state->resources.food,
        state->wave_number,
        (int)state->elapsed_time / 60,
        (int)state->elapsed_time % 60);
    
    // TODO: Render text with proper font system
}

void render_building_menu(SDL_Renderer* renderer, const struct GameState* state) {
    // Menu background
    SDL_FRect menu_rect = {WINDOW_WIDTH - 200, 40, 200, WINDOW_HEIGHT - 40};
    SDL_SetRenderDrawColor(renderer, COLOR_UI_BG.r, COLOR_UI_BG.g, COLOR_UI_BG.b, COLOR_UI_BG.a);
    SDL_RenderFillRect(renderer, &menu_rect);
    
    // Building buttons
    for (int i = 0; i < BUILDING_TYPE_COUNT; i++) {
        const Button* btn = &state->building_buttons[i];
        SDL_SetRenderDrawColor(renderer,
            btn->is_hovered ? COLOR_UI_HOVER.r : COLOR_UI_NORMAL.r,
            btn->is_hovered ? COLOR_UI_HOVER.g : COLOR_UI_NORMAL.g,
            btn->is_hovered ? COLOR_UI_HOVER.b : COLOR_UI_NORMAL.b,
            255);
        SDL_RenderFillRect(renderer, &btn->rect);
        
        // TODO: Render button text and building cost with proper font system
    }
}

void render_start_wave_button(SDL_Renderer* renderer, const struct GameState* state) {
    const Button* btn = &state->start_wave_button;
    SDL_SetRenderDrawColor(renderer, COLOR_BUTTON_GREEN.r, COLOR_BUTTON_GREEN.g, COLOR_BUTTON_GREEN.b, COLOR_BUTTON_GREEN.a);
    SDL_RenderFillRect(renderer, &btn->rect);
    
    // TODO: Render button text with proper font system
}

void render_enemies(SDL_Renderer* renderer, const EnemyManager* manager) {
    SDL_SetRenderDrawColor(renderer, 180, 0, 0, 255);
    
    for (int i = 0; i < manager->count; i++) {
        const Enemy* enemy = &manager->enemies[i];
        if (enemy->is_active) {
            SDL_FPoint screen_pos = world_to_screen(enemy->x, enemy->y, 0);
            
            // Enemy body
            SDL_FRect enemy_rect = {
                screen_pos.x - TILE_SIZE/3,
                screen_pos.y - TILE_SIZE/3,
                TILE_SIZE*2/3,
                TILE_SIZE*2/3
            };
            SDL_RenderFillRect(renderer, &enemy_rect);
            
            // Health bar
            float health_percent = (float)enemy->health / enemy->max_health;
            SDL_FRect health_rect = {
                screen_pos.x - TILE_SIZE/3,
                screen_pos.y - TILE_SIZE/2,
                TILE_SIZE*2/3 * health_percent,
                3
            };
            SDL_SetRenderDrawColor(renderer, 
                (1.0f - health_percent) * 255,
                health_percent * 255,
                0,
                255);
            SDL_RenderFillRect(renderer, &health_rect);
        }
    }
}

void render_game_world(SDL_Renderer* renderer, const struct GameState* state, const Camera* camera) {
    render_state.camera = *camera;
    
    SDL_SetRenderDrawColor(renderer, 
        render_state.background_color.r,
        render_state.background_color.g,
        render_state.background_color.b,
        render_state.background_color.a);
    SDL_RenderClear(renderer);
    
    render_world_grid(renderer);
    render_buildings(renderer, &state->building_manager);
    render_enemies(renderer, &state->enemy_manager);
    render_player(renderer, &state->player);
    
    render_ui(renderer, (GameState*)state);
    
    SDL_RenderPresent(renderer);
}

void render_ui(SDL_Renderer* renderer, struct GameState* state) {
    // Render resource bar at top
    render_resource_bar(renderer, state);
    
    // Render building menu if in building mode
    if (state->mode == GAME_STATE_BUILDING) {
        render_building_menu(renderer, state);
        render_start_wave_button(renderer, state);
    }
    
    // Render wave info if wave is active
    if (state->mode == GAME_STATE_WAVE) {
        render_wave_info(renderer, state);
    }
}

void render_wave_info(SDL_Renderer* renderer, const struct GameState* state) {
    // Wave info background
    SDL_FRect info_rect = {10, WINDOW_HEIGHT - 60, 200, 50};
    SDL_SetRenderDrawColor(renderer, COLOR_UI_BG.r, COLOR_UI_BG.g, COLOR_UI_BG.b, COLOR_UI_BG.a);
    SDL_RenderFillRect(renderer, &info_rect);
    
    // Wave text
    char wave_text[64];
    snprintf(wave_text, sizeof(wave_text), "Wave %d", state->wave_number);
    render_text(renderer, wave_text, info_rect.x + 10, info_rect.y + 5, COLOR_TEXT);
    
    // Progress bar
    float progress = (state->elapsed_time - state->wave_start_time) / state->wave_duration;
    SDL_FRect progress_rect = {
        info_rect.x + 10,
        info_rect.y + 30,
        180 * progress,
        10
    };
    SDL_SetRenderDrawColor(renderer, COLOR_BUTTON_GREEN.r, COLOR_BUTTON_GREEN.g, COLOR_BUTTON_GREEN.b, COLOR_BUTTON_GREEN.a);
    SDL_RenderFillRect(renderer, &progress_rect);
}

void render_buildings(SDL_Renderer* renderer, const struct BuildingManager* manager) {
    // Render each building
    for (int i = 0; i < manager->building_count; i++) {
        const Building* building = &manager->buildings[i];
        SDL_FPoint screen_pos = world_to_screen(building->x, building->y, 0);
        
        // Building rectangle
        SDL_FRect building_rect = {
            screen_pos.x,
            screen_pos.y,
            GRID_SIZE,
            GRID_SIZE
        };
        
        // Choose color based on building type
        switch (building->type) {
            case WALL:
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                break;
            case TURRET:
                SDL_SetRenderDrawColor(renderer, 0, 150, 150, 255);
                break;
            case HOUSE:
                SDL_SetRenderDrawColor(renderer, 150, 100, 0, 255);
                break;
            // Add more building types
        }
        
        SDL_RenderFillRect(renderer, &building_rect);
        
        // Health bar
        float health_percent = (float)building->health / building->max_health;
        SDL_FRect health_rect = {
            screen_pos.x,
            screen_pos.y - 5,
            GRID_SIZE * health_percent,
            3
        };
        SDL_SetRenderDrawColor(renderer, 
            (1.0f - health_percent) * 255,
            health_percent * 255,
            0,
            255);
        SDL_RenderFillRect(renderer, &health_rect);
    }
}

void render_player(SDL_Renderer* renderer, const struct Player* player) {
    SDL_FPoint screen_pos = world_to_screen(player->x, player->y, 0);
    
    // Player circle (approximated with rectangles for now)
    SDL_FRect player_rect = {
        screen_pos.x - PLAYER_SIZE/2,
        screen_pos.y - PLAYER_SIZE/2,
        PLAYER_SIZE,
        PLAYER_SIZE
    };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &player_rect);
    
    // Health bar
    float health_percent = (float)player->health / PLAYER_MAX_HEALTH;
    SDL_FRect health_rect = {
        screen_pos.x - PLAYER_SIZE/2,
        screen_pos.y - PLAYER_SIZE/2 - 5,
        PLAYER_SIZE * health_percent,
        3
    };
    SDL_SetRenderDrawColor(renderer, 
        (1.0f - health_percent) * 255,
        health_percent * 255,
        0,
        255);
    SDL_RenderFillRect(renderer, &health_rect);
}

void render_survivors(SDL_Renderer* renderer, const struct SurvivorManager* manager) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    
    for (int i = 0; i < manager->count; i++) {
        const Survivor* survivor = &manager->survivors[i];
        if (survivor->is_active) {
            SDL_FPoint screen_pos = world_to_screen(survivor->x, survivor->y, 0);
            SDL_FRect survivor_rect = {
                screen_pos.x - TILE_SIZE/4,
                screen_pos.y - TILE_SIZE/4,
                TILE_SIZE/2,
                TILE_SIZE/2
            };
            SDL_RenderFillRect(renderer, &survivor_rect);
        }
    }
}

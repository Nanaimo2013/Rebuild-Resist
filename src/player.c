#include "player.h"
#include "game.h"
#include "math_utils.h"  // Add this include
#include <SDL3/SDL.h>
#include <string.h>  // For memcpy and memset

#define MAX_KEYBOARD_KEYS 512  // Safe maximum for keyboard scancodes

static struct {
    bool keys[MAX_KEYBOARD_KEYS];
    bool prev_keys[MAX_KEYBOARD_KEYS];
    int mouse_x, mouse_y;
    bool mouse_buttons[3];
    bool prev_mouse_buttons[3];
} input_state = {0};

static void update_input_state(void) {
    // Store previous key states
    memcpy(input_state.prev_keys, input_state.keys, sizeof(input_state.keys));
    
    // Get current keyboard state
    int numkeys = 0;
    const Uint8* keyboard = SDL_GetKeyboardState(&numkeys);
    if (keyboard && numkeys > 0) {
        memcpy(input_state.keys, keyboard, SDL_min((size_t)numkeys, sizeof(input_state.keys)));
    }
    
    // Store previous mouse button states
    memcpy(input_state.prev_mouse_buttons, input_state.mouse_buttons, sizeof(input_state.mouse_buttons));
    
    // Get current mouse state
    uint32_t mouse_buttons = SDL_GetMouseState(&input_state.mouse_x, &input_state.mouse_y);
    input_state.mouse_buttons[0] = (mouse_buttons & SDL_BUTTON_LMASK) != 0;
    input_state.mouse_buttons[1] = (mouse_buttons & SDL_BUTTON_MMASK) != 0;
    input_state.mouse_buttons[2] = (mouse_buttons & SDL_BUTTON_RMASK) != 0;
}

static bool key_pressed(SDL_Scancode key) {
    return input_state.keys[key] && !input_state.prev_keys[key];
}

static bool key_held(SDL_Scancode key) {
    return input_state.keys[key];
}

void init_player(Player* player) {
    if (!player) return;
    
    player->x = (float)WINDOW_WIDTH / 2;
    player->y = (float)WINDOW_HEIGHT / 2;
    player->health = 100;
    player->max_health = 100;
    player->resources = 0;
    player->movement_speed = 5.0f;
    player->is_building_mode = false;
    player->is_moving = false;
    player->is_attacking = false;
    player->is_gathering = false;
    
    // Initialize input state
    memset(&input_state, 0, sizeof(input_state));
}

void update_player(Player* player) {
    if (!player) return;
    
    update_input_state();
    
    // Reset movement flag
    player->is_moving = false;
    
    // Movement
    float dx = 0.0f, dy = 0.0f;
    
    if (key_held(SDL_SCANCODE_W)) dy -= 1.0f;
    if (key_held(SDL_SCANCODE_S)) dy += 1.0f;
    if (key_held(SDL_SCANCODE_A)) dx -= 1.0f;
    if (key_held(SDL_SCANCODE_D)) dx += 1.0f;
    
    // Normalize diagonal movement
    if (dx != 0.0f && dy != 0.0f) {
        float length = my_sqrtf(dx * dx + dy * dy);
        if (length > 0) {  // Add check to prevent division by zero
            dx /= length;
            dy /= length;
        }
    }
    
    // Apply movement
    if (dx != 0.0f || dy != 0.0f) {
        player->x += dx * player->movement_speed;
        player->y += dy * player->movement_speed;
        player->is_moving = true;
    }
    
    // Toggle states with key presses (not holds)
    if (key_pressed(SDL_SCANCODE_B)) {
        player->is_building_mode = !player->is_building_mode;
        player->is_gathering = false;
        player->is_attacking = false;
    }
    
    if (key_pressed(SDL_SCANCODE_G)) {
        player->is_gathering = !player->is_gathering;
        player->is_building_mode = false;
        player->is_attacking = false;
    }
    
    // Attack with left mouse button
    player->is_attacking = input_state.mouse_buttons[0];
    if (player->is_attacking) {
        player->is_building_mode = false;
        player->is_gathering = false;
    }
    
    // Keep player in bounds
    player->x = SDL_clamp(player->x, 0.0f, (float)WINDOW_WIDTH);
    player->y = SDL_clamp(player->y, 0.0f, (float)WINDOW_HEIGHT);
}

void render_player(SDL_Renderer* renderer, const Player* player) {
    if (!player || !renderer) return;
    
    SDL_FRect player_rect = {
        .x = player->x - TILE_SIZE/2,
        .y = player->y - TILE_SIZE/2,
        .w = TILE_SIZE,
        .h = TILE_SIZE
    };
    
    // Change color based on player state
    if (player->is_building_mode) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green for building
    } else if (player->is_attacking) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red for attacking
    } else if (player->is_gathering) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Yellow for gathering
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Default green
    }
    
    SDL_RenderFillRect(renderer, &player_rect);
    
    // Draw direction indicator if moving
    if (player->is_moving) {
        // TODO: Add direction indicator
    }
}

#include "input.h"
#include "game.h"

void handle_keyboard(GameState* state, SDL_Scancode scancode) {
    switch (scancode) {
        case SDL_SCANCODE_ESCAPE:
            state->mode = GAME_STATE_PAUSED;
            break;
        // Add more keyboard controls
    }
}

void handle_menu_click(GameState* state, int x, int y) {
    // Calculate button positions
    const int button_height = 60;
    const int button_width = 180;
    const int menu_x = WINDOW_WIDTH - 190;
    int y_offset = 50;
    
    // Check each building button
    for (int i = 0; i < BUILDING_TYPE_COUNT; i++) {
        SDL_FRect button = {
            menu_x,
            y_offset,
            button_width,
            button_height
        };
        
        if (x >= button.x && x <= button.x + button.w &&
            y >= button.y && y <= button.y + button.h) {
            state->selected_building_type = i;
            return;
        }
        
        y_offset += button_height + 10;
    }
}

void handle_mouse_click(GameState* state, int x, int y) {
    // Check start wave button
    if (state->mode == GAME_STATE_BUILDING) {
        Button* start_btn = &state->start_wave_button;
        if (x >= start_btn->rect.x && x <= start_btn->rect.x + start_btn->rect.w &&
            y >= start_btn->rect.y && y <= start_btn->rect.y + start_btn->rect.h) {
            start_wave(state);
            return;
        }
        
        // Check building buttons
        for (int i = 0; i < BUILDING_TYPE_COUNT; i++) {
            Button* btn = &state->building_buttons[i];
            if (x >= btn->rect.x && x <= btn->rect.x + btn->rect.w &&
                y >= btn->rect.y && y <= btn->rect.y + btn->rect.h) {
                state->selected_building_type = i;
                state->building_mode = true;
                return;
            }
        }
        
        // Handle building placement
        if (state->building_mode && state->selected_building_type != -1) {
            int grid_x = ((int)state->camera_x + x) / GRID_SIZE;
            int grid_y = ((int)state->camera_y + y) / GRID_SIZE;
            add_building(&state->building_manager, grid_x, grid_y, state->selected_building_type);
        }
    }
} 
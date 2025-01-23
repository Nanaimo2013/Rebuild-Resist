#include "wave.h"
#include "data/wave_settings.h"
#include "text.h"

void start_wave(GameState* state) {
    state->mode = GAME_STATE_WAVE;
    state->wave_started = true;
}

void render_wave_info(SDL_Renderer* renderer, struct GameState* state) {
    if (state->mode == GAME_STATE_WAVE) {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "Wave %d", state->wave_number);
        
        SDL_Color text_color = {255, 255, 255, 255}; // White text
        render_text(renderer, buffer, 10, 10, text_color);
        
        // Add wave progress
        if (state->wave_started) {
            snprintf(buffer, sizeof(buffer), "Progress: %d%%", 
                (int)((state->elapsed_time - state->wave_start_time) / state->wave_duration * 100));
            render_text(renderer, buffer, 10, 30, text_color);
        }
    }
} 
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "game.h"
#include "enemy.h"
#include "renderer.h"
#include "survivor.h"

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
#else
int main(int argc, char* argv[]) {
#endif
    // Remove these lines if not needed
    // (void)argc;
    // (void)argv;

    GameState game_state = {0};  // Initialize all to zero
    game_state.is_running = true;
    game_state.wave_number = 1;
    game_state.time_of_day = 0.0f;
    game_state.delta_time = FRAME_TIME;

    // Initialize SDL and game systems
    if (!init_game(&game_state)) {
        printf("Failed to initialize game\n");
        return 1;
    }

    // Initialize renderer
    init_renderer();

    // Add initial survivors
    for (int i = 0; i < 5; i++) {
        add_survivor(
            game_state.survivor_manager,
            WINDOW_WIDTH / 2 + (rand() % 100 - 50),
            WINDOW_HEIGHT / 2 + (rand() % 100 - 50)
        );
    }

    Uint64 previous_time = SDL_GetTicks();

    // Main game loop
    while (game_state.is_running) {
        // Calculate delta time
        Uint64 current_time = SDL_GetTicks();
        game_state.delta_time = (current_time - previous_time) / 1000.0f;
        previous_time = current_time;

        // Handle input
        handle_input(&game_state);
        
        // Update game logic
        update_game(&game_state);
        
        // Render everything
        render_game(&game_state);
        
        // Control frame rate
        if (game_state.delta_time < FRAME_TIME) {
            SDL_Delay((Uint32)((FRAME_TIME - game_state.delta_time) * 1000.0f));
        }
    }

    cleanup_game(&game_state);
    return 0;
}

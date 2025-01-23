#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "camera.h"
#include "building_manager.h"
#include "enemy_manager.h"
#include "survivor_manager.h"
#include "player.h"
#include "data/game_settings.h"
#include "game_types.h"

bool init_game(struct GameState* state);
void update_game(struct GameState* state);
void render_game(struct GameState* state);
void cleanup_game(struct GameState* state);
void handle_input(struct GameState* state);
void handle_mouse_click(struct GameState* state, int x, int y);
void handle_keyboard(struct GameState* state, const uint8_t* keyboard_state);
void handle_menu_click(struct GameState* state, int x, int y);

#endif // GAME_H

#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>
#include "game.h"

void handle_keyboard(GameState* state, SDL_Scancode scancode);
void handle_menu_click(GameState* state, int x, int y);

#endif 
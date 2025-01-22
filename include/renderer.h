#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>
#include "game.h"

void init_renderer(void);
SDL_FPoint world_to_screen(float world_x, float world_y);
void render_ui(SDL_Renderer* renderer, const GameState* state);
void render_world_grid(SDL_Renderer* renderer);
void render_game_world(SDL_Renderer* renderer, const GameState* state);

#endif 
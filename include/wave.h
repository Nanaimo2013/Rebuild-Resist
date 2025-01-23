#ifndef WAVE_H
#define WAVE_H

#include "game.h"

void start_wave(GameState* state);
void render_wave_info(SDL_Renderer* renderer, struct GameState* state);

#endif 
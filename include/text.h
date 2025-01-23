#ifndef TEXT_H
#define TEXT_H

#include <SDL3/SDL.h>

void init_text_renderer(SDL_Renderer* renderer);
void render_text(SDL_Renderer* renderer, const char* text, float x, float y, SDL_Color color);
void cleanup_text_renderer(void);

#endif 
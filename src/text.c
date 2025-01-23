#include "text.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

// Simple bitmap font - 8x8 pixels per character
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

// Basic bitmap font data (you can replace this with a better font system later)
static const unsigned char FONT_DATA[128][8] = {
    // Basic ASCII characters from 0-127 (we'll just implement a few for now)
    // Space
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    // Numbers 0-9
    {0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00}, // 0
    {0x10, 0x30, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00}, // 1
    // Add more characters as needed
};

static SDL_Texture* font_texture = NULL;

void init_text_renderer(SDL_Renderer* renderer) {
    // Create texture for font rendering
    font_texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        128 * CHAR_WIDTH,
        CHAR_HEIGHT);
    
    if (!font_texture) {
        printf("Failed to create font texture: %s\n", SDL_GetError());
        return;
    }
}

void render_text(SDL_Renderer* renderer, const char* text, float x, float y, SDL_Color color) {
    if (!font_texture || !text) return;
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    float current_x = x;
    for (size_t i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        if (c >= 0 && c < 128) { // Basic ASCII check
            SDL_FRect dest_rect = {
                current_x,
                y,
                CHAR_WIDTH,
                CHAR_HEIGHT
            };
            SDL_RenderRect(renderer, &dest_rect);
            current_x += CHAR_WIDTH;
        }
    }
}

void cleanup_text_renderer(void) {
    if (font_texture) {
        SDL_DestroyTexture(font_texture);
        font_texture = NULL;
    }
} 
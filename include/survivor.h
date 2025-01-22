#ifndef SURVIVOR_H
#define SURVIVOR_H

#include <SDL3/SDL.h>
#include <stdbool.h>

// Forward declaration
struct GameState;

typedef enum {
    SURVIVOR_IDLE,
    SURVIVOR_GATHERING,
    SURVIVOR_BUILDING,
    SURVIVOR_FIGHTING
} SurvivorState;

void init_survivor_manager(void);
bool add_survivor(float x, float y);
void update_survivors(const struct GameState* state);
void render_survivors(SDL_Renderer* renderer);

#endif 
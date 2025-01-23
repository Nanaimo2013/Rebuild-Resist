#ifndef WAVE_SETTINGS_H
#define WAVE_SETTINGS_H

#include "enemy_settings.h"

typedef struct {
    EnemyType type;
    int level;
    int count;
    float delay;  // Added delay for spawn timing
} WaveSpawn;

typedef struct {
    WaveSpawn* spawns;
    int spawn_count;
    float preparation_time;
    float difficulty_multiplier;
    // Additional useful fields
    const char* name;
    const char* description;
    int reward_money;
    const char* music_track;
} WaveData;

// Predefined waves
extern const WaveData WAVE_DATA[];
extern const int WAVE_COUNT;

// Wave-related functions
const WaveData* get_wave_data(int wave_number);
void spawn_wave(int wave_number, const WaveData* wave_data);

// Helper functions
const char* get_wave_name(int wave_number);
const char* get_wave_description(int wave_number);
float get_wave_difficulty(int wave_number);

#endif 
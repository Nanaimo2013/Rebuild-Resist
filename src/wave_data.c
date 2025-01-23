#include "data/wave_settings.h"

// Wave 1 - Introduction
static const WaveSpawn WAVE_1_SPAWNS[] = {
    {ENEMY_ZOMBIE, 1, 10, 0.0f},
    {ENEMY_RUNNER, 1, 5, 5.0f}
};

// Wave 2 - Adding Tanks
static const WaveSpawn WAVE_2_SPAWNS[] = {
    {ENEMY_ZOMBIE, 1, 15, 0.0f},
    {ENEMY_RUNNER, 2, 8, 3.0f},
    {ENEMY_TANK, 1, 2, 10.0f}
};

// Wave 3 - Boss Wave
static const WaveSpawn WAVE_3_SPAWNS[] = {
    {ENEMY_ZOMBIE, 2, 20, 0.0f},
    {ENEMY_RUNNER, 2, 10, 5.0f},
    {ENEMY_TANK, 2, 4, 10.0f},
    {ENEMY_BOSS, 1, 1, 15.0f}
};

const WaveData WAVE_DATA[] = {
    {
        .name = "First Contact",
        .description = "The first wave of enemies approaches...",
        .spawns = (WaveSpawn*)WAVE_1_SPAWNS,
        .spawn_count = sizeof(WAVE_1_SPAWNS) / sizeof(WaveSpawn),
        .preparation_time = 30.0f,
        .difficulty_multiplier = 1.0f,
        .reward_money = 100,
        .music_track = "music/wave1.ogg"
    },
    {
        .name = "Heavy Resistance",
        .description = "They're bringing in the heavy units!",
        .spawns = (WaveSpawn*)WAVE_2_SPAWNS,
        .spawn_count = sizeof(WAVE_2_SPAWNS) / sizeof(WaveSpawn),
        .preparation_time = 45.0f,
        .difficulty_multiplier = 1.5f,
        .reward_money = 200,
        .music_track = "music/wave2.ogg"
    },
    {
        .name = "Boss Battle",
        .description = "A powerful enemy approaches...",
        .spawns = (WaveSpawn*)WAVE_3_SPAWNS,
        .spawn_count = sizeof(WAVE_3_SPAWNS) / sizeof(WaveSpawn),
        .preparation_time = 60.0f,
        .difficulty_multiplier = 2.0f,
        .reward_money = 500,
        .music_track = "music/wave3.ogg"
    }
};

const int WAVE_COUNT = sizeof(WAVE_DATA) / sizeof(WaveData);

// Implementation of helper functions
const char* get_wave_name(int wave_number) {
    if (wave_number >= 0 && wave_number < WAVE_COUNT) {
        return WAVE_DATA[wave_number].name;
    }
    return "Unknown Wave";
}

const char* get_wave_description(int wave_number) {
    if (wave_number >= 0 && wave_number < WAVE_COUNT) {
        return WAVE_DATA[wave_number].description;
    }
    return "No description available";
}

const WaveData* get_wave_data(int wave_number) {
    if (wave_number >= 0 && wave_number < WAVE_COUNT) {
        return &WAVE_DATA[wave_number];
    }
    return NULL;
}

float get_wave_difficulty(int wave_number) {
    if (wave_number >= 0 && wave_number < WAVE_COUNT) {
        return WAVE_DATA[wave_number].difficulty_multiplier;
    }
    return 1.0f;
} 
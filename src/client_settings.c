#include "data/client_settings.h"
#include <stdio.h>

const ClientSettings DEFAULT_CLIENT_SETTINGS = {
    // Video Settings
    .window_width = 1280,
    .window_height = 720,
    .fullscreen = false,
    .vsync = true,
    .fps_limit = 60,
    
    // Audio Settings
    .master_volume = 1.0f,
    .music_volume = 0.7f,
    .sfx_volume = 1.0f,
    .mute_audio = false,
    
    // Gameplay Settings
    .show_grid = true,
    .show_health_bars = true,
    .show_damage_numbers = true,
    .camera_speed = 500.0f,
    .zoom_speed = 0.1f,
    
    // UI Settings
    .show_fps = true,
    .show_wave_timer = true,
    .show_resource_tooltips = true,
    .ui_scale = 1.0f
};

ClientSettings current_settings;

void load_client_settings(void) {
    FILE* file = fopen("settings.cfg", "rb");
    if (file) {
        fread(&current_settings, sizeof(ClientSettings), 1, file);
        fclose(file);
    } else {
        // If no settings file exists, use defaults
        current_settings = DEFAULT_CLIENT_SETTINGS;
    }
}

void save_client_settings(void) {
    FILE* file = fopen("settings.cfg", "wb");
    if (file) {
        fwrite(&current_settings, sizeof(ClientSettings), 1, file);
        fclose(file);
    }
}

void reset_client_settings(void) {
    current_settings = DEFAULT_CLIENT_SETTINGS;
    save_client_settings();
} 
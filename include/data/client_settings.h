#ifndef CLIENT_SETTINGS_H
#define CLIENT_SETTINGS_H

#include <stdbool.h>

typedef struct {
    // Video Settings
    int window_width;
    int window_height;
    bool fullscreen;
    bool vsync;
    int fps_limit;
    
    // Audio Settings
    float master_volume;
    float music_volume;
    float sfx_volume;
    bool mute_audio;
    
    // Gameplay Settings
    bool show_grid;
    bool show_health_bars;
    bool show_damage_numbers;
    float camera_speed;
    float zoom_speed;
    
    // UI Settings
    bool show_fps;
    bool show_wave_timer;
    bool show_resource_tooltips;
    float ui_scale;
} ClientSettings;

// Default settings
extern const ClientSettings DEFAULT_CLIENT_SETTINGS;

// Current settings (can be modified)
extern ClientSettings current_settings;

// Functions to load/save settings
void load_client_settings(void);
void save_client_settings(void);
void reset_client_settings(void);

#endif 
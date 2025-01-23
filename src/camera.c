#include "camera.h"

void init_camera(Camera* camera) {
    camera->x = 0.0f;
    camera->y = 0.0f;
    camera->zoom = 1.0f;
    camera->target_x = 0.0f;
    camera->target_y = 0.0f;
    camera->target_zoom = 1.0f;
    camera->lerp_speed = 0.1f;
}

void update_camera(Camera* camera, float target_x, float target_y) {
    camera->target_x = target_x;
    camera->target_y = target_y;
    
    // Smoothly interpolate camera position
    camera->x += (camera->target_x - camera->x) * camera->lerp_speed;
    camera->y += (camera->target_y - camera->y) * camera->lerp_speed;
    camera->zoom += (camera->target_zoom - camera->zoom) * camera->lerp_speed;
}

void set_camera_zoom(Camera* camera, float zoom) {
    camera->target_zoom = zoom;
} 
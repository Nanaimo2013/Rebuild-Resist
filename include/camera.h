#ifndef CAMERA_H
#define CAMERA_H

typedef struct {
    float x;
    float y;
    float zoom;
    float target_x;
    float target_y;
    float target_zoom;
    float lerp_speed;
} Camera;

void init_camera(Camera* camera);
void update_camera(Camera* camera, float target_x, float target_y);
void set_camera_zoom(Camera* camera, float zoom);

#endif // CAMERA_H 
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <math.h>

// Constants
#define PI 3.14159265358979323846f

// Convert degrees to radians
static inline float deg_to_rad(float degrees) {
    return degrees * (PI / 180.0f);
}

// Convert radians to degrees
static inline float rad_to_deg(float radians) {
    return radians * (180.0f / PI);
}

// Trig functions that work with degrees
static inline float cos_deg(float degrees) {
    return cosf(deg_to_rad(degrees));
}

static inline float sin_deg(float degrees) {
    return sinf(deg_to_rad(degrees));
}

// Basic trig functions using Taylor series approximation
static inline float my_cosf(float x) {
    // Normalize angle to -PI to PI
    while (x > PI) x -= 2 * PI;
    while (x < -PI) x += 2 * PI;
    
    float x2 = x * x;
    // Taylor series: 1 - x^2/2! + x^4/4! - x^6/6!
    return 1.0f - x2/2.0f + x2*x2/24.0f - x2*x2*x2/720.0f;
}

static inline float my_sinf(float x) {
    // sin(x) = cos(x - PI/2)
    return my_cosf(x - PI/2);
}

// Other useful math functions
static inline float my_sqrtf(float x) {
    // Simple Newton's method
    if (x <= 0) return 0;
    float guess = x / 2.0f;
    for (int i = 0; i < 10; i++) {
        guess = (guess + x/guess) / 2.0f;
    }
    return guess;
}

// Utility functions
static inline float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

static inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

#endif 
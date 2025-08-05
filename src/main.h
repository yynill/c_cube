#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "renderer.h"

#define PHI 1.618033988749895    // Golden ratio (φ)
#define PI 3.141592653589793     // Pi (π)
#define SQRT_2 1.414213562373095 // Square root of 2
#define DEG_PER_RAD (180.0 / PI) // Conversion factor: radians to degrees

#define COLOR_RESET      "\033[0m"
#define COLOR_HEADER     "\033[1;36m"  // Bold Cyan
#define COLOR_LABEL      "\033[1;37m"  // Bright White
#define COLOR_VALUE      "\033[1;33m"  // Yellow
#define COLOR_SECTION    "\033[1;32m"  // Green
#define COLOR_SUBTLE     "\033[0;37m"  // Light Gray


typedef struct Pyramid { // Square Pyramid
    // Inputs
    double B;         // Outer base length of the pyramid (in meters)
    double H;         // Outer height of the pyramid (in meters)
    double k;         // Scaling factor for the inner hollow pyramid (0 < k < 1)

    // Calculated variables
    double t;                     // Horizontal wall thickness (in meters)
    double t_percent;             // Wall thickness as a percentage of the base length
    double b;                     // Base length of the inner hollow pyramid (in meters)
    double h;                     // Height of the inner hollow pyramid (in meters)
    double V_total;               // Total volume of the outer pyramid (in cubic meters)
    double V_hollow;              // Volume of the hollow space (in cubic meters)
    double V_stone;               // Volume of the stone material (in cubic meters)
    double hollow_volume_percent; // Percentage of total volume that is hollow
    double stone_volume_percent;  // Percentage of total volume that is stone
    double slope_angle_face;      // Face slope angle of the pyramid (in degrees)
    double slope_angle_edge;      // Edge slope angle of the pyramid (in degrees)
    double d_outer;               // Diagonal of the outer base (in meters, incorporates √2)
    double d_inner;               // Diagonal of the inner base (in meters, incorporates √2)
    double s_outer;               // Slant height of the outer pyramid (in meters)
    double s_inner;               // Slant height of the inner pyramid (in meters)
    double l_outer;               // Edge length of the outer pyramid (in meters)
    double l_inner;               // Edge length of the inner pyramid (in meters)
    double A_inner;               // Base area of the inner (hollow) pyramid (in square meters)
    double A_outer;               // Base area of the outer pyramid (in square meters)
} Pyramid;

Pyramid *init_pyramid(double B, double H, double k);
void print_pyramid_properties(Pyramid *p);

#endif // MAIN_H

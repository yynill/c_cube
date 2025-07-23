
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

typedef struct Vector {
    int x;
    int y;
    int z;
} Vector;

typedef struct Camera {
    float pos_x;
    float pos_y;
    float pos_z;

    float rotation_x;
    float rotation_y;
    float rotation_z;

    float zoom;
} Camera;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Point *origin;
extern SDL_Texture *circle_texture;
extern Camera *camera;

int init_sdl(void);
void cleanup_sdl(void);
void draw_origin();
void draw_point(Vector p, SDL_Color color);
void draw_line(Vector p1, Vector p2, SDL_Color color);
void draw_square_face(Vector p1, Vector p2, Vector p3, Vector p4,SDL_Color color);
Vector apply_camera(Vector p);

#endif // MAIN_H


#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "main.h"

typedef struct Pyramid Pyramid;

typedef struct Vector {
    int x;
    int y;
    int z;
} Vector;

typedef struct Camera {
    float rotation_x;
    float rotation_y;
    float rotation_z;

    float zoom;
} Camera;

extern SDL_Color white;
extern SDL_Color red;
extern SDL_Color blue;
extern SDL_Color green;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Point *origin;
extern SDL_Texture *circle_texture;
extern Camera *camera;

int init_sdl(void);
void cleanup_sdl(void);
void draw_origin();
void draw_gizmo();
void draw_point(Vector p, SDL_Color color);
void draw_line(Vector p1, Vector p2, SDL_Color color);

void draw_line_moved(Vector p1, Vector p2, int x, int y, SDL_Color color);

void draw_square_face(Vector p1, Vector p2, Vector p3, Vector p4,SDL_Color color);
Vector apply_camera(Vector p);
void draw_pyramid(Pyramid *p);

#endif // RENDERER_H

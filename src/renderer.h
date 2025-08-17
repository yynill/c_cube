
#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#include "main.h"

typedef struct Pyramid Pyramid;

typedef struct Vector {
    float x;
    float y;
    float z;
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
extern TTF_Font *font;
extern Camera *camera;

Vector subtract(Vector a, Vector b);
Vector normalize(Vector v);
Vector cross(Vector a, Vector b);
Vector scale(Vector v, double scalar);

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
void draw_pyramid_cross_section(Pyramid *p);
void render_stats(Pyramid *p);
void render_text(const char *text, int x, int y, SDL_Color color);
void draw_height_line(int x, int y1, int y2, const char *label, SDL_Color color);
void draw_width_line(int y, int x1, int x2, const char *label, SDL_Color color);
void render_stats(Pyramid *p);

#endif // RENDERER_H

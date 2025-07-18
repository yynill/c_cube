
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

typedef struct Point {
    int x;
    int y;
    int z;
} Point;

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
void draw_point(Point p, SDL_Color color);
void draw_line(Point p1, Point p2, SDL_Color color);
void draw_square_face(Point p1, Point p2, Point p3, Point p4, SDL_Color color);
Point apply_camera(Point p);

#endif // MAIN_H

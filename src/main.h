
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Point {
    int x;
    int y;
    int z;
} Point;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Point *origin;
extern SDL_Texture *circle_texture;


int init_sdl(void);
void cleanup_sdl(void);
void draw_origin();
void draw_point(Point p, SDL_Color color);
void draw_line(Point p1, Point p2, SDL_Color color);

#endif // MAIN_H

#include "main.h"

int SCREEN_WIDTH = 1200;
int SCREEN_HEIGHT = 800;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Point *origin = NULL;
SDL_Texture *circle_texture = NULL;

int init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow("SDL Game Loop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    origin = malloc(sizeof(SDL_Point));
    origin->x = SCREEN_WIDTH / 2;
    origin->y = SCREEN_HEIGHT / 2;

    circle_texture = IMG_LoadTexture(renderer, "./assets/circle.png");

    return 1;
}

void cleanup_sdl(void) {
    if (circle_texture) {
        SDL_DestroyTexture(circle_texture);
        circle_texture = NULL;
    }
    if (origin) {
        free(origin);
        origin = NULL;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
}

int main() {
    if (!init_sdl()) return 1;

    int running = 1;
    SDL_Event event;


    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        draw_origin();

        Point p0 = {-50, -50, -50};
        Point p1 = {-50, -50,  50};
        Point p2 = {-50,  50, -50};
        Point p3 = {-50,  50,  50};
        Point p4 = { 50, -50, -50};
        Point p5 = { 50, -50,  50};
        Point p6 = { 50,  50, -50};
        Point p7 = { 50,  50,  50};
        SDL_Color white = {255, 255, 255, 255};

        draw_line(p0, p1, white);
        draw_line(p0, p2, white);
        draw_line(p0, p4, white);
        draw_line(p1, p3, white);
        draw_line(p1, p5, white);
        draw_line(p2, p3, white);
        draw_line(p2, p6, white);
        draw_line(p3, p7, white);
        draw_line(p4, p5, white);
        draw_line(p4, p6, white);
        draw_line(p5, p7, white);
        draw_line(p6, p7, white);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    cleanup_sdl();
    return 0;
}

void draw_origin() {
    Point p0 = {0, 0, 0};
    Point p1 = {0, 0, 10};
    Point p2 = {0, 10, 0};
    Point p3 = {10, 0, 0};

    SDL_Color red = {255, 0, 0, 255};
    SDL_Color yellow = {0, 255, 0, 255};
    SDL_Color blue = {0, 0, 255, 255};

    draw_line(p0, p1, blue);
    draw_line(p0, p2, yellow);
    draw_line(p0, p3, red);
}

void draw_point(Point p, SDL_Color color) {
    float x_2d = origin->x + p.y + (-0.5f * p.x);
    float y_2d = origin->y - p.z + (0.5f * p.x);

    int radius = 2;
    SDL_Rect circle_rect = {(int)x_2d - radius, (int)y_2d - radius, radius * 2, radius * 2};

    SDL_SetTextureColorMod(circle_texture, color.r, color.g, color.b);
    SDL_RenderCopy(renderer, circle_texture, NULL, &circle_rect);
}

void draw_line(Point p1, Point p2, SDL_Color color) {
    float x1_2d = origin->x + p1.y + (-0.5f * p1.x);
    float y1_2d = origin->y - p1.z + (0.5f * p1.x);
    float x2_2d = origin->x + p2.y + (-0.5f * p2.x);
    float y2_2d = origin->y - p2.z + (0.5f * p2.x);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, (int)x1_2d, (int)y1_2d, (int)x2_2d, (int)y2_2d);
}

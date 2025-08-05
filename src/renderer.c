#include "renderer.h"

int SCREEN_WIDTH = 1200;
int SCREEN_HEIGHT = 800;
int GIZMO_SIZE = 50;

SDL_Color white = {255, 255, 255, 255};
SDL_Color red   = {255, 0, 0, 255};
SDL_Color blue  = {0, 0, 255, 255};
SDL_Color green = {0, 255, 0, 255};

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Point *origin = NULL;
SDL_Texture *circle_texture = NULL;
Camera *camera = NULL;

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

    camera = malloc(sizeof(Camera));
    camera->rotation_x = 0;
    camera->rotation_y = 0;
    camera->rotation_z = 0;
    camera->zoom = 1;

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

Vector apply_camera(Vector p) {
    // X-axis
    float cos_x = cosf(camera->rotation_x);
    float sin_x = sinf(camera->rotation_x);
    float y1 = p.y * cos_x - p.z * sin_x;
    float z1 = p.y * sin_x + p.z * cos_x;

    // Y-axis
    float cos_y = cosf(camera->rotation_y);
    float sin_y = sinf(camera->rotation_y);
    float x2 = p.x * cos_y + z1 * sin_y;
    float z2 = -p.x * sin_y + z1 * cos_y;

    // Z-axis
    float cos_z = cosf(camera->rotation_z);
    float sin_z = sinf(camera->rotation_z);
    float x3 = x2 * cos_z - y1 * sin_z;
    float y3 = x2 * sin_z + y1 * cos_z;

    Vector rotated = {(int)x3, (int)y3, (int)z2};
    return rotated;
}

void draw_origin() {
    Vector p0 = {0, 0, 0};
    Vector p1 = {0, 0, 10};
    Vector p2 = {0, 10, 0};
    Vector p3 = {10, 0, 0};

    draw_line(p0, p1, blue);
    draw_line(p0, p2, green);
    draw_line(p0, p3, red);
}

void draw_gizmo() {
    Vector p0 = {0, 0, 0};
    Vector p1 = {0, 0, GIZMO_SIZE / 2};
    Vector p2 = {0, GIZMO_SIZE / 2, 0};
    Vector p3 = {GIZMO_SIZE / 2, 0, 0};

    int x_offset = -(SCREEN_WIDTH / 2 ) + GIZMO_SIZE;
    int y_offset = -(SCREEN_HEIGHT / 2) + GIZMO_SIZE;

    draw_line_moved(p0, p1, x_offset, y_offset, blue);
    draw_line_moved(p0, p2, x_offset, y_offset, green);
    draw_line_moved(p0, p3, x_offset, y_offset, red);
}

SDL_Point dim_transform(Vector p){
    float x_2d = origin->x + p.y + (-0.5f * p.x);
    float y_2d = origin->y - p.z + (0.5f * p.x);

    SDL_Point point_2d = {x_2d, y_2d};
    return point_2d;
}

void draw_point(Vector p, SDL_Color color) {
    Vector rotated = apply_camera(p);
    SDL_Point point = dim_transform(rotated);

    int radius = 2;
    SDL_Rect circle_rect = {(int)point.x - radius, (int)point.y - radius, radius * 2, radius * 2};

    SDL_SetTextureColorMod(circle_texture, color.r, color.g, color.b);
    SDL_RenderCopy(renderer, circle_texture, NULL, &circle_rect);
}

void draw_line(Vector p1, Vector p2, SDL_Color color) {
    Vector rotated_p1 = apply_camera(p1);
    Vector rotated_p2 = apply_camera(p2);

    SDL_Point r1 = dim_transform(rotated_p1);
    SDL_Point r2 = dim_transform(rotated_p2);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, (int)r1.x, (int)r1.y, (int)r2.x, (int)r2.y);
}

void draw_line_moved(Vector p1, Vector p2, int x, int y, SDL_Color color) {
    Vector rotated_p1 = apply_camera(p1);
    Vector rotated_p2 = apply_camera(p2);

    SDL_Point r1 = dim_transform(rotated_p1);
    SDL_Point r2 = dim_transform(rotated_p2);

    r1.x += x;
    r2.x += x;
    r1.y += y;
    r2.y += y;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, (int)r1.x, (int)r1.y, (int)r2.x, (int)r2.y);
}


Vector subtract(Vector a, Vector b) {
    Vector result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

Vector normalize(Vector v) {
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0.0f) return (Vector){0, 0, 0};
    return (Vector){v.x / length, v.y / length, v.z / length};
}

Vector cross(Vector a, Vector b) {
    Vector result = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    return result;
}

Vector scale(Vector v, int scalar) {
    Vector result = {
        v.x * scalar,
        v.y * scalar,
        v.z * scalar,
    };
    return result;
}

void draw_square_face(Vector p1, Vector p2, Vector p3, Vector p4, SDL_Color color) {
    Vector p1_ = apply_camera(p1);
    Vector p2_ = apply_camera(p2);
    Vector p3_ = apply_camera(p3);
    Vector p4_ = apply_camera(p4);

    SDL_Point r1 = dim_transform(p1_);
    SDL_Point r2 = dim_transform(p2_);
    SDL_Point r3 = dim_transform(p3_);
    SDL_Point r4 = dim_transform(p4_);

    // lines
    // int num_line = 200;
    // SDL_Point delta_r1_r4 = {.x = (r4.x - r1.x),
    //                          .y = (r4.y - r1.y)};

    // for (int i = 0; i < num_line + 1; i++) {
    //     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    //     SDL_RenderDrawLine(renderer,
    //                        (int)(r1.x + (delta_r1_r4.x * i / num_line)),
    //                        (int)(r1.y + (delta_r1_r4.y * i / num_line)),
    //                        (int)(r2.x + (delta_r1_r4.x * i / num_line)),
    //                        (int)(r2.y + (delta_r1_r4.y * i / num_line)));
    // }

    // filledPolygonRGBA
    Sint16 vx[4] = {r1.x, r2.x, r3.x, r4.x};
    Sint16 vy[4] = {r1.y, r2.y, r3.y, r4.y};

    filledPolygonRGBA(renderer, vx, vy, 4, color.r, color.g, color.b, color.a);
}

void draw_pyramid(Pyramid *p) {
    Vector top = {
        .x = 0,
        .y = 0,
        .z = (int)p->H};

    int half_B = (int)(p->B / 2);

    Vector fl = {.x = half_B, .y = half_B, .z = 0};
    Vector fr = {.x = -half_B, .y = half_B, .z = 0};
    Vector bl = {.x = half_B, .y = -half_B, .z = 0};
    Vector br = {.x = -half_B, .y = -half_B, .z = 0};

    scale(top, camera->zoom);
    scale(fl, camera->zoom);
    scale(fr, camera->zoom);
    scale(bl, camera->zoom);
    scale(br, camera->zoom);

    draw_point(top, red);
    draw_point(fl, red);
    draw_point(fr, red);
    draw_point(bl, red);
    draw_point(br, red);

    draw_line(top, fl, white);
    draw_line(top, fr, white);
    draw_line(top, bl, white);
    draw_line(top, br, white);

    draw_line(fl, fr, white);
    draw_line(fr, br, white);
    draw_line(br, bl, white);
    draw_line(bl, fl, white);

}

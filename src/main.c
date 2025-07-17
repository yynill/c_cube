#include "main.h"

int SCREEN_WIDTH = 1200;
int SCREEN_HEIGHT = 800;

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
    camera->pos_x = 0;
    camera->pos_y = 0;
    camera->pos_z = 0;
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

Point apply_camera(Point p) {
    p.x += camera->pos_x;
    p.y += camera->pos_y;
    p.z += camera->pos_z;

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

    Point rotated = {(int)x3, (int)y3, (int)z2};
    return rotated;
}

int main() {
    if (!init_sdl()) return 1;

    int running = 1;
    SDL_Event event;
    const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }

        float rotation_speed = 0.05f;
        float move_speed = 2.0f;

        if (keyboard_state[SDL_SCANCODE_X]) {
            if (keyboard_state[SDL_SCANCODE_UP]) camera->rotation_x += rotation_speed;
            if (keyboard_state[SDL_SCANCODE_DOWN]) camera->rotation_x -= rotation_speed;
        }
        if (keyboard_state[SDL_SCANCODE_Y]) {
            if (keyboard_state[SDL_SCANCODE_UP]) camera->rotation_y += rotation_speed;
            if (keyboard_state[SDL_SCANCODE_DOWN]) camera->rotation_y -= rotation_speed;
        }
        if (keyboard_state[SDL_SCANCODE_Z]) {
            if (keyboard_state[SDL_SCANCODE_UP]) camera->rotation_z += rotation_speed;
            if (keyboard_state[SDL_SCANCODE_DOWN]) camera->rotation_z -= rotation_speed;
        }
        if (keyboard_state[SDL_SCANCODE_W]) camera->pos_z -= move_speed;
        if (keyboard_state[SDL_SCANCODE_S]) camera->pos_z += move_speed;
        if (keyboard_state[SDL_SCANCODE_A]) camera->pos_x -= move_speed;
        if (keyboard_state[SDL_SCANCODE_D]) camera->pos_x += move_speed;
        if (keyboard_state[SDL_SCANCODE_Q]) camera->pos_y -= move_speed;
        if (keyboard_state[SDL_SCANCODE_E]) camera->pos_y += move_speed;

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

        // draw_square_face();

        // Point q0 = {200 - 50, 200 - 50, 200 - 50};
        // Point q1 = {200 - 50, 200 - 50, 200 + 50};
        // Point q2 = {200 - 50, 200 + 50, 200 - 50};
        // Point q3 = {200 - 50, 200 + 50, 200 + 50};
        // Point q4 = {200 + 50, 200 - 50, 200 - 50};
        // Point q5 = {200 + 50, 200 - 50, 200 + 50};
        // Point q6 = {200 + 50, 200 + 50, 200 - 50};
        // Point q7 = {200 + 50, 200 + 50, 200 + 50};

        // SDL_Color green = {0, 255, 0, 255};

        // draw_line(q0, q1, green);
        // draw_line(q0, q2, green);
        // draw_line(q0, q4, green);
        // draw_line(q1, q3, green);
        // draw_line(q1, q5, green);
        // draw_line(q2, q3, green);
        // draw_line(q2, q6, green);
        // draw_line(q3, q7, green);
        // draw_line(q4, q5, green);
        // draw_line(q4, q6, green);
        // draw_line(q5, q7, green);
        // draw_line(q6, q7, green);

        // Sint16 a = (Sint16)1000000000000000;
        // Sint16 b = (Sint16)1000000000000000;

        // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // filledPolygonRGBA(renderer, &a, &b, 1000, 255, 255, 255, 255);

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
    Point rotated = apply_camera(p);

    float x_2d = origin->x + rotated.y + (-0.5f * rotated.x);
    float y_2d = origin->y - rotated.z + (0.5f * rotated.x);

    int radius = 2;
    SDL_Rect circle_rect = {(int)x_2d - radius, (int)y_2d - radius, radius * 2, radius * 2};

    SDL_SetTextureColorMod(circle_texture, color.r, color.g, color.b);
    SDL_RenderCopy(renderer, circle_texture, NULL, &circle_rect);
}

void draw_line(Point p1, Point p2, SDL_Color color) {
    Point rotated_p1 = apply_camera(p1);
    Point rotated_p2 = apply_camera(p2);

    float x1_2d = origin->x + rotated_p1.y + (-0.5f * rotated_p1.x);
    float y1_2d = origin->y - rotated_p1.z + (0.5f * rotated_p1.x);
    float x2_2d = origin->x + rotated_p2.y + (-0.5f * rotated_p2.x);
    float y2_2d = origin->y - rotated_p2.z + (0.5f * rotated_p2.x);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, (int)x1_2d, (int)y1_2d, (int)x2_2d, (int)y2_2d);
}

// void draw_square_face(Point p1, Point p2, Point p3, Point p4, SDL_Color color) {

// }

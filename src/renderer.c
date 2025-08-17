#include "renderer.h"

int SCREEN_WIDTH = 1200;
int SCREEN_HEIGHT = 800;
int GIZMO_SIZE = 10;

SDL_Color white = {255, 255, 255, 255};
SDL_Color red   = {255, 0, 0, 255};
SDL_Color blue  = {0, 0, 255, 255};
SDL_Color green = {0, 255, 0, 255};

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Point *origin = NULL;
SDL_Texture *circle_texture = NULL;
TTF_Font *font = NULL;
Camera *camera = NULL;

int init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 0;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF_Init failed: %s\n", TTF_GetError());
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

    font = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", 16);

    return 1;
}

void cleanup_sdl(void) {
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
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
    TTF_Quit();
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
    Vector scaled = scale(rotated, camera->zoom);

    return scaled;
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
    Vector p1 = {0, 0, GIZMO_SIZE};
    Vector p2 = {0, GIZMO_SIZE, 0};
    Vector p3 = {GIZMO_SIZE, 0, 0};

    int x_offset = -(SCREEN_WIDTH / 2) + 5 * GIZMO_SIZE;
    int y_offset = -(SCREEN_HEIGHT / 2) + 5 * GIZMO_SIZE;

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
    Vector scaled = apply_camera(p);
    SDL_Point point = dim_transform(scaled);
    int radius = 2;
    SDL_Rect circle_rect = {(int)point.x - radius, (int)point.y - radius, radius * 2, radius * 2};
    SDL_SetTextureColorMod(circle_texture, color.r, color.g, color.b);
    SDL_RenderCopy(renderer, circle_texture, NULL, &circle_rect);
}

void draw_line(Vector p1, Vector p2, SDL_Color color) {
    Vector scaled_p1 = apply_camera(p1);
    Vector scaled_p2 = apply_camera(p2);

    SDL_Point r1 = dim_transform(scaled_p1);
    SDL_Point r2 = dim_transform(scaled_p2);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, (int)r1.x, (int)r1.y, (int)r2.x, (int)r2.y);
}

void draw_line_moved(Vector p1, Vector p2, int x, int y, SDL_Color color) {
    Vector scaled_p1 = apply_camera(p1);
    Vector scaled_p2 = apply_camera(p2);

    SDL_Point r1 = dim_transform(scaled_p1);
    SDL_Point r2 = dim_transform(scaled_p2);

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

Vector scale(Vector v, double scalar) {
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

    Sint16 vx[4] = {r1.x, r2.x, r3.x, r4.x};
    Sint16 vy[4] = {r1.y, r2.y, r3.y, r4.y};

    filledPolygonRGBA(renderer, vx, vy, 4, color.r, color.g, color.b, color.a);
}

void draw_pyramid(Pyramid *p) {
    Vector top = {
        .x = 0,
        .y = 0,
        .z = (int)p->H
    };

    int half_B = (int)(p->B / 2);
    Vector fl = {.x = half_B, .y = half_B, .z = 0};  // Front-left
    Vector fr = {.x = -half_B, .y = half_B, .z = 0}; // Front-right
    Vector bl = {.x = half_B, .y = -half_B, .z = 0}; // Back-left
    Vector br = {.x = -half_B, .y = -half_B, .z = 0}; // Back-right

    Vector inner_top = {
        .x = 0,
        .y = 0,
        .z = (int)p->h
    };

    int half_b = (int)(p->b / 2);
    Vector inner_fl = {.x = half_b, .y = half_b, .z = 0};  // Inner front-left
    Vector inner_fr = {.x = -half_b, .y = half_b, .z = 0}; // Inner front-right
    Vector inner_bl = {.x = half_b, .y = -half_b, .z = 0}; // Inner back-left
    Vector inner_br = {.x = -half_b, .y = -half_b, .z = 0}; // Inner back-right

    draw_point(top, red);
    draw_point(fl, red);
    draw_point(fr, red);
    draw_point(bl, red);
    draw_point(br, red);

    draw_point(inner_top, blue);
    draw_point(inner_fl, blue);
    draw_point(inner_fr, blue);
    draw_point(inner_bl, blue);
    draw_point(inner_br, blue);

    draw_line(top, fl, white);
    draw_line(top, fr, white);
    draw_line(top, bl, white);
    draw_line(top, br, white);
    draw_line(fl, fr, white);
    draw_line(fr, br, white);
    draw_line(br, bl, white);
    draw_line(bl, fl, white);

    draw_line(inner_top, inner_fl, green);
    draw_line(inner_top, inner_fr, green);
    draw_line(inner_top, inner_bl, green);
    draw_line(inner_top, inner_br, green);
    draw_line(inner_fl, inner_fr, green);
    draw_line(inner_fr, inner_br, green);
    draw_line(inner_br, inner_bl, green);
    draw_line(inner_bl, inner_fl, green);
}

void draw_pyramid_cross_section(Pyramid *p) {
    SDL_Point top = {0 + SCREEN_WIDTH / 2, -(int)(p->H/2) * camera->zoom + SCREEN_HEIGHT / 2};
    SDL_Point left_base = {(int)((p->B / 2) * camera->zoom) + SCREEN_WIDTH / 2, (int)(p->H / 2) * camera->zoom + SCREEN_HEIGHT / 2};
    SDL_Point right_base = {-(int)((p->B / 2) * camera->zoom) + SCREEN_WIDTH / 2, (int)(p->H / 2) * camera->zoom + SCREEN_HEIGHT / 2};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, top.x, top.y, left_base.x, left_base.y);
    SDL_RenderDrawLine(renderer, top.x, top.y, right_base.x, right_base.y);
    SDL_RenderDrawLine(renderer, left_base.x, left_base.y, right_base.x, right_base.y);

    SDL_Point inner_top = {0 + SCREEN_WIDTH / 2, left_base.y - (int)(p->h * camera->zoom)};
    SDL_Point inner_left_base = {(int)((p->b / 2) * camera->zoom) + SCREEN_WIDTH / 2, left_base.y};
    SDL_Point inner_right_base = {-(int)((p->b / 2) * camera->zoom) + SCREEN_WIDTH / 2, right_base.y};

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, inner_top.x, inner_top.y, inner_left_base.x, inner_left_base.y);
    SDL_RenderDrawLine(renderer, inner_top.x, inner_top.y, inner_right_base.x, inner_right_base.y);
    SDL_RenderDrawLine(renderer, inner_left_base.x, inner_left_base.y, inner_right_base.x, inner_right_base.y);

    SDL_Color outer_color = {255, 255, 255, 255};
    SDL_Color inner_color = {0, 255, 0, 255};
    SDL_Color thickness_color = {255, 255, 0, 255};
    char label[50];

    sprintf(label, "H=%.1fm", p->H);
    draw_height_line(left_base.x + 100, top.y, left_base.y, label, outer_color);

    sprintf(label, "h=%.1fm", p->h);
    draw_height_line(left_base.x + 50, inner_top.y, inner_left_base.y, label, inner_color);

    sprintf(label, "B=%.1fm", p->B);
    draw_width_line(left_base.y + 40, left_base.x, right_base.x, label, outer_color);

    sprintf(label, "b=%.1fm", p->b);
    draw_width_line(left_base.y + 70, inner_left_base.x, inner_right_base.x, label, inner_color);

    sprintf(label, "t=%.1fm", p->t);
    draw_width_line(left_base.y + 70, inner_left_base.x, left_base.x, label, thickness_color);

    render_stats(p);
}

void render_stats(Pyramid *p) {
    SDL_Color header_color = {0, 255, 255, 255};    // Cyan for headers
    SDL_Color label_color = {255, 255, 255, 255};   // White for labels
    SDL_Color value_color = {255, 255, 0, 255};     // Yellow for values

    char stats_text[200];
    int text_x = 20;
    int text_y = 20;
    int line_height = 22;
    int indent = 15;

    // INPUT PARAMETERS
    sprintf(stats_text, "INPUT PARAMETERS");
    render_text(stats_text, text_x, text_y, header_color);
    text_y += line_height;

    sprintf(stats_text, "Outer Base Length (B, m):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->B);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Outer Height (H, m):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->H);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Scaling Factor (k):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.3f", p->k);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height + 5;

    // WALL THICKNESS & INNER PYRAMID
    sprintf(stats_text, "WALL THICKNESS & INNER PYRAMID");
    render_text(stats_text, text_x, text_y, header_color);
    text_y += line_height;

    sprintf(stats_text, "Wall Thickness (t, m):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->t);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Wall Thickness %% of Base:");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f%%", p->t_percent);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Inner Base Length (b, m):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->b);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Inner Height (h, m):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->h);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height + 5;

    // GEOMETRIC DIMENSIONS
    sprintf(stats_text, "GEOMETRIC DIMENSIONS");
    render_text(stats_text, text_x, text_y, header_color);
    text_y += line_height;

    sprintf(stats_text, "Outer Base Diagonal (m):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->d_outer);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Inner Base Diagonal (m):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->d_inner);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Outer Slant Height (m):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->s_outer);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Inner Slant Height (m):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->s_inner);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Outer Edge Length (m):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->l_outer);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Inner Edge Length (m):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->l_inner);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Outer Surface Area (m²):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.0f", p->A_outer);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Inner Surface Area (m²):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.0f", p->A_inner);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height + 5;

    // VOLUMETRIC DATA
    sprintf(stats_text, "VOLUMETRIC DATA");
    render_text(stats_text, text_x, text_y, header_color);
    text_y += line_height;

    sprintf(stats_text, "Total Volume (m³):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->V_total);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Hollow Volume (m³):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->V_hollow);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Stone Volume (m³):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->V_stone);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Hollow Volume %%:");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f%%", p->hollow_volume_percent);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Stone Volume %%:");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f%%", p->stone_volume_percent);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height + 5;

    // SLOPES
    sprintf(stats_text, "SLOPES");
    render_text(stats_text, text_x, text_y, header_color);
    text_y += line_height;

    sprintf(stats_text, "Face Slope Angle (°):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->slope_angle_face);
    render_text(stats_text, text_x + 250, text_y, value_color);
    text_y += line_height;

    sprintf(stats_text, "Edge Slope Angle (°):");
    render_text(stats_text, text_x + indent, text_y, label_color);
    sprintf(stats_text, "%.1f", p->slope_angle_edge);
    render_text(stats_text, text_x + 250, text_y, value_color);
}

void render_text(const char *text, int x, int y, SDL_Color color) {
    if (!font) return;

    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    if (!surface) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void draw_height_line(int x, int y1, int y2, const char *label, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x, y1, x, y2);

    int tick_length = 5;
    SDL_RenderDrawLine(renderer, x - tick_length, y1, x + tick_length, y1);
    SDL_RenderDrawLine(renderer, x - tick_length, y2, x + tick_length, y2);

    if (label) {
        render_text(label, x + 10, (y1 + y2) / 2 - 8, color);
    }
}

void draw_width_line(int y, int x1, int x2, const char *label, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y, x2, y);

    int tick_length = 5;
    SDL_RenderDrawLine(renderer, x1, y - tick_length, x1, y + tick_length);
    SDL_RenderDrawLine(renderer, x2, y - tick_length, x2, y + tick_length);

    if (label) {
        render_text(label, (x1 + x2) / 2 - 20, y + 10, color);
    }
}

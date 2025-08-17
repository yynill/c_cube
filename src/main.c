#include "main.h"

Pyramid *init_pyramid(double B, double H, double k) {
    Pyramid *p = malloc(sizeof(Pyramid));
    p->B = B;
    p->H = H;
    p->k = k;

    p->b = p->k * p->B;
    p->h = p->k * p->H;

    p->t = (p->B - p->b) / 2.0;
    p->t_percent = (p->t / p->B) * 100.0;
    p->V_total = (1.0 / 3.0) * p->B * p->B * p->H;
    p->V_hollow = p->k * p->k * p->V_total;
    p->V_stone = p->V_total - p->V_hollow;
    p->hollow_volume_percent = p->V_hollow / p->V_total * 100;
    p->stone_volume_percent = p->V_stone / p->V_total * 100;
    p->slope_angle_face = DEG_PER_RAD * atan(2.0 * H / B);
    p->slope_angle_edge = DEG_PER_RAD * atan((2.0 * H) / (B * SQRT_2));
    p->d_outer = p->B * SQRT_2;
    p->d_inner = p->b * SQRT_2;
    p->s_outer = sqrt(p->H * p->H + (p->B / 2.0) * (p->B / 2.0));
    p->s_inner = p->k * p->s_outer;
    p->l_outer = sqrt(p->H * p->H + (p->B * p->B / 2.0));
    p->l_inner = p->k * p->l_outer;
    p->A_inner = p->b * p->b;
    p->A_outer = p->B * p->B;

    return p;
}

void print_pyramid_properties(Pyramid *p) {
    printf("\n%s=== Pyramid Properties ===%s\n", COLOR_HEADER, COLOR_RESET);

    // INPUTS
    printf("\n%s[INPUT PARAMETERS]%s\n", COLOR_SECTION, COLOR_RESET);
    printf("%s  %-35s%s %.1f%s\n", COLOR_LABEL, "Outer Base Length (B, m):", COLOR_VALUE, p->B, COLOR_RESET);
    printf("%s  %-35s%s %.1f%s\n", COLOR_LABEL, "Outer Height (H, m):", COLOR_VALUE, p->H, COLOR_RESET);
    printf("%s  %-35s%s %.3f%s\n", COLOR_LABEL, "Scaling Factor (k):", COLOR_VALUE, p->k, COLOR_RESET);

    // THICKNESS & INTERNAL SIZE
    printf("\n%s[WALL THICKNESS & INNER PYRAMID]%s\n", COLOR_SECTION, COLOR_RESET);
    printf("%s  %-35s%s %.1f%s\n", COLOR_LABEL, "Wall Thickness (t, m):", COLOR_VALUE, p->t, COLOR_RESET);
    printf("%s  %-35s%s %.1f%%%s\n", COLOR_LABEL, "Wall Thickness %% of Base:", COLOR_VALUE, p->t_percent, COLOR_RESET);
    printf("%s  %-35s%s %.1f%s\n", COLOR_LABEL, "Inner Base Length (b, m):", COLOR_VALUE, p->b, COLOR_RESET);
    printf("%s  %-35s%s %.1f%s\n", COLOR_LABEL, "Inner Height (h, m):", COLOR_VALUE, p->h, COLOR_RESET);

    // GEOMETRY
    printf("\n%s[GEOMETRIC DIMENSIONS]%s\n", COLOR_SECTION, COLOR_RESET);
    printf("%s  %-35s%s %.1f%s\n", COLOR_LABEL, "Outer Base Diagonal (m):", COLOR_VALUE, p->d_outer, COLOR_RESET);
    printf("%s  %-35s%s %.1f%s\n", COLOR_LABEL, "Inner Base Diagonal (m):", COLOR_VALUE, p->d_inner, COLOR_RESET);
    printf("%s  %-35s%s %.1f%s\n", COLOR_LABEL, "Outer Slant Height (m):", COLOR_VALUE, p->s_outer, COLOR_RESET);
    printf("%s  %-35s%s %.1f%s\n", COLOR_LABEL, "Inner Slant Height (m):", COLOR_VALUE, p->s_inner, COLOR_RESET);
    printf("%s  %-35s%s %.1f%s\n", COLOR_LABEL, "Outer Edge Length (m):", COLOR_VALUE, p->l_outer, COLOR_RESET);
    printf("%s  %-35s%s %.1f%s\n", COLOR_LABEL, "Inner Edge Length (m):", COLOR_VALUE, p->l_inner, COLOR_RESET);
    printf("%s  %-36s%s %.0f%s\n", COLOR_LABEL, "Outer Surface Area (m²):", COLOR_VALUE, p->A_outer, COLOR_RESET);
    printf("%s  %-36s%s %.0f%s\n", COLOR_LABEL, "Inner Surface Area (m²):", COLOR_VALUE, p->A_inner, COLOR_RESET);

    // VOLUMES
    printf("\n%s[VOLUMETRIC DATA]%s\n", COLOR_SECTION, COLOR_RESET);
    printf("%s  %-36s%s %.1f%s\n", COLOR_LABEL, "Total Volume (m³):", COLOR_VALUE, p->V_total, COLOR_RESET);
    printf("%s  %-36s%s %.1f%s\n", COLOR_LABEL, "Hollow Volume (m³):", COLOR_VALUE, p->V_hollow, COLOR_RESET);
    printf("%s  %-36s%s %.0f%s\n", COLOR_LABEL, "Stone Volume (m³):", COLOR_VALUE, p->V_stone, COLOR_RESET);
    printf("%s  %-35s%s %.1f%%%s\n", COLOR_LABEL, "Hollow Volume %%:", COLOR_VALUE, p->hollow_volume_percent, COLOR_RESET);
    printf("%s  %-35s%s %.1f%%%s\n", COLOR_LABEL, "Stone Volume %%:", COLOR_VALUE, p->stone_volume_percent, COLOR_RESET);

    // SLOPES
    printf("\n%s[SLOPES]%s\n", COLOR_SECTION, COLOR_RESET);
    printf("%s  %-36s%s %.1f%s\n", COLOR_LABEL, "Face Slope Angle (°):", COLOR_VALUE, p->slope_angle_face, COLOR_RESET);
    printf("%s  %-36s%s %.1f%s\n", COLOR_LABEL, "Edge Slope Angle (°):", COLOR_VALUE, p->slope_angle_edge, COLOR_RESET);

    printf("\n");
}

int main() {
    Pyramid *p = init_pyramid(100, 100, 0.5);    // 100
    // Pyramid *p = init_pyramid(230, 138.5, 0.5);     // giza
    // Pyramid *p = init_pyramid(460, 297, 0.5);     // 2 * giza
    // Pyramid *p = init_pyramid(144.0, 89.0, 0.5); // own
    // Pyramid *p = init_pyramid(144.0, 89.0, 0.5); // biggest building
    print_pyramid_properties(p);

    if (!init_sdl()) return 1;

    int running = 1;
    int view_mode = 0; // 0 = 3D view, 1 = 2D cross-section view
    SDL_Event event;
    const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE) view_mode = !view_mode;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        float zoom_speed = 0.01f;
        if (keyboard_state[SDL_SCANCODE_UP]) camera->zoom += zoom_speed;
        if (keyboard_state[SDL_SCANCODE_DOWN]) camera->zoom -= zoom_speed;
        if (camera->zoom <= 0) camera->zoom = 0;

        if (view_mode == 0) {
            draw_gizmo();
            draw_pyramid(p);

            float rotation_speed = 0.05f;

            if (keyboard_state[SDL_SCANCODE_X]) {
                if (keyboard_state[SDL_SCANCODE_RIGHT]) camera->rotation_x += rotation_speed;
                if (keyboard_state[SDL_SCANCODE_LEFT]) camera->rotation_x -= rotation_speed;
            }
            if (keyboard_state[SDL_SCANCODE_Y]) {
                if (keyboard_state[SDL_SCANCODE_RIGHT]) camera->rotation_y += rotation_speed;
                if (keyboard_state[SDL_SCANCODE_LEFT]) camera->rotation_y -= rotation_speed;
            }
            if (keyboard_state[SDL_SCANCODE_Z]) {
                if (keyboard_state[SDL_SCANCODE_RIGHT]) camera->rotation_z += rotation_speed;
                if (keyboard_state[SDL_SCANCODE_LEFT]) camera->rotation_z -= rotation_speed;
            }

        } else if (view_mode == 1) {
            draw_pyramid_cross_section(p);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    cleanup_sdl();
    return 0;
}

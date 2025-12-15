//
#include "config.h"
#include "sand.h"
#include "ui.h"
#include "render.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static uint8_t* grid_ptr = NULL;
static struct menu* Moption_ptr = NULL;

int draw_grid_or_rulers_BUT_NOT_THOSE_FUNCTIONS = 0;

//helper functions

void clear_screen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void draw_cursor(int x, int y) {
    SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);
    SDL_FRect c = { x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE };
    SDL_RenderFillRect(renderer, &c);
}

void draw_rulers() {
    SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255);
    for (int x = 0; x < GRID_W; x++)
        SDL_RenderLine(
            renderer,
            x * GRID_SIZE, 0, x * GRID_SIZE, WIN_H
        );
    for (int y = 0; y < GRID_H; y++)
        SDL_RenderLine(
            renderer,
            0, y * GRID_SIZE, WIN_W, y * GRID_SIZE
        );
}

int draw_grid(uint8_t *GRID) {
    if (GRID == NULL)
        return 0;
    for (int x = 0; x < GRID_W; x++)
        for (int y = 0; y < GRID_H; y++) {
            SDL_FRect rect = {
                x * GRID_SIZE,
                y * GRID_SIZE,
                GRID_SIZE,
                GRID_SIZE
            };
            switch (GRID[GRID_INDEX(x, y)]) {
            case GRID_SAND:
                SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
                break;
            default:
                SDL_SetRenderDrawColor(renderer, 0, x, y, 255);
                break;
            };
            if (GRID[GRID_INDEX(x, y)] != GRID_AIR)
                SDL_RenderFillRect(renderer, &rect);
        }
    return 1;
}

#define MENU_TXT_PADDING 5
int draw_menu(struct menu* MENU) {
    if (MENU == NULL)
        return 0;

    //render menu
    SDL_FRect m = {
        MENU->pos[0],
        MENU->pos[1],
        MENU->size[0],
        MENU->size[1]
    };
    SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
    SDL_RenderFillRect(renderer, &m);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &m);
    char* m_txt = MENU->title;
    int m_txt_w = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * strlen(m_txt);
    SDL_RenderDebugText(renderer, m.x + (m.w / 2) - (m_txt_w / 2), m.y + MENU_TXT_PADDING, m_txt);

    //render buttons
    for (int i = 0; i < MENU->buttons_num; i++) {
        SDL_FRect b = {
            MENU->pos[0] + MENU->buttons_ptr[i].offset[0],
            MENU->pos[1] + MENU->buttons_ptr[i].offset[1],
            MENU->buttons_ptr[i].size[0],
            MENU->buttons_ptr[i].size[1]
        };
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderFillRect(renderer, &b);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &b);
        char* b_txt = MENU->buttons_ptr[i].txt;
        SDL_RenderDebugText(renderer, b.x + MENU_TXT_PADDING, b.y + MENU_TXT_PADDING, b_txt);
    }
}

void render_toggleGrid() {
    draw_grid_or_rulers_BUT_NOT_THOSE_FUNCTIONS = !draw_grid_or_rulers_BUT_NOT_THOSE_FUNCTIONS;
}

int render_Init() {
    if (!SDL_CreateWindowAndRenderer("Hello World", WIN_W, WIN_H, SDL_WINDOW_EXTERNAL, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    grid_ptr = grid_returnPtr();
    if (grid_ptr == NULL) {
        SDL_Log("Couldn't get grid ptr.");
        return SDL_APP_FAILURE;
    }
    Moption_ptr = ui_returnOptionPTR();
    if (Moption_ptr == NULL) {
        SDL_Log("Coulnt get menu (option) ptr.");
        return SDL_APP_FAILURE;
    }
}

void render_Loop() {
    clear_screen();

    if (!draw_grid(grid_ptr))
        SDL_Log("Failed to render grid, faulty pointer.");
    //draw_cursor(0, 0);
    if (draw_grid_or_rulers_BUT_NOT_THOSE_FUNCTIONS)
        draw_rulers();
    if (!draw_menu(Moption_ptr))
        SDL_Log("Failed to render options menu, faulty pointer");

    SDL_RenderPresent(renderer);
}
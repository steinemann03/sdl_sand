#include "config.h"

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "sand.h"
#include "ui.h"
#include "render.h"

int pause = 0;

//main
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    grid_Init(); //so far grid only uses stack and BSS
    if (!render_Init()) {
        SDL_Log("Failed to init RENDER component.");
        return SDL_APP_FAILURE;
    }
    if (!ui_Init()) {
        SDL_Log("Failed to init UI component.");
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    ui_EventHandle(event);

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.key == SDLK_ESCAPE)
            return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    //input
    {
        ui_Loop();
    }

    //logic
    if (!pause) {
        const int grid_Loop_tick_max = 64;
        static int grid_Loop_tick = 0;
        grid_Loop_tick++;
        if (grid_Loop_tick > grid_Loop_tick_max) {
            grid_Loop();
            grid_Loop_tick = 0;
        }
    }

    //render
    {
        render_Loop();
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}

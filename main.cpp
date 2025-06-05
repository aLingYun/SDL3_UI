/*
 * This example creates an SDL window and renderer, and then draws some text
 * using SDL_RenderDebugText() every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <cstdio>
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "lib/button.h"

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Button *button = NULL;

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Debug Texture", "1.0", "com.example.renderer-debug-text");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/debug-text", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_FRect background = {100, 100, 64, 24};
    button = new Button(renderer, "Button", background);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    if( event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        //获得鼠标坐标
        SDL_FPoint mousePosition = {event->motion.x, event->motion.y};
        if (button->isClicked(mousePosition)) {
            button->modifyText("Clicked!");
        }
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    const int charsize = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;

    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */

    button->update();

    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    delete button;
    /* SDL will clean up the window/renderer for us. */
}

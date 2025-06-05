/*
 * This example creates an SDL window and renderer, and then draws some text
 * using SDL_RenderDebugText() every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_oldnames.h>
#include <cstdint>
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "lib/button.h"

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Button *startButton = NULL;
static Button *exitButton = NULL;
static Button *backButton = NULL;
static uint16_t frameCount = 0;

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

    SDL_FRect startButtonRect = {(WINDOW_WIDTH - 100) / 2, WINDOW_HEIGHT * 1 / 4, 100, 32};
    startButton = new Button(renderer, "Start", startButtonRect);
    SDL_FRect exitButtonRect = {(WINDOW_WIDTH - 100) / 2, WINDOW_HEIGHT * 2 / 4, 100, 32};
    exitButton = new Button(renderer, "Exit", exitButtonRect);
    SDL_FRect backButtonRect = {WINDOW_WIDTH - 100 - 10, WINDOW_HEIGHT - 32 - 10, 100, 32};
    backButton = new Button(renderer, "Back", backButtonRect);

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
        if (startButton->isClicked(mousePosition)) {
            frameCount = 1;
            SDL_Log("Game Start! Frame %d", frameCount);
        } else if (exitButton->isClicked(mousePosition)) {
            return SDL_APP_SUCCESS;
        } else if (backButton->isClicked(mousePosition)) {
            frameCount = 0;
            SDL_Log("Back! Frame %d", frameCount);
        }
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    switch (frameCount) {
        case 0:
            /* as you can see from this, rendering draws over whatever was drawn before it. */
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
            SDL_RenderClear(renderer);  /* start with a blank canvas. */
            startButton->update();
            exitButton->update();
            break;
        case 1:
            /* as you can see from this, rendering draws over whatever was drawn before it. */
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  /* white, full alpha */
            SDL_RenderClear(renderer);  /* start with a blank canvas. */
            backButton->update();
            break;
        default:
            frameCount = 1;
            break;
    }

    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    delete startButton;
    delete exitButton;
    /* SDL will clean up the window/renderer for us. */
}

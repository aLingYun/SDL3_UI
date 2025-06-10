/*
 * This example creates an SDL window and renderer, and then draws some text
 * using SDL_RenderDebugText() every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "lib/button.h"
#include "lib/image_show.h"

#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 900
#define TANK_SIZE 50

class GameMap {
public:
    GameMap(SDL_Renderer* renderer, ImageShow* picture) {
        this->renderer = renderer;
        for (int x = 0; x < WINDOW_WIDTH/TANK_SIZE; x++) {
            for (int y = 0; y < WINDOW_HEIGHT/TANK_SIZE; y++) {
                this->picture[x][y] = picture;
            }
        }
    }

    void setFromConfigFile(std::string configFile) {

    }
    void show() {
        for (int x = 0; x < WINDOW_WIDTH/TANK_SIZE; x++) {
            for (int y = 0; y < WINDOW_HEIGHT/TANK_SIZE; y++) {
                SDL_FRect tmp_rect = {x * (float)TANK_SIZE, y * (float)TANK_SIZE, TANK_SIZE, TANK_SIZE};
                if (this->picture[x][y] == NULL) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &tmp_rect);
                } else {
                    this->picture[x][y]->update(&tmp_rect);
                }
            }
        }
    }

private:
    SDL_Renderer* renderer;
    ImageShow* picture[WINDOW_WIDTH/TANK_SIZE][WINDOW_HEIGHT/TANK_SIZE];
};


/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Button *startButton = NULL;
static Button *exitButton = NULL;
static Button *backButton = NULL;
static uint16_t frameCount = 0;

static ImageShow* frame0Background = NULL;
static ImageShow* frame1Tank = NULL;
static ImageShow* frame1Wall = NULL;

static GameMap* gameMap = NULL;

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

    SDL_FRect startButtonRect = {((float)WINDOW_WIDTH - 100) / 2, (float)WINDOW_HEIGHT * 1 / 4, 100, 32};
    startButton = new Button(renderer, "Start", startButtonRect);
    SDL_FRect exitButtonRect = {((float)WINDOW_WIDTH - 100) / 2, (float)WINDOW_HEIGHT * 2 / 4, 100, 32};
    exitButton = new Button(renderer, "Exit", exitButtonRect);
    SDL_FRect backButtonRect = {WINDOW_WIDTH - 100 - 10, WINDOW_HEIGHT - 32 - 10, 100, 32};
    backButton = new Button(renderer, "Back", backButtonRect);
    SDL_FRect srcRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_FRect dstRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    frame0Background = new ImageShow("res/start.jpg", renderer, srcRect, dstRect);
    srcRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    dstRect = {(float)(WINDOW_WIDTH - 4*TANK_SIZE) / 2 - TANK_SIZE, WINDOW_HEIGHT - TANK_SIZE, TANK_SIZE, TANK_SIZE};
    frame1Tank = new ImageShow("res/tank_1.png", renderer, srcRect, dstRect);
    srcRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    dstRect = {(float)(WINDOW_WIDTH - 4*TANK_SIZE) / 2, WINDOW_HEIGHT - TANK_SIZE, TANK_SIZE, TANK_SIZE};
    frame1Wall = new ImageShow("res/wall.png", renderer, srcRect, dstRect);
    gameMap = new GameMap(renderer, frame1Tank);

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
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.key) {
            case SDLK_DOWN:
                frame1Tank->downMove();
                break;
            case SDLK_UP:
                frame1Tank->upMove();
                break;
            case SDLK_LEFT:
                frame1Tank->leftMove();
                break;
            case SDLK_RIGHT:
                frame1Tank->rightMove();
                break;
            default:
                break;
        }
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */
    switch (frameCount) {
        case 0:
            frame0Background->update();
            startButton->update();
            exitButton->update();
            backButton->setHide(true);
            break;
        case 1:
            startButton->setHide(true);
            exitButton->setHide(true);
            gameMap->show();
            SDL_FRect wallDstRects[8];
            wallDstRects[0] = {(float)(WINDOW_WIDTH - 4*TANK_SIZE) / 2, WINDOW_HEIGHT - TANK_SIZE, TANK_SIZE, TANK_SIZE};
            wallDstRects[1] = {(float)(WINDOW_WIDTH - 4*TANK_SIZE) / 2, WINDOW_HEIGHT - 2*TANK_SIZE, TANK_SIZE, TANK_SIZE};
            wallDstRects[2] = {(float)(WINDOW_WIDTH - 4*TANK_SIZE) / 2, WINDOW_HEIGHT - 3*TANK_SIZE, TANK_SIZE, TANK_SIZE};
            wallDstRects[3] = {(float)(WINDOW_WIDTH - 4*TANK_SIZE) / 2 + TANK_SIZE, WINDOW_HEIGHT - 3*TANK_SIZE, TANK_SIZE, TANK_SIZE};
            wallDstRects[4] = {(float)(WINDOW_WIDTH - 4*TANK_SIZE) / 2 + 2*TANK_SIZE, WINDOW_HEIGHT - 3*TANK_SIZE, TANK_SIZE, TANK_SIZE};
            wallDstRects[5] = {(float)(WINDOW_WIDTH - 4*TANK_SIZE) / 2 + 3*TANK_SIZE, WINDOW_HEIGHT - 3*TANK_SIZE, TANK_SIZE, TANK_SIZE};
            wallDstRects[6] = {(float)(WINDOW_WIDTH - 4*TANK_SIZE) / 2 + 3*TANK_SIZE, WINDOW_HEIGHT - 2*TANK_SIZE, TANK_SIZE, TANK_SIZE};
            wallDstRects[7] = {(float)(WINDOW_WIDTH - 4*TANK_SIZE) / 2 + 3*TANK_SIZE, WINDOW_HEIGHT - TANK_SIZE, TANK_SIZE, TANK_SIZE};
            frame1Wall->updateMulti(wallDstRects, sizeof(wallDstRects) / sizeof(SDL_FRect));
            backButton->update();
            frame1Tank->update();
            break;
        default:
            frameCount = 0;
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
    delete backButton;
    delete frame1Wall;
    delete frame1Tank;
    delete frame0Background;
    delete gameMap;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    /* SDL will clean up the window/renderer for us. */
}

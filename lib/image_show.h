#ifndef IMAGE_SHOW_H
#define IMAGE_SHOW_H
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL.h>
#include <cstdint>
#include <iostream>

class ImageShow {
public:
    ImageShow(std::string fileName, SDL_Renderer* renderer, SDL_FRect srcRect, SDL_FRect dstRect);
    void update();
    void update(SDL_FRect* dstRect);
    void updateMulti(SDL_FRect dstRect[], uint16_t len);
    void leftMove();
    void rightMove();
    void upMove();
    void downMove();
    ~ImageShow();

private:
    std::string fileName;
    SDL_FRect srcRect;
    SDL_FRect dstRect;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
};

#endif

#include "image_show.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_oldnames.h>
#include <sys/types.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

ImageShow::~ImageShow() {
    SDL_DestroyTexture(this->texture);
}

ImageShow::ImageShow(std::string fileName, SDL_Renderer* renderer, SDL_FRect srcRect, SDL_FRect dstRect) {
    this->fileName = fileName;
    this->srcRect = srcRect;
    this->dstRect = dstRect;
    this->renderer = renderer;
    this->texture = IMG_LoadTexture(renderer, fileName.c_str());
    if (this->texture == NULL) {
        printf("Failed to load texture: %s\n", SDL_GetError());
    }
}

void ImageShow::update() {
    float width, height;
    if (SDL_GetTextureSize(this->texture, &width, &height) == false) {
        SDL_Log("Failed to get texture size: %s. W=%f, H=%f\n", SDL_GetError(), width, height);
    } else {
        this->srcRect.w = width;
        this->srcRect.h = height;
    }
    /* Display the image */
    if (SDL_RenderTexture(this->renderer, this->texture, &this->srcRect, &this->dstRect) == false) {
        printf("Failed to render texture: %s\n", SDL_GetError());
    }
    // SDL_RenderPresent(this->renderer);
}

void ImageShow::updateMulti(SDL_FRect dstRect[], uint16_t len) {
    float width, height;
    if (SDL_GetTextureSize(this->texture, &width, &height) == false) {
        SDL_Log("Failed to get texture size: %s. W=%f, H=%f\n", SDL_GetError(), width, height);
    } else {
        this->srcRect.w = width;
        this->srcRect.h = height;
    }
    /* Display the image */
    for (int i = 0; i < len; i++) {
        if (SDL_RenderTexture(this->renderer, this->texture, &this->srcRect, &dstRect[i]) == false) {
            printf("Failed to render texture: %s\n", SDL_GetError());
        }
    }
    // SDL_RenderPresent(this->renderer);
}

void ImageShow::leftMove() {
    this->dstRect.x -= 10;
}

void ImageShow::upMove() {
    this->dstRect.y -= 10;
}

void ImageShow::downMove() {
    this->dstRect.y += 10;
}

void ImageShow::rightMove() {
    this->dstRect.x += 10;
}

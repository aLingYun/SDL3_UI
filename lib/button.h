#ifndef BUTTON_H
#define BUTTON_H
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <iostream>

class Button {
public:
    Button(SDL_Renderer* renderer, const std::string& text, const SDL_FRect& background);
    bool isClicked(const SDL_FPoint& mousePosition);
    void modifyText(const std::string& newText);
    void update();
    ~Button();

private:
    SDL_Renderer* renderer;
    std::string text;
    SDL_FRect background;
};

#endif

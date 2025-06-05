#include "button.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL.h>

bool Button::isClicked(const SDL_FPoint& mousePosition) {
    return background.x <= mousePosition.x && mousePosition.x <= background.x + background.w &&
           background.y <= mousePosition.y && mousePosition.y <= background.y + background.h;
}

Button::Button(SDL_Renderer* renderer, const std::string& text, const SDL_FRect& background) {
    this->text = text;
    this->background = background;
    this->renderer = renderer;
}

void Button::modifyText(const std::string& newText) {
    this->text = newText;
}

void Button::update() {
    SDL_SetRenderDrawColor(this->renderer, 55, 155, 155, SDL_ALPHA_OPAQUE);  /* white, full alpha */
    SDL_RenderFillRect(this->renderer, &this->background);
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* green, full alpha */
    SDL_RenderDebugText(this->renderer, (float)((this->background.w - (SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * this->text.length())) / 2 + this->background.x),
        (float)(this->background.h / 2 + this->background.y - 3), this->text.c_str());
}

Button::~Button() {}

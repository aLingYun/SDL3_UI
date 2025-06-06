#include "button.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL.h>

bool Button::isClicked(const SDL_FPoint& mousePosition) {
    if (this->hide) return false;
    return background.x <= mousePosition.x && mousePosition.x <= background.x + background.w &&
           background.y <= mousePosition.y && mousePosition.y <= background.y + background.h;
}

Button::Button(SDL_Renderer* renderer, const std::string& text, const SDL_FRect& background) {
    this->text = text;
    this->background = background;
    this->renderer = renderer;
    this->hide = false;
}

void Button::modifyText(const std::string& newText) {
    this->text = newText;
}

void Button::setHide(bool hide) {
    this->hide = hide;
}

void Button::update() {
    this->hide = false;
    // SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);  /* white, full alpha */
    // SDL_RenderFillRect(this->renderer, &this->background);
    SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);  /* red, full alpha */
    // SDL_SetRenderScale(this->renderer, 4.0f, 4.0f);
    SDL_RenderDebugText(this->renderer, (float)((this->background.w - (SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * this->text.length())) / 2 + this->background.x),
        (float)(this->background.h / 2 + this->background.y - 3), this->text.c_str());
    // SDL_SetRenderScale(this->renderer, 1.0f, 1.0f);
}

Button::~Button() {}

#pragma once

#include "../ECS/ECS.h"
#include "SDL2/SDL.h"

#include "../EventBus/Event.h"

class KeyPressedEvent: public Event {
public:
    SDL_Keycode symbol;
    KeyPressedEvent(SDL_Keycode symbol): symbol(symbol) {}
};
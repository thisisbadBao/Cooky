#pragma once

#include "../ECS/ECS.h"
#include "../System/CallbackEventSystem.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"

class KeyboardSystem: public System {
public:
    KeyboardSystem() {
        keyName.resize(SDL_NUM_SCANCODES);

        keyName[SDLK_0] = '0';
        keyName[SDLK_1] = '1';
        keyName[SDLK_2] = '2';
        keyName[SDLK_3] = '3';
        keyName[SDLK_4] = '4';
        keyName[SDLK_5] = '5';
        keyName[SDLK_6] = '6';
        keyName[SDLK_7] = '7';
        keyName[SDLK_8] = '8';
        keyName[SDLK_9] = '9';

        keyName[SDLK_a] = 'a';
        keyName[SDLK_b] = 'b';
        keyName[SDLK_c] = 'c';
        keyName[SDLK_d] = 'd';
        keyName[SDLK_e] = 'e';
        keyName[SDLK_f] = 'f';
        keyName[SDLK_g] = 'g';
        keyName[SDLK_h] = 'h';
        keyName[SDLK_i] = 'i';
        keyName[SDLK_j] = 'j';
        keyName[SDLK_k] = 'k';
        keyName[SDLK_l] = 'l';
        keyName[SDLK_m] = 'm';
        keyName[SDLK_n] = 'n';
        keyName[SDLK_o] = 'o';
        keyName[SDLK_p] = 'p';
        keyName[SDLK_q] = 'q';
        keyName[SDLK_r] = 'r';
        keyName[SDLK_s] = 's';
        keyName[SDLK_t] = 't';
        keyName[SDLK_u] = 'u';
        keyName[SDLK_v] = 'v';
        keyName[SDLK_w] = 'w';
        keyName[SDLK_x] = 'x';
        keyName[SDLK_y] = 'y';
        keyName[SDLK_z] = 'z';

        keyName[SDL_SCANCODE_RIGHT] = "Right";
        keyName[SDL_SCANCODE_LEFT] = "Left";
        keyName[SDL_SCANCODE_DOWN] = "Down";
        keyName[SDL_SCANCODE_UP] = "Up";

        keyName[SDL_SCANCODE_F1] = "F1";
        keyName[SDL_SCANCODE_F2] = "F2";
        keyName[SDL_SCANCODE_F3] = "F3";
        keyName[SDL_SCANCODE_F4] = "F4";
        keyName[SDL_SCANCODE_F5] = "F5";
        keyName[SDL_SCANCODE_F6] = "F6";
        keyName[SDL_SCANCODE_F7] = "F7";
        keyName[SDL_SCANCODE_F8] = "F8";
        keyName[SDL_SCANCODE_F9] = "F9";
        keyName[SDL_SCANCODE_F10] = "F10";
        keyName[SDL_SCANCODE_F11] = "F11";
        keyName[SDL_SCANCODE_F12] = "F12";

        keyName[SDL_SCANCODE_LCTRL] = "LCTRL";
        keyName[SDL_SCANCODE_LSHIFT] = "LSHIFT";
        keyName[SDL_SCANCODE_LALT] = "LALT";
        keyName[SDL_SCANCODE_LGUI] = "LGUI";
        keyName[SDL_SCANCODE_RCTRL] = "RCTRL";
        keyName[SDL_SCANCODE_RSHIFT] = "RSHIFT";
        keyName[SDL_SCANCODE_RALT] = "RALT";
        keyName[SDL_SCANCODE_RGUI] = "RGUI";
    }

    void HandleKeyEvent(SDL_Event& event, const std::unique_ptr<Registry>& registry) {
        if (Game::isPaused) return;
        switch (event.type) {
            case SDL_KEYDOWN: {
                int key;
                if ((event.key.keysym.sym & SDLK_SCANCODE_MASK) == 0) {
                    // for key code
                    key = event.key.keysym.sym;
                } else {
                    // for scan code
                    key = event.key.keysym.sym & ~SDLK_SCANCODE_MASK;
                }
                if (!keyStatusMap[keyName[key]]) {
                    keyStatusMap[keyName[key]] = true;
                    std::string eventName = keyName[key] + "_down";
                    registry->entityEventBus->EmitEvent<CallbackEvent>(eventName);
                }
                break;
            }
            case SDL_KEYUP: {
                int key;
                if ((event.key.keysym.sym & SDLK_SCANCODE_MASK) == 0) {
                    key = event.key.keysym.sym;
                } else {
                    key = event.key.keysym.sym & ~SDLK_SCANCODE_MASK;
                }
                keyStatusMap[keyName[key]] = false;
                std::string eventName = keyName[key] + "_up";
                registry->entityEventBus->EmitEvent<CallbackEvent>(eventName);
                break;
            }
        }
    }

    bool IsKeyDown(std::string keyName) {
        if (keyStatusMap.find(keyName) != keyStatusMap.end()) {
            return keyStatusMap[keyName];
        }
        return false;
    }


    void Update() {

    }

private:
    std::vector<std::string> keyName;
    std::map<std::string, bool> keyStatusMap;
};
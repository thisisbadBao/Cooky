#pragma once

#include "../ECS/ECS.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidBodyComponent.h"

#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"

class KeyboardControlSystem: public System {
public:
    KeyboardControlSystem() {
        RequireComponent<KeyboardControlComponent>();
        RequireComponent<SpriteComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void SubscribeToEvent(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
    }

    void OnKeyPressed(KeyPressedEvent& event) {
        std::string keyCode = std::to_string(event.symbol);
        std::string keySymbol(1, event.symbol);
        Logger::LogD("Key pressed event emitted: [" + keyCode + "] " + keySymbol);
        for (auto entity : GetSystemEntities()) {
            const auto keyboardControl = entity.GetComponent<KeyboardControlComponent>();
            auto& sprite = entity.GetComponent<SpriteComponent>();
            auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

            switch (event.symbol)
            {
            case SDLK_UP:
                rigidbody.velocity = keyboardControl.upVelocity;
                sprite.srcRect.y = sprite.height * 0;
                break;
            case SDLK_RIGHT:
                rigidbody.velocity = keyboardControl.rightVelocity;
                sprite.srcRect.y = sprite.height * 1;
                break;
            case SDLK_DOWN:
                rigidbody.velocity = keyboardControl.downVelocity;
                sprite.srcRect.y = sprite.height * 2;
                break;
            case SDLK_LEFT:
                rigidbody.velocity = keyboardControl.leftVelocity;
                sprite.srcRect.y = sprite.height * 3;
                break;
            default:
                break;
            }
        }
    }

    void Update() {

    }
};
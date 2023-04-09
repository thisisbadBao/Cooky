#pragma once

#include <box2d/box2d.h>
#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../Game/Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

const int MET2PIX = 80;

const int SCALED_WIDTH = 1200 / MET2PIX;  // 15
const int SCALED_HEIGHT = 900 / MET2PIX;  // 11.25

//1 rad × 180/π = 57,296°
const float RAD2DEG = 180 / M_PI;

class PhysicsSystem: public System {
public:
    PhysicsSystem() {
        RequireComponent<RigidBodyComponent>();
        RequireComponent<BoxColliderComponent>();
        RequireComponent<TransformComponent>();
        world = new b2World(b2Vec2(0.0f, 9.81f));
    }

    void Update(SDL_Renderer* renderer) {
        for (auto entity : GetSystemEntities()) {
            auto& rb = entity.GetComponent<RigidBodyComponent>();
            auto& transform = entity.GetComponent<TransformComponent>();
            if (!rb.isInit) {

                b2PolygonShape dynamicBox;
                dynamicBox.SetAsBox((100 / 2.0f) - dynamicBox.m_radius,
                    (100 / 2.0f) - dynamicBox.m_radius);
                rb.fixtureDef.shape = &dynamicBox;

                boxBody = world->CreateBody(&rb.bodyDef);
                boxBody->SetLinearVelocity(b2Vec2(rb.vel.x, rb.vel.y));
                boxBody->CreateFixture(&rb.fixtureDef);
                rb.isInit = true;
            }
            b2Vec2 pos = boxBody->GetPosition();
            transform.position = Vec2(pos.x * MET2PIX, pos.y * MET2PIX);
        }

        world->Step(1.0f / 60.0f, 6.0f, 2.0f); // update
    }

private:
    b2World* world;
    b2Body* boxBody; // TODO: body list
    // b2Body* platform;
    // SDL_Rect box;
};
#pragma once

#include <box2d/box2d.h>
#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../Game/Game.h"
#include "../Utils/CookyUtils.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/PolygonColliderComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/TransformComponent.h"

const int SCALED_WIDTH = 1200 / MET2PIX;  // 15
const int SCALED_HEIGHT = 900 / MET2PIX;  // 11.25

class PhysicsSystem: public System {
public:
    PhysicsSystem() {
        RequireComponent<RigidBodyComponent>();
        RequireComponent<TransformComponent>();
        world = new b2World(b2Vec2(0.0f, 9.81f));
    }

    void Update() {
        for (auto entity : GetSystemEntities()) {
            auto rb = entity.GetComponent<RigidBodyComponent>();
            if (!rb.isInit) {
                Logger::Log("continue");
                continue;
            }
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& polygon = entity.GetComponent<PolygonColliderComponent>();  // 改成collider
            b2Vec2 pos = entityToBody[entity.GetId()]->GetPosition();
            transform.position = Vec2(pos.x, pos.y);
            b2Vec2 center = entityToBody[entity.GetId()]->GetWorldCenter();
            polygon.dcenter = Vec2(center.x, center.y) - polygon.center;
            // Logger::Log("box center: x:" + std::to_string(center.x) + ", y:" + std::to_string(center.y));
        }

        world->Step(1.0f / 60.0f, 6.0f, 2.0f); // update
    }

    void AddPolygon(Entity entity, b2BodyDef bodyDef, float width, float height, float friction, float restitution, float density) {
        b2FixtureDef fixtureDef;
        fixtureDef.friction = friction;
        fixtureDef.restitution = restitution;
        fixtureDef.density = density;

        b2PolygonShape boxShape;
        boxShape.SetAsBox((width / 2.0f) - boxShape.m_radius, (height / 2.0f) - boxShape.m_radius);
        fixtureDef.shape = &boxShape;

        b2Body* body;
        body = world->CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);
        entityToBody.emplace(entity.GetId(), body);
        entity.AddComponent<ColliderComponent>(CShape::POLYGON);
        std::vector<Vec2> vertices(4);
        for (int i = 0; i < 4; i++) {
            vertices[i] = Vec2(boxShape.m_vertices[i].x, boxShape.m_vertices[i].y);
        }
        entity.AddComponent<PolygonColliderComponent>(vertices, 4);

        entity.GetComponent<RigidBodyComponent>().isInit = true;
    }

    void AddPolygon(Entity entity, b2BodyDef bodyDef, b2Vec2* points, int count, float friction, float restitution, float density) {
        b2FixtureDef fixtureDef;
        fixtureDef.friction = friction;
        fixtureDef.restitution = restitution;
        fixtureDef.density = density;

        b2PolygonShape polygon;
        polygon.Set(points, count);
        fixtureDef.shape = &polygon;

        b2Body* body;
        body = world->CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);
        entityToBody.emplace(entity.GetId(), body);

        entity.AddComponent<ColliderComponent>(CShape::POLYGON);
        std::vector<Vec2> vertices(count);
        for (int i = 0; i < count; i++) {
            vertices[i] = Vec2(polygon.m_vertices[i].x, polygon.m_vertices[i].y);
        }
        Vec2 center = Vec2(body->GetWorldCenter().x, body->GetWorldCenter().y);
        Logger::Log("box center: x:" + std::to_string(center.x) + ", y:" + std::to_string(center.y));

        entity.AddComponent<PolygonColliderComponent>(vertices, count, center);

        entity.GetComponent<RigidBodyComponent>().isInit = true;
    }

private:
    b2World* world;
    std::unordered_map<int, b2Body*> entityToBody;

};
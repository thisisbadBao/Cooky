#pragma once

#include <box2d/box2d.h>
#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../Game/Game.h"
#include "../Utils/CookyUtils.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/PolygonColliderComponent.h"
#include "../Components/CircleColliderComponnet.h"
#include "../Components/ColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Physics/ContactListener.h"

const int SCALED_WIDTH = 1200 / MET2PIX;  // 15
const int SCALED_HEIGHT = 900 / MET2PIX;  // 11.25

class PhysicsSystem: public System {
public:
    PhysicsSystem() {
        RequireComponent<RigidBodyComponent>();
        RequireComponent<TransformComponent>();
        RequireComponent<ColliderComponent>();
        world = std::make_unique<b2World>(b2Vec2(0.0f, 9.81f));
        contactListener = std::make_shared<ContactListener>();
        world->SetContactListener(contactListener.get());
    }

    void Update() {
        for (auto entity : GetSystemEntities())
        {
            auto rb = entity.GetComponent<RigidBodyComponent>();
            if (!rb.isInit) {
                Logger::Log("continue");
                continue;
            }
            auto collider = entity.GetComponent<ColliderComponent>();
            auto &transform = entity.GetComponent<TransformComponent>();
            b2Vec2 pos = entityToBody[entity.GetId()]->GetPosition();
            b2Vec2 curCenter = entityToBody[entity.GetId()]->GetWorldCenter();

            transform.position = Vec2(pos.x, pos.y);
            float dRotation = entityToBody[entity.GetId()]->GetAngle() - transform.rotation;
            transform.rotation += dRotation;

            if (collider.shape == CShape::POLYGON) {
                auto& polygon = entity.GetComponent<PolygonColliderComponent>();
                Vec2 dcenter = {curCenter.x - polygon.center.x, curCenter.y - polygon.center.y};
                polygon.center.Set(curCenter.x, curCenter.y);
                for (int i = 0; i < polygon.vertices.size(); i++) {
                    polygon.vertices[i] += dcenter;
                    polygon.vertices[i].Rotate(polygon.center, dRotation);
                }
            }
            if (collider.shape == CShape::CIRCLE) {
                auto& circle = entity.GetComponent<CircleColliderComponnet>();
                circle.center.Set(pos.x, pos.y);
            }
        }
        world->Step(1.0f / 60.0f, 6.0f, 2.0f); // update
    }

    void Reset() {
        for (auto body : entityToBody) {
            world->DestroyBody(body.second);
        }
        entityToBody.clear();
    }

    void RemoveBody(Entity entity) {
        int entityId = entity.GetId();
        if (entityToBody.find(entityId) != entityToBody.end()) {
            world->DestroyBody(entityToBody[entityId]);
            entityToBody.erase(entityId);
        }
    }

    void AddPolygon(Entity& entity, b2BodyDef bodyDef, float width, float height, float friction, float restitution, float density) {
        if (entity.HasComponent<TransformComponent>()) {
            auto transform = entity.GetComponent<TransformComponent>();
            bodyDef.position.Set(transform.position.x, transform.position.y);
        } else {
            entity.AddComponent<TransformComponent>(Vec2(bodyDef.position.x, bodyDef.position.y));
        }
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
        std::vector<Vec2> vertices(4);
        for (int i = 0; i < 4; i++) {
            vertices[i] = Vec2(boxShape.m_vertices[i].x + bodyDef.position.x,
                boxShape.m_vertices[i].y + bodyDef.position.y);
        }

        body->GetUserData().pointer = (uintptr_t)&entity;

        Vec2 center = Vec2(body->GetWorldCenter().x, body->GetWorldCenter().y);
        entity.AddComponent<ColliderComponent>(CShape::POLYGON);
        entity.AddComponent<PolygonColliderComponent>(vertices, 4, center);
        entity.AddComponent<RigidBodyComponent>();
        entity.GetComponent<RigidBodyComponent>().isInit = true;
    }

    void AddPolygon(Entity& entity, b2BodyDef bodyDef, b2Vec2* points, int count, float friction, float restitution, float density) {
        if (entity.HasComponent<TransformComponent>()) {
            auto transform = entity.GetComponent<TransformComponent>();
            bodyDef.position.Set(transform.position.x, transform.position.y);
        } else {
            entity.AddComponent<TransformComponent>(Vec2(bodyDef.position.x, bodyDef.position.y));
        }
        b2FixtureDef fixtureDef;
        fixtureDef.friction = friction;
        fixtureDef.restitution = restitution;
        fixtureDef.density = density;

        b2PolygonShape polygon;
        polygon.Set(points, count);
        for (int i = 0; i < count; i++) {
            points[i] -= polygon.m_centroid;
        }
        polygon.Set(points, count); // Set polygon's center to Zero

        fixtureDef.shape = &polygon;
        b2Body* body;
        body = world->CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);
        entityToBody.emplace(entity.GetId(), body);

        std::vector<Vec2> vertices(count);
        for (int i = 0; i < count; i++) {
            vertices[i] = Vec2(polygon.m_vertices[i].x + bodyDef.position.x,
                polygon.m_vertices[i].y + bodyDef.position.y);
        }
        body->GetUserData().pointer = (uintptr_t)&entity;
        Vec2 center = Vec2(body->GetWorldCenter().x, body->GetWorldCenter().y);
        // b2Vec2 pos = entityToBody[entity.GetId()]->GetPosition();
        // Logger::Log("poly center: x:" + std::to_string(center.x) + ", y:" + std::to_string(center.y));
        // Logger::Log("poly pos: x:" + std::to_string(pos.x) + ", y:" + std::to_string(pos.y));
        entity.AddComponent<ColliderComponent>(CShape::POLYGON);
        entity.AddComponent<PolygonColliderComponent>(vertices, count, center);
        entity.AddComponent<RigidBodyComponent>();
        entity.GetComponent<RigidBodyComponent>().isInit = true;
    }

    void AddCircle(Entity& entity, b2BodyDef bodyDef, float radius, float friction, float restitution, float density) {
        if (entity.HasComponent<TransformComponent>()) {
            auto transform = entity.GetComponent<TransformComponent>();
            bodyDef.position.Set(transform.position.x, transform.position.y);
        } else {
            entity.AddComponent<TransformComponent>(Vec2(bodyDef.position.x, bodyDef.position.y));
        }
        b2FixtureDef fixtureDef;
        fixtureDef.friction = friction;
        fixtureDef.restitution = restitution;
        fixtureDef.density = density;

        b2CircleShape circle;
        circle.m_radius = radius;
        Vec2 center = {bodyDef.position.x, bodyDef.position.y};

        fixtureDef.shape = &circle;
        b2Body* body;
        body = world->CreateBody(&bodyDef);
        body->CreateFixture(&fixtureDef);
        entityToBody.emplace(entity.GetId(), body);

        body->GetUserData().pointer = (uintptr_t)&entity;
        entity.AddComponent<ColliderComponent>(CShape::CIRCLE);
        entity.AddComponent<CircleColliderComponnet>(center, radius);
        entity.AddComponent<RigidBodyComponent>();
        entity.GetComponent<RigidBodyComponent>().isInit = true;
    }
    bool IsBodyCreated(int entityId) {
        if (entityToBody.find(entityId) != entityToBody.end()) {
            return true;
        }
        return false;
    }

    void SetLinearVelocity(int entityId, Vec2 velocity) {
        if (IsBodyCreated(entityId)) {
            // Logger::Log("SetLinearVelocity: " + std::to_string(velocity.x) + ", " + std::to_string(velocity.y));
            entityToBody[entityId]->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
        }
    }

    void SetLinearDamping(int entityId, float linearDamping) {
        if (IsBodyCreated(entityId)) {
            entityToBody[entityId]->SetLinearDamping(linearDamping);
        }
    }

    void SetAngularVelocity(int entityId, float angularVelocity) {
        if (IsBodyCreated(entityId)) {
            entityToBody[entityId]->SetAngularVelocity(angularVelocity);
        }
    }

    void SetAngularDamping(int entityId, float angularDamping) {
        if (IsBodyCreated(entityId)) {
            entityToBody[entityId]->SetAngularDamping(angularDamping);
        }
    }

    void ApplyForceToCenter(int entityId, Vec2 force) {
        if (IsBodyCreated(entityId)) {
            entityToBody[entityId]->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
        }
    }

    void SetGravityScale(int entityId, float scale) {
        if (IsBodyCreated(entityId)) {
            entityToBody[entityId]->SetGravityScale(scale);
        }
    }

    void SetSensor(int entityId, bool sensor) {
        if (IsBodyCreated(entityId)) {
            b2Body* body = entityToBody[entityId];
            b2Fixture* fixtureList = body->GetFixtureList();
            fixtureList[0].SetSensor(sensor);
        }
    }

private:
    std::unique_ptr<b2World> world;
    std::shared_ptr<ContactListener> contactListener;
    std::unordered_map<int, b2Body*> entityToBody;
};
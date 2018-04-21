//
// Created by loic on 21/04/18.
//

#ifndef LD41_PHYSICSSYSTEM_HPP
#define LD41_PHYSICSSYSTEM_HPP

#include <Fluffy/ECS/System.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <iostream>
#include "../components/SpeedComponent.hpp"
#include "../components/Body.hpp"

using namespace Fluffy::ECS;

class PhysicsSystem : public System<PhysicsSystem>
{
public:
    PhysicsSystem() = default;

    void update(EntityManager& entityManager, Time dt) override
    {
        std::vector<ComponentHandle<BodyComponent>> collisionBodies;

        // Physics
        for (auto entity : entityManager.each<BodyComponent, SpeedComponent>()) {
            auto body = entity.component<BodyComponent>();
            auto speed = entity.component<SpeedComponent>();

            body->setPosition(body->getPosition().x + speed->x * dt.seconds(), body->getPosition().y + speed->y * dt.seconds());

            if (body->gravity && !body->resting) {
                speed->y += 900.f * dt.seconds();
            }
        }

        // Collisions
        for (auto entity : entityManager.each<BodyComponent>()) {
            auto body = entity.component<BodyComponent>();

            if (body->collidable) {
                collisionBodies.push_back(body);
            }
        }

        for (auto body : collisionBodies) {
            for (auto with : collisionBodies) {
                if (body != with && body->boundings().intersects(with->boundings())) {
                    auto callback = body->collisionCallbacks.find(with->type);
                    if (callback != body->collisionCallbacks.end()) {
                        callback->second(body.getEntity(), with.getEntity());
                    }
                }
            }
        }
    }

private:
};


#endif //LD41_PHYSICSSYSTEM_HPP

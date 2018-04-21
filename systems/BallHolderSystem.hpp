//
// Created by loic on 21/04/18.
//

#ifndef LD41_BALLHOLDERSYSTEM_HPP
#define LD41_BALLHOLDERSYSTEM_HPP

#include <Fluffy/ECS/System.hpp>
#include "../components/BallHolder.hpp"
#include "../components/Body.hpp"

using namespace Fluffy::ECS;

class BallHolderSystem : public System<BallHolderSystem>
{
public:
    void update(EntityManager &entityManager, Fluffy::Utility::Time dt) override
    {
        for (auto entity : entityManager.each<BallHolderComponent, BodyComponent>()) {
            auto holder = entity.component<BallHolderComponent>();
            if (holder->holding) {
                auto entityBody = entity.component<BodyComponent>();
                auto ballBody = holder->ball.component<BodyComponent>();
                ballBody->setPosition(entityBody->getPosition() + holder->handPosition + sf::Vector2f(0, 7.f));
            }
        }
    }
};

#endif //LD41_BALLHOLDERSYSTEM_HPP

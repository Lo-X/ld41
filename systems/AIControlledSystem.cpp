//
// Created by loic on 22/04/18.
//

#include "AIControlledSystem.hpp"
#include "../components/AIControlled.hpp"
#include "../components/Body.hpp"
#include "../components/SpeedComponent.hpp"
#include "../components/PlayerAnimation.hpp"

void AIControlledSystem::update(EntityManager &entityManager, Fluffy::Utility::Time dt)
{
    for (auto entity : entityManager.each<AIControlledComponent, SpeedComponent, BodyComponent, PlayerAnimationComponent>()) {
        auto skeleton = entity.component<AIControlledComponent>();
        auto body = entity.component<BodyComponent>();
        auto speed = entity.component<SpeedComponent>();
        auto animation = entity.component<PlayerAnimationComponent>();

        if (skeleton->action == AIControlledComponent::WalkRight || skeleton->action == AIControlledComponent::WalkLeft) {
            skeleton->action = AIControlledComponent::Standby;
            animation->currentAnimation = PlayerAnimationComponent::Standing;
            speed->x = 0;
        }

        if (skeleton->action != AIControlledComponent::Jump) {
            body->resting = false;
        }
    }
}

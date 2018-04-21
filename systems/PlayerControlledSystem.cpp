//
// Created by loic on 21/04/18.
//

#include "PlayerControlledSystem.hpp"
#include "../components/PlayerControlled.hpp"
#include "../components/SpeedComponent.hpp"
#include "../components/Body.hpp"
#include "../components/PlayerAnimation.hpp"

void PlayerControlledSystem::update(EntityManager &entityManager, Fluffy::Utility::Time dt)
{
    for (auto entity : entityManager.each<PlayerControlledComponent, SpeedComponent, BodyComponent, PlayerAnimationComponent>()) {
        auto player = entity.component<PlayerControlledComponent>();
        auto body = entity.component<BodyComponent>();
        auto speed = entity.component<SpeedComponent>();
        auto animation = entity.component<PlayerAnimationComponent>();

        if (player->action == PlayerControlledComponent::WalkRight || player->action == PlayerControlledComponent::WalkLeft) {
            player->action = PlayerControlledComponent::Standby;
            animation->currentAnimation = PlayerAnimationComponent::Standing;
            speed->x = 0;
        }

        if (player->action != PlayerControlledComponent::Jump) {
            body->resting = false;
        }
    }
}

//
// Created by loic on 21/04/18.
//

#include "AnimationSystem.hpp"
#include "../components/PlayerAnimation.hpp"
#include "../components/Drawable.hpp"
#include "../components/PlayerControlled.hpp"

void AnimationSystem::update(EntityManager &entityManager, Fluffy::Utility::Time dt)
{
    // Player animations
    for (auto entity : entityManager.each<PlayerAnimationComponent, DrawableComponent>()) {
        auto drawable = entity.component<DrawableComponent>();
        auto animation = entity.component<PlayerAnimationComponent>();
        auto player = entity.component<PlayerControlledComponent>();

        if (animation->currentAnimation == PlayerAnimationComponent::Attacking && animation->animations[animation->currentAnimation].isFinished()) {
            animation->currentAnimation = PlayerAnimationComponent::Standing;
        }

        animation->update(dt);
        drawable->sprite = animation->currentSprite();
    }

    // Creatures animations

}

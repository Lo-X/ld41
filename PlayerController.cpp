//
// Created by loic on 21/04/18.
//

#include <Fluffy/Event/EventManager.hpp>
#include <Fluffy/ECS/EntityManager.hpp>
#include <iostream>
#include "PlayerController.hpp"
#include "components/PlayerControlled.hpp"
#include "components/Body.hpp"
#include "components/SpeedComponent.hpp"

using namespace Fluffy::ECS;
using namespace Fluffy::Event;

PlayerController::PlayerController(ServiceContainer *container)
: mContainer(container)
{
    mJoystickButtonPressedSlot = mContainer->get<EventManager>()->connect<JoystickButtonPressedEvent>(std::bind(
            &PlayerController::onJoystickButtonPressedEvent,
            this,
            std::placeholders::_1
    ));

    mJoystickXAxisUsedSlot = mContainer->get<EventManager>()->connect<JoystickXAnalogEvent>(std::bind(
            &PlayerController::onJoystickXAnalogUsedEvent,
            this,
            std::placeholders::_1
    ));
}

PlayerController::~PlayerController()
{
    mJoystickButtonPressedSlot.disconnect();
    mJoystickXAxisUsedSlot.disconnect();
}

void PlayerController::onJoystickButtonPressedEvent(const JoystickButtonPressedEvent &event)
{
    switch (event.button) {
        // Jump (A)
        case 0:
            for (auto entity : mContainer->get<EntityManager>()->each<PlayerControlledComponent>()) {
                auto player = entity.component<PlayerControlledComponent>();
//                auto animation = entity.component<PlayerAnimationComponent>();
//                if (animation.isValid() && player->action != PlayerControlledComponent::Actions::Attack) {
                    auto speed = entity.component<SpeedComponent>();
                    auto body = entity.component<BodyComponent>();
//                    animation->currentAnimation = PlayerAnimationComponent::Animations::Jumping;
                    player->action = PlayerControlledComponent::Actions::Jump;
                    speed->y = -player->speed.y;
                    body->resting = false;
//                }
            }
            break;

            // Attack (B)
        case 1:
            for (auto entity : mContainer->get<EntityManager>()->each<PlayerControlledComponent>()) {
                auto player = entity.component<PlayerControlledComponent>();
//                auto animation = entity.component<PlayerAnimationComponent>();
//                if (
//                        animation.isValid() &&
//                        player->action != PlayerControlledComponent::Actions::Attack
//                        ) {
//                    std::cout << "ATTACK" << std::endl;
//                    animation->currentAnimation = PlayerAnimationComponent::Animations::Attacking;
//                    animation->animations[animation->currentAnimation].restart();
//                    player->action = PlayerControlledComponent::Actions::Attack;
//                    mAttackAction.execute(player);
//                }
            }
            break;

        default:break;
    }
}

void PlayerController::onJoystickXAnalogUsedEvent(const JoystickXAnalogEvent &event)
{
    for (auto entity : mContainer->get<EntityManager>()->each<PlayerControlledComponent>()) {
        auto player = entity.component<PlayerControlledComponent>();
//        auto animation = entity.component<PlayerAnimationComponent>();
        auto speed = entity.component<SpeedComponent>();
//        if (animation.isValid()) {
            if (player->action == PlayerControlledComponent::Actions::Standby) {
                if (event.position >= 0) {
//                    animation->currentAnimation = PlayerAnimationComponent::Animations::Walking;
//                    animation->setDirection(PlayerAnimationComponent::Directions::Right);
                    speed->x = player->speed.x;
                    player->action = PlayerControlledComponent::Actions::WalkRight;
                } else {
//                    animation->currentAnimation = PlayerAnimationComponent::Animations::Walking;
//                    animation->setDirection(PlayerAnimationComponent::Directions::Left);
                    speed->x = -player->speed.x;
                    player->action = PlayerControlledComponent::Actions::WalkLeft;
                }
            } else if (player->action == PlayerControlledComponent::Actions::Jump) {
                if (event.position >= 0) {
//                    animation->setDirection(PlayerAnimationComponent::Directions::Right);
                    speed->x = player->speed.x;
                } else {
//                    animation->setDirection(PlayerAnimationComponent::Directions::Left);
                    speed->x = -player->speed.x;
                }
            }
//        }
    }
}
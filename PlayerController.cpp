//
// Created by loic on 21/04/18.
//

#include <Fluffy/Event/EventManager.hpp>
#include <Fluffy/ECS/EntityManager.hpp>
#include <SFML/System/Lock.hpp>
#include <iostream>
#include "PlayerController.hpp"
#include "components/PlayerControlled.hpp"
#include "components/Body.hpp"
#include "components/SpeedComponent.hpp"
#include "components/PlayerAnimation.hpp"
#include "components/BallHolder.hpp"

using namespace Fluffy::ECS;
using namespace Fluffy::Event;

AttackAction::AttackAction()
        : mThread(&AttackAction::runTask, this),
          mFinished(false)
{
}

void AttackAction::execute(ComponentHandle<PlayerControlledComponent> player, ComponentHandle<PlayerAnimationComponent> animation, ComponentHandle<BallHolderComponent> holder)
{
    mFinished = false;
    mPlayer = player;
    mAnimation = animation;
    mHolder = holder;
    mClock.restart();
    mThread.launch();
}

bool AttackAction::isFinished()
{
    sf::Lock lock(mMutex);

    return mFinished;
}

void AttackAction::runTask()
{
    bool ended = false;

    if (mAnimation.isValid() && mHolder.isValid() && mHolder->holding && mHolder->ball.isValid()) {
        auto speed = mHolder->ball.component<SpeedComponent>();
        auto body = mHolder->ball.component<BodyComponent>();

        if (mAnimation->direction == PlayerAnimationComponent::Directions::Right) {
            body->move(PlayerHandPositionThrowingRight);
            speed->x = mHolder->ballSpeed.x;
            speed->y = mHolder->ballSpeed.y;
        } else {
            body->move(PlayerHandPositionThrowingLeft);
            speed->x = -mHolder->ballSpeed.x;
            speed->y = mHolder->ballSpeed.y;
        }

        body->resting = false;
        mHolder->holding = false;
    }

    while (!ended) {
        if (mClock.elapsedTime() >= milliseconds(430)) {
            ended = true;
        }
    }

    {
        sf::Lock lock(mMutex);
        if (mPlayer.isValid()) {
            mPlayer->action = PlayerControlledComponent::Actions::Standby;
        }
        if (mAnimation.isValid()) {
            mAnimation->currentAnimation = PlayerAnimationComponent::Standing;
        }
        mFinished = true;
    }
}

/**********************************************************************************************************************/

PlayerController::PlayerController(ServiceContainer *container)
: mContainer(container)
{
    mBeforeTick = mContainer->get<EventManager>()->connect<BeforeGameTickEvent>(std::bind(
            &PlayerController::onBeforeTick,
            this,
            std::placeholders::_1
    ));

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
    mBeforeTick.disconnect();
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
                auto animation = entity.component<PlayerAnimationComponent>();
                if (player->action != PlayerControlledComponent::Actions::Throw && player->action != PlayerControlledComponent::Actions::Jump) {
                    auto speed = entity.component<SpeedComponent>();
                    auto body = entity.component<BodyComponent>();
                    animation->currentAnimation = PlayerAnimationComponent::Jumping;
                    player->action = PlayerControlledComponent::Actions::Jump;
                    speed->y = -player->speed.y;
                    body->resting = false;
                    body->setPosition(body->getPosition().x + 2.f, body->getPosition().y);
                }
            }
            break;

            // Attack (B)
        case 1:
            for (auto entity : mContainer->get<EntityManager>()->each<PlayerControlledComponent>()) {
                auto player = entity.component<PlayerControlledComponent>();
                if (player->action != PlayerControlledComponent::Actions::Throw) {
                    auto animation = entity.component<PlayerAnimationComponent>();
                    auto holder = entity.component<BallHolderComponent>();

                    if (holder->holding) {
                        animation->currentAnimation = PlayerAnimationComponent::Attacking;
                        animation->animations[animation->currentAnimation].restart();
                        player->action = PlayerControlledComponent::Actions::Throw;
                        mAttackAction.execute(player, animation, holder);
                    }
                }
            }
            break;

        default:break;
    }
}

void PlayerController::onJoystickXAnalogUsedEvent(const JoystickXAnalogEvent &event)
{
    for (auto entity : mContainer->get<EntityManager>()->each<PlayerControlledComponent>()) {
        auto player = entity.component<PlayerControlledComponent>();
        auto animation = entity.component<PlayerAnimationComponent>();
        auto speed = entity.component<SpeedComponent>();
        auto holder = entity.component<BallHolderComponent>();

        if (player->action == PlayerControlledComponent::Actions::Standby) {
            if (event.position >= 0) {
                animation->currentAnimation = PlayerAnimationComponent::Walking;
                animation->setDirection(PlayerAnimationComponent::Directions::Right);
                speed->x = player->speed.x;
                player->action = PlayerControlledComponent::Actions::WalkRight;
                holder->handPosition = PlayerHandPositionStandingRight;
            } else {
                animation->currentAnimation = PlayerAnimationComponent::Walking;
                animation->setDirection(PlayerAnimationComponent::Directions::Left);
                speed->x = -player->speed.x;
                player->action = PlayerControlledComponent::Actions::WalkLeft;
                holder->handPosition = PlayerHandPositionStandingLeft;
            }
        } else if (player->action == PlayerControlledComponent::Actions::Jump) {
            if (event.position >= 0) {
                animation->setDirection(PlayerAnimationComponent::Directions::Right);
                speed->x = player->speed.x;
                holder->handPosition = PlayerHandPositionStandingRight;
            } else {
                animation->setDirection(PlayerAnimationComponent::Directions::Left);
                speed->x = -player->speed.x;
                holder->handPosition = PlayerHandPositionStandingLeft;
            }
        }
    }
}

void PlayerController::onBeforeTick(const BeforeGameTickEvent &event)
{
//    for (auto entity : mContainer->get<EntityManager>()->each<PlayerControlledComponent>()) {
//        auto player = entity.component<PlayerControlledComponent>();
//        auto animation = entity.component<PlayerAnimationComponent>();
//
//        if (player->action == PlayerControlledComponent::Actions::WalkRight || player->action == PlayerControlledComponent::Actions::WalkLeft) {
//            player->action = PlayerControlledComponent::Standby;
//            animation->currentAnimation = PlayerAnimationComponent::Standing;
//        }
//    }
}

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
#include "components/Ball.hpp"

using namespace Fluffy::ECS;
using namespace Fluffy::Event;

ThrowAction::ThrowAction()
        : mThread(&ThrowAction::runTask, this),
          mFinished(false)
{
}

void ThrowAction::execute(ComponentHandle<PlayerControlledComponent> player, ComponentHandle<PlayerAnimationComponent> animation, ComponentHandle<BallHolderComponent> holder)
{
    mFinished = false;
    mPlayer = player;
    mAnimation = animation;
    mHolder = holder;
    mClock.restart();
    mThread.launch();
}

bool ThrowAction::isFinished()
{
    sf::Lock lock(mMutex);

    return mFinished;
}

void ThrowAction::runTask()
{
    bool ended = false;

    if (mAnimation.isValid() && mHolder.isValid() && mHolder->holding && mHolder->ball.isValid()) {
        auto speed = mHolder->ball.component<SpeedComponent>();
        auto body = mHolder->ball.component<BodyComponent>();
        auto taken = mHolder->ball.component<BallComponent>();

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
        taken->drop();
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

AttackAction::AttackAction()
        : mThread(&AttackAction::runTask, this),
          mFinished(false)
{
}

void AttackAction::execute(ComponentHandle<PlayerControlledComponent> player, ComponentHandle<PlayerAnimationComponent> animation)
{
    mFinished = false;
    mPlayer = player;
    mAnimation = animation;
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
    while (!ended) {
        if (mClock.elapsedTime() >= milliseconds(250)) {
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

    mKeyPressedSlot = mContainer->get<EventManager>()->connect<KeyPressedEvent>(std::bind(
            &PlayerController::onKeyPressedEvent,
            this,
            std::placeholders::_1
    ));

    mKeyDownSlot = mContainer->get<EventManager>()->connect<KeyDownEvent>(std::bind(
            &PlayerController::onKeyDownEvent,
            this,
            std::placeholders::_1
    ));
}

PlayerController::~PlayerController()
{
    mKeyPressedSlot.disconnect();
    mKeyDownSlot.disconnect();
    mJoystickButtonPressedSlot.disconnect();
    mJoystickXAxisUsedSlot.disconnect();
}

void PlayerController::onJoystickButtonPressedEvent(const JoystickButtonPressedEvent &event)
{
    switch (event.button) {
        // Jump (A)
        case 0:
            jump();
            break;

            // Attack (B)
        case 1:
            attack();
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

void PlayerController::onKeyPressedEvent(const KeyPressedEvent &event)
{
    switch (event.key) {
        case sf::Keyboard::Space:
            jump();
            break;

        case sf::Keyboard::LShift:
        case sf::Keyboard::RShift:
            attack();
            break;

        default:break;
    }
}

void PlayerController::onKeyDownEvent(const KeyDownEvent &event)
{
    for (auto entity : mContainer->get<EntityManager>()->each<PlayerControlledComponent>()) {
        auto player = entity.component<PlayerControlledComponent>();
        auto animation = entity.component<PlayerAnimationComponent>();
        auto speed = entity.component<SpeedComponent>();
        auto holder = entity.component<BallHolderComponent>();

        if (player->action == PlayerControlledComponent::Actions::Standby) {
            if (event.key == sf::Keyboard::E || event.key == sf::Keyboard::D || event.key == sf::Keyboard::Right) {
                animation->currentAnimation = PlayerAnimationComponent::Walking;
                animation->setDirection(PlayerAnimationComponent::Directions::Right);
                speed->x = player->speed.x;
                player->action = PlayerControlledComponent::Actions::WalkRight;
                holder->handPosition = PlayerHandPositionStandingRight;
            } else if (event.key == sf::Keyboard::A || event.key == sf::Keyboard::Q || event.key == sf::Keyboard::Left) {
                animation->currentAnimation = PlayerAnimationComponent::Walking;
                animation->setDirection(PlayerAnimationComponent::Directions::Left);
                speed->x = -player->speed.x;
                player->action = PlayerControlledComponent::Actions::WalkLeft;
                holder->handPosition = PlayerHandPositionStandingLeft;
            }
        } else if (player->action == PlayerControlledComponent::Actions::Jump) {
            if (event.key == sf::Keyboard::E || event.key == sf::Keyboard::D || event.key == sf::Keyboard::Right) {
                animation->setDirection(PlayerAnimationComponent::Directions::Right);
                speed->x = player->speed.x;
                holder->handPosition = PlayerHandPositionStandingRight;
            } else if (event.key == sf::Keyboard::A || event.key == sf::Keyboard::Q || event.key == sf::Keyboard::Left) {
                animation->setDirection(PlayerAnimationComponent::Directions::Left);
                speed->x = -player->speed.x;
                holder->handPosition = PlayerHandPositionStandingLeft;
            }
        }
    }
}

void PlayerController::attack()
{
    for (auto entity : mContainer->get<EntityManager>()->each<PlayerControlledComponent>()) {
        auto player = entity.component<PlayerControlledComponent>();

        if (player->canMove()) {
            auto animation = entity.component<PlayerAnimationComponent>();
            auto holder = entity.component<BallHolderComponent>();

            if (holder->holding) {
                animation->currentAnimation = PlayerAnimationComponent::Throwing;
                animation->animations[animation->currentAnimation].restart();
                player->action = PlayerControlledComponent::Throw;
                mThrowAction.execute(player, animation, holder);
            } else {
                animation->currentAnimation = PlayerAnimationComponent::Attacking;
                animation->animations[animation->currentAnimation].restart();
                player->action = PlayerControlledComponent::Attack;
                mAttackAction.execute(player, animation);
            }
        }
    }
}

void PlayerController::jump()
{
    for (auto entity : mContainer->get<EntityManager>()->each<PlayerControlledComponent>()) {
        auto player = entity.component<PlayerControlledComponent>();
        auto animation = entity.component<PlayerAnimationComponent>();
        if (player->action != PlayerControlledComponent::Throw && player->action != PlayerControlledComponent::Attack && player->action != PlayerControlledComponent::Jump) {
            auto speed = entity.component<SpeedComponent>();
            auto body = entity.component<BodyComponent>();
            animation->currentAnimation = PlayerAnimationComponent::Jumping;
            player->action = PlayerControlledComponent::Jump;
            speed->y = -player->speed.y;
            body->resting = false;
            body->setPosition(body->getPosition().x + 2.f, body->getPosition().y);
        }
    }
}

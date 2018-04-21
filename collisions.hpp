//
// Created by loic on 21/04/18.
//

#ifndef LD41_COLLISIONS_HPP
#define LD41_COLLISIONS_HPP


#include "components/Body.hpp"
#include "components/SpeedComponent.hpp"
#include "components/PlayerAnimation.hpp"
#include "components/BallHolder.hpp"

void playerCollideWithPlatform(Entity player, Entity platform)
{
    auto playerBody = player.component<BodyComponent>();

    if (playerBody->resting) {
        return;
    }

    auto playerSpeed = player.component<SpeedComponent>();
    auto playerAnimation = player.component<PlayerAnimationComponent>();
    auto playerControlled = player.component<PlayerControlledComponent>();
    auto platformBody = platform.component<BodyComponent>();

    playerBody->setPosition(playerBody->getPosition().x, platformBody->getPosition().y);
    playerBody->resting = true;
    if (playerSpeed.isValid()) {
        playerSpeed->x = 0;
        playerSpeed->y = 0;
    }
    if (playerControlled.isValid() && playerControlled->action != PlayerControlledComponent::Throw) {
        playerControlled->action = PlayerControlledComponent::Standby;

        if (playerAnimation.isValid()) {
            playerAnimation->currentAnimation = PlayerAnimationComponent::Standing;
        }
    }
}

void playerCollideWithWall(Entity player, Entity wall)
{
    auto playerBody = player.component<BodyComponent>();

    if (playerBody->resting) {
        return;
    }

    auto wallBody = wall.component<BodyComponent>();

    if (playerBody->getPosition().x < wallBody->getPosition().x) {
        // Player at the left of the wall
        playerBody->setPosition(wallBody->getPosition().x - playerBody->getSize().x / 2, playerBody->getPosition().y);
    } else {
        // Player at the right of the wall
        playerBody->setPosition(wallBody->getPosition().x + wallBody->getSize().x + playerBody->getSize().x / 2, playerBody->getPosition().y);
    }
}

void playerCollideWithBall(Entity player, Entity ball)
{
    auto holder = player.component<BallHolderComponent>();
    auto controlled = player.component<PlayerControlledComponent>();

    if (controlled->action != PlayerControlledComponent::Throw) {
        holder->ball = ball;
        holder->holding = true;
    }
}

void ballCollideWithPlatform(Entity ball, Entity platform)
{
    auto ballBody = ball.component<BodyComponent>();

    if (ballBody->resting) {
        return;
    }

    auto ballSpeed = ball.component<SpeedComponent>();
    auto platformBody = platform.component<BodyComponent>();

    ballBody->setPosition(ballBody->getPosition().x, platformBody->getPosition().y - ballBody->getOrigin().y);
    ballBody->resting = true;
    if (ballSpeed.isValid()) {
        ballSpeed->x = 0;
        ballSpeed->y = 0;
    }
}

void ballCollideWithWall(Entity ball, Entity wall)
{
    auto ballBody = ball.component<BodyComponent>();
    auto ballSpeed = ball.component<SpeedComponent>();

    if (ballBody->resting) {
        return;
    }

    auto wallBody = wall.component<BodyComponent>();

    if (ballSpeed->x > 0) {
        // Player at the left of the wall
        ballBody->setPosition(wallBody->getPosition().x - ballBody->getSize().x / 2, ballBody->getPosition().y);
    } else {
        // Player at the right of the wall
        ballBody->setPosition(wallBody->getPosition().x + wallBody->getSize().x + ballBody->getSize().x / 2, ballBody->getPosition().y);
    }
    ballSpeed->x = 0;
}

#endif //LD41_COLLISIONS_HPP

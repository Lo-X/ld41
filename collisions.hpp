//
// Created by loic on 21/04/18.
//

#ifndef LD41_COLLISIONS_HPP
#define LD41_COLLISIONS_HPP


#include "components/Body.hpp"
#include "components/SpeedComponent.hpp"
#include "components/PlayerAnimation.hpp"
#include "components/BallHolder.hpp"
#include "components/AIControlled.hpp"
#include "components/Ball.hpp"

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
    if (playerControlled.isValid() && playerControlled->action != PlayerControlledComponent::Throw && playerControlled->action != PlayerControlledComponent::Attack) {
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
    auto taken = ball.component<BallComponent>();

    if (!taken->taken() && controlled->canMove()) {
        holder->ball = ball;
        holder->holding = true;
        taken->take(player);
    }
}

void playerCollideWithSkeleton(Entity player, Entity skeleton)
{
    auto playerBody = player.component<BodyComponent>();
    auto playerControlled = player.component<PlayerControlledComponent>();
    auto skeletonBody = skeleton.component<BodyComponent>();
    auto skeletonControlled = skeleton.component<AIControlledComponent>();

    if (playerControlled->action == PlayerControlledComponent::Attack) {
        auto skeletonSpeed = skeleton.component<SpeedComponent>();
        auto skeletonHolder = skeleton.component<BallHolderComponent>();

        if (skeletonHolder->holding) {
            auto ball = skeletonHolder->ball.component<BallComponent>();
            auto ballBody = skeletonHolder->ball.component<BodyComponent>();
            ball->drop();
            ballBody->resting = false;
            skeletonHolder->holding = false;
        }
        skeletonControlled->action = AIControlledComponent::Stunned;

        if (playerBody->getPosition().x < skeletonBody->getPosition().x) {
            // Player at the left of the skeleton
            skeletonSpeed->x = 500.f;
            skeletonSpeed->y = -100.f;
        } else {
            // Player at the right of the skeleton
            skeletonSpeed->x = -500.f;
            skeletonSpeed->y = -100.f;
        }
    } else if (skeletonControlled->action == AIControlledComponent::Attack) {
        auto playerSpeed = player.component<SpeedComponent>();
        auto playerHolder = player.component<BallHolderComponent>();

        if (playerHolder->holding) {
            auto ball = playerHolder->ball.component<BallComponent>();
            auto ballBody = playerHolder->ball.component<BodyComponent>();
            ball->drop();
            ballBody->resting = false;
            playerHolder->holding = false;
        }
        playerControlled->action = PlayerControlledComponent::Stunned;

        if (playerBody->getPosition().x < skeletonBody->getPosition().x) {
            // Player at the left of the skeleton
            playerSpeed->x = -500.f;
            playerSpeed->y = -100.f;
        } else {
            // Player at the right of the skeleton
            playerSpeed->x = 500.f;
            playerSpeed->y = -100.f;
        }
    } else {
        if (abs((int)playerBody->getPosition().x - (int)skeletonBody->getPosition().x) > ((int)playerBody->getSize().x / 2 - 10)) {
            return;
        }
        if (playerBody->getPosition().x < skeletonBody->getPosition().x) {
            // Player at the left of the skeleton
            playerBody->setPosition(skeletonBody->getPosition().x - 15, playerBody->getPosition().y);
            skeletonBody->setPosition(skeletonBody->getPosition().x + 15, skeletonBody->getPosition().y);
        } else {
            // Player at the right of the skeleton
            playerBody->setPosition(skeletonBody->getPosition().x + 15, playerBody->getPosition().y);
            skeletonBody->setPosition(skeletonBody->getPosition().x - 15, skeletonBody->getPosition().y);
        }
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
    ballSpeed->x = -ballSpeed->x;
}

void skeletonCollideWithPlatform(Entity skeleton, Entity platform)
{
    auto skeletonBody = skeleton.component<BodyComponent>();

    if (skeletonBody->resting) {
        return;
    }

    auto skeletonSpeed = skeleton.component<SpeedComponent>();
    auto skeletonAnimation = skeleton.component<PlayerAnimationComponent>();
    auto skeletonControlled = skeleton.component<AIControlledComponent>();
    auto platformBody = platform.component<BodyComponent>();

    skeletonBody->setPosition(skeletonBody->getPosition().x, platformBody->getPosition().y);
    skeletonBody->resting = true;
    if (skeletonSpeed.isValid()) {
        skeletonSpeed->x = 0;
        skeletonSpeed->y = 0;
    }
    if (skeletonControlled.isValid() && skeletonControlled->action != AIControlledComponent::Throw && skeletonControlled->action != AIControlledComponent::Attack) {
        skeletonControlled->action = AIControlledComponent::Standby;

        if (skeletonAnimation.isValid()) {
            skeletonAnimation->currentAnimation = PlayerAnimationComponent::Standing;
        }
    }
}

void skeletonCollideWithWall(Entity skeleton, Entity wall)
{
    auto skeletonBody = skeleton.component<BodyComponent>();
    auto skeletonSpeed = skeleton.component<SpeedComponent>();

    if (skeletonBody->resting) {
        return;
    }

    auto wallBody = wall.component<BodyComponent>();

    if (skeletonSpeed->x > 0) {
        // Player at the left of the wall
        skeletonBody->setPosition(wallBody->getPosition().x - skeletonBody->getSize().x / 2, skeletonBody->getPosition().y);
    } else {
        // Player at the right of the wall
        skeletonBody->setPosition(wallBody->getPosition().x + wallBody->getSize().x + skeletonBody->getSize().x / 2, skeletonBody->getPosition().y);
    }
}

void skeletonCollideWithBall(Entity skeleton, Entity ball)
{
    auto holder = skeleton.component<BallHolderComponent>();
    auto controlled = skeleton.component<AIControlledComponent>();
    auto taken = ball.component<BallComponent>();

    if (!taken->taken() && controlled->canMove()) {
        holder->ball = ball;
        holder->holding = true;
        taken->take(skeleton);
    }
}

//void skeletonCollideWithPlayer(Entity skeleton, Entity player)
//{
//    auto skeletonBody = skeleton.component<BodyComponent>();
//    auto skeletonControlled = skeleton.component<AIControlledComponent>();
//    auto playerBody = player.component<BodyComponent>();
//
//    if (skeletonControlled->action == AIControlledComponent::Attack) {
//        auto playerSpeed = player.component<SpeedComponent>();
//        auto playerHolder = player.component<BallHolderComponent>();
//        auto playerControlled = player.component<PlayerControlledComponent>();
//
//        if (playerHolder->holding) {
//            auto ball = playerHolder->ball.component<BallComponent>();
//            auto ballBody = playerHolder->ball.component<BodyComponent>();
//            ball->drop();
//            ballBody->resting = false;
//            playerHolder->holding = false;
//        }
//        playerControlled->action = PlayerControlledComponent::Stunned;
//
//        if (skeletonBody->getPosition().x < playerBody->getPosition().x) {
//            // Player at the left of the player
//            playerSpeed->x = 500.f;
//            playerSpeed->y = -100.f;
//        } else {
//            // Player at the right of the player
//            playerSpeed->x = -500.f;
//            playerSpeed->y = -100.f;
//        }
//    } else {
//        if (abs((int)skeletonBody->getPosition().x - (int)playerBody->getPosition().x) > ((int)skeletonBody->getSize().x / 2 - 10)) {
//            return;
//        }
//        if (skeletonBody->getPosition().x < playerBody->getPosition().x) {
//            // Player at the left of the player
//            skeletonBody->setPosition(playerBody->getPosition().x - 15, skeletonBody->getPosition().y);
//        } else {
//            // Player at the right of the player
//            skeletonBody->setPosition(playerBody->getPosition().x + 15, skeletonBody->getPosition().y);
//        }
//    }
//
//}

#endif //LD41_COLLISIONS_HPP

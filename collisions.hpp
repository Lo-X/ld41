//
// Created by loic on 21/04/18.
//

#ifndef LD41_COLLISIONS_HPP
#define LD41_COLLISIONS_HPP


#include "components/Body.hpp"
#include "components/SpeedComponent.hpp"

void playerCollideWithPlatform(Entity player, Entity platform)
{
    auto playerBody = player.component<BodyComponent>();

    if (playerBody->resting) {
        return;
    }

    auto playerSpeed = player.component<SpeedComponent>();
    auto playerControlled = player.component<PlayerControlledComponent>();
    auto platformBody = platform.component<BodyComponent>();

    playerBody->setPosition(playerBody->getPosition().x, platformBody->getPosition().y);
    playerBody->resting = true;
    if (playerSpeed.isValid()) {
        playerSpeed->x = 0;
        playerSpeed->y = 0;
    }
    if (playerControlled.isValid()) {
        playerControlled->action = PlayerControlledComponent::Standby;
    }
}

void playerCollideWithWall(Entity player, Entity wall)
{
    auto playerBody = player.component<BodyComponent>();

    if (playerBody->resting) {
        return;
    }

    auto playerSpeed = player.component<SpeedComponent>();
    auto playerControlled = player.component<PlayerControlledComponent>();
    auto wallBody = wall.component<BodyComponent>();

    if (playerBody->getPosition().x < wallBody->getPosition().x) {
        // Player at the left of the wall
        playerBody->setPosition(wallBody->getPosition().x - playerBody->getSize().x / 2, playerBody->getPosition().y);
    } else {
        // Player at the right of the wall
        playerBody->setPosition(wallBody->getPosition().x + wallBody->getSize().x + playerBody->getSize().x / 2, playerBody->getPosition().y);
    }
}

#endif //LD41_COLLISIONS_HPP

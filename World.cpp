//
// Created by loic on 21/04/18.
//

#include <Fluffy/ECS/EntityManager.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "World.hpp"
#include "typedefs.hpp"
#include "components/Body.hpp"
#include "components/Drawable.hpp"
#include "components/Camera.hpp"
#include "components/SpeedComponent.hpp"
#include "components/PlayerControlled.hpp"
#include "collisions.hpp"
#include "components/PlayerAnimation.hpp"
#include "components/BallHolder.hpp"
#include "PlayerController.hpp"

World::World(ServiceContainer &container)
: mContainer(container)
, mWorldView()
{
    loadTextures();
    buildScene();

    // camera


}

void World::update(Time dt)
{
    handleCollisions();
    markDeadEntities();
}

void World::loadTextures()
{

}

void World::buildScene()
{
    mWorldView.setSize(1280.f, 720.f);
    mWorldView.setCenter(1280.f/2, 720.f/2);

    auto textures = mContainer.get<TextureHolder>();
    auto entityManager = mContainer.get<EntityManager>();

    auto camera = entityManager->createEntity();
    camera.assign<CameraComponent>(mWorldView);

    // @todo dynamic loading from file // Tiled?
    auto background = entityManager->createEntity();
    auto body = background.assign<BodyComponent>(BodyComponent::None, false, false);
    background.assign<DrawableComponent>(sf::Sprite(mContainer.get<TextureHolder>()->get("background")), DrawableComponent::Layer::Background);

    auto floor = entityManager->createEntity();
    body = floor.assign<BodyComponent>(BodyComponent::Platform, true, false);
    body->setPosition(0, 720-150);
    body->setSize({1280*4, 150});
    floor.assign<DrawableComponent>(sf::Sprite(textures->get("floor"), {0, 0, 1280*4, 150}), DrawableComponent::Layer::Scene);

    auto wall1 = entityManager->createEntity();
    body = wall1.assign<BodyComponent>(BodyComponent::Wall, true, false);
    body->setPosition(800, 720-150-150);
    body->setSize({62, 150});
    wall1.assign<DrawableComponent>(sf::Sprite(textures->get("wall"), {0, 0, (int)body->getSize().x, (int)body->getSize().y}), DrawableComponent::Layer::Scene);

    auto platform = entityManager->createEntity();
    body = platform.assign<BodyComponent>(BodyComponent::Platform, true, false);
    body->setPosition(800, 720-150-150-62);
    body->setSize({380, 62});
    platform.assign<DrawableComponent>(sf::Sprite(textures->get("tile1"), {0, 0, 380, 62}), DrawableComponent::Layer::Scene);

    auto player = entityManager->createEntity();
    body = player.assign<BodyComponent>(BodyComponent::Player, true, true);
    body->setSize({36.f, 96.f});
    body->setOrigin({36.f/2, 96.f});
    body->setPosition(250.f, 720-150.f);
    body->collisionCallbacks[BodyComponent::Type::Platform] = playerCollideWithPlatform;
    body->collisionCallbacks[BodyComponent::Type::Wall] = playerCollideWithWall;
    body->collisionCallbacks[BodyComponent::Type::Ball] = playerCollideWithBall;
    player.assign<SpeedComponent>();
    player.assign<PlayerControlledComponent>();
    player.assign<DrawableComponent>();
    auto holder = player.assign<BallHolderComponent>();
    holder->handPosition = PlayerHandPositionStandingRight;
    auto animation = player.assign<PlayerAnimationComponent>();
    animation->currentAnimation = PlayerAnimationComponent::Standing;
    // Standing
    animation->animations[PlayerAnimationComponent::Standing].setTexture(textures->get("player_standing"));
    animation->animations[PlayerAnimationComponent::Standing].setFrameSize({64, 128});
    animation->animations[PlayerAnimationComponent::Standing].setFrameOrigin({0, 0});
    animation->animations[PlayerAnimationComponent::Standing].setOrigin({32.f, 128.f});
    animation->animations[PlayerAnimationComponent::Standing].setNumFrames(5);
    animation->animations[PlayerAnimationComponent::Standing].setDuration(milliseconds(1000));
    animation->animations[PlayerAnimationComponent::Standing].setRepeating(true);
    // Walking
    animation->animations[PlayerAnimationComponent::Walking].setTexture(textures->get("player_running"));
    animation->animations[PlayerAnimationComponent::Walking].setFrameSize({64, 128});
    animation->animations[PlayerAnimationComponent::Walking].setFrameOrigin({0, 0});
    animation->animations[PlayerAnimationComponent::Walking].setOrigin({32.f, 128.f});
    animation->animations[PlayerAnimationComponent::Walking].setNumFrames(3);
    animation->animations[PlayerAnimationComponent::Walking].setDuration(milliseconds(375));
    animation->animations[PlayerAnimationComponent::Walking].setRepeating(true);
    // Jumping
    animation->animations[PlayerAnimationComponent::Jumping].setTexture(textures->get("player_jumping"));
    animation->animations[PlayerAnimationComponent::Jumping].setFrameSize({64, 128});
    animation->animations[PlayerAnimationComponent::Jumping].setFrameOrigin({0, 0});
    animation->animations[PlayerAnimationComponent::Jumping].setOrigin({32.f, 128.f});
    animation->animations[PlayerAnimationComponent::Jumping].setNumFrames(2);
    animation->animations[PlayerAnimationComponent::Jumping].setDuration(milliseconds(250));
    animation->animations[PlayerAnimationComponent::Jumping].setRepeating(true);
    // Attacking
    animation->animations[PlayerAnimationComponent::Attacking].setTexture(textures->get("player_attacking"));
    animation->animations[PlayerAnimationComponent::Attacking].setFrameSize({64, 128});
    animation->animations[PlayerAnimationComponent::Attacking].setFrameOrigin({0, 0});
    animation->animations[PlayerAnimationComponent::Attacking].setOrigin({32.f, 128.f});
    animation->animations[PlayerAnimationComponent::Attacking].setNumFrames(4);
    animation->animations[PlayerAnimationComponent::Attacking].setDuration(milliseconds(333));
    animation->animations[PlayerAnimationComponent::Attacking].setRepeating(false);
    // Dead
    animation->animations[PlayerAnimationComponent::Dead].setTexture(textures->get("player_dead"));
    animation->animations[PlayerAnimationComponent::Dead].setFrameSize({85, 20});
    animation->animations[PlayerAnimationComponent::Dead].setFrameOrigin({0, 0});
    animation->animations[PlayerAnimationComponent::Dead].setOrigin({43.f, 20.f});
    animation->animations[PlayerAnimationComponent::Dead].setNumFrames(1);
    animation->animations[PlayerAnimationComponent::Dead].setDuration(milliseconds(2000));
    animation->animations[PlayerAnimationComponent::Dead].setRepeating(false);

    auto ball = entityManager->createEntity();
    body = ball.assign<BodyComponent>(BodyComponent::Ball, true, true);
    body->setPosition(500, 100);
    body->setSize({15, 15});
    body->setOrigin({8, 7});
    body->resting = false;
    body->collisionCallbacks[BodyComponent::Platform] = ballCollideWithPlatform;
    body->collisionCallbacks[BodyComponent::Wall] = ballCollideWithWall;
    ball.assign<SpeedComponent>();
    auto drawable = ball.assign<DrawableComponent>(sf::Sprite(textures->get("ball")), DrawableComponent::Layer::Scene);
    drawable->sprite.setOrigin(8, 7);
}

void World::handleCollisions()
{

}

void World::markDeadEntities()
{

}

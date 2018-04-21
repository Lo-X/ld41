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
    player.assign<SpeedComponent>();
    player.assign<PlayerControlledComponent>();
    player.assign<DrawableComponent>(sf::Sprite(textures->get("player")), DrawableComponent::Layer::Scene);
}

void World::handleCollisions()
{

}

void World::markDeadEntities()
{

}

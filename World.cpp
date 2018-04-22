//
// Created by loic on 21/04/18.
//

#include <Fluffy/ECS/EntityManager.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
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
#include "components/AIControlled.hpp"
#include "AIController.hpp"
#include "components/Ball.hpp"
#include "components/Team.hpp"
#include "components/HelpMessage.hpp"
#include "components/PointsDisplay.hpp"
#include "components/GoalMessage.hpp"
#include "components/Text.hpp"

World::World(ServiceContainer &container)
: mContainer(container)
, mWorldView()
, mBackgroundView()
{
    loadTextures();
    buildScene();
    buildUi();

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
    mBackgroundView.setSize(1280.f, 720.f);
    mBackgroundView.setCenter(1280.f/2, 720.f/2);

    auto textures = mContainer.get<TextureHolder>();
    auto entityManager = mContainer.get<EntityManager>();

    // @todo dynamic loading from file // Tiled?
    auto background = entityManager->createEntity();
    auto body = background.assign<BodyComponent>(BodyComponent::None, false, false);
    body->setSize({2*worldSize, 720});
    body->setPosition(-1280.f, 0);
    background.assign<DrawableComponent>(sf::Sprite(mContainer.get<TextureHolder>()->get("background"), {0,0, 2*(int)worldSize, 720}), DrawableComponent::Layer::Background);

    auto floor = entityManager->createEntity();
    body = floor.assign<BodyComponent>(BodyComponent::Platform, true, false);
    body->setPosition(-1280, 720-150);
    body->setSize({2*worldSize, 150});
    floor.assign<DrawableComponent>(sf::Sprite(textures->get("floor"), {0, 0, 1280*4, 150}), DrawableComponent::Layer::Scene);

    auto wall1 = entityManager->createEntity();
    body = wall1.assign<BodyComponent>(BodyComponent::Wall, true, false);
    body->setPosition(-1280/2.f, 0);
    body->setSize({1280/2.f, 720});
    wall1.assign<DrawableComponent>(sf::Sprite(textures->get("wall"), {0, 0, (int)body->getSize().x, (int)body->getSize().y}), DrawableComponent::Layer::Scene);

    auto wall2 = entityManager->createEntity();
    body = wall2.assign<BodyComponent>(BodyComponent::Wall, true, false);
    body->setPosition(1280*2.f, 0);
    body->setSize({1280/2.f, 720});
    wall2.assign<DrawableComponent>(sf::Sprite(textures->get("wall"), {0, 0, (int)body->getSize().x, (int)body->getSize().y}), DrawableComponent::Layer::Scene);

    auto platform1 = entityManager->createEntity();
    body = platform1.assign<BodyComponent>(BodyComponent::Platform, true, false);
    body->setPosition(ballSpawnX-380/2.f, 160);
    body->setSize({380, 62});
    platform1.assign<DrawableComponent>(sf::Sprite(textures->get("tile1"), {0, 0, 380, 62}), DrawableComponent::Layer::Scene);

    auto platform2 = entityManager->createEntity();
    body = platform2.assign<BodyComponent>(BodyComponent::Platform, true, false);
    body->setPosition(ballSpawnX-380-380/2.f, 358);
    body->setSize({380, 62});
    platform2.assign<DrawableComponent>(sf::Sprite(textures->get("tile1"), {0, 0, 380, 62}), DrawableComponent::Layer::Scene);

    auto platform3 = entityManager->createEntity();
    body = platform3.assign<BodyComponent>(BodyComponent::Platform, true, false);
    body->setPosition(ballSpawnX+380-380/2.f, 358);
    body->setSize({380, 62});
    platform3.assign<DrawableComponent>(sf::Sprite(textures->get("tile1"), {0, 0, 380, 62}), DrawableComponent::Layer::Scene);

    auto player = entityManager->createEntity();
    body = player.assign<BodyComponent>(BodyComponent::Player, true, true);
    body->setSize({36.f, 96.f});
    body->setOrigin({36.f/2, 96.f});
    body->setPosition(ballSpawnX - 200, 10);
    body->collisionCallbacks[BodyComponent::Type::Platform] = playerCollideWithPlatform;
    body->collisionCallbacks[BodyComponent::Type::Wall] = playerCollideWithWall;
    body->collisionCallbacks[BodyComponent::Type::Ball] = playerCollideWithBall;
    body->collisionCallbacks[BodyComponent::Type::Creature] = playerCollideWithSkeleton;
    player.assign<SpeedComponent>();
    player.assign<PlayerControlledComponent>();
    player.assign<DrawableComponent>();
    player.assign<TeamComponent>(TeamComponent::Player);
    player.assign<CameraComponent>(mWorldView);
    player.assign<BackgroundCameraComponent>(mBackgroundView, .75);
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
    // Throwing
    animation->animations[PlayerAnimationComponent::Throwing].setTexture(textures->get("player_throwing"));
    animation->animations[PlayerAnimationComponent::Throwing].setFrameSize({64, 128});
    animation->animations[PlayerAnimationComponent::Throwing].setFrameOrigin({0, 0});
    animation->animations[PlayerAnimationComponent::Throwing].setOrigin({32.f, 128.f});
    animation->animations[PlayerAnimationComponent::Throwing].setNumFrames(4);
    animation->animations[PlayerAnimationComponent::Throwing].setDuration(milliseconds(333));
    animation->animations[PlayerAnimationComponent::Throwing].setRepeating(false);
    // Attacking
    animation->animations[PlayerAnimationComponent::Attacking].setTexture(textures->get("player_attacking"));
    animation->animations[PlayerAnimationComponent::Attacking].setFrameSize({64, 128});
    animation->animations[PlayerAnimationComponent::Attacking].setFrameOrigin({0, 0});
    animation->animations[PlayerAnimationComponent::Attacking].setOrigin({32.f, 128.f});
    animation->animations[PlayerAnimationComponent::Attacking].setNumFrames(4);
    animation->animations[PlayerAnimationComponent::Attacking].setDuration(milliseconds(180));
    animation->animations[PlayerAnimationComponent::Attacking].setRepeating(false);
    // Dead
    animation->animations[PlayerAnimationComponent::Dead].setTexture(textures->get("player_dead"));
    animation->animations[PlayerAnimationComponent::Dead].setFrameSize({85, 20});
    animation->animations[PlayerAnimationComponent::Dead].setFrameOrigin({0, 0});
    animation->animations[PlayerAnimationComponent::Dead].setOrigin({43.f, 20.f});
    animation->animations[PlayerAnimationComponent::Dead].setNumFrames(1);
    animation->animations[PlayerAnimationComponent::Dead].setDuration(milliseconds(2000));
    animation->animations[PlayerAnimationComponent::Dead].setRepeating(false);

    mBall = entityManager->createEntity();
    body = mBall.assign<BodyComponent>(BodyComponent::Ball, true, true);
    body->setPosition(ballSpawnX, ballSpawnY);
    body->setSize({15, 15});
    body->setOrigin({8, 7});
    body->resting = false;
    body->collisionCallbacks[BodyComponent::Platform] = ballCollideWithPlatform;
    body->collisionCallbacks[BodyComponent::Wall] = ballCollideWithWall;
    body->collisionCallbacks[BodyComponent::Goal] = [&] (Entity ball, Entity goal) {
        auto taken = ball.component<BallComponent>();

        if (!taken->taken()) {
            auto body = ball.component<BodyComponent>();
            auto speed = ball.component<SpeedComponent>();
            auto team = goal.component<TeamComponent>();
            body->setPosition(ballSpawnX + 400, -80000);
            body->resting = false;
            speed->x = 0;
            speed->y = 0;

            mGoalWaiting = true;
            TeamComponent::Teams t = team->getOpposing(team->team);
            mContainer.get<EventManager>()->emit(GoalScoredEvent(t));
        }
    };
    mBall.assign<SpeedComponent>();
    mBall.assign<BallComponent>(mContainer.get<EventManager>());
    auto drawable = mBall.assign<DrawableComponent>(sf::Sprite(textures->get("ball")), DrawableComponent::Layer::Scene);
    drawable->sprite.setOrigin(8, 7);

    spawnSkeleton({ballSpawnX + 200, 10}, AIControlledComponent::Attacker);
    spawnSkeleton({ballSpawnX + 900, 10}, AIControlledComponent::Keeper);

    auto goal1 = entityManager->createEntity();
    body = goal1.assign<BodyComponent>(BodyComponent::Goal, true, false);
    body->setPosition(20, 720-150);
    body->setSize({40, 120});
    body->setOrigin({20, 120});
    goal1.assign<TeamComponent>(TeamComponent::Player);
    drawable = goal1.assign<DrawableComponent>(sf::Sprite(textures->get("goal")), DrawableComponent::Layer::Scene);
    drawable->sprite.setOrigin({20, 120});
    drawable->sprite.setScale({-1.f, 1});

    auto goal2 = entityManager->createEntity();
    body = goal2.assign<BodyComponent>(BodyComponent::Goal, true, false);
    body->setPosition(worldSize-20, 720-150);
    body->setSize({40, 120});
    body->setOrigin({20, 120});
    goal2.assign<TeamComponent>(TeamComponent::AI);
    drawable = goal2.assign<DrawableComponent>(sf::Sprite(textures->get("goal")), DrawableComponent::Layer::Scene);
    drawable->sprite.setOrigin({20, 120});
}

void World::buildUi()
{
    auto entityManager = mContainer.get<EntityManager>();

    auto help = entityManager->createEntity();
    auto body = help.assign<BodyComponent>(BodyComponent::None, false, false);
    body->setSize({653, 124});
    body->setPosition(1280.f/2 - 652.f/2, 720-124-8);
    help.assign<DrawableComponent>(sf::Sprite(mContainer.get<TextureHolder>()->get("ui_help")), DrawableComponent::Layer::Foreground);
    help.assign<HelpMessageComponent>();

    auto goal = entityManager->createEntity();
    body = goal.assign<BodyComponent>(BodyComponent::None, false, false);
    body->setSize({460, 124});
    body->setPosition(1280.f/2 - 460.f/2, 720.f/2 - 124/2.f);
    auto drawable = goal.assign<DrawableComponent>(sf::Sprite(mContainer.get<TextureHolder>()->get("ui_goal")), DrawableComponent::Layer::Foreground);
    auto message = goal.assign<HelpMessageComponent>();
    goal.assign<GoalMessageComponent>();
    drawable->visible = false;
    message->visible = false;

    auto points_player = entityManager->createEntity();
    body = points_player.assign<BodyComponent>(BodyComponent::None, false, false);
    body->setSize({121, 29});
    body->setPosition(10, 10);
    points_player.assign<DrawableComponent>(sf::Sprite(mContainer.get<TextureHolder>()->get("ui_points_player")), DrawableComponent::Layer::Foreground);
    points_player.assign<PointsDisplayComponent>(TeamComponent::Player);
    auto text = points_player.assign<TextComponent>();
    text->text.setFont(mContainer.get<FontHolder>()->get("main"));
    text->text.setCharacterSize(16);
    text->text.setFillColor(sf::Color(36, 36, 36));
    text->text.setString("0");
    text->text.setPosition(42, 15);

    auto points_ai = entityManager->createEntity();
    body = points_ai.assign<BodyComponent>(BodyComponent::None, false, false);
    body->setSize({121, 29});
    body->setPosition(1280-10-121, 10);
    points_ai.assign<DrawableComponent>(sf::Sprite(mContainer.get<TextureHolder>()->get("ui_points_ai")), DrawableComponent::Layer::Foreground);
    points_ai.assign<PointsDisplayComponent>(TeamComponent::AI);
    text = points_ai.assign<TextComponent>();
    text->text.setFont(mContainer.get<FontHolder>()->get("main"));
    text->text.setCharacterSize(16);
    text->text.setFillColor(sf::Color(36, 36, 36));
    text->text.setString("0");
    text->text.setPosition(1280-10-121 + 30, 15);
}

void World::spawnSkeleton(sf::Vector2f position, AIControlledComponent::Role role) const
{
    auto entityManager = mContainer.get<EntityManager>();
    auto textures = mContainer.get<TextureHolder>();
    auto skeleton = entityManager->createEntity();
    auto body = skeleton.assign<BodyComponent>(BodyComponent::Creature, true, true);
    body->setSize({36.f, 96.f});
    body->setOrigin({36.f/2, 96.f});
    body->setPosition(position);
    body->collisionCallbacks[BodyComponent::Platform] = skeletonCollideWithPlatform;
    body->collisionCallbacks[BodyComponent::Wall] = skeletonCollideWithWall;
    body->collisionCallbacks[BodyComponent::Ball] = skeletonCollideWithBall;
    skeleton.assign<SpeedComponent>();
    skeleton.assign<AIControlledComponent>(role);
    skeleton.assign<DrawableComponent>();
    skeleton.assign<TeamComponent>(TeamComponent::AI);
    auto holder = skeleton.assign<BallHolderComponent>();
    holder->handPosition = SkeletonHandPositionStandingRight;
    auto animation = skeleton.assign<PlayerAnimationComponent>();
    animation->currentAnimation = PlayerAnimationComponent::Standing;
    // Standing
    animation->animations[PlayerAnimationComponent::Standing].setTexture(textures->get("ai_standing"));
    animation->animations[PlayerAnimationComponent::Standing].setFrameSize({64, 110});
    animation->animations[PlayerAnimationComponent::Standing].setFrameOrigin({0, 0});
    animation->animations[PlayerAnimationComponent::Standing].setOrigin({32.f, 110.f});
    animation->animations[PlayerAnimationComponent::Standing].setNumFrames(2);
    animation->animations[PlayerAnimationComponent::Standing].setDuration(milliseconds(666));
    animation->animations[PlayerAnimationComponent::Standing].setRepeating(true);
    // Walking
    animation->animations[PlayerAnimationComponent::Walking].setTexture(textures->get("ai_running"));
    animation->animations[PlayerAnimationComponent::Walking].setFrameSize({64, 110});
    animation->animations[PlayerAnimationComponent::Walking].setFrameOrigin({0, 0});
    animation->animations[PlayerAnimationComponent::Walking].setOrigin({32.f, 110.f});
    animation->animations[PlayerAnimationComponent::Walking].setNumFrames(2);
    animation->animations[PlayerAnimationComponent::Walking].setDuration(milliseconds(400));
    animation->animations[PlayerAnimationComponent::Walking].setRepeating(true);
    // Jumping
    animation->animations[PlayerAnimationComponent::Jumping].setTexture(textures->get("ai_standing"));
    animation->animations[PlayerAnimationComponent::Jumping].setFrameSize({64, 110});
    animation->animations[PlayerAnimationComponent::Jumping].setFrameOrigin({0, 0});
    animation->animations[PlayerAnimationComponent::Jumping].setOrigin({32.f, 110.f});
    animation->animations[PlayerAnimationComponent::Jumping].setNumFrames(1);
    animation->animations[PlayerAnimationComponent::Jumping].setDuration(milliseconds(400));
    animation->animations[PlayerAnimationComponent::Jumping].setRepeating(true);
    // Throwing
    animation->animations[PlayerAnimationComponent::Throwing].setTexture(textures->get("ai_attacking"));
    animation->animations[PlayerAnimationComponent::Throwing].setFrameSize({64, 110});
    animation->animations[PlayerAnimationComponent::Throwing].setFrameOrigin({0, 0});
    animation->animations[PlayerAnimationComponent::Throwing].setOrigin({32.f, 110.f});
    animation->animations[PlayerAnimationComponent::Throwing].setNumFrames(2);
    animation->animations[PlayerAnimationComponent::Throwing].setDuration(milliseconds(400));
    animation->animations[PlayerAnimationComponent::Throwing].setRepeating(false);
    // Attacking
    animation->animations[PlayerAnimationComponent::Attacking].setTexture(textures->get("ai_attacking"));
    animation->animations[PlayerAnimationComponent::Attacking].setFrameSize({64, 110});
    animation->animations[PlayerAnimationComponent::Attacking].setFrameOrigin({0, 0});
    animation->animations[PlayerAnimationComponent::Attacking].setOrigin({32.f, 110.f});
    animation->animations[PlayerAnimationComponent::Attacking].setNumFrames(2);
    animation->animations[PlayerAnimationComponent::Attacking].setDuration(milliseconds(400));
    animation->animations[PlayerAnimationComponent::Attacking].setRepeating(false);
}

void World::handleCollisions()
{

}

void World::markDeadEntities()
{

}

void World::respawnBall()
{
    auto body = mBall.component<BodyComponent>();
    auto speed = mBall.component<SpeedComponent>();
    auto team = mBall.component<TeamComponent>();
    body->setPosition(ballSpawnX, ballSpawnY);
    body->resting = false;
    speed->x = 0;
    speed->y = 0;
    mGoalWaiting = false;
}

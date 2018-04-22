//
// Created by loic on 22/04/18.
//

#include "SoundSystem.hpp"
#include "../components/SoundPlayer.hpp"

namespace {
std::random_device random_device;
std::seed_seq      seed({ random_device(), random_device(), random_device(), random_device(), random_device() });
std::mt19937       random_engine(seed);

float custom_rand(float min, float max)
{
    using distribution_type = std::uniform_real_distribution<float>;
    auto dist = distribution_type(min, max);

    return dist(random_engine);
}
}

SoundSystem::~SoundSystem()
{
    mGoalSlot.disconnect();
    mBallTaken.disconnect();
    mJumpAi.disconnect();
    mJumpPlayer.disconnect();
    mPunch.disconnect();
    mThrow.disconnect();
    mAfterGameTick.disconnect();
}

void SoundSystem::configure(EntityManager &entityManager, EventManager &eventManager)
{
    mEntityManager = &entityManager;

    mGoalSlot = eventManager.connect<GoalScoredEvent>(std::bind(&SoundSystem::onGoalScored, this, std::placeholders::_1));
    mBallTaken = eventManager.connect<BallTakenEvent>(std::bind(&SoundSystem::onBallTaken, this, std::placeholders::_1));
    mJumpAi = eventManager.connect<JumpAiEvent>(std::bind(&SoundSystem::onJumpAi, this, std::placeholders::_1));
    mJumpPlayer = eventManager.connect<JumpPlayerEvent>(std::bind(&SoundSystem::onJumpPlayer, this, std::placeholders::_1));
    mPunch = eventManager.connect<PunchEvent>(std::bind(&SoundSystem::onPunch, this, std::placeholders::_1));
    mThrow = eventManager.connect<ThrowEvent>(std::bind(&SoundSystem::onThrow, this, std::placeholders::_1));
    mAfterGameTick = eventManager.connect<AfterGameTickEvent>(std::bind(&SoundSystem::onAfterGameTick, this, std::placeholders::_1));
}

void SoundSystem::onGoalScored(const GoalScoredEvent &event)
{
    for (auto entity: mEntityManager->each<SoundPlayerComponent>()) {
        auto sound = entity.component<SoundPlayerComponent>();
        sound->player->play("goal", {0, 0, 0}, custom_rand(0.5f, 1.7f));
        sound->player->play("applause", {0, 0, 0}, custom_rand(0.8f, 1.7f));
    }
}

void SoundSystem::onJumpAi(const JumpAiEvent &event)
{
    for (auto entity: mEntityManager->each<SoundPlayerComponent>()) {
        auto sound = entity.component<SoundPlayerComponent>();
        sound->player->play("jump_ai", {0, 0, 0}, custom_rand(0.8f, 1.2f));
    }
}

void SoundSystem::onJumpPlayer(const JumpPlayerEvent &event)
{
    for (auto entity: mEntityManager->each<SoundPlayerComponent>()) {
        auto sound = entity.component<SoundPlayerComponent>();
        sound->player->play("jump_player", {0, 0, 0}, custom_rand(0.8f, 1.2f));
    }
}

void SoundSystem::onBallTaken(const BallTakenEvent &event)
{
    for (auto entity: mEntityManager->each<SoundPlayerComponent>()) {
        auto sound = entity.component<SoundPlayerComponent>();
        sound->player->play("pickup", {0, 0, 0}, custom_rand(0.8f, 1.2f));
    }
}

void SoundSystem::onPunch(const PunchEvent &event)
{
    for (auto entity: mEntityManager->each<SoundPlayerComponent>()) {
        auto sound = entity.component<SoundPlayerComponent>();
        sound->player->play("punch", {0, 0, 0}, custom_rand(0.8f, 1.2f));
    }
}

void SoundSystem::onThrow(const ThrowEvent &event)
{
    for (auto entity: mEntityManager->each<SoundPlayerComponent>()) {
        auto sound = entity.component<SoundPlayerComponent>();
        sound->player->play("throw", {0, 0, 0}, custom_rand(0.8f, 1.2f));
    }
}

void SoundSystem::onAfterGameTick(const AfterGameTickEvent &event)
{
    for (auto entity: mEntityManager->each<SoundPlayerComponent>()) {
        auto sound = entity.component<SoundPlayerComponent>();
        sound->player->removeStoppedSounds();
    }
}

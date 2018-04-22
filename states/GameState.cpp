//
// Created by loic on 21/04/18.
//

#include <Fluffy/ECS/SystemManager.hpp>
#include "GameState.hpp"
#include "../systems/PlayerControlledSystem.hpp"

using namespace Fluffy::ECS;

void GameState::initialize(ServiceContainer &serviceContainer)
{
    // Create world
    mWorld = new World(serviceContainer);

    mTickSlot = serviceContainer.get<EventManager>()->connect<GameTickEvent>(
        std::bind(&GameState::onGameTick, this, std::placeholders::_1)
    );
    mAfterTickSlot = serviceContainer.get<EventManager>()->connect<AfterGameTickEvent>(
        std::bind(&GameState::onAfterGameTick, this, std::placeholders::_1)
    );
    mGoalScored = serviceContainer.get<EventManager>()->connect<GoalScoredEvent>(
            std::bind(&GameState::onGoalScored, this, std::placeholders::_1)
    );

    serviceContainer.get<MusicHolder>()->play("main");
}

void GameState::terminate()
{
    mTickSlot.disconnect();
    mAfterTickSlot.disconnect();
    // remove systems

    delete mWorld;
}

void GameState::onGameTick(const GameTickEvent &event)
{
    static AIControlledComponent::Role lastSpawned = AIControlledComponent::Role::Attacker;

    if (!mWorld->started()) {
        mStartTimer += event.dt;
        if (mStartTimer > seconds(5.1f)) {
            mWorld->startGame();
        }
    }

    if (!isPaused()) {
        mWorld->update(event.dt);
//        mSpawnNext += event.dt;

        if (mWorld->waiting()) {
            mAfterGoalTimer += event.dt;
            if (mAfterGoalTimer > seconds(3.1f)) {
                mWorld->respawnBall();
                mAfterGoalTimer = Time::Zero;
            }
        }

//        if (mSpawnNext > seconds(45) && mSpawned < 2) {
//            if (lastSpawned == AIControlledComponent::Role::Attacker) {
//                lastSpawned = AIControlledComponent::Role::Keeper;
//            } else {
//                lastSpawned = AIControlledComponent::Role::Attacker;
//            }
//            mWorld->spawnSkeleton({1500, 100}, lastSpawned);
//            mSpawnNext = Time::Zero;
//            mSpawned++;
//        }
    }
}

void GameState::onAfterGameTick(const AfterGameTickEvent &event)
{
    // world destroy dead entities
    mWorld->destroyDeadEntities();
}

void GameState::onGoalScored(const GoalScoredEvent &event)
{
    if (event.team == TeamComponent::Player && mSpawned < 10) {
        mSpawned++;
        if (mSpawned % 2 == 0) {
            mWorld->spawnSkeleton({1500, 100}, AIControlledComponent::Role::Attacker);
        }
    }
}

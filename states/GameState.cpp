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
    if (!isPaused()) {
        mWorld->update(event.dt);
    }
}

void GameState::onAfterGameTick(const AfterGameTickEvent &event)
{
    // world destroy dead entities
    mWorld->destroyDeadEntities();
}

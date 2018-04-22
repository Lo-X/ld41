//
// Created by loic on 21/04/18.
//

#ifndef LD41_GAMESTATE_HPP
#define LD41_GAMESTATE_HPP

#include <Fluffy/Event/Event.hpp>
#include <Fluffy/State/State.hpp>
#include "../World.hpp"

using namespace Fluffy::State;
using namespace Fluffy::Event;

class GameState : public State<GameState>
{
public:
    void initialize(ServiceContainer &serviceContainer) override;
    void terminate() override;

    void onGameTick(const GameTickEvent& event);
    void onAfterGameTick(const AfterGameTickEvent& event);

private:
    World* mWorld;
    Slot mTickSlot;
    Slot mAfterTickSlot;
    Time mAfterGoalTimer;
    Time mSpawnNext;
    unsigned int mSpawned = 0;
};


#endif //LD41_GAMESTATE_HPP

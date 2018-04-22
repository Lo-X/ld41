//
// Created by loic on 21/04/18.
//

#ifndef LD41_GAMESTATE_HPP
#define LD41_GAMESTATE_HPP

#include <Fluffy/Event/Event.hpp>
#include <Fluffy/State/State.hpp>
#include "../World.hpp"
#include "../events.hpp"

using namespace Fluffy::State;
using namespace Fluffy::Event;

class GameState : public State<GameState>
{
public:
    void initialize(ServiceContainer &serviceContainer) override;
    void terminate() override;

    void onGameTick(const GameTickEvent& event);
    void onAfterGameTick(const AfterGameTickEvent& event);
    void onGoalScored(const GoalScoredEvent& event);

private:
    World* mWorld;
    Slot mTickSlot;
    Slot mAfterTickSlot;
    Slot mGoalScored;
    Time mAfterGoalTimer = Time::Zero;
    Time mSpawnNext = Time::Zero;
    Time mStartTimer = Time::Zero;
    unsigned int mSpawned = 0;
};


#endif //LD41_GAMESTATE_HPP

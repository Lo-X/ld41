//
// Created by loic on 22/04/18.
//

#ifndef LD41_SOUNDSYSTEM_HPP
#define LD41_SOUNDSYSTEM_HPP

#include <Fluffy/ECS/System.hpp>
#include "../events.hpp"

using namespace Fluffy::ECS;

class SoundSystem : public System<SoundSystem>
{
public:
    ~SoundSystem();
    void configure(EntityManager &entityManager, EventManager &eventManager) override;
    void update(EntityManager &entityManager, Fluffy::Utility::Time dt) override {}

    void onGoalScored(const GoalScoredEvent& event);
    void onJumpAi(const JumpAiEvent& event);
    void onJumpPlayer(const JumpPlayerEvent& event);
    void onBallTaken(const BallTakenEvent& event);
    void onPunch(const PunchEvent& event);
    void onThrow(const ThrowEvent& event);

    void onAfterGameTick(const AfterGameTickEvent& event);

private:
    EntityManager* mEntityManager = nullptr;
    Slot mGoalSlot;
    Slot mJumpAi;
    Slot mJumpPlayer;
    Slot mBallTaken;
    Slot mPunch;
    Slot mThrow;
    Slot mAfterGameTick;
};


#endif //LD41_SOUNDSYSTEM_HPP

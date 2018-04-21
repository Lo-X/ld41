//
// Created by loic on 21/04/18.
//

#ifndef LD41_PLAYERCONTROLLER_HPP
#define LD41_PLAYERCONTROLLER_HPP

#include <Fluffy/Event/Event.hpp>
#include <Fluffy/Utility/Clock.hpp>
#include <Fluffy/Utility/ServiceContainer.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include "events.hpp"
#include "components/PlayerControlled.hpp"
#include "components/PlayerAnimation.hpp"
#include "components/BallHolder.hpp"

using namespace Fluffy::Utility;

#define PlayerHandPositionStandingRight sf::Vector2f(-20.f, -36.f)
#define PlayerHandPositionStandingLeft sf::Vector2f(20.f, -36.f)
#define PlayerHandPositionThrowingRight sf::Vector2f(-20.f, -60.f)
#define PlayerHandPositionThrowingLeft sf::Vector2f(20.f, -60.f)

class AttackAction
{
public:
    AttackAction();
    void execute(ComponentHandle<PlayerControlledComponent> player, ComponentHandle<PlayerAnimationComponent> animation, ComponentHandle<BallHolderComponent> holder);
    bool isFinished();

private:
    void runTask();

private:
    sf::Thread mThread;
    sf::Mutex  mMutex;
    bool       mFinished;
    Clock      mClock;
    ComponentHandle<PlayerControlledComponent> mPlayer;
    ComponentHandle<PlayerAnimationComponent>  mAnimation;
    ComponentHandle<BallHolderComponent>       mHolder;

};


class PlayerController
{
public:
    explicit PlayerController(ServiceContainer* container);
    ~PlayerController();

    void onBeforeTick(const BeforeGameTickEvent& event);
    void onJoystickButtonPressedEvent(const JoystickButtonPressedEvent& event);
    void onJoystickXAnalogUsedEvent(const JoystickXAnalogEvent& event);
private:
    ServiceContainer* mContainer;
    Slot mBeforeTick;
    Slot mJoystickButtonPressedSlot;
    Slot mJoystickXAxisUsedSlot;
    AttackAction mAttackAction;
};


#endif //LD41_PLAYERCONTROLLER_HPP

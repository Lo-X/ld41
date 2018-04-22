//
// Created by loic on 22/04/18.
//

#ifndef LD41_AICONTROLLER_HPP
#define LD41_AICONTROLLER_HPP

#include <Fluffy/Event/Event.hpp>
#include <Fluffy/Utility/Clock.hpp>
#include <Fluffy/Utility/ServiceContainer.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include "events.hpp"
#include "components/PlayerAnimation.hpp"
#include "components/BallHolder.hpp"
#include "components/AIControlled.hpp"

using namespace Fluffy::Utility;

#define SkeletonHandPositionStandingRight sf::Vector2f(-20.f, -36.f)
#define SkeletonHandPositionStandingLeft sf::Vector2f(20.f, -36.f)
#define SkeletonHandPositionThrowingRight sf::Vector2f(-20.f, -60.f)
#define SkeletonHandPositionThrowingLeft sf::Vector2f(20.f, -60.f)

class SThrowAction
{
public:
    SThrowAction();
    void execute(ComponentHandle<AIControlledComponent> skeleton, ComponentHandle<PlayerAnimationComponent> animation, ComponentHandle<BallHolderComponent> holder);
    bool isFinished();

private:
    void runTask();

private:
    sf::Thread mThread;
    sf::Mutex  mMutex;
    bool       mFinished;
    Clock      mClock;
    ComponentHandle<AIControlledComponent>     mSkeleton;
    ComponentHandle<PlayerAnimationComponent>  mAnimation;
    ComponentHandle<BallHolderComponent>       mHolder;

};


class AIController
{
public:
    explicit AIController(ServiceContainer* container);
    ~AIController();
private:
    ServiceContainer* mContainer;
    Slot mBeforeTick;
    Slot mJoystickButtonPressedSlot;
    Slot mJoystickXAxisUsedSlot;
    SThrowAction mAttackAction;
};


#endif //LD41_AICONTROLLER_HPP

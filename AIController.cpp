//
// Created by loic on 22/04/18.
//

#include <Fluffy/Event/EventManager.hpp>
#include <Fluffy/ECS/EntityManager.hpp>
#include <SFML/System/Lock.hpp>
#include <iostream>
#include "AIController.hpp"
#include "components/Body.hpp"
#include "components/SpeedComponent.hpp"
#include "components/PlayerAnimation.hpp"
#include "components/BallHolder.hpp"
#include "components/Ball.hpp"

using namespace Fluffy::ECS;
using namespace Fluffy::Event;

SThrowAction::SThrowAction()
        : mThread(&SThrowAction::runTask, this),
          mFinished(false)
{
}

void SThrowAction::execute(ComponentHandle<AIControlledComponent> skeleton, ComponentHandle<PlayerAnimationComponent> animation, ComponentHandle<BallHolderComponent> holder)
{
    mFinished = false;
    mSkeleton = skeleton;
    mAnimation = animation;
    mHolder = holder;
    mClock.restart();
    mThread.launch();
}

bool SThrowAction::isFinished()
{
    sf::Lock lock(mMutex);

    return mFinished;
}

void SThrowAction::runTask()
{
    bool ended = false;

    if (mAnimation.isValid() && mHolder.isValid() && mHolder->holding && mHolder->ball.isValid()) {
        auto speed = mHolder->ball.component<SpeedComponent>();
        auto body = mHolder->ball.component<BodyComponent>();
        auto taken = mHolder->ball.component<BallComponent>();

        if (mAnimation->direction == PlayerAnimationComponent::Directions::Right) {
            body->move(SkeletonHandPositionThrowingRight);
            speed->x = mHolder->ballSpeed.x;
            speed->y = mHolder->ballSpeed.y;
        } else {
            body->move(SkeletonHandPositionThrowingLeft);
            speed->x = -mHolder->ballSpeed.x;
            speed->y = mHolder->ballSpeed.y;
        }

        body->resting = false;
        mHolder->holding = false;
        taken->taken = false;
    }

    while (!ended) {
        if (mClock.elapsedTime() >= milliseconds(430)) {
            ended = true;
        }
    }

    {
        sf::Lock lock(mMutex);
        if (mSkeleton.isValid()) {
            mSkeleton->action = AIControlledComponent::Actions::Standby;
        }
        if (mAnimation.isValid()) {
            mAnimation->currentAnimation = PlayerAnimationComponent::Standing;
        }
        mFinished = true;
    }
}

/**********************************************************************************************************************/

AIController::AIController(ServiceContainer *container)
        : mContainer(container)
{

}

AIController::~AIController()
{

}
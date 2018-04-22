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
#include "components/PlayerControlled.hpp"

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
        taken->drop();
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

SAttackAction::SAttackAction()
        : mThread(&SAttackAction::runTask, this),
          mFinished(false)
{
}

void SAttackAction::execute(ComponentHandle<AIControlledComponent> entity, ComponentHandle<PlayerAnimationComponent> animation)
{
    mFinished = false;
    mEntity = entity;
    mAnimation = animation;
    mClock.restart();
    mThread.launch();
}

bool SAttackAction::isFinished()
{
    sf::Lock lock(mMutex);

    return mFinished;
}

void SAttackAction::runTask()
{
    bool ended = false;
    while (!ended) {
        if (mClock.elapsedTime() >= milliseconds(250)) {
            ended = true;
        }
    }

    {
        sf::Lock lock(mMutex);
        if (mEntity.isValid()) {
            mEntity->action = AIControlledComponent::Actions::Standby;
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
    mTick = mContainer->get<EventManager>()->connect<GameTickEvent>(std::bind(&AIController::onTick, this, std::placeholders::_1));
    mBallTaken = mContainer->get<EventManager>()->connect<BallTakenEvent>(std::bind(&AIController::onBallTaken, this, std::placeholders::_1));
    mBallDropped = mContainer->get<EventManager>()->connect<BallDroppedEvent>(std::bind(&AIController::onBallDropped, this, std::placeholders::_1));
    mGoalScored = mContainer->get<EventManager>()->connect<GoalScoredEvent>(std::bind(&AIController::onGoalScored, this, std::placeholders::_1));
}

AIController::~AIController()
{
    mTick.disconnect();
    mBallTaken.disconnect();
    mBallDropped.disconnect();
    mGoalScored.disconnect();
}

void AIController::onTick(const GameTickEvent &tick)
{
    auto entityManager = mContainer->get<EntityManager>();

    for (auto entity : entityManager->each<AIControlledComponent, BodyComponent, BallHolderComponent>()) {
        auto controller = entity.component<AIControlledComponent>();

        switch (controller->state) {

            /****************************************************************/
            /*********************** GO FOR BALL ****************************/
            /****************************************************************/
            case AIControlledComponent::LookingForBall: {
                auto body = entity.component<BodyComponent>();
                auto holder = entity.component<BallHolderComponent>();
                auto animation = entity.component<PlayerAnimationComponent>();
                auto speed = entity.component<SpeedComponent>();

                //
                Entity ball;
                for (auto b : entityManager->each<BallComponent>()) {
                    ball = b;
                    break;
                };
                if (!ball.isValid()) { return; }

                //
                auto ballBody = ball.component<BodyComponent>();
                sf::Vector2f diff = body->getPosition() - ballBody->getPosition();

                if (controller->action == AIControlledComponent::Actions::Standby) {
                    if (diff.y > 120 && controller->canMove() && controller->action != AIControlledComponent::Actions::Jump) {
                        animation->currentAnimation = PlayerAnimationComponent::Jumping;
                        if (diff.x < 0) {
                            animation->setDirection(PlayerAnimationComponent::Directions::Right);
                        } else {
                            animation->setDirection(PlayerAnimationComponent::Directions::Left);
                        }
                        controller->action = AIControlledComponent::Actions::Jump;
                        speed->y = -controller->speed.y;
                        body->resting = false;
                        body->setPosition(body->getPosition().x + 2.f, body->getPosition().y);
                    } else {
                        if (diff.x < 0) {
                            animation->currentAnimation = PlayerAnimationComponent::Walking;
                            animation->setDirection(PlayerAnimationComponent::Directions::Right);
                            speed->x = controller->speed.x;
                            controller->action = AIControlledComponent::Actions::WalkRight;
                            holder->handPosition = SkeletonHandPositionStandingRight;
                        } else {
                            animation->currentAnimation = PlayerAnimationComponent::Walking;
                            animation->setDirection(PlayerAnimationComponent::Directions::Left);
                            speed->x = -controller->speed.x;
                            controller->action = AIControlledComponent::Actions::WalkLeft;
                            holder->handPosition = SkeletonHandPositionStandingLeft;
                        }
                    }
                } else if (controller->action == AIControlledComponent::Actions::Jump) {
                    if (diff.x < 0) {
                        animation->setDirection(PlayerAnimationComponent::Directions::Right);
                        speed->x = controller->speed.x;
                        holder->handPosition = SkeletonHandPositionStandingRight;
                    } else {
                        animation->setDirection(PlayerAnimationComponent::Directions::Left);
                        speed->x = -controller->speed.x;
                        holder->handPosition = SkeletonHandPositionStandingLeft;
                    }
                }
            }
                break;



            /****************************************************************/
            /********************** GO FOR PLAYER ***************************/
            /****************************************************************/
            case AIControlledComponent::GoForPlayer: {
                auto body = entity.component<BodyComponent>();
                auto holder = entity.component<BallHolderComponent>();
                auto animation = entity.component<PlayerAnimationComponent>();
                auto speed = entity.component<SpeedComponent>();

                //
                Entity player;
                for (auto e : entityManager->each<BodyComponent, TeamComponent, PlayerControlledComponent>()) {
                    if (e.component<BodyComponent>()->type == BodyComponent::Player && e.component<TeamComponent>()->team == TeamComponent::Player) {
                        player = e;
                        break;
                    }
                };
                if (!player.isValid()) { return; }

                //
                auto playerBody = player.component<BodyComponent>();
                sf::Vector2f diff = body->getPosition() - playerBody->getPosition();

                if (controller->action == AIControlledComponent::Actions::Standby) {
                    if (diff.y > 120 && controller->canMove() && controller->action != AIControlledComponent::Actions::Jump) {
                        animation->currentAnimation = PlayerAnimationComponent::Jumping;
                        if (diff.x < 0) {
                            animation->setDirection(PlayerAnimationComponent::Directions::Right);
                        } else {
                            animation->setDirection(PlayerAnimationComponent::Directions::Left);
                        }
                        controller->action = AIControlledComponent::Actions::Jump;
                        speed->y = -controller->speed.y;
                        body->resting = false;
                        body->setPosition(body->getPosition().x + 2.f, body->getPosition().y);
                    } else {
                        if (!holder->holding && abs((int)diff.x) <= (body->getSize().x) && controller->canMove()) {
                            // Set the right direction just in case
                            if (diff.x < 0) {
                                animation->setDirection(PlayerAnimationComponent::Directions::Right);
                            } else {
                                animation->setDirection(PlayerAnimationComponent::Directions::Left);
                            }

                            // PUNCH IT!!!
                            animation->currentAnimation = PlayerAnimationComponent::Attacking;
                            animation->animations[animation->currentAnimation].restart();
                            controller->action = AIControlledComponent::Actions::Attack;
                            mAttackAction.execute(controller, animation);
                        } else {
                            if (diff.x < 0) {
                                animation->currentAnimation = PlayerAnimationComponent::Walking;
                                animation->setDirection(PlayerAnimationComponent::Directions::Right);
                                speed->x = controller->speed.x;
                                controller->action = AIControlledComponent::Actions::WalkRight;
                                holder->handPosition = SkeletonHandPositionStandingRight;
                            } else {
                                animation->currentAnimation = PlayerAnimationComponent::Walking;
                                animation->setDirection(PlayerAnimationComponent::Directions::Left);
                                speed->x = -controller->speed.x;
                                controller->action = AIControlledComponent::Actions::WalkLeft;
                                holder->handPosition = SkeletonHandPositionStandingLeft;
                            }
                        }
                    }
                } else if (controller->action == AIControlledComponent::Actions::Jump) {
                    if (diff.x < 0) {
                        animation->setDirection(PlayerAnimationComponent::Directions::Right);
                        speed->x = controller->speed.x;
                        holder->handPosition = SkeletonHandPositionStandingRight;
                    } else {
                        animation->setDirection(PlayerAnimationComponent::Directions::Left);
                        speed->x = -controller->speed.x;
                        holder->handPosition = SkeletonHandPositionStandingLeft;
                    }
                }
            }
                break;


            /****************************************************************/
            /*********************** GO FOR BALL ****************************/
            /****************************************************************/
            case AIControlledComponent::GoForGoal: {
                auto body = entity.component<BodyComponent>();
                auto holder = entity.component<BallHolderComponent>();
                auto animation = entity.component<PlayerAnimationComponent>();
                auto speed = entity.component<SpeedComponent>();

                //
                Entity goal;
                for (auto e : entityManager->each<BodyComponent, TeamComponent>()) {
                    if (e.component<BodyComponent>()->type == BodyComponent::Goal && e.component<TeamComponent>()->team == TeamComponent::Player) {
                        goal = e;
                        break;
                    }
                };
                if (!goal.isValid()) { return; }

                //
                auto ballBody = goal.component<BodyComponent>();
                float diff = body->getPosition().x - ballBody->getPosition().x;

                if (controller->action == AIControlledComponent::Actions::Standby) {
                    if (holder->holding && abs((int)diff) < 200 && controller->canMove()) {
                        // Set the right direction just in case
                        if (diff < 0) {
                            animation->setDirection(PlayerAnimationComponent::Directions::Right);
                        } else {
                            animation->setDirection(PlayerAnimationComponent::Directions::Left);
                        }

                        // Throw
                        animation->currentAnimation = PlayerAnimationComponent::Throwing;
                        animation->animations[animation->currentAnimation].restart();
                        controller->action = AIControlledComponent::Actions::Throw;
                        mThrowAction.execute(controller, animation, holder);
                    } else {
                        if (diff < 0) {
                            animation->currentAnimation = PlayerAnimationComponent::Walking;
                            animation->setDirection(PlayerAnimationComponent::Directions::Right);
                            speed->x = controller->speed.x;
                            controller->action = AIControlledComponent::Actions::WalkRight;
                            holder->handPosition = SkeletonHandPositionStandingRight;
                        } else {
                            animation->currentAnimation = PlayerAnimationComponent::Walking;
                            animation->setDirection(PlayerAnimationComponent::Directions::Left);
                            speed->x = -controller->speed.x;
                            controller->action = AIControlledComponent::Actions::WalkLeft;
                            holder->handPosition = SkeletonHandPositionStandingLeft;
                        }
                    }

                } else if (controller->action == AIControlledComponent::Actions::Jump) {
                    if (diff < 0) {
                        animation->setDirection(PlayerAnimationComponent::Directions::Right);
                        speed->x = controller->speed.x;
                        holder->handPosition = SkeletonHandPositionStandingRight;
                    } else {
                        animation->setDirection(PlayerAnimationComponent::Directions::Left);
                        speed->x = -controller->speed.x;
                        holder->handPosition = SkeletonHandPositionStandingLeft;
                    }
                }
            }
                break;
        }
    }
}

void AIController::onBallTaken(const BallTakenEvent &event)
{
    Entity holder = event.holder;
    if (event.team == TeamComponent::AI) {
        holder.component<AIControlledComponent>()->state = AIControlledComponent::GoForGoal;
    } else {
        auto entityManager = mContainer->get<EntityManager>();

        for (auto entity : entityManager->each<AIControlledComponent>()) {
            auto controller = entity.component<AIControlledComponent>();
            controller->state = AIControlledComponent::GoForPlayer;
        }
    }
}

void AIController::onBallDropped(const BallDroppedEvent &event)
{
    auto entityManager = mContainer->get<EntityManager>();

    for (auto entity : entityManager->each<AIControlledComponent>()) {
        auto controller = entity.component<AIControlledComponent>();
        controller->state = AIControlledComponent::LookingForBall;
    }
}

void AIController::onGoalScored(const GoalScoredEvent &event)
{
    auto entityManager = mContainer->get<EntityManager>();

    for (auto entity : entityManager->each<AIControlledComponent>()) {
        auto controller = entity.component<AIControlledComponent>();
        controller->state = AIControlledComponent::LookingForBall;
    }
}

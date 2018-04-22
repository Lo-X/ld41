//
// Created by loic on 22/04/18.
//

#ifndef LD41_AICONTROLLED_HPP
#define LD41_AICONTROLLED_HPP

#include <Fluffy/ECS/Component.hpp>
#include <SFML/System/Vector2.hpp>

using namespace Fluffy::ECS;

struct AIControlledComponent : public Component<AIControlledComponent>
{
    enum Actions {
        Standby,
        WalkRight,
        WalkLeft,
        Throw,
        Attack,
        Jump,
        Stunned,
    };

    enum State {
        LookingForBall,
        GoForPlayer,
        GoForGoal
    };

    AIControlledComponent() = default;

    bool canMove() const {
        return action != Throw && action != Attack && action != Stunned;
    }

    Actions action = Standby;
    State state = LookingForBall;
    sf::Vector2f speed = {230.f, 600.f}; // tweak to jump
};

#endif //LD41_AICONTROLLED_HPP

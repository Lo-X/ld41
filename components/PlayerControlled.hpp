//
// Created by loic on 21/04/18.
//

#ifndef LD41_PLAYERCONTROLLED_HPP
#define LD41_PLAYERCONTROLLED_HPP

#include <Fluffy/ECS/Component.hpp>
#include <SFML/System/Vector2.hpp>

using namespace Fluffy::ECS;

struct PlayerControlledComponent : public Component<PlayerControlledComponent>
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

    PlayerControlledComponent() = default;

    bool canMove() const {
        return action != Throw && action != Attack && action != Stunned;
    }

    Actions action = Actions::Standby;
    sf::Vector2f speed = {250.f, 600.f}; // tweak to jump
};

#endif //LD41_PLAYERCONTROLLED_HPP

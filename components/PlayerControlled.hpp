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
        Jump,
    };

    PlayerControlledComponent() = default;

    Actions action = Actions::Standby;
    sf::Vector2f speed = {200.f, 600.f}; // tweak to jump
};

#endif //LD41_PLAYERCONTROLLED_HPP

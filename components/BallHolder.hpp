//
// Created by loic on 21/04/18.
//

#ifndef LD41_BALLHOLDER_HPP
#define LD41_BALLHOLDER_HPP

#include <Fluffy/ECS/Component.hpp>
#include <SFML/System/Vector2.hpp>

using namespace Fluffy::ECS;

struct BallHolderComponent : public Component<BallHolderComponent>
{
    Entity ball;
    bool holding = false;
    sf::Vector2f handPosition;
    sf::Vector2f ballSpeed = {800.f, -100.f};
};

#endif //LD41_BALLHOLDER_HPP

//
// Created by loic on 22/04/18.
//

#ifndef LD41_BALL_HPP
#define LD41_BALL_HPP

#include <Fluffy/ECS/Component.hpp>

using namespace Fluffy::ECS;

struct BallComponent : public Component<BallComponent>
{
    bool taken = false;
};

#endif //LD41_BALL_HPP

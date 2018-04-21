//
// Created by loic on 21/04/18.
//

#ifndef LD41_SPEEDCOMPONENT_HPP
#define LD41_SPEEDCOMPONENT_HPP

#include <Fluffy/ECS/Component.hpp>

using namespace Fluffy::ECS;

struct SpeedComponent : public Component<SpeedComponent>
{
    SpeedComponent() = default;
    SpeedComponent(float x, float y) : x(x), y(y) {}
    float x = 0;
    float y = 0;
};

#endif //LD41_SPEEDCOMPONENT_HPP

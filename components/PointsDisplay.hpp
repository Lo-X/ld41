//
// Created by loic on 22/04/18.
//

#ifndef LD41_POINTSDISPLAY_HPP
#define LD41_POINTSDISPLAY_HPP

#include <Fluffy/ECS/Component.hpp>
#include "Team.hpp"

using namespace Fluffy::ECS;
using namespace Fluffy::Utility;

struct PointsDisplayComponent : public Component<PointsDisplayComponent>
{
    PointsDisplayComponent() = default;
    explicit PointsDisplayComponent(TeamComponent::Teams team) : team(team) {}

    unsigned int points = 0;
    TeamComponent::Teams team = TeamComponent::AI;
};

#endif //LD41_POINTSDISPLAY_HPP

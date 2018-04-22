//
// Created by loic on 22/04/18.
//

#ifndef LD41_TEAM_HPP
#define LD41_TEAM_HPP

#include <Fluffy/ECS/Component.hpp>

using namespace Fluffy::ECS;

struct TeamComponent : public Component<TeamComponent>
{
    enum Teams {
        Player,
        AI,
    };

    TeamComponent() = default;
    explicit TeamComponent(Teams team) : team(team) {}

    Teams team = Teams::AI;
};

#endif //LD41_TEAM_HPP

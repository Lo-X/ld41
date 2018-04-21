//
// Created by loic on 21/04/18.
//

#ifndef LD41_PLAYERCONTROLLEDSYSTEM_HPP
#define LD41_PLAYERCONTROLLEDSYSTEM_HPP

#include <Fluffy/ECS/System.hpp>

using namespace Fluffy::ECS;

class PlayerControlledSystem : public System<PlayerControlledSystem>
{
public:
    void update(EntityManager &entityManager, Fluffy::Utility::Time dt) override;
};


#endif //LD41_PLAYERCONTROLLEDSYSTEM_HPP

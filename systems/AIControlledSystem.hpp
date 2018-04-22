//
// Created by loic on 22/04/18.
//

#ifndef LD41_AICONTROLLEDSYSTEM_HPP
#define LD41_AICONTROLLEDSYSTEM_HPP

#include <Fluffy/ECS/System.hpp>

using namespace Fluffy::ECS;

class AIControlledSystem : public System<AIControlledSystem>
{
public:
    void update(EntityManager &entityManager, Fluffy::Utility::Time dt) override;
};


#endif //LD41_AICONTROLLEDSYSTEM_HPP

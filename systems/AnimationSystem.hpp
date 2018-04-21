//
// Created by loic on 21/04/18.
//

#ifndef LD41_ANIMATIONSYSTEM_HPP
#define LD41_ANIMATIONSYSTEM_HPP

#include <Fluffy/ECS/System.hpp>

using namespace Fluffy::ECS;

class AnimationSystem : public System<AnimationSystem>
{
public:
    void update(EntityManager &entityManager, Fluffy::Utility::Time dt) override;
};


#endif //LD41_ANIMATIONSYSTEM_HPP

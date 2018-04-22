//
// Created by loic on 22/04/18.
//

#ifndef LD41_CAMERASYSTEM_HPP
#define LD41_CAMERASYSTEM_HPP

#include <Fluffy/ECS/System.hpp>

using namespace Fluffy::ECS;

class CameraSystem : public System<CameraSystem>
{
public:
    void update(EntityManager &entityManager, Fluffy::Utility::Time dt) override;
};


#endif //LD41_CAMERASYSTEM_HPP

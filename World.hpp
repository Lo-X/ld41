//
// Created by loic on 21/04/18.
//

#ifndef LD41_WORLD_HPP
#define LD41_WORLD_HPP

#include <Fluffy/ECS/Entity.hpp>
#include <Fluffy/Utility/ServiceContainer.hpp>
#include <Fluffy/Utility/Time.hpp>
#include <SFML/Graphics/View.hpp>

using namespace Fluffy::ECS;
using namespace Fluffy::Utility;

class World
{
public:
    explicit World(ServiceContainer& container);

    void update(Time dt);
    void destroyDeadEntities() {}

private:
    void        loadTextures();
    void        buildScene();
    void        handleCollisions();
    void        markDeadEntities();

private:
    ServiceContainer& mContainer;
    sf::View          mWorldView;
    std::vector<Entity> mMarkedForDestruction;
};


#endif //LD41_WORLD_HPP

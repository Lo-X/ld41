//
// Created by loic on 21/04/18.
//

#ifndef LD41_WORLD_HPP
#define LD41_WORLD_HPP

#include <Fluffy/ECS/Entity.hpp>
#include <Fluffy/Utility/ServiceContainer.hpp>
#include <Fluffy/Utility/Time.hpp>
#include <SFML/Graphics/View.hpp>
#include <Fluffy/Asset/AssetHolder.hpp>
#include "typedefs.hpp"
#include "components/AIControlled.hpp"

using namespace Fluffy::ECS;
using namespace Fluffy::Utility;

class World
{
public:
    static constexpr float worldSize = 2*1280.f;
    static constexpr float ballSpawnX = 1280.f;
    static constexpr float ballSpawnY = 0.f;

public:
    explicit World(ServiceContainer& container);

    void update(Time dt);
    void destroyDeadEntities() {}

    bool waiting() const { return mGoalWaiting; }
    void respawnBall();
    void spawnSkeleton(sf::Vector2f position, AIControlledComponent::Role role) const;

private:
    void        loadTextures();
    void        buildUi();
    void        buildScene();
    void        handleCollisions();

    void        markDeadEntities();
private:
    ServiceContainer& mContainer;
    sf::View          mWorldView;
    sf::View          mBackgroundView;

    std::vector<Entity> mMarkedForDestruction;
    bool              mGoalWaiting = false;
    Entity            mBall;
};


#endif //LD41_WORLD_HPP

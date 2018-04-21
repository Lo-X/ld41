//
// Created by loic on 21/04/18.
//

#ifndef LD41_BODY_HPP
#define LD41_BODY_HPP

#include <Fluffy/ECS/Component.hpp>
#include <Fluffy/Scene/SceneNode.hpp>
#include <SFML/Graphics/Rect.hpp>

using namespace Fluffy::ECS;
using namespace Fluffy::Scene;

struct BodyComponent : public Component<BodyComponent>, public sf::Transformable
{
    enum Type {
        None,
        Wall,
        Platform,
        Player,
        Ball,
        Bullet,
        Creature,
        Goal,
    };

    using CollisionCallback = std::function<void (Entity source, Entity with)>;

    BodyComponent() = default;
    explicit BodyComponent(Type type) : type(type) {}
    BodyComponent(Type type, bool collidable, bool gravity) : type(type), collidable(collidable), gravity(gravity) {}

    Type type = Type::None;
    bool collidable = true;
    bool gravity = true;
    bool resting = true;
    sf::Vector2f size;
    std::map<Type, CollisionCallback> collisionCallbacks;

    const sf::Vector2f &getSize() const
    {
        return size;
    }

    void setSize(const sf::Vector2f &size)
    {
        BodyComponent::size = size;
    }

    sf::FloatRect boundings() {
        return sf::FloatRect(getPosition() - getOrigin(), size);
    }
};

#endif //LD41_BODY_HPP

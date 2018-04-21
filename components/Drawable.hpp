//
// Created by loic on 21/04/18.
//

#ifndef LD41_DRAWABLE_HPP
#define LD41_DRAWABLE_HPP

#include <Fluffy/ECS/Component.hpp>
#include <SFML/Graphics/Sprite.hpp>

using namespace Fluffy::ECS;

struct DrawableComponent : public Component<DrawableComponent>
{
    enum Layer {
        Background = 1,
        Scene = 2,
        Foreground = 3,
        COUNT
    };

    DrawableComponent() = default;
    explicit DrawableComponent(sf::Sprite&& sprite, Layer layer = Layer::Scene) : sprite(sprite), layer(layer) {}

    Layer layer = Layer::Scene;
    sf::Sprite sprite;
};


#endif //LD41_DRAWABLE_HPP

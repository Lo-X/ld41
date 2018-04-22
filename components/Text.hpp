//
// Created by loic on 22/04/18.
//

#ifndef LD41_TEXTCOMPONENT_HPP
#define LD41_TEXTCOMPONENT_HPP

#include <Fluffy/ECS/Component.hpp>
#include <SFML/Graphics/Text.hpp>

using namespace Fluffy::ECS;

struct TextComponent : public Component<TextComponent>
{
    TextComponent() = default;

    sf::Text text;
};

#endif //LD41_TEXTCOMPONENT_HPP

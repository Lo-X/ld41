//
// Created by loic on 21/04/18.
//

#ifndef LD41_CAMERACOMPONENT_HPP
#define LD41_CAMERACOMPONENT_HPP

#include <Fluffy/ECS/Component.hpp>
#include <SFML/Graphics/View.hpp>

using namespace Fluffy::ECS;

struct CameraComponent : public Component<CameraComponent>
{
    CameraComponent() = default;
    explicit CameraComponent(sf::View view) : view(view) {}

    sf::View view;
};

#endif //LD41_CAMERACOMPONENT_HPP

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
    explicit CameraComponent(sf::View view, float ratio = 1.f) : view(view), ratio(ratio) {}

    sf::View view;
    float ratio = 1.f;
};

struct BackgroundCameraComponent : public CameraComponent, public Component<BackgroundCameraComponent>
{
    BackgroundCameraComponent() = default;
    explicit BackgroundCameraComponent(sf::View view, float ratio = 1.f) : CameraComponent(view, ratio) {}
};

#endif //LD41_CAMERACOMPONENT_HPP

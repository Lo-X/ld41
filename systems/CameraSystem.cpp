//
// Created by loic on 22/04/18.
//

#include "CameraSystem.hpp"
#include "../components/Camera.hpp"
#include "../components/Body.hpp"

void CameraSystem::update(EntityManager &entityManager, Time dt)
{
    for (auto entity : entityManager.each<CameraComponent, BodyComponent>()) {
        auto camera = entity.component<CameraComponent>();
        auto body = entity.component<BodyComponent>();

        camera->view.setCenter(body->getPosition().x * camera->ratio, camera->view.getCenter().y);
    }

    for (auto entity : entityManager.each<BackgroundCameraComponent, BodyComponent>()) {
        auto camera = entity.component<BackgroundCameraComponent>();
        auto body = entity.component<BodyComponent>();

        camera->view.setCenter(body->getPosition().x * camera->ratio, camera->view.getCenter().y);
    }
}

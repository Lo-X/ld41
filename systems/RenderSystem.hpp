//
// Created by loic on 21/04/18.
//

#ifndef LD41_RENDERSYSTEM_HPP
#define LD41_RENDERSYSTEM_HPP

#include <Fluffy/ECS/System.hpp>
#include <Fluffy/Scene/Scene.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "../components/Drawable.hpp"
#include "../scene/LayerNode.hpp"
#include "../components/Body.hpp"
#include "../components/Camera.hpp"
#include "../components/Text.hpp"

using namespace Fluffy::ECS;
using namespace Fluffy::Scene;

using SceneLayers = std::array<std::vector<ComponentHandle<DrawableComponent>>, DrawableComponent::Layer::COUNT>;

class RenderSystem : public System<RenderSystem>
{
public:
    RenderSystem() = default;

    void update(EntityManager& entityManager, Time dt) override {}

    void configure(EntityManager &entityManager, EventManager &eventManager) override
    {
        BaseSystem::configure(entityManager, eventManager);
        mEntityManager = &entityManager;
    }

    void draw(sf::RenderWindow& window)
    {
        if (!mEntityManager) { return; }
        SceneLayers layers;
        std::vector<sf::Text> texts;
        std::vector<sf::RectangleShape> debugBoxes;

        ComponentHandle<CameraComponent> camera;
        ComponentHandle<BackgroundCameraComponent> bgcamera;
        for (auto entity : mEntityManager->each<CameraComponent>()) {
            camera = entity.component<CameraComponent>();
            break;
        }
        for (auto entity : mEntityManager->each<BackgroundCameraComponent>()) {
            bgcamera = entity.component<BackgroundCameraComponent>();
            break;
        }

        // Sort by layers
        for (auto entity : mEntityManager->each<DrawableComponent, BodyComponent>()) {
            auto drawable = entity.component<DrawableComponent>();
            auto body = entity.component<BodyComponent>();

            // Change position... of the drawable
            if (drawable->visible) {
                drawable->sprite.setPosition(body->getPosition());
                layers[drawable->layer].push_back(drawable);

//                auto boundingRect = body->boundings();
//                sf::RectangleShape rectangleShape;
//                rectangleShape.setSize({boundingRect.width, boundingRect.height});
//                rectangleShape.setPosition({boundingRect.left, boundingRect.top});
//                rectangleShape.setOutlineColor(sf::Color::Red);
//                rectangleShape.setOutlineThickness(1.f);
//                rectangleShape.setFillColor(sf::Color::Transparent);
//                debugBoxes.push_back(rectangleShape);
            }
        }
        for (auto entity : mEntityManager->each<TextComponent>()) {
            auto text = entity.component<TextComponent>();
            texts.push_back(text->text);
        }

        // Display
        window.setView(bgcamera->view);
        for (auto drawable_component : layers[DrawableComponent::Layer::Background]) {
            window.draw(drawable_component->sprite);
        }

        window.setView(camera->view);
        for (auto drawable_component : layers[DrawableComponent::Layer::Scene]) {
            window.draw(drawable_component->sprite);
        }

//        for (auto shape : debugBoxes) {
//            window.draw(shape);
//        }
        window.setView(window.getDefaultView());

        for (auto drawable_component : layers[DrawableComponent::Layer::Foreground]) {
            window.draw(drawable_component->sprite);
        }
        for (auto text : texts) {
            window.draw(text);
        }
    }

private:

private:
    EntityManager* mEntityManager = nullptr;
    SceneLayers mScenes;
};


#endif //LD41_RENDERSYSTEM_HPP

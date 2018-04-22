//
// Created by loic on 22/04/18.
//

#include "UISystem.hpp"
#include "../components/HelpMessage.hpp"
#include "../components/Drawable.hpp"
#include "../components/GoalMessage.hpp"
#include "../components/PointsDisplay.hpp"
#include "../components/Text.hpp"

void UISystem::update(EntityManager &entityManager, Fluffy::Utility::Time dt)
{
    for (auto entity : entityManager.each<HelpMessageComponent, DrawableComponent>()) {
        auto message = entity.component<HelpMessageComponent>();
        auto drawable = entity.component<DrawableComponent>();

        if (message->visible) {
            message->age += dt;
            if (message->age > message->duration) {
                message->visible = false;
                drawable->visible = false;
            }
        }
    }
}

void UISystem::onGoalScored(const GoalScoredEvent &event)
{
    for (auto entity : mEntityManager->each<HelpMessageComponent, GoalMessageComponent, DrawableComponent>()) {
        auto message = entity.component<HelpMessageComponent>();
        auto drawable = entity.component<DrawableComponent>();

        message->age = Time::Zero;
        message->visible = true;
        drawable->visible = true;
    }

    for (auto entity : mEntityManager->each<PointsDisplayComponent, TextComponent>()) {
        auto points = entity.component<PointsDisplayComponent>();
        auto text = entity.component<TextComponent>();

        if (points->team == event.team) {
            points->points++;
            text->text.setString(toString(points->points));
        }
    }
}

void UISystem::configure(EntityManager &entityManager, EventManager &eventManager)
{
    mEntityManager = &entityManager;
    mGoalSlot = eventManager.connect<GoalScoredEvent>(std::bind(&UISystem::onGoalScored, this, std::placeholders::_1));
}

UISystem::~UISystem()
{
    mGoalSlot.disconnect();
}

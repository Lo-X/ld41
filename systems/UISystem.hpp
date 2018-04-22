//
// Created by loic on 22/04/18.
//

#ifndef LD41_UISYSTEM_HPP
#define LD41_UISYSTEM_HPP

#include <Fluffy/ECS/System.hpp>
#include "../events.hpp"

using namespace Fluffy::ECS;

class UISystem : public System<UISystem>
{
public:
    ~UISystem();
    void configure(EntityManager &entityManager, EventManager &eventManager) override;
    void update(EntityManager &entityManager, Fluffy::Utility::Time dt) override;

    void onGoalScored(const GoalScoredEvent& event);

private:
    EntityManager* mEntityManager = nullptr;
    Slot mGoalSlot;
};


#endif //LD41_UISYSTEM_HPP

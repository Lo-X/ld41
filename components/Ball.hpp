//
// Created by loic on 22/04/18.
//

#ifndef LD41_BALL_HPP
#define LD41_BALL_HPP

#include <Fluffy/ECS/Component.hpp>
#include "../events.hpp"

using namespace Fluffy::ECS;

struct BallComponent : public Component<BallComponent>
{
public:
    BallComponent() = default;
    explicit BallComponent(EventManager* events) : mEvents(events) {}

    void take(Entity entity) {
        mTaken = true;
        if (mEvents) {
            mEvents->emit(BallTakenEvent(entity));
        }
    }

    void drop() {
        mTaken = false;
        if (mEvents) {
            mEvents->emit(BallDroppedEvent());
        }
    }

    bool taken() const { return mTaken; }

private:
    EventManager* mEvents = nullptr;
    bool mTaken = false;
};

#endif //LD41_BALL_HPP

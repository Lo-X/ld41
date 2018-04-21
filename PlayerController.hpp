//
// Created by loic on 21/04/18.
//

#ifndef LD41_PLAYERCONTROLLER_HPP
#define LD41_PLAYERCONTROLLER_HPP

#include <Fluffy/Utility/ServiceContainer.hpp>
#include <Fluffy/Event/Event.hpp>
#include "events.hpp"

using namespace Fluffy::Utility;

class PlayerController
{
public:
    explicit PlayerController(ServiceContainer* container);
    ~PlayerController();

    void onJoystickButtonPressedEvent(const JoystickButtonPressedEvent& event);
    void onJoystickXAnalogUsedEvent(const JoystickXAnalogEvent& event);
private:
    ServiceContainer* mContainer;
    Slot mJoystickButtonPressedSlot;
    Slot mJoystickXAxisUsedSlot;
};


#endif //LD41_PLAYERCONTROLLER_HPP

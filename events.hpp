//
// Created by loic on 21/04/18.
//

#ifndef LD41_EVENTS_HPP
#define LD41_EVENTS_HPP

#include <Fluffy/Event/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "components/Team.hpp"

using namespace Fluffy::Event;

struct RenderEvent : public Event<RenderEvent>
{
    explicit RenderEvent(sf::RenderWindow& window) : window(window) {}

    sf::RenderWindow& window;
};

struct KeyPressedEvent : public Event<KeyPressedEvent>
{
    explicit KeyPressedEvent(sf::Keyboard::Key key) : key(key) {}

    sf::Keyboard::Key key;
};

struct KeyDownEvent : public Event<KeyDownEvent>
{
    explicit KeyDownEvent(sf::Keyboard::Key key) : key(key) {}

    sf::Keyboard::Key key;
};

struct JoystickButtonPressedEvent : public Event<JoystickButtonPressedEvent>
{
    explicit JoystickButtonPressedEvent(unsigned int button) : button(button) {}

    unsigned int button;
};

struct JoystickXAnalogEvent : public Event<JoystickXAnalogEvent>
{
    explicit JoystickXAnalogEvent(float position) : position(position) {}

    float position = 0.f;
};

/*******************************************************************************/
/***************************** Game Events *************************************/
/*******************************************************************************/

struct BallDroppedEvent : public Event<BallDroppedEvent>
{
};

struct BallTakenEvent : public Event<BallTakenEvent>
{
    explicit BallTakenEvent(Entity holder) : holder(holder) {
        team = holder.component<TeamComponent>()->team;
    }

    Entity holder;
    TeamComponent::Teams team;
};

struct GoalScoredEvent : public Event<GoalScoredEvent>
{
    GoalScoredEvent(TeamComponent::Teams team) : team(team) {}

    TeamComponent::Teams team;
};



#endif //LD41_EVENTS_HPP

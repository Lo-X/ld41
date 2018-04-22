//
// Created by loic on 21/04/18.
//

#include "Application.hpp"
#include "events.hpp"
#include "typedefs.hpp"
#include "states/GameState.hpp"
#include "systems/PlayerControlledSystem.hpp"
#include "PlayerController.hpp"
#include "systems/PhysicsSystem.hpp"
#include "systems/AnimationSystem.hpp"
#include "systems/BallHolderSystem.hpp"
#include "AIController.hpp"
#include "systems/AIControlledSystem.hpp"
#include "systems/CameraSystem.hpp"
#include "systems/UISystem.hpp"
#include "components/SoundPlayer.hpp"
#include "systems/SoundSystem.hpp"
#include <Fluffy/ECS/EntityManager.hpp>
#include <Fluffy/ECS/SystemManager.hpp>
#include <Fluffy/Utility/Clock.hpp>
#include <SFML/Window/Event.hpp>

const Time Application::timePerFrame = seconds(1.f / 60.f);

Application::Application(unsigned int width, unsigned int height, const std::string &title)
: mStateStack(mServiceContainer)
{
    mWindow = new sf::RenderWindow({width, height}, title.c_str(), sf::Style::Close);
    mWindow->setKeyRepeatEnabled(false);

    mServiceContainer.set<EventManager>();
    mServiceContainer.set<EntityManager, EventManager>();
    mServiceContainer.set<SystemManager, EntityManager, EventManager>();

    mServiceContainer.set<TextureHolder>();
    mServiceContainer.set<FontHolder>();
    mServiceContainer.set<SoundHolder>();
    mServiceContainer.set<MusicHolder>();

    mServiceContainer.set<PlayerController>(&mServiceContainer);
    mServiceContainer.set<AIController>(&mServiceContainer);

    mServiceContainer.give<sf::RenderWindow>(mWindow);

    // Load assets here
    mServiceContainer.get<FontHolder>()->load("main", "assets/fonts/main.ttf");

    mServiceContainer.get<TextureHolder>()->load("background", "assets/textures/background.png");
    mServiceContainer.get<TextureHolder>()->get("background").setRepeated(true);
    mServiceContainer.get<TextureHolder>()->load("floor", "assets/textures/floor.png");
    mServiceContainer.get<TextureHolder>()->get("floor").setRepeated(true);
    mServiceContainer.get<TextureHolder>()->load("wall", "assets/textures/wall.png");
    mServiceContainer.get<TextureHolder>()->get("wall").setRepeated(true);
    mServiceContainer.get<TextureHolder>()->load("tile1", "assets/textures/tile1.png");
    mServiceContainer.get<TextureHolder>()->get("tile1").setRepeated(true);
    mServiceContainer.get<TextureHolder>()->load("player_throwing", "assets/textures/player_throwing.png");
    mServiceContainer.get<TextureHolder>()->load("player_attacking", "assets/textures/player_attacking.png");
    mServiceContainer.get<TextureHolder>()->load("player_standing", "assets/textures/player_standing.png");
    mServiceContainer.get<TextureHolder>()->load("player_jumping", "assets/textures/player_jumping.png");
    mServiceContainer.get<TextureHolder>()->load("player_running", "assets/textures/player_running.png");
    mServiceContainer.get<TextureHolder>()->load("ai_attacking", "assets/textures/ai_attacking.png");
    mServiceContainer.get<TextureHolder>()->load("ai_standing", "assets/textures/ai_standing.png");
    mServiceContainer.get<TextureHolder>()->load("ai_running", "assets/textures/ai_running.png");
    mServiceContainer.get<TextureHolder>()->load("player_dead", "assets/textures/player_dead.png");
    mServiceContainer.get<TextureHolder>()->load("ball", "assets/textures/ball.png");
    mServiceContainer.get<TextureHolder>()->load("goal", "assets/textures/goal.png");
    mServiceContainer.get<TextureHolder>()->load("ui_help", "assets/textures/ui_help.png");
    mServiceContainer.get<TextureHolder>()->load("ui_goal", "assets/textures/ui_goal.png");
    mServiceContainer.get<TextureHolder>()->load("ui_points_player", "assets/textures/ui_points_player.png");
    mServiceContainer.get<TextureHolder>()->load("ui_points_ai", "assets/textures/ui_points_ai.png");
    mServiceContainer.get<TextureHolder>()->load("ui_title", "assets/textures/ui_title.png");

    mServiceContainer.get<SoundHolder>()->load("applause", "assets/sounds/applause.ogg");
    mServiceContainer.get<SoundHolder>()->load("goal", "assets/sounds/goal.ogg");
    mServiceContainer.get<SoundHolder>()->load("jump_ai", "assets/sounds/jump_ai.ogg");
    mServiceContainer.get<SoundHolder>()->load("jump_player", "assets/sounds/jump_player.ogg");
    mServiceContainer.get<SoundHolder>()->load("pickup", "assets/sounds/pickup.ogg");
    mServiceContainer.get<SoundHolder>()->load("punch", "assets/sounds/punch.ogg");
    mServiceContainer.get<SoundHolder>()->load("throw", "assets/sounds/throw.ogg");

    mServiceContainer.get<MusicHolder>()->load("main", "assets/musics/main.ogg");
    mServiceContainer.get<MusicHolder>()->setVolume(65);

    // Stats
    mStatisticsText.setFont(mServiceContainer.get<FontHolder>()->get("main"));
    mStatisticsText.setPosition(5.f,5.f);
    mStatisticsText.setCharacterSize(12);
    mStatisticsText.setFillColor(sf::Color::White);
    mStatisticsText.setOutlineColor(sf::Color::Black);

    // States
    mStateStack.registerState<GameState>();
    mStateStack.push<GameState>();
    mStateStack.forcePendingChanges();

    // Systems
    mRenderSystem = mServiceContainer.get<SystemManager>()->add<RenderSystem>();
    mServiceContainer.get<SystemManager>()->add<AnimationSystem>();
    mServiceContainer.get<SystemManager>()->add<PhysicsSystem>();
    mServiceContainer.get<SystemManager>()->add<BallHolderSystem>();
    mServiceContainer.get<SystemManager>()->add<PlayerControlledSystem>();
    mServiceContainer.get<SystemManager>()->add<AIControlledSystem>();
    mServiceContainer.get<SystemManager>()->add<CameraSystem>();
    mServiceContainer.get<SystemManager>()->add<UISystem>();
    mServiceContainer.get<SystemManager>()->add<SoundSystem>();
    // ...
    mServiceContainer.get<SystemManager>()->configure();
}

void Application::run()
{
    Clock clock;
    Time timeSinceLastUpdate = Time::Zero;

    while (mWindow->isOpen()) {
        Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate >= timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            BeforeGameTickEvent beforeGameTickEvent;
            mServiceContainer.get<EventManager>()->emit(beforeGameTickEvent);

            processEvents();
            update(timePerFrame);

            AfterGameTickEvent afterGameTickEvent;
            mServiceContainer.get<EventManager>()->emit(afterGameTickEvent);

            if (mStateStack.isEmpty()) {
                mWindow->close();
            }
        }

        updateStatistics(elapsedTime);
        render();
    }
}

void Application::processEvents()
{
    sf::Event event;
    auto eventManager = mServiceContainer.get<EventManager>();

    while (mWindow->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                mWindow->close();
                break;

            case sf::Event::JoystickButtonPressed:
                eventManager->emit(JoystickButtonPressedEvent(event.joystickButton.button));
                break;

            case sf::Event::KeyPressed:
                eventManager->emit(KeyPressedEvent(event.key.code));
                break;

            default:break;
        }
    }

    if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 45 || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < -45) {
        eventManager->emit(JoystickXAnalogEvent(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X)));
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { eventManager->emit(KeyDownEvent(sf::Keyboard::A)); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { eventManager->emit(KeyDownEvent(sf::Keyboard::Q)); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) { eventManager->emit(KeyDownEvent(sf::Keyboard::E)); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { eventManager->emit(KeyDownEvent(sf::Keyboard::D)); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { eventManager->emit(KeyDownEvent(sf::Keyboard::Right)); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { eventManager->emit(KeyDownEvent(sf::Keyboard::Left)); }
}

void Application::update(Time dt)
{
    auto tick = GameTickEvent(dt);
    mServiceContainer.get<EventManager>()->emit(tick);
    mServiceContainer.get<SystemManager>()->updateAll(dt);
}

void Application::render()
{
    mWindow->clear();
    mWindow->setView(mWindow->getDefaultView());

    mServiceContainer.get<EventManager>()->emit(RenderEvent(*mWindow));
    mRenderSystem->draw(*mWindow);

//    mWindow->draw(mStatisticsText);
    mWindow->display();
}

void Application::updateStatistics(Time dt)
{
    mStatisticsUpdateTime += dt;
    mStatisticsNumFrames += 1;

    if(mStatisticsUpdateTime >= seconds(1.f))
    {
        mStatisticsText.setString(
                "Frames/sec = " + toString(mStatisticsNumFrames) + "\n" +
                "Time/update = " + toString(mStatisticsUpdateTime.milliseconds() / mStatisticsNumFrames) + "ms"
        );

        mStatisticsNumFrames = 0;
        mStatisticsUpdateTime -= seconds(1.f);
    }
}

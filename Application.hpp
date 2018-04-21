//
// Created by loic on 21/04/18.
//

#ifndef LD41_APPLICATION_HPP
#define LD41_APPLICATION_HPP

#include <Fluffy/Utility/Time.hpp>
#include <Fluffy/State/StateStack.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>

using namespace Fluffy::Utility;
using namespace Fluffy::State;

class Application
{
public:
    Application(unsigned int width, unsigned int height, const std::string& title);

    void run();

private:
    void processEvents();
    void update(Time dt);
    void render();

    void updateStatistics(Time dt);

private:
    static const Time timePerFrame;
    sf::RenderWindow* mWindow = nullptr;
    ServiceContainer mServiceContainer;
    StateStack mStateStack;

//    std::shared_ptr<RenderSystem> mRenderSystem;

    // Stats
    sf::Text mStatisticsText;
    Time mStatisticsUpdateTime;
    std::size_t mStatisticsNumFrames;
};


#endif //LD41_APPLICATION_HPP

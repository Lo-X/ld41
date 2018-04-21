//
// Created by loic on 21/04/18.
//

#ifndef LD41_PLAYERANIMATION_HPP
#define LD41_PLAYERANIMATION_HPP

#include <Fluffy/ECS/Component.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "../Animation.hpp"

using namespace Fluffy::ECS;

struct PlayerAnimationComponent : public Component<PlayerAnimationComponent>
{
    enum Animations {
        Standing = 0,
        Walking,
        Jumping,
        Attacking,
        Dead,
        COUNT
    };

    enum Directions {
        Right,
        Left
    };

    PlayerAnimationComponent() = default;

    std::array<Animation, Animations::COUNT> animations;
    Animations  currentAnimation = Standing;

    void update(Time dt) {
        animations[currentAnimation].update(dt);
    }

    sf::Sprite currentSprite() {
        return animations[currentAnimation].getCurrentSprite();
    }

    void setDirection(Directions dir) {
        direction = dir;

        switch (dir) {
            case Right:
                animations[Animations::Standing].setScale({1.f, 1.f});
                animations[Animations::Walking].setScale({1.f, 1.f});
                animations[Animations::Jumping].setScale({1.f, 1.f});
                animations[Animations::Attacking].setScale({1.f, 1.f});
                break;

            case Left:
                animations[Animations::Standing].setScale({-1.f, 1.f});
                animations[Animations::Walking].setScale({-1.f, 1.f});
                animations[Animations::Jumping].setScale({-1.f, 1.f});
                animations[Animations::Attacking].setScale({-1.f, 1.f});
                break;
        }
    }

    Directions direction;
};

#endif //LD41_PLAYERANIMATION_HPP

//
// Created by loic on 21/04/18.
//

#ifndef LD41_ANIMATION_HPP
#define LD41_ANIMATION_HPP

#include <Fluffy/Utility/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

using namespace Fluffy::Utility;

class Animation
{
public:
    Animation();
    explicit Animation(const sf::Texture& texture);

    void                setTexture(const sf::Texture& texture);
    const sf::Texture*  getTexture() const;

    void                setFrameOrigin(sf::Vector2i origin);
    sf::Vector2i        getFrameOrigin() const;

    void                setFrameSize(sf::Vector2i size);
    sf::Vector2i        getFrameSize() const;

    void                setNumFrames(std::size_t numFrames);
    std::size_t         getNumFrames() const;

    void                setScale(sf::Vector2f scale);
    sf::Vector2f        getScale() const;

    void                setOrigin(sf::Vector2f origin);
    sf::Vector2f        getOrigin() const;

    void                setDuration(Time duration);
    Time                getDuration() const;

    void                setRepeating(bool flag);
    bool                isRepeating() const;

    void                restart();
    bool                isFinished() const;

    void                update(Time dt);

    sf::Sprite          getCurrentSprite() const { return mSprite; }

private:
    sf::Sprite          mSprite;
    sf::Vector2i        mFrameSize;
    sf::Vector2i        mFrameOrigin;
    std::size_t         mNumFrames;
    std::size_t         mCurrentFrame;
    sf::Vector2f        mScale;
    Time                mDuration;
    Time                mElapsedTime;
    bool                mRepeat;
};


#endif //LD41_ANIMATION_HPP

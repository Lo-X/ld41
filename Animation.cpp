//
// Created by loic on 21/04/18.
//

#include <iostream>
#include "Animation.hpp"

Animation::Animation() :
        mSprite(),
        mFrameSize(),
        mNumFrames(0),
        mCurrentFrame(0),
        mScale({1.f, 1.f}),
        mDuration(Time::Zero),
        mElapsedTime(Time::Zero),
        mRepeat(false)
{
}

Animation::Animation(const sf::Texture &texture) :
        mSprite(texture),
        mFrameSize(),
        mNumFrames(0),
        mCurrentFrame(0),
        mScale({1.f, 1.f}),
        mDuration(Time::Zero),
        mElapsedTime(Time::Zero),
        mRepeat(false)
{
}

void Animation::setTexture(const sf::Texture &texture)
{
    mSprite.setTexture(texture);
}

const sf::Texture* Animation::getTexture() const
{
    return mSprite.getTexture();
}

void Animation::setFrameSize(sf::Vector2i frameSize)
{
    mFrameSize = frameSize;
}

sf::Vector2i Animation::getFrameSize() const
{
    return mFrameSize;
}

void Animation::setFrameOrigin(sf::Vector2i origin)
{
    mFrameOrigin = origin;
}

sf::Vector2i Animation::getFrameOrigin() const
{
    return mFrameOrigin;
}

void Animation::setNumFrames(std::size_t numFrames)
{
    mNumFrames = numFrames;
}

std::size_t Animation::getNumFrames() const
{
    return mNumFrames;
}

void Animation::setScale(sf::Vector2f scale)
{
    mScale = scale;
}

sf::Vector2f Animation::getScale() const
{
    return mScale;
}

void Animation::setDuration(Time duration)
{
    mDuration = duration;
}

Time Animation::getDuration() const
{
    return mDuration;
}

void Animation::setRepeating(bool flag)
{
    mRepeat = flag;
}

bool Animation::isRepeating() const
{
    return mRepeat;
}

void Animation::restart()
{
    mCurrentFrame = 0;
}

bool Animation::isFinished() const
{
    return mCurrentFrame >= mNumFrames;
}

void Animation::setOrigin(sf::Vector2f origin)
{
    mSprite.setOrigin(origin);
}

sf::Vector2f Animation::getOrigin() const
{
    return mSprite.getOrigin();
}

void Animation::update(Time dt)
{
    Time timePerFrame = mDuration / static_cast<float>(mNumFrames);
    mElapsedTime += dt;

    sf::Vector2i textureBounds(mFrameSize * (int)mNumFrames);
    sf::IntRect textureRect = mSprite.getTextureRect();

    if (mCurrentFrame == 0) {
        textureRect = sf::IntRect(mFrameOrigin.x, mFrameOrigin.y, mFrameSize.x, mFrameSize.y);
    }

    // While we have a frame to process
    while (mElapsedTime >= timePerFrame && (mCurrentFrame < mNumFrames || mRepeat))
    {
        // And progress to next frame
        mElapsedTime -= timePerFrame;
        if (mRepeat) {
            mCurrentFrame = (mCurrentFrame + 1) % mNumFrames;

            if (mCurrentFrame == 0) {
                textureRect = sf::IntRect(mFrameOrigin.x, mFrameOrigin.y, mFrameSize.x, mFrameSize.y);
            } else {
                textureRect.left += textureRect.width;
            }
        } else {
            if (textureRect.left + textureRect.width < textureBounds.x) {
                textureRect.left += textureRect.width;
                mCurrentFrame++;
            }
        }
    }

    mSprite.setScale(mScale);
    mSprite.setTextureRect(textureRect);
}
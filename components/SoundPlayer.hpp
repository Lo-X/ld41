//
// Created by loic on 22/04/18.
//

#ifndef LD41_SOUNDPLAYER_HPP
#define LD41_SOUNDPLAYER_HPP

#include <Fluffy/ECS/Component.hpp>
#include <Fluffy/Asset/SoundPlayer.hpp>
#include "../typedefs.hpp"

using namespace Fluffy::ECS;
using namespace Fluffy::Asset;

struct SoundPlayerComponent : public Component<SoundPlayerComponent>
{
    SoundPlayerComponent() = default;
    explicit SoundPlayerComponent(SoundHolder* player) : player(player) {}

    SoundHolder* player = nullptr;
};

#endif //LD41_SOUNDPLAYER_HPP

//
// Created by loic on 22/04/18.
//

#ifndef LD41_HELPMESSAGE_HPP
#define LD41_HELPMESSAGE_HPP

#include <Fluffy/ECS/Component.hpp>
#include <Fluffy/Utility/Time.hpp>

using namespace Fluffy::ECS;
using namespace Fluffy::Utility;

struct HelpMessageComponent : public Component<HelpMessageComponent>
{
    HelpMessageComponent() = default;

    Time age;
    bool visible = true;
};

#endif //LD41_HELPMESSAGE_HPP

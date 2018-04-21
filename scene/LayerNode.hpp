//
// Created by loic on 21/04/18.
//

#ifndef LD41_LAYERNODE_HPP
#define LD41_LAYERNODE_HPP

#include <Fluffy/Scene/SceneNode.hpp>
#include "../components/Drawable.hpp"

using namespace Fluffy::Scene;

class LayerNode : public SceneNode
{
public:
    SCENE_NODE(LayerNode)

    LayerNode(std::string&& name, DrawableComponent::Layer layer) : SceneNode(std::move(name)), mLayer(layer)
    {
    }

private:
    DrawableComponent::Layer mLayer;
};

#endif //LD41_LAYERNODE_HPP

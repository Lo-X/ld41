//
// Created by loic on 21/04/18.
//

#ifndef LD41_TYPEDEFS_HPP
#define LD41_TYPEDEFS_HPP

#include <Fluffy/Asset/AssetHolder.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

using namespace Fluffy::Asset;

typedef AssetHolder<sf::Texture, std::string> TextureHolder;
typedef AssetHolder<sf::Font, std::string> FontHolder;

#endif //LD41_TYPEDEFS_HPP
